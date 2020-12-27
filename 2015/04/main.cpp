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
#include <sb/md5.h> // md5::sum()

//const bool debug=false;

std::string load(std::istream& is=std::cin)
{
   return aoc::get_line(is);
}


void part1(const std::string& s)
{
   unsigned i=0;
   for(;;)
   {
      std::string secret;
      secret.reserve(16);
      secret += s;
      secret += std::to_string(i);
      sb::md5::hash_t h = sb::md5::sum(secret);
      if(!h[0] && !h[1] && (h[2] < 0x10))
         break;
      ++i;
   }
   std::cout << sb::white << "Part 1: " << sb::reset << i << "\n";
}


void part2(const std::string& s)
{
   unsigned i=0;
   for(;;)
   {
      std::string secret;
      secret.reserve(16);
      secret += s;
      secret += std::to_string(i);
      sb::md5::hash_t h = sb::md5::sum(secret);
      if(!h[0] && !h[1] && !h[2])
         break;
      ++i;
   }
   std::cout << sb::white << "Part 2: " << sb::reset << i << "\n";
}


int main(int,char**)
{
   auto time0 = std::chrono::high_resolution_clock::now();

   std::string list = load();

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
