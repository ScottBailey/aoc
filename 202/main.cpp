#include <iostream>
#include <chrono>
#include <algorithm>
#include <stdint.h>
#include <aoc/get.h>            // aoc::get_line()
#include <sb/string.h>          // sb::string::parse(), sb::string::from()
#include <bitset>
#include <map>
#include <assert.h>
#include<math.h>

//const bool debug=false;

enum facing {
   north = 0,
   east = 1,
   south = 2,
   west = 3,

   size = 4,

   rnorth = 10,
   reast = 11,
   rsouth = 12,
   rwest = 13,

   bad = 100,
};

class tile
{
public:
   //using map_t = std::map<unsigned,tile>;
   using list_t = std::vector<tile>;
   using mosaic_t = std::vector<std::vector<tile>>;

   using edge_t = uint16_t;

   tile() : m_number{0}, m_data{0}, m_edges1{0}, m_edges2{0} {}
   tile(unsigned number) : m_number{number}, m_data{0}, m_edges1{0}, m_edges2{0} {}

   // tile number
   unsigned number() const { return m_number; }


   void make_west(const edge_t& w);

   edge_t edge(facing f) const { return m_edges1[f]; }
   edge_t redge(facing f) const { return m_edges2[f]; }

   facing edge(const edge_t& b) const;
   bool has_front(const edge_t& b) const { return edge(b) < facing::size; }
   bool has(const edge_t& b) const { return edge(b) < facing::bad; }


   bool load(std::istream& is=std::cin);
   void dump() const;

   // all unique edges, forward and reverse
   std::vector<edge_t> unique_edges() const;

   bool operator==(const tile& rhs) const {return m_number == rhs.m_number; }
   bool operator<(const tile& rhs) const {return m_number < rhs.m_number; }

private:

   void flip_l2r();   // flip left to right
   void flip_t2b();   // flip top tobottom
   void rotate();     // rotate 90 degree clockwise

   void make_edges();  // populate m_edges1 and m_edges2 from m_data

   static edge_t reverse(const edge_t& n);
   static bool get(const edge_t& b, size_t n);
   static void set(edge_t& b, size_t n, bool val);
   static std::string str1(edge_t n);
   static std::string str2(edge_t n);
private:
   unsigned m_number;
   std::array<edge_t,10> m_data;

   std::array<edge_t,facing::size> m_edges1;
   std::array<edge_t,facing::size> m_edges2;
};


facing tile::edge(const edge_t& b) const
{
   for(size_t i=0; i < facing::size; ++i)
   {
      if(b == m_edges1[i])
         return facing(i);
      if(b == m_edges2[i])
         return facing(i + rnorth);
   }
   return facing::bad;
}


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
   std::array<edge_t,10> orig{m_data};

   for(size_t i = 0; i < 10; ++i) // these are, essentially, the columns when rotate 90 deg clockwise (col = 9-i)
   {
      size_t column = 9-i; // cloumnin new orientation
      for(size_t j = 0; j < 10; ++j) // row in new orientation
         set(m_data[j], column, get(orig[i],j));
   }

}



void tile::make_west(const edge_t& w)
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

bool tile::get(const edge_t& b, size_t n)
{
   unsigned mask = 1 << n;
   return b & mask;
}


void tile::set(edge_t& b, size_t n, bool val)
{
   unsigned mask = 1 << n;
   b &= ~mask;
   if(val)
      b |= mask;
}


std::string tile::str1(edge_t n)
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
std::string tile::str2(edge_t n)
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


tile::edge_t tile::reverse(const edge_t& n)
{
   edge_t rv{0};
   for(size_t i =0; i < 10; ++i)
      set(rv, 9-i, get(n,i));
   return rv;
}


std::vector<tile::edge_t> tile::unique_edges() const
{
   std::vector<edge_t> rv;
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


void dump(const tile::mosaic_t& mosaic)
{
   for(const auto& r : mosaic)
   {
      for(const auto& t : r)
         t.dump();
      std::cout << "\n";
   }
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


// return 4 corners rotated so the border is to the north west
tile::list_t find_corners(const tile::list_t& tile_list)
{
   tile::list_t rv;

   std::map<tile::edge_t,std::vector<unsigned>> counts;
   for(const auto& t : tile_list)
   {
      auto edge_list = t.unique_edges();
      for(auto edge : edge_list)
         counts[edge].push_back(t.number());
   }

   std::map<unsigned,std::vector<tile::edge_t>> tiles_edges;
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
         auto t = *std::lower_bound(tile_list.begin(), tile_list.end(), search); // no sanity check dangerous!


         // get 2 corners on the tile in the same facing
         std::vector<tile::edge_t> temp;
         for(auto e : edge_list)
         {
            if(t.has_front(e))
               temp.push_back(e);
         }
         assert(temp.size() == 2);
         facing f1 = t.edge(temp.front());
         facing f2 = t.edge(temp.back());
         if((f1 < f2) && ((f2 != west) && (f1 != north)))
            t.make_west(temp.front());
         else
            t.make_west(temp.back());
         rv.push_back(t);
      }
   }
   assert(corners.size() == 4);
   return rv;
}


tile::list_t find_border(const tile::list_t& tile_list)  // return all borders with the edge to the west
{
   tile::list_t rv;

   std::map<tile::edge_t,std::vector<unsigned>> counts;
   for(const auto& t : tile_list)
   {
      auto edge_list = t.unique_edges();
      for(auto edge : edge_list)
         counts[edge].push_back(t.number());
   }

   std::map<unsigned,std::vector<tile::edge_t>> tiles_edges;
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
      if(edge_list.size() == 2) // it's an edge!!!
      {
         tile search(tile_number);
         auto t = *std::lower_bound(tile_list.begin(), tile_list.end(), search);
         t.make_west(edge_list.front());
         rv.push_back(t);
      }
   }
   return rv;
}


inline void remove(tile::list_t& list, const tile& t)
{
   auto i = std::lower_bound(list.begin(),list.end(),t);
   list.erase(i);
}


void build_mosaic(tile::list_t list)
{
   size_t sz = static_cast<size_t>(sqrt(list.size()));

   // populate the empty mosaic
   tile::mosaic_t mosaic;
   for(size_t i = 0; i < sz; ++i)
      mosaic.emplace_back(std::vector<tile>(sz));

   // get all the border
   tile::list_t corners = find_corners(list);
   tile::list_t border = find_border(list);

   // remove the corners and border from the list
   for(auto& a : corners)
      remove(list,a);
   for(auto& a : border)
      remove(list,a);

   // select a single corner and put it in the nw corner of the mosaic removing it from the corner list
   mosaic[0][0] = corners.back();
   corners.pop_back();

   // fill the north border
   for(size_t c=1; c < sz-1; ++c) // c is for column
   {
      const tile& left = mosaic[0][c-1];
      tile::edge_t search_edge = left.redge(east);
      auto i = std::find_if(border.begin(), border.end(), [search_edge](const tile& t){ return t.has(search_edge); });
      i->make_west(search_edge);
      mosaic[0][c] = *i;
      border.erase(i);
   }
   dump(mosaic);


}


//----------------------------------------------------------------

int main(int,char**)
{
   auto time_start = std::chrono::high_resolution_clock::now();

   tile::list_t tile_list = load();
   //dump(tile_list);

   std::cout << tile_list.size() << "\n";

   build_mosaic(tile_list);


   auto time_end = std::chrono::high_resolution_clock::now();
   std::cout << "\ntime: " << std::chrono::duration_cast<std::chrono::microseconds>(time_end-time_start).count() << " us" << std::endl;
   return 0;
}
