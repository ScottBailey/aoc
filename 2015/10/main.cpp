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

using container_t = std::string;

/*
std::ostream& operator<<(std::ostream& os, const container_t& list)
{
   sb::ignore(list);
   return os;
}
*/

container_t load(std::istream& is=std::cin)
{
   container_t rv;
   is >> rv;
   return rv;
}


std::string look_and_say(const std::string& in)
{
   std::string rv;
   rv.reserve(in.size()*2);

   char last = in[0];
   size_t count = 1;
   for(size_t i=1; i < in.size(); ++i)
   {
      if(in[i] == last)
      {
         ++count;
         continue;
      }
      if(count > 9)
         throw -1;
      rv.push_back('0'+count);
      rv.push_back(last);
      last = in[i];
      count = 1;
   }
   rv.push_back('0'+count);
   rv.push_back(last);

   return rv;
}


void part1(const container_t& list)
{
   container_t cur = list;
   for(size_t i = 0; i < 40; ++i)
      cur = look_and_say(cur);

   unsigned result = cur.size();
   std::cout << sb::white << "Part 1: " << sb::reset << result << "\n";
}


void part2(const container_t& list)
{
   container_t cur = list;
   for(size_t i = 0; i < 50; ++i)
      cur = look_and_say(cur);

   uint64_t result = cur.size();
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
