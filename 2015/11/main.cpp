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

container_t load(std::istream& is=std::cin)
{
   container_t rv;
   is >> rv;
   return rv;
}


void increment(std::string& pw)
{
   for(size_t i = pw.size()-1; i < pw.size(); --i) // unsigned rollover
   {
      if(pw[i] < 'z')
      {
         ++pw[i];
         // rule 2 could be applied here to optimize the search...
         return;
      }
      pw[i] = 'a';
   }

   pw.push_back('a');
}


bool rule1(const std::string& pw)
{
   for(size_t i=0; i < pw.size()-2; ++i)
   {
      if((pw[i] + 1 == pw[i+1]) && (pw[i] + 2 == pw[i+2]))
         return true;
   }
   return false;
}


bool rule2(const std::string& pw)
{
   for(size_t i=0; i < pw.size(); ++i)
   {
      if((pw[i] == 'i') || (pw[i] == 'o') || (pw[i] == 'l'))
         return false;
   }
   return true;
}


bool rule3(const std::string& pw)
{
   // find the first pair
   size_t i=1;
   for( ; i < pw.size(); ++i)
   {
      if(pw[i-1] == pw[i])
         break;
   }
   for( i += 2; i < pw.size(); ++i)
   {
      if(pw[i-1] == pw[i])
         return true;
   }

   return false;
}


std::string find_next(std::string pw)
{
   increment(pw);
   while(!rule2(pw) || !rule1(pw) || !rule3(pw))
      increment(pw);
   return pw;
}


void part1(const container_t& pw)
{
   std::string result = find_next(pw);
   std::cout << sb::white << "Part 1: " << sb::reset << result << "\n";
}


void part2(const container_t& pw)
{
   std::string result = find_next(pw);
   std::cout << sb::white << "Part 2: " << sb::reset << result << "\n";
}


int main(int,char**)
{
   auto time0 = std::chrono::high_resolution_clock::now();

   container_t list = load();

   auto time1 = std::chrono::high_resolution_clock::now();

   part1(list);

   auto time2  = std::chrono::high_resolution_clock::now();

   part2("hepxxyzz");

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
