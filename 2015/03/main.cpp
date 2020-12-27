#include <iostream>
#include <chrono>
#include <algorithm>
#include <stdint.h>
#include <aoc/get.h>            // aoc::get_line()
#include <sb/string.h>          // sb::string::parse(), sb::string::from()
#include <sb/ansi.h>            // ansi colors: sb::red, sb::yellow, etc
#include <sb/ignore.h>          // sb::string::parse(), sb::string::from()
#include <vector>
#include <numeric>

//const bool debug=false;

struct move
{
   long x;
   long y;
};


struct location
{
   long x=0;
   long y=0;

   location& operator+=(const move& rhs)
   {
      x += rhs.x;
      y += rhs.y;
      return *this;
   }

   bool operator<(const location& rhs) {
      if(x < rhs.x)
         return true;
      if(x > rhs.x)
         return false;
      return y < rhs.y;
   }
   bool operator==(const location& rhs) { return x == rhs.x && y == rhs.y; }
};
std::ostream& operator<<(std::ostream& os, const location& l) { os << l.x << "," << l.y; return os; }


using list_t = std::vector<move>;


list_t load(std::istream& is=std::cin)
{
   list_t rv;
   rv.reserve(9000);

   char ch;
   while(is.get(ch))
   {
      switch(ch)
      {
         case '^': rv.emplace_back(move{0,-1}); break;
         case 'v': rv.emplace_back(move{0,1}); break;
         case '>': rv.emplace_back(move{1,0}); break;
         case '<': rv.emplace_back(move{-1,0}); break;

         case '\n': break;

         default:
            throw std::invalid_argument("bad input");
            break;
      }
   }
   return rv;
}


void part1(const list_t& list)
{
   location l;

   std::vector<location> locs;
   locs.reserve(list.size()+1);
   locs.push_back(l);

   for(auto& a : list)
   {
      l += a;
      locs.push_back(l);
   }

   std::sort(locs.begin(),locs.end());
   locs.erase(std::unique(locs.begin(),locs.end()),locs.end());

   int result = locs.size();
   std::cout << sb::white << "Part 1: " << sb::reset << result << "\n";
}


void part2(const list_t& list)
{
   std::array<location,2> l;

   std::vector<location> locs;
   locs.reserve(list.size()+1);

   locs.push_back(l[0]);

   for(size_t i=0; i < list.size(); ++i)
   {
      size_t index = i & 1; // 0 or 1
      l[index] += list[i];
      locs.push_back(l[index]);
   }

   std::sort(locs.begin(),locs.end());
   locs.erase(std::unique(locs.begin(),locs.end()),locs.end());

   int result = locs.size();
   std::cout << sb::white << "Part 2: " << sb::reset << result << "\n";
}


int main(int,char**)
{
   auto time0 = std::chrono::high_resolution_clock::now();

   list_t list = load();

   auto time1 = std::chrono::high_resolution_clock::now();

   part1(list);

   auto time2  = std::chrono::high_resolution_clock::now();

   part2(list);

   auto time3  = std::chrono::high_resolution_clock::now();


   std::cout
      << sb::white
      << "\nload time:  " << std::chrono::duration_cast<std::chrono::microseconds>(time1-time0).count() << " us\n"
      << "part1 time: " << std::chrono::duration_cast<std::chrono::microseconds>(time2-time1).count() << " us\n"
      << "part2 time: " << std::chrono::duration_cast<std::chrono::microseconds>(time3-time2).count() << " us\n"
      << std::flush
      ;
   return 0;
}
