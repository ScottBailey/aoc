#include <iostream>
#include <chrono>
#include <algorithm>
#include <stdint.h>
#include <aoc/get.h>            // aoc::get_line()
#include <sb/string.h>          // sb::string::parse(), sb::string::from()
#include <bitset>
#include <map>

//const bool debug=false;

enum facing {
   north = 0,
   east = 1,
   south = 2,
   west = 3,

   size = 4,

};

class tile
{
public:
   //using map_t = std::map<unsigned,tile>;
   using list_t = std::vector<tile>;
   using bits_t = uint16_t;

   tile() : m_number{0}, m_data{0}, m_edges1{0}, m_edges2{0} {}
   tile(unsigned number) : m_number{number}, m_data{0}, m_edges1{0}, m_edges2{0} {}

   // tile number
   unsigned number() const { return m_number; }


   void make_west(const bits_t& w);

   bits_t edge(facing f) const { return m_edges1[f]; }
   bits_t redge(facing f) const { return m_edges2[f]; }


   bool load(std::istream& is=std::cin);
   void dump() const;

   // all unique edges, forward and reverse
   std::vector<bits_t> unique_edges() const;

   bool operator==(const tile& rhs) const {return m_number == rhs.m_number; }
   bool operator<(const tile& rhs) const {return m_number < rhs.m_number; }

private:

   void flip_l2r();   // flip left to right
   void flip_t2b();   // flip top tobottom
   void rotate();     // rotate 90 degree clockwise

   void make_edges();  // populate m_edges1 and m_edges2 from m_data

   static bits_t reverse(const bits_t& n);
   static bool get(const bits_t& b, size_t n);
   static void set(bits_t& b, size_t n, bool val);
   static std::string str1(bits_t n);
   static std::string str2(bits_t n);
private:
   unsigned m_number;
   std::array<bits_t,10> m_data;

   std::array<bits_t,facing::size> m_edges1;
   std::array<bits_t,facing::size> m_edges2;
};



void tile::flip_t2b()
{
   std::swap(m_data[0],m_data[9]);
   std::swap(m_data[1],m_data[8]);
   std::swap(m_data[2],m_data[7]);
   std::swap(m_data[3],m_data[6]);
   std::swap(m_data[4],m_data[5]);
}

void tile::flip_l2r()
{
   for(size_t i = 0; i < m_data.size(); ++i)
      m_data[i] = reverse(m_data[i]);
}


void tile::rotate()
{
   std::array<bits_t,10> orig{m_data};

   for(size_t i = 0; i < 10; ++i) // these are, essentially, the columns when rotate 90 deg clockwise (col = 9-i)
   {
      size_t column = 9-i; // cloumnin new orientation
      for(size_t j = 0; j < 10; ++j) // row in new orientation
         set(m_data[j], column, get(orig[i],j));
   }

}



void tile::make_west(const bits_t& w)
{
   // already there?
   if(w ==  m_edges1[west])
      return;

   if(w == m_edges1[north])
   {
      rotate();
      flip_t2b();
      flip_l2r();
   }
   else if(w == m_edges1[east])
   {
      flip_t2b();
      flip_l2r();
   }
   else if(w == m_edges1[south])
   {
      rotate();
   }

   else if(w ==  m_edges2[west])
   {
      flip_t2b();
   }
   else if(w == m_edges2[north])
   {
      rotate();
      flip_l2r();
   }
   else if(w == m_edges2[east])
   {
      flip_l2r();
   }
   else if(w == m_edges2[south])
   {
      rotate();
      flip_t2b();
   }
   else
   {
      std::cerr << "Tile " << m_number << " does not contain edge " << w << "\n";
      exit(-1);
   }

   make_edges();
}

void tile::dump() const
{
   std::cout << "Tile " << m_number << ":\n";
   for(const auto& a : m_data)
      std::cout << str2(a) << '\n';
   std::cout << '\n';
   /*
   for(const auto& a : m_edges1)
      std::cout << str1(a) << '\n';
   std::cout << '\n';
   for(const auto& a : m_edges2)
      std::cout << str1(a) << '\n';
   std::cout << '\n';
   */
}

bool tile::get(const bits_t& b, size_t n)
{
   unsigned mask = 1 << n;
   return b & mask;
}


void tile::set(bits_t& b, size_t n, bool val)
{
   unsigned mask = 1 << n;
   b &= ~mask;
   if(val)
      b |= mask;
}


std::string tile::str1(bits_t n)
{
   std::string rv;
   rv.reserve(10);
   for(size_t i=0; i < 10; ++i)
   {
      rv.push_back( n & 1 ? '1' : '0');
      n >>= 1;
   }
   return rv;
}
std::string tile::str2(bits_t n)
{
   std::string rv;
   rv.reserve(10);
   for(size_t i=0; i < 10; ++i)
   {
      rv.push_back( n & 1 ? '#' : '.');
      n >>= 1;
   }
   return rv;
}



bool tile::load(std::istream& is)
{
   char ch;
   m_number=0;
   // "Tile xxxx:"
   // read "Tile x"
   while(is.get(ch))
   {
      unsigned val= ch-'0';
      if(val > 9)
         continue;
      m_number = val;
      break;
   }
   // read the rest of the number
   while(is.get(ch))
   {
      unsigned val= ch-'0';
      if(val > 9)    // ':'
         break;
      m_number *= 10;
      m_number += val;
   }
   is.get(ch); // trailing EOL

   // read all the tile data
   for(size_t i=0; i < 10; ++i)
   {
      for(size_t j=0; j < 10; ++j)
      {
         if(!is.get(ch))
            return false;
         if(ch == '.')
            set(m_data[i],j,false);
         else if(ch == '#')
            set(m_data[i],j,true);
         else
            return false;
      }
      is.get(ch); // trailing EOL
   }
   is.get(ch); // trailing EOL

   make_edges();

   return true;
}


void tile::make_edges()
{
   // north
   m_edges1[north] = m_data[0];
   m_edges2[north] = reverse(m_data[0]);
   // south
   m_edges1[south] = reverse(m_data[9]);
   m_edges2[south] = m_data[9];
   // east
   for(size_t i=0; i < 10; ++i)
      set( m_edges1[east], i, get(m_data[i],9) );
   m_edges2[east] = reverse(m_edges1[east]);
   // west
   for(size_t i=0; i < 10; ++i)
      set( m_edges2[west], i, get(m_data[i],0) );
   m_edges1[west] = reverse(m_edges2[west]);
}


tile::bits_t tile::reverse(const bits_t& n)
{
   bits_t rv{0};
   for(size_t i =0; i < 10; ++i)
      set(rv, 9-i, get(n,i));
   return rv;
}


std::vector<tile::bits_t> tile::unique_edges() const
{
   std::vector<bits_t> rv;
   rv.reserve(8);
   for(const auto& a : m_edges1)
      rv.push_back(a);
   for(const auto& a : m_edges2)
      rv.push_back(a);
   std::sort(rv.begin(),rv.end());
   auto i = std::unique(rv.begin(),rv.end());
   rv.erase(i,rv.end());
   return rv;
}


//--------------------------------------------------------

void dump(const tile::list_t& l)
{
   for(const auto& t : l)
      t.dump();
}


tile::list_t load(std::istream& is=std::cin)
{
   tile::list_t rv;
   while(!is.eof())
   {
      tile t;
      if(!t.load())
         break;
      rv.emplace_back(std::move(t));
   }
   std::sort(rv.begin(),rv.end());
   return rv;
}


tile::list_t find_corners(const tile::list_t& tile_list)
{
   tile::list_t rv;

   std::map<tile::bits_t,std::vector<unsigned>> counts;
   for(const auto& t : tile_list)
   {
      auto edge_list = t.unique_edges();
      for(auto edge : edge_list)
         counts[edge].push_back(t.number());
   }

   std::map<unsigned,std::vector<tile::bits_t>> tiles_edges;
   for(const auto& [edge,list] : counts)
   {
      if(list.size() == 1)
      {
         //std::cout << "Tile " << list.front() << " edge " << edge << "\n";
         tiles_edges[list.front()].push_back(edge);
      }
   }

   for(const auto& [tile_number,edge_list] : tiles_edges)
   {
      if(edge_list.size() == 4) // it's a corner!!!
      {
         tile search(tile_number);
         auto i = std::lower_bound(tile_list.begin(), tile_list.end(), search);
         rv.push_back(*i);
      }
   }
   return rv;
}


//----------------------------------------------------------------



int main(int,char**)
{
   auto time_start = std::chrono::high_resolution_clock::now();

   tile::list_t tile_list = load();
   //dump(tile_list);

   std::cout << tile_list.size() << "\n";

   tile::list_t corners = find_corners(tile_list);
   if(corners.size() != 4)
   {
      std::cerr << "Bad corner count: " << corners.size() << "\n";
      return -1;
   }

   uint64_t product = 1;
   for(const auto& a : corners)
      product *= a.number();
   std::cout << product << "\n";


   std::cout << "testing \n";
   tile t = corners[0];
   tile::bits_t e = t.redge(west);
   t.dump();
   t.make_west(e);
   t.dump();



   auto time_end = std::chrono::high_resolution_clock::now();
   std::cout << "\ntime: " << std::chrono::duration_cast<std::chrono::microseconds>(time_end-time_start).count() << " us" << std::endl;
   return 0;
}
