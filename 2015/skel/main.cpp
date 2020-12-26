#include <iostream>
#include <chrono>
#include <algorithm>
#include <stdint.h>
#include <aoc/get.h>            // aoc::get_line()
#include <sb/string.h>          // sb::string::parse(), sb::string::from()
#include <sb/ignore.h>          // sb::string::parse(), sb::string::from()
#include <vector>


//const bool debug=false;

using list_t= std::vector<unsigned>;

list_t load(std::istream& is=std::cin)
{
   sb::ignore(is);
   return list_t{};
}


void part1(const list_t& list)
{
   sb::ignore(list);
}


void part2(const list_t& list)
{
   sb::ignore(list);
}


int main(int,char**)
{
   auto time0 = std::chrono::high_resolution_clock::now();

   list_t load();

   auto time1 = std::chrono::high_resolution_clock::now();

   list_t part1();

   auto time2  = std::chrono::high_resolution_clock::now();

   list_t part1();

   auto time3  = std::chrono::high_resolution_clock::now();


   std::cout
      << "\nload time:  " << std::chrono::duration_cast<std::chrono::microseconds>(time1-time0).count() << " us\n"
      << "\npart1 time: " << std::chrono::duration_cast<std::chrono::microseconds>(time2-time1).count() << " us\n"
      << "\npart2 time: " << std::chrono::duration_cast<std::chrono::microseconds>(time3-time2).count() << " us\n"
      << std::flush
      ;
   return 0;
}
