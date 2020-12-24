#include <iostream>
#include <chrono>
#include <algorithm>
#include <stdint.h>
#include <aoc/get.h>            // aoc::get_line()
#include <vector>
#include <map>

//const bool debug=false;

class hex_loc
{
public:
   using self_type=hex_loc;
   using list_t=std::vector<self_type>;

   bool operator<(const self_type& rhs) const;
   bool operator==(const self_type& rhs) const;
   void print(std::ostream& is) const;

   void clear();

   static bool load(self_type& t);
   static list_t load();

   hex_loc east() const;
   hex_loc south_east() const;
   hex_loc south_west() const;
   hex_loc west() const;
   hex_loc north_west() const;
   hex_loc north_east() const;
   list_t adjacent() const;
private:
   int m_x=0;
   int m_y=0;
};
std::ostream& operator<<(std::ostream& os, const hex_loc& o) { o.print(os); return os;}

bool hex_loc::operator<(const self_type& rhs) const
{
   if( m_x != rhs.m_x)
      return m_x < rhs.m_x;
   return m_y < rhs.m_y;
}


bool hex_loc::operator==(const self_type& rhs) const
{
   return m_x == rhs.m_x && m_y == rhs.m_y;
}


hex_loc::list_t hex_loc::adjacent() const
{
   return list_t{ east(), south_east(), south_west(), west(), north_west(), north_east() };
}


void hex_loc::clear()
{
   m_x = 0;
   m_y = 0;
}


hex_loc hex_loc::east() const
{
   hex_loc rv = *this;
   rv.m_x += 2;
   return rv;
}


hex_loc hex_loc::south_east() const
{
   hex_loc rv = *this;
   rv.m_x += 1;
   rv.m_y -= 1;
   return rv;
}


hex_loc hex_loc::south_west() const
{
   hex_loc rv = *this;
   rv.m_x -= 1;
   rv.m_y -= 1;
   return rv;
}


hex_loc hex_loc::west() const
{
   hex_loc rv = *this;
   rv.m_x -= 2;
   return rv;
}


hex_loc hex_loc::north_west() const
{
   hex_loc rv = *this;
   rv.m_x -= 1;
   rv.m_y += 1;
   return rv;
}


hex_loc hex_loc::north_east() const
{
   hex_loc rv = *this;
   rv.m_x += 1;
   rv.m_y += 1;
   return rv;
}



bool hex_loc::load(self_type& rv)
{
   rv.clear();

   std::string line = aoc::get_line();
   if(line.empty())
      return false;

   // e  : ++x, ++x
   // se : ++x, --y
   // sw : --x, --y
   // w  : --x, --x
   // nw : --x, ++y
   // ne : ++x, ++y

   size_t pos=0;
   while(pos < line.size())
   {
      if(line[pos] == 'e')
      {
         rv.m_x += 2;
         ++pos;
      }
      else if(line[pos] == 'w')
      {
         rv.m_x -= 2;
         ++pos;
      }
      else if(pos+1 == line.size())
      {
         std::cerr << "bad input" << std::endl;
         throw -1;
         return false;
      }
      else
      {
         std::string temp = line.substr(pos,2);
         if(temp == "se")
         {
            ++rv.m_x;
            --rv.m_y;
            pos += 2;
         }
         else if(temp == "sw")
         {
            --rv.m_x;
            --rv.m_y;
            pos += 2;
         }
         else if(temp == "nw")
         {
            --rv.m_x;
            ++rv.m_y;
            pos += 2;
         }
         else if(temp == "ne")
         {
            ++rv.m_x;
            ++rv.m_y;
            pos += 2;
         }
         else
         {
            std::cerr << "bad input" << std::endl;
            throw -1;
            return false;
         }
      }
   }
   return true;
}


hex_loc::list_t hex_loc::load()
{
   hex_loc::list_t rv;
   hex_loc temp;
   while(load(temp))
      rv.push_back(temp);
   std::sort(rv.begin(),rv.end());
   return rv;
}


void hex_loc::print(std::ostream& os) const
{
   os << m_x << "," << m_y;
}


void dump(const hex_loc::list_t& l)
{
   for(const auto& a : l)
      std::cout << a << "\n";
}


hex_loc::list_t solve1(const hex_loc::list_t& list)
{
   std::map<hex_loc,size_t> m;
   for(const auto& a : list)
   {
      if(!m.count(a))
         m[a] = 0;
      ++m[a];
   }

   hex_loc::list_t rv;
   for(const auto& [hex,count] : m)
   {
      if(count & 1) // oddnumber is black
         rv.push_back(hex);
   }
   //std::cout << "part1: " << rv.size() << "\n";
   return rv;
}


void flip(hex_loc::list_t& black)
{
   //std::sort(black.begin(),black.end());

   hex_loc::list_t rv;
   rv.reserve(black.size()*3);

   hex_loc::list_t white; // these start as potentially white...
   white.reserve(black.size()*6);

   for(const auto& a : black)
   {
      size_t count=0;
      for(auto b : a.adjacent())
      {
         white.push_back(b);
         if(count < 3)
            count += std::binary_search(black.begin(), black.end(), b);
      }
      if(count == 1 || count ==2)
         rv.push_back(a);
   }

   // clean up the white list
   {
      std::sort(white.begin(),white.end());
      white.erase( std::unique(white.begin(),white.end()), white.end());

      // remove any whites that are realy black
      auto i = black.begin();
      auto j = white.begin();
      while(i != black.end() && j != white.end())
      {
         if(*i == *j)
         {
            ++i;
            j = white.erase(j);
         }
         else if(*i < *j)
            ++i;
         else // if(*j < *i)
            ++j;
      }
   }

   // test white list to see if it should become black
   for(const auto& w : white)
   {
      size_t count=0;
      for(auto b : w.adjacent())
      {
         count += std::binary_search(black.begin(), black.end(), b);
         if(count > 2)
            break;
      }
      if(count == 2)
         rv.push_back(w);
   }

   std::sort(rv.begin(),rv.end());
   rv.swap(black);
}


int main(int,char**)
{
   auto time_start = std::chrono::high_resolution_clock::now();

   hex_loc::list_t list = hex_loc::load();
   //std::sort(list.begin(),list.end());
   //dump(list);
   list = solve1(list);
   for(size_t i = 0; i < 100; ++i)
      flip(list);
   std::cout << list.size() << "\n";

   auto time_end = std::chrono::high_resolution_clock::now();
   std::cout << "\ntime: " << std::chrono::duration_cast<std::chrono::microseconds>(time_end-time_start).count() << " us" << std::endl;
   return 0;
}
