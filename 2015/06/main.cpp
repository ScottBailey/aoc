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

struct coord
{
   unsigned x;
   unsigned y;
};
std::ostream& operator<<(std::ostream& os, const coord& c) { os << c.x << "," << c.y; return os;}

struct command
{
   enum action_t {
      off,
      on,
      toggle,
   };

   action_t action;
   coord p0;
   coord p1;
};

constexpr size_t max{1000};

using list_t = std::vector<command>;
using matrix_t = std::vector<std::vector<unsigned>>;

std::ostream& operator<<(std::ostream& os, const list_t& list)
{
   for(const auto& a : list)
      os << a.action << "  " << a.p0 << " to " << a.p1 << "\n";
   return os;
}


inline void input_check(std::istream& is)
{
   if(!is)
      throw std::invalid_argument("bad input");
}


list_t load(std::istream& is=std::cin)
{
   list_t rv;

   std::string action_string;
   while(is >> action_string)
   {
      command cmd;
      if(action_string == "toggle")
         cmd.action = command::toggle;
      else
      {
         input_check(is >> action_string);
         if(action_string == "on")
            cmd.action = command::on;
         else
            cmd.action = command::off;
      }

      char ch;

      input_check(is >> cmd.p0.x);
      is.get(ch);
      input_check(is >> cmd.p0.y);

      input_check(is >> action_string); // through

      input_check(is >> cmd.p1.x);
      is.get(ch);
      input_check(is >> cmd.p1.y);

      rv.push_back(cmd);
   }

   return rv;
}


void toggle(matrix_t& m, const coord& first, const coord& last)
{
   for(size_t x=first.x; x <= last.x; ++x)
   {
      for(size_t y=first.y; y <= last.y; ++y)
         m[x][y] = ((m[x][y] + 1) & 1);
   }
}


void set(matrix_t& m, const coord& first, const coord& last, unsigned val)
{
   for(size_t x=first.x; x <= last.x; ++x)
   {
      for(size_t y=first.y; y <= last.y; ++y)
         m[x][y] = val;
   }
}


size_t count(const matrix_t& m)
{
   size_t rv=0;
   for(const auto& a : m)
   {
      for( const auto& c : a)
         rv += c;
   }
   return rv;
}


void part1(const list_t& list)
{
   std::vector<std::vector<unsigned>> v;
   for(size_t i = 0; i < max; ++i)
      v.emplace_back(std::vector<unsigned>(max,0));

   for(const auto& a : list)
   {
      switch(a.action)
      {
         case command::off:    set(v, a.p0, a.p1, 0); break;
         case command::on:     set(v, a.p0, a.p1, 1); break;
         case command::toggle: toggle(v, a.p0, a.p1); break;
         default:
            throw std::invalid_argument("bad action");
            break;
      }
   }

   std::cout << sb::white << "Part 1: " << sb::reset << count(v) << "\n";
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
