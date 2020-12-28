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

using list_t = std::vector<int>;

std::ostream& operator<<(std::ostream& os, const list_t& list)
{
   sb::ignore(list);
   return os;
}


list_t load(std::istream& is=std::cin)
{
   list_t rv;
   char ch;
   while(is.get(ch))
   {
      //throw std::invalid_argument("bad input");
   }
   return rv;
}


void part1(const list_t& list)
{
   int result = list.size();
   std::cout << sb::white << "Part 1: " << sb::reset << result << "\n";
}


void part2(const list_t& list)
{
   int result = list.size();

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
