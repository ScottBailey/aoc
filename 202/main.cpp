#include <iostream>
#include <chrono>
#include <algorithm>
#include <stdint.h>
#include <aoc/get.h>            // aoc::get_line()
#include <sb/string.h>          // sb::string::parse(), sb::string::from()
#include <bitset>
#include <map>
#include <assert.h>
#include <math.h>

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
   bool valid() const { return m_number > 0; }

   void make_west(const edge_t& w);
   void make_north(const edge_t& w);

   edge_t edge(facing f) const { return m_edges1[f]; }
   edge_t redge(facing f) const { return m_edges2[f]; }

   facing edge(const edge_t& b) const;
   bool has_front(const edge_t& b) const { return edge(b) < facing::size; }
   bool has(const edge_t& b) const { return edge(b) < facing::bad; }


   bool load(std::istream& is=std::cin);
   void dump() const;

   // all unique edges, forward and reverse
   std::vector<edge_t> unique_edges() const;


   void border(edge_t e1) { m_border.clear(); m_border.push_back(e1); }
   void border(edge_t e1, edge_t e2) { m_border.clear(); m_border.push_back(e1); m_border.push_back(e2); }

   bool value(size_t r, size_t y) const;  // get the row column value of the internal image, r < c, c < 8; borders ignored

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

   std::vector<edge_t> m_border; // debugging tool
};


bool tile::value(size_t r, size_t c) const
{
   edge_t n = m_data[r+1];
   n >>= (c+1);
   return n&1;
}


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
      throw -1;
      exit(-1);
   }

   make_edges();
}


void tile::make_north(const edge_t& n)
{
   // this is atemporary, but bad hack
   make_west(n);
   rotate();
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
      std::cout << "\n\n";
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
         if((f1 < f2) && !((f2 == west) && (f1 == north)))
         {
            t.make_west(temp.front());
            t.border(temp.front(),temp.back());
         }
         else
         {
            t.make_west(temp.back());
            t.border(temp.back(),temp.front());
         }
         rv.push_back(t);
      }
   }
   assert(rv.size() == 4);
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
         t.border(edge_list.front());
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


void validate_mosaic(const tile::mosaic_t& m)
{
   for(size_t r=0; r < m.size(); ++r)
   {
      for(size_t c=0; c < m.size(); ++c)
      {
         if(!m[r][c].valid())
         {
            std::cerr << "invalid " << r << "," << c << "\n";
            throw -1;
         }

         if(c+1 < m.size() && m[r][c].edge(east) != m[r][c+1].redge(west))
         {
            std::cerr << "mismatch right " << r << "," << c << "\n";
            throw -1;
         }
         if(r+1 < m.size() && m[r][c].edge(south) != m[r+1][c].redge(north))
         {
            std::cerr << "mismatch below " << r << "," << c << "\n";
            throw -1;
         }
      }
   }
}


tile::mosaic_t build_mosaic(tile::list_t list)
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
      tile::edge_t search_edge = mosaic[0][c-1].redge(east);
      auto i = std::find_if(border.begin(), border.end(),
            [search_edge](const tile& t){return t.edge(south) == search_edge || t.redge(north) == search_edge;} );

      if(i == border.end())
      {
         i = std::find_if(border.begin(), border.end(),
               [search_edge](const tile& t){return t.has(search_edge); } );
         throw -1;
      }

      i->make_west(search_edge);
      mosaic[0][c] = *i;
      border.erase(i);
   }

   // populate the north west corner
   {
      tile::edge_t search_edge = mosaic[0][sz-2].redge(east);
      auto i = std::find_if(corners.begin(), corners.end(), [search_edge](const tile& t){ return t.has(search_edge); });
      i->make_west(search_edge);
      mosaic[0][sz-1] = *i;
      corners.erase(i);
   }

   // now populate the western border
   for(size_t r=1; r < sz-1; ++r) // r is for row
   {
      tile::edge_t search_edge = mosaic[r-1][0].redge(south);
      auto i = std::find_if(border.begin(), border.end(),
            [search_edge](const tile& t){return t.edge(north) == search_edge || t.redge(south) == search_edge;} );
      i->make_north(search_edge);
      mosaic[r][0] = *i;
      border.erase(i);
   }

   // now populate the eastern border
   for(size_t r=1; r < sz-1; ++r) // r is for row
   {
      tile::edge_t search_edge = mosaic[r-1][sz-1].redge(south);
      auto i = std::find_if(border.begin(), border.end(),
            [search_edge](const tile& t){return t.edge(south) == search_edge || t.redge(north) == search_edge;} );
      i->make_north(search_edge);
      mosaic[r][sz-1] = *i;
      border.erase(i);
   }

   // populate the south west corner
   {
      tile::edge_t search_edge = mosaic[sz-2][0].redge(south);
      auto i = std::find_if(corners.begin(), corners.end(), [search_edge](const tile& t){ return t.has(search_edge); });
      i->make_north(search_edge);
      mosaic[sz-1][0] = *i;
      corners.erase(i);
   }

   // populate the south east corner
   {
      tile::edge_t search_edge = mosaic[sz-2][sz-1].redge(south); // although there is only one, it still needs to be rotated
      corners.front().make_north(search_edge);
      mosaic[sz-1][sz-1] = corners.front();
      corners.clear(); // there's only the one...
   }

   // populate the south border
   for(size_t c=1; c < sz-1; ++c) // c is for column
   {
      tile::edge_t search_edge = mosaic[sz-1][c-1].redge(east);
      auto i = std::find_if(border.begin(), border.end(),
            [search_edge](const tile& t){return t.edge(north) == search_edge || t.redge(south) == search_edge;} );
      i->make_west(search_edge);
      mosaic[sz-1][c] = *i;
      border.erase(i);
   }


   // populate the inside!
   for(size_t r=1; r+1 < mosaic.size(); ++r)
   {
      for(size_t c=1; c+1 < mosaic.size(); ++c)
      {
         tile::edge_t search_above = mosaic[r-1][c].redge(south);
         tile::edge_t search_left = mosaic[r][c-1].redge(east);
         // this doesnt test the values are in the right places... might be an issue later.
         auto i = std::find_if(list.begin(), list.end(),
               [search_above,search_left](const tile& t) { return t.has(search_above) && t.has(search_left); } );
         if(i == list.end())
         {
             std::cerr << "failed to find tile for: " << r << "," << c << "\n";
             throw -1;
         }
         i->make_west(search_left);
         mosaic[r][c] = *i;
         list.erase(i);
      }
   }

   //validate_mosaic(mosaic);

   return mosaic;
}


std::vector<std::vector<bool>> build_image(const tile::mosaic_t& m)
{
   std::vector<std::vector<bool>> rv;
   for(size_t i= 0; i < m.size()*8; ++i)
      rv.emplace_back( std::vector<bool>(m.size()*8, false) );

   for(size_t i=0; i < m.size(); ++i)
   {
      for(size_t j=0; j < m.size(); ++j)
      {
         const tile& t = m[i][j];
         for(size_t r=0; r < 8; ++r)
         {
            for(size_t c=0; c < 8; ++c)
               rv[i*8+r][j*8+c] = t.value(r,c);
         }
      }
   }

   return rv;
}


inline void max_a(size_t& l, const size_t& r)
{
   if(l < r)
      l = r;
}


size_t count(const std::vector<std::vector<bool>>& img)
{
   //            11111111111
   //   01234567890123456789
   //
   // 0                   #  2
   // 1 #    ##    ##    ### 1
   // 2  #  #  #  #  #  #    0
   //
   //   98765432109876543210
   //   1111111111

   size_t right_normal{0};
   size_t right_inverted{0};
   size_t left_normal{0};
   size_t left_inverted{0};
   size_t down_normal{0};
   size_t down_inverted{0};
   size_t up_normal{0};
   size_t up_inverted{0};

   for(size_t r=0; r < img.size(); ++r)
   {
      for(size_t c=0; c < img.size(); ++c)
      {
         size_t remain_r = img.size() - r;
         size_t remain_c = img.size() - c;

         // right left
         if((remain_c >= 20) && (remain_r >= 3))
         {
            // center line - right
            if( img[r+1][c]
                  && img[r+1][c+5] && img[r+1][c+6]
                  && img[r+1][c+11] && img[r+1][c+12]
                  && img[r+1][c+17] && img[r+1][c+18] && img[r+1][c+19] )
            {
               // top and bottom - normal
               if(img[r][c+18]
                     && img[r+2][c+1] && img[r+2][c+4] && img[r+2][c+7] && img[r+2][c+10] && img[r+2][c+13] && img[r+2][c+16])
               {
                  ++right_normal;
               }

               // top and bottom - inverted
               if(img[r+2][c+18]
                     && img[r][c+1] && img[r][c+4] && img[r][c+7] && img[r][c+10] && img[r][c+13] && img[r][c+16])
               {
                  ++right_inverted;
               }

            }
            // center line - left
            if( img[r+1][c] && img[r+1][c+1] && img[r+1][c+2]
                  && img[r+1][c+7] && img[r+1][c+8]
                  && img[r+1][c+13] && img[r+1][c+14]
                  && img[r+1][c+19] )
            {
               // top and bottom - normal
               if(img[r][c+1]
                     && img[r+2][c+3] && img[r+2][c+6] && img[r+2][c+9] && img[r+2][c+12] && img[r+2][c+15] && img[r+2][c+18])
               {
                  ++left_normal;
               }
               // top and bottom - inverted
               if(img[r+2][c+1]
                     && img[r][c+3] && img[r][c+6] && img[r][c+9] && img[r][c+12] && img[r][c+15] && img[r][c+18])
               {
                  ++left_inverted;
               }

            }
         }

         // up down
         if((remain_c >= 3) && (remain_r >= 20))
         {
            // center line - down
            if( img[r][c+1]
                  && img[r+5][c+1] && img[r+6][c+1]
                  && img[r+11][c+1] && img[r+12][c+1]
                  && img[r+17][c+1] && img[r+18][c+1] && img[r+19][c+1] )
            {

               // top and bottom - normal
               if(img[r+18][c]
                     && img[r+1][c+2] && img[r+4][c+2] && img[r+7][c+2] && img[r+10][c+2] && img[r+13][c+2] && img[r+16][c+2])
               {
                  ++down_normal;
               }
               // top and bottom - inverted
               if(img[r+18][c+2]
                     && img[r+1][c] && img[r+4][c] && img[r+7][c] && img[r+10][c] && img[r+13][c] && img[r+16][c])
               {
                  ++down_inverted;
               }

            }

            // center line - up
            if( img[r][c+1] && img[r+1][c+1] && img[r+2][c+1]
                  && img[r+7][c+1] && img[r+8][c+1]
                  && img[r+13][c+1] && img[r+14][c+1]
                  && img[r+19][c+1] )
            {
               // top and bottom - normal
               if(img[r+1][c]
                     && img[r+3][c+2] && img[r+6][c+2] && img[r+9][c+2] && img[r+12][c+2] && img[r+15][c+2] && img[r+18][c+2])
               {
                  ++up_normal;
               }
               // top and bottom - inverted
               if(img[r+1][c+2]
                     && img[r+3][c] && img[r+6][c] && img[r+9][c] && img[r+12][c] && img[r+15][c] && img[r+18][c])
               {
                  ++up_inverted;
               }

            }
         }
      }
   }


   if(0)
   {
      std::cout
         << "right_normal " << right_normal << "\n"
         << "right_inverted " << right_inverted << "\n"
         << "left_normal " << left_normal << "\n"
         << "left_inverted " << left_inverted << "\n"
         << "down_normal " << down_normal << "\n"
         << "down_inverted " << down_inverted << "\n"
         << "up_normal " << up_normal << "\n"
         << "up_inverted " << up_inverted << "\n"
         ;
   }

   size_t sum = 0;
   max_a(sum,right_normal);
   max_a(sum,right_inverted);
   max_a(sum,left_normal);
   max_a(sum,left_inverted);
   max_a(sum,down_normal);
   max_a(sum,down_inverted);
   max_a(sum,up_normal);
   max_a(sum,up_inverted);
   return sum;
}


void dump(const std::vector<std::vector<bool>>& img)
{
   for(const auto& r : img)
   {
      for(const auto& c : r)
         std::cout << (c ? "#" : ".");
      std::cout << "\n";
   }
}





//----------------------------------------------------------------

int main(int,char**)
{
   auto time_start = std::chrono::high_resolution_clock::now();

   tile::list_t tile_list = load();
   //dump(tile_list);

   tile::mosaic_t tile_mosaic = build_mosaic(tile_list);
   auto img = build_image(tile_mosaic);
   size_t sum = count(img);

   size_t waves = 0;
   // std::vector<std::vector<bool>>& img
   for(const auto& r : img)
   {
      for(const auto& c : r)
         waves += c;
   }
   waves -= (sum * 15);
   std::cout << waves << "\n";

   auto time_end = std::chrono::high_resolution_clock::now();
   std::cout << "\ntime: " << std::chrono::duration_cast<std::chrono::microseconds>(time_end-time_start).count() << " us" << std::endl;
   return 0;
}
