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

using container_t = std::vector<std::string>;

std::ostream& operator<<(std::ostream& os, const container_t& list)
{
   sb::ignore(list);
   return os;
}


container_t load(std::istream& is=std::cin)
{
   container_t rv;
   for(std::string temp = aoc::get_line(is); !temp.empty(); temp = aoc::get_line(is))
      rv.push_back(temp);
   return rv;
}


size_t count1(const container_t& list)
{
   size_t rv=0;
   for(const auto& a : list)
      rv += a.size();
   return rv;
}


void solve1(container_t& list)
{
   for(std::string& line : list)
   {
      std::string temp;
      temp.reserve(line.size());
      size_t i=0;
      while(i < line.size())
      {
         if((i == 0 || (i+1) == line.size()) && (line[i] == '"'))
         {
            ++i;
            continue;
         }

         if(line[i] != '\\')
         {
            temp.push_back(line[i]);
            ++i;
            continue;
         }
         // not escaped
         ++i;
         if(i == line.size())
            throw std::invalid_argument("trailing escape");
         switch(line[i])
         {
            case '"': temp.push_back('"'); ++i; continue;
            case '\\': temp.push_back('\\'); ++i; continue;
            case 'x': break;
            default:
               throw std::invalid_argument("unexpected escape sequence");
               break;
         }
         if(i+2 >= line.size())
            throw std::invalid_argument("unexpected end of input");
         ++i;
         char c=0;
         if(!sb::string::from_hex(std::string_view(line.data()+i,2),c))
            throw std::invalid_argument("char conversion error");
         temp.push_back(c);
         i+=2;
      }
      line.swap(temp);
   }
}


void part1(const container_t& list)
{
   container_t solved = list;
   solve1(solved);
   size_t result = count1(list) - count1(solved);
   std::cout << sb::white << "Part 1: " << sb::reset << result << "\n";
}


void solve2(container_t& list)
{
   for(std::string& line : list)
   {
      std::string temp;
      temp.reserve(line.size()+6);
      temp.push_back('"');

      for(char c : line)
      {
         if(c == '"' || c == '\\')
            temp.push_back('\\');
         temp.push_back(c);
      }

      temp.push_back('"');

      line.swap(temp);
   }
}


void part2(const container_t& list)
{
   container_t solved = list;
   solve2(solved);
   size_t result =  count1(solved) - count1(list);
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
