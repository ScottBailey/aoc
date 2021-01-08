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
#include <map>

//const bool debug=false;

using map_t = std::map<std::string,unsigned>;
using container_t = std::map<unsigned,map_t>;

const map_t ticker_tape_sue{
   {"children", 3},
   {"cats", 7},
   {"samoyeds", 2},
   {"pomeranians", 3},
   {"akitas", 0},
   {"vizslas", 0},
   {"goldfish", 5},
   {"trees", 3},
   {"cars", 2},
   {"perfumes", 1}
};


std::ostream& operator<<(std::ostream& os, const container_t& list)
{
   for(const auto& [name,map] : list)
   {
      os << name << ":\n";
      for(const auto& [tag,count] : map)
         os << "   " << tag << ": " << count << "\n";
   }
   return os;
}


container_t load(std::istream& is=std::cin)
{
   container_t rv;
   for(std::string line = aoc::get_line(is); line.size(); line = aoc::get_line(is))
   {
      auto strs = sb::string::parse<std::vector<std::string>>(line, ", :");
      if(strs.size() < 4)
         throw std::invalid_argument("bad input");
      unsigned index = 0;
      if(!sb::string::from(strs[1],index) || !index)
         throw std::invalid_argument("bad input");
      map_t& m = rv[index];
      for(size_t i=3; i < strs.size(); i+=2)
      {
         unsigned count = std::numeric_limits<unsigned>::max();
         if(!sb::string::from(strs[i],count) || (count == std::numeric_limits<unsigned>::max()))
            throw std::invalid_argument("bad input");
         m[strs[i-1]]=count;
      }
   }
   return rv;
}


void part1(container_t list)
{
   for(const auto& [tag,count] : ticker_tape_sue)
   {
      container_t::iterator i=list.begin();
      while(i != list.end())
      {
         const map_t& m = i->second;
         // skip empty tags and those with matching counts
         if(!m.count(tag) || (m.at(tag) == count))
            ++i;
         else
            i = list.erase(i); // remove mismatches
      }
   }

   unsigned result = std::numeric_limits<unsigned>::max();
   if(list.size() == 1)
      result = list.begin()->first;

   std::cout << sb::white << "Part 1: " << sb::reset << result << "\n";
}


void part2(container_t list)
{
   for(const auto& [tag,count] : ticker_tape_sue)
   {
      if(tag == "cats" || tag == "trees")
      {
         container_t::iterator i=list.begin();
         while(i != list.end())
         {
            const map_t& m = i->second;
            // skip empty tags and those with matching counts
            if(!m.count(tag) || (m.at(tag) > count))
               ++i;
            else
               i = list.erase(i); // remove mismatches
         }
      }
      else if(tag == "pomeranians" || tag == "goldfish")
      {
         container_t::iterator i=list.begin();
         while(i != list.end())
         {
            const map_t& m = i->second;
            // skip empty tags and those with matching counts
            if(!m.count(tag) || (m.at(tag) < count))
               ++i;
            else
               i = list.erase(i); // remove mismatches
         }
      }
      else
      {
         container_t::iterator i=list.begin();
         while(i != list.end())
         {
            const map_t& m = i->second;
            // skip empty tags and those with matching counts
            if(!m.count(tag) || (m.at(tag) == count))
               ++i;
            else
               i = list.erase(i); // remove mismatches
         }
      }

   }

   unsigned result = std::numeric_limits<unsigned>::max();
   if(list.size() == 1)
      result = list.begin()->first;

   std::cout << sb::white << "Part 2: " << sb::reset << result << "\n";
}


int main(int,char**)
{
   auto time0 = std::chrono::high_resolution_clock::now();

   container_t list = load();

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
