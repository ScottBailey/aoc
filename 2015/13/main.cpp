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
#include <map>

//const bool debug=false;


using cost_t = std::map<std::string,int>;
using container_t = std::map<std::string,cost_t>;
using names_t = std::vector<std::string>;


std::ostream& operator<<(std::ostream& os, const container_t& map)
{
   for(const auto& [who,cost] : map)
   {
      os << who << ":\n";
      for(const auto& [other,val] : cost)
         os << "   " << other << "  " << val << "\n";
   }
   return os;
}


container_t load(std::istream& is=std::cin)
{
   container_t rv;
   std::string temp;
   while(is >> temp)
   {
      // we read the name, so get or create it's cost map
      cost_t& cost = rv[temp];

      if(!(is >> temp)) // would
         throw std::invalid_argument("bad input");
      if(!(is >> temp)) // gain/lose
         throw std::invalid_argument("bad input");
      int mult=1;
      if(temp == "lose")
         mult = -1;

      int val;
      if(!(is >> val))
         throw std::invalid_argument("bad input");
      if( !(is >> temp) || !(is >> temp) || !(is >> temp) || !(is >> temp) || !(is >> temp) || !(is >> temp) )
         throw std::invalid_argument("bad input");
      if( !(is >> temp) ) // who
         throw std::invalid_argument("bad input");

      if(temp.back() != '.')
          throw std::invalid_argument("bad input");
      temp.pop_back();

      cost[temp] = val * mult;
   }

   return rv;
}


names_t get_names(const container_t& map)
{
   names_t rv;
   rv.reserve(map.size());
   for(const auto& [who,cost] : map)
      rv.push_back(who);
   return rv;
}


int accum(const names_t& names, const container_t& map)
{
   int rv=0;
   for(size_t i=0; i < names.size(); ++i)
   {
      const cost_t& cost = map.at(names[i]);

      size_t l = (i == 0 ? names.size()-1 : i-1);
      rv += cost.at(names[l]);

      size_t r = (i+1) % names.size();
      rv += cost.at(names[r]);
   }

   return rv;
}


std::vector<std::pair<int,names_t>> solve1(const container_t& map)
{
   using entry_t = std::pair<int,names_t>;
   std::vector<entry_t> rv;

   {
      size_t n=1;
      for(size_t i=map.size()-1; i > 1; --i)
         n *= i;
      rv.reserve(n);
   }

   names_t names = get_names(map);
   std::sort(names.begin(),names.end());
   rv.emplace_back( entry_t{accum(names,map), names} );


   while(std::next_permutation(names.begin()+1,names.end()))
      rv.emplace_back( entry_t{accum(names,map), names} );

   std::sort(rv.begin(),rv.end(), [](const entry_t& l, const entry_t& r) { return l.first < r.first; } );

   return rv;
}


void part1(const container_t& map)
{
   std::vector<std::pair<int,names_t>> temp = solve1(map);
   int result = temp.back().first;
   std::cout << sb::white << "Part 1: " << sb::reset << result << "\n";
}


void part2(container_t map)
{
   cost_t my_cost;
   for(auto& [name,cost] : map)
   {
      cost["me"] = 0;
      my_cost[name] = 0;
   }
   map["me"] = my_cost;

   std::vector<std::pair<int,names_t>> temp = solve1(map);
   int result = temp.back().first;
   std::cout << sb::white << "Part 2: " << sb::reset << result << "\n";
}


int main(int,char**)
{
   auto time0 = std::chrono::high_resolution_clock::now();

   container_t map = load();

   auto time1 = std::chrono::high_resolution_clock::now();

   part1(map);

   auto time2  = std::chrono::high_resolution_clock::now();

   part2(map);

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
