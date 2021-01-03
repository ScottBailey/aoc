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

std::ostream& operator<<(std::ostream& os, const container_t& list)
{
   sb::ignore(list);
   return os;
}


container_t load(std::istream& is=std::cin)
{
   container_t rv;
   char ch;
   while(is.get(ch))
   {
      rv.push_back(ch);
      //throw std::invalid_argument("bad input");
   }
   return rv;
}


void part1(const container_t& str)
{
   int64_t sum = 0;

   size_t pos =0;
   while(pos < str.size())
   {
      // find the first char
      while(pos < str.size() && (str[pos] < '0' || str[pos] > '9'))
         ++pos;
      if(pos > str.size())
         break;

      size_t first = pos;
      if(pos > 0 && str[pos-1] == '-')
         --first;

      while(pos < str.size() && (str[pos] >= '0' && str[pos] <= '9'))
         ++pos;

      int64_t temp=0;
      if(!sb::string::from( std::string_view(str.data()+first, pos-first), temp))
         throw -1;

      sum += temp;
   }

   std::cout << sb::white << "Part 1: " << sb::reset << sum << "\n";
}


void part2(const container_t& list)
{
   int result = list.size();

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
