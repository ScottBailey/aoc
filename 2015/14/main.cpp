#include <iostream>
#include <chrono>
#include <algorithm>
#include <stdint.h>
#include <aoc/get.h>            // aoc::get_line()
#include <sb/string.h>          // sb::string::parse(), sb::string::from()
#include <sb/ansi.h>            // ansi colors: sb::red, sb::yellow, etc
#include <sb/ignore.h>          // sb::string::parse(), sb::string::from()
#include <vector>
#include <map>
#include <numeric>

//const bool debug=false;

struct entry_t
{
   std::string name;
   uint64_t speed=0;
   uint64_t work=0;
   uint64_t rest=0;
};

using container_t = std::vector<entry_t>;

std::ostream& operator<<(std::ostream& os, const container_t& list)
{
   for(const auto& a : list)
      os << a.name << " can fly " << a.speed << " km/s for " << a.work << " seconds, but then must rest for " << a.rest << " seconds.\n";
   return os;
}


container_t load(std::istream& is=std::cin)
{
   container_t rv;

   // Rudolph can fly 22 km/s for 8 seconds, but then must rest for 165 seconds.
   std::string temp;
   while(is >> temp)
   {
      entry_t e;
      e.name = temp;
      //       can             fly              22                  km/s            for              8                 seconds,
      if( !(is >> temp) || !(is >> temp) || !(is >> e.speed) || !(is >> temp) || !(is >> temp)  || !(is >> e.work)  || !(is >> temp)
            // but              then             must             rest             for           165                 seconds.
            || !(is >> temp) || !(is >> temp) || !(is >> temp) || !(is >> temp) || !(is >> temp) || !(is >> e.rest) || !(is >> temp))
         throw std::invalid_argument("bad input");
      rv.push_back(e);
   }
   return rv;
}


void part1(const container_t& list)
{
   uint64_t result = 0;
   uint64_t time = 2503;
   for(const entry_t& e : list)
   {
      uint64_t dist = (e.speed*e.work) * (time / (e.work + e.rest));
      dist += e.speed * std::min(e.work, (time % (e.work + e.rest)));
      if(dist > result)
         result = dist;
   }

   std::cout << sb::white << "Part 1: " << sb::reset << result << "\n";
}


void part2(const container_t& list)
{
   std::map<std::string,unsigned> points;
   for(const entry_t& e : list)
      points[e.name] = 0;

   for(size_t time=1; time <= 2503; ++time)
   {
      uint64_t best = 0;
      std::vector<std::string> who;
      for(const entry_t& e : list)
      {
         uint64_t dist = (e.speed*e.work) * (time / (e.work + e.rest));
         dist += e.speed * std::min(e.work, (time % (e.work + e.rest)));
         if(dist == best)
            who.push_back(e.name);
         else if(dist > best)
         {
            best = dist;
            who.clear();
            who.push_back(e.name);
         }
      }
      for(const std::string& name : who)
         ++points[name];
   }

   uint64_t result = 0;
   for(const auto& [name, val] : points)
      if(val > result)
         result = val;

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
