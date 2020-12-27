#include <iostream>
#include <chrono>
#include <algorithm>
#include <stdint.h>
#include <aoc/get.h>            // aoc::get_line()
#include <sb/string.h>          // sb::string::parse(), sb::string::from()
#include <sb/ansi.h>            // ansi colors: sb::red, sb::yellow, etc
#include <sb/ignore.h>          // sb::string::parse(), sb::string::from()
#include <vector>
#include <array>
#include <numeric>

//const bool debug=false;

using item_t = std::array<unsigned,3>;
using list_t = std::vector<item_t>;

std::ostream& operator<<(std::ostream& os, const item_t& item)
{
   os << item[0] << "x" << item[1] << "x" << item[2];
   return os;
}


std::ostream& operator<<(std::ostream& os, const list_t& list)
{
   for(auto& a : list)
      os << a << "\n";
   return os;
}


list_t load(std::istream& is=std::cin)
{
   list_t rv;
   item_t box;
   size_t side=0;
   unsigned dimension=0;
   char ch;
   while(is.get(ch))
   {
      unsigned val = ch-'0';
      if(val < 10)
      {
         dimension *= 10;
         dimension += val;
         continue;
      }
      if(!dimension)
         break;
      box[side] = dimension;
      dimension = 0;
      if(ch != '\n')
         ++side;
      else
      {
         side = 0;
         rv.push_back(box);
      }
   }
   if(dimension)
   {
      if(side != 2)
         throw std::invalid_argument("bad input");
      box[side] = dimension;
      rv.push_back(box);
   }
   return rv;
}


void part1(const list_t& list)
{
   uint64_t result = 0;
   for(auto& a : list)
   {
      unsigned s1 = a[0]*a[1];
      unsigned s2 = a[0]*a[2];
      unsigned s3 = a[1]*a[2];
      unsigned extra = std::min(s1,std::min(s2,s3));
      result += (2*s1) + (2*s2) + (2*s3) + extra;
   }
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
