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

using list_t = std::vector<std::string>;

list_t load(std::istream& is=std::cin)
{
   list_t rv;
   std::string temp;
   while(is >> temp)
      rv.push_back(temp);
   return rv;
}


// -1 naughty
// 0 nothing
// 1+ nice
int string_test(const std::string& s)
{
   char last = ' ';
   size_t vowel_count = 0;
   size_t double_count = 0;
   size_t bad_count = 0;
   for(char ch : s)
   {
      if(ch == last)
         ++double_count;
      switch(ch)
      {
         case 'a':
         case 'e':
         case 'i':
         case 'o':
         case 'u':
            ++vowel_count;
            break;

         case 'b': // ab
            bad_count += (last == 'a');
            break;
         case 'd': // cd
            bad_count += (last == 'c');
            break;
         case 'q': // pq
            bad_count += (last == 'p');
            break;
         case 'y': // xy
            bad_count += (last == 'x');
            break;
      }
      last = ch;
   }
   if(bad_count)
      return -1;
   if((vowel_count > 2) && double_count)
      return 1;
   return 0;
}


void part1(const list_t& list)
{
   size_t nice_count = 0;
   for(const auto& a : list)
      nice_count += (string_test(a) > 0);

   std::cout << sb::white << "Part 1: " << sb::reset << nice_count << "\n";
}


bool string_test_2a(const std::string& s)
{
   for(size_t i=0; i+3 < s.size(); ++i)
   {
      std::string search = s.substr(i,2);
      if(s.find(search,2+i) != std::string::npos)
         return true;
   }
   return false;
}

bool string_test_2b(const std::string& s)
{
   for(size_t i=0; i+2 < s.size(); ++i)
   {
      if(s[i] == s[i+2])
         return true;
   }
   return false;
}

bool string_test_2(const std::string& s)
{
   return string_test_2a(s) && string_test_2b(s);
}


void part2(const list_t& list)
{
   int result = 0;
   for(auto& a : list)
      result += string_test_2(a);
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
