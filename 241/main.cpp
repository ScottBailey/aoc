#include <iostream>
#include <chrono>
#include <algorithm>
#include <stdint.h>
#include <aoc/get.h>            // aoc::get_line()
#include <sb/string.h>          // sb::string::parse(), sb::string::from()
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


void hex_loc::clear()
{
   m_x = 0;
   m_y = 0;
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


void solve1(const hex_loc::list_t& list)
{
   std::map<hex_loc,size_t> m;
   for(const auto& a : list)
   {
      if(!m.count(a))
         m[a] = 0;
      ++m[a];
   }

   size_t black=0;
   for(const auto& [hex,count] : m)
      black += (count & 1); // oddnumber is black

   std::cout << "part1: " <<black << "\n";
}


int main(int,char**)
{
   auto time_start = std::chrono::high_resolution_clock::now();

   hex_loc::list_t list = hex_loc::load();
   //std::sort(list.begin(),list.end());
   //dump(list);
   solve1(list);

   auto time_end = std::chrono::high_resolution_clock::now();
   std::cout << "\ntime: " << std::chrono::duration_cast<std::chrono::microseconds>(time_end-time_start).count() << " us" << std::endl;
   return 0;
}
