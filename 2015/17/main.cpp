#include <iostream>
#include <chrono>
#include <algorithm>
#include <stdint.h>
#include <aoc/get.h>            // aoc::get_line()
#include <sb/string.h>          // sb::string::parse(), sb::string::from()
#include <sb/ansi.h>            // ansi colors: sb::red, sb::yellow, etc
#include <sb/ignore.h>          // sb::string::parse(), sb::string::from()
#include <vector>
#include <boost/container/devector.hpp>
#include <numeric>

//const bool debug=false;


using container_t = std::vector<unsigned>;
using solution_t = std::vector<unsigned>;
using solutions_t = std::vector<solution_t>;

std::ostream& operator<<(std::ostream& os, const container_t& list)
{
   for(auto& a: list)
      os << a << " ";
   return os;
}


std::ostream& operator<<(std::ostream& os, const solutions_t& list)
{
   for(auto& a: list)
      os << a << "\n";
   return os;
}


void append(solutions_t& rv, const solutions_t& rhs)
{
   if(rhs.empty())
      return;
   rv.reserve(rv.size()+rhs.size());
   for(const auto& a : rhs)
   {
      if(!a.empty())
         rv.push_back(a);
   }
}


container_t load(std::istream& is=std::cin)
{
   container_t rv;
   unsigned temp;
   while(is >> temp)
   {
      rv.push_back(temp);
      //throw std::invalid_argument("bad input");
   }
   std::sort(rv.begin(),rv.end());
   return rv;
}


solutions_t solve(solution_t so_far, container_t remain, unsigned target)
{
   unsigned sum = std::accumulate(so_far.begin(), so_far.end(), 0);
   if(sum == target)
      return solutions_t{so_far};
   if(sum > target || remain.empty())
      return solutions_t{};

   solutions_t rv;

   unsigned val = remain.back();
   remain.pop_back();

   append(rv, solve(so_far, remain, target));

   so_far.push_back(val);
   append(rv, solve(so_far, remain, target));

   return rv;
}


inline solutions_t solve(const container_t& list, unsigned target)
{
   solution_t temp;
   solutions_t rv = solve(temp, list, target);
   //std::sort(rv.begin(),rv.end());
   return rv;
}


void part1(const container_t& list)
{
   unsigned target = 150;
   if(list.size() <= 5)
      target = 25;
   solutions_t sol = solve(list, target);
   unsigned result = sol.size();
   std::cout << sb::white << "Part 1: " << sb::reset << result << "\n";
}


void part2(const container_t& list)
{
   unsigned target = 150;
   if(list.size() <= 5)
      target = 25;
   solutions_t sol = solve(list, target);

   if(sol.empty())
      throw -1;

   size_t min = std::numeric_limits<size_t>::max();
   size_t result = 0;

   for(const auto& a : sol)
   {
      if(a.size() == min)
         ++result;
      else if(a.size() < min)
      {
         result = 1;
         min = a.size();
      }
   }

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
