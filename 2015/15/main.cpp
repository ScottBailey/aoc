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
#include <cassert>

//const bool debug=false;
struct entry_t
{
   std::string name;
   int64_t capacity=0;
   int64_t durability=0;
   int64_t flavor=0;
   int64_t texture=0;
   int64_t calories=0;
};

using container_t = std::vector<entry_t>;

std::ostream& operator<<(std::ostream& os, const container_t& list)
{
   for(const auto& a : list)
      os
         << a.name << ": capacity " << a.capacity
         << ", durability " << a.durability
         << ", flavor " << a.flavor
         << ", texture " << a.texture
         << ", calories " << a.calories
         << "\n"
         ;
   return os;
}


container_t load(std::istream& is=std::cin)
{
   container_t rv;

   // Frosting: capacity 4, durability -2, flavor 0, texture 0, calories 5
   std::string temp;
   while(is >> temp)
   {
      entry_t e;
      temp.pop_back();
      e.name = temp;
      if( !(is >> temp) || !(is >> e.capacity)
            || !(is >> temp) || !(is >> temp) || !(is >> e.durability)
            || !(is >> temp) || !(is >> temp) || !(is >> e.flavor)
            || !(is >> temp) || !(is >> temp) || !(is >> e.texture)
            || !(is >> temp) || !(is >> temp) || !(is >> e.calories) )
         throw std::invalid_argument("bad input");
      rv.push_back(e);
   }
   return rv;
}


uint64_t calculate1(const std::vector<unsigned>& n, const container_t& list)
{
   assert(n.size() == list.size());

   int64_t c = 0;
   int64_t d = 0;
   int64_t f = 0;
   int64_t t = 0;

   for(size_t i=0; i < n.size(); ++i)
   {
      c += list[i].capacity * n[i];
      d += list[i].durability * n[i];
      f += list[i].flavor * n[i];
      t += list[i].texture * n[i];
   }
   if(c < 0 || d < 0 || f < 0 || t < 0)
      return 0;

   return static_cast<uint64_t>(c) * static_cast<uint64_t>(d) * static_cast<uint64_t>(f) * static_cast<uint64_t>(t);
}


uint64_t calculate2(const std::vector<unsigned>& n, const container_t& list, bool debug= false)
{
   assert(n.size() == list.size());

   int64_t c = 0;
   int64_t d = 0;
   int64_t f = 0;
   int64_t t = 0;
   int64_t cal = 0;

   for(size_t i=0; i < n.size(); ++i)
   {
      c += list[i].capacity * n[i];
      d += list[i].durability * n[i];
      f += list[i].flavor * n[i];
      t += list[i].texture * n[i];
      cal += list[i].calories * n[i];
      if(debug)
      {
         std::cout << list[i].capacity * n[i] << "   " << list[i].durability * n[i]
                   << "   " << list[i].flavor * n[i] << "   " << list[i].texture * n[i] << "\n";
      }
   }
   if(debug)
   {
      std::cout << c << " *  " << d << " *  " << f << " *  " << t << " = "
                << static_cast<uint64_t>(c) * static_cast<uint64_t>(d) * static_cast<uint64_t>(f) * static_cast<uint64_t>(t) << "\n";
   }

   if(c < 0 || d < 0 || f < 0 || t < 0 || cal != 500)
      return 0;

   return static_cast<uint64_t>(c) * static_cast<uint64_t>(d) * static_cast<uint64_t>(f) * static_cast<uint64_t>(t);
}


void part1(const container_t& list)
{
   uint64_t best = 0;
   for(unsigned a=0; a <= 100; ++a)
      for(unsigned b=0; a+b <= 100; ++b)
         for(unsigned c=0; a+b+c <= 100; ++c)
         {
            std::vector<unsigned> n{a,b,c,100-(a+b+c)};
            uint64_t temp = calculate1(n, list);
            if(temp > best)
               best = temp;
         }
   std::cout << sb::white << "Part 1: " << sb::reset << best << "\n";
}


void part2(const container_t& list)
{
   uint64_t best = 0;
   for(unsigned a=0; a <= 100; ++a)
      for(unsigned b=0; a+b <= 100; ++b)
         for(unsigned c=0; a+b+c <= 100; ++c)
         {
            std::vector<unsigned> n{a,b,c,100-(a+b+c)};
            uint64_t temp = calculate2(n, list);
            if(temp > best)
               best = temp;
         }
   std::cout << sb::white << "Part 2: " << sb::reset << best << "\n";
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
