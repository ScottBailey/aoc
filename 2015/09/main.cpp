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
#include <thread>

//const bool debug=false;


struct dist_t
{
   std::string a;
   std::string b;
   unsigned dist;
   bool has(const std::string& loc) { return loc == a || loc == b; }
   bool has_a(const std::string& loc) {
      if(loc == a) return true;
      if(loc == b) {
         swap();
         return true;
      }
      return false;
   }
   void swap() { std::swap(a,b); }
   bool operator<(const dist_t& rhs) const { return dist < rhs.dist; };
   bool operator==(const dist_t& rhs) const { return (a == rhs.a && b == rhs.b) || (a == rhs.b && b == rhs.a); }
};
std::ostream& operator<<(std::ostream& os, const dist_t& d)
{
   os << d.a << " to " << d.b << "  " << d.dist;
   return os;
}

using container_t = std::vector<dist_t>;
using result_t = std::pair<container_t,unsigned>;

bool operator<(const result_t& lhs, const result_t& rhs) { return lhs.second < rhs.second; }


std::ostream& operator<<(std::ostream& os, const container_t& list)
{
   for(auto& a : list)
      os << a << "\n";
   return os;
}


container_t load(std::istream& is=std::cin)
{
   container_t rv;
   dist_t d;
   std::string temp;
   while( is >> d.a
         && is >> temp
         && is >> d.b
         && is >> temp
         && is >> d.dist)
   {
      rv.push_back(d);
   }
   return rv;
}


result_t route_solve1(container_t list, container_t route, dist_t node, unsigned dist, unsigned best)
{
   // add d to route
   route.push_back(node);

   // remove any impossible nodes
   for(container_t::iterator i=list.begin(); i != list.end(); ) // increment inside
   {
      if(i->has(node.a))
         i = list.erase(i);
      else
         ++i;
   }

   // exit condition
   if(list.empty())
      return std::make_pair(route, dist);

   // select new paths
   result_t best_result{container_t{},std::numeric_limits<unsigned>::max()};
   for(dist_t& a : list)
   {
      if(!a.has_a(node.b))
         continue;
      unsigned new_dist = dist + a.dist;
      if(new_dist >= best)
         continue;
      result_t temp = route_solve1(list, route, a, new_dist, best);
      if(temp.second >= best)
         continue;
      best = temp.second;
      best_result = temp;
   }
   return best_result;
}


void part1(const container_t& list)
{
   std::vector<std::string> locs;
   for(const auto& a : list)
   {
      locs.push_back(a.a);
      locs.push_back(a.b);
   }
   std::sort(locs.begin(),locs.end());
   locs.erase(std::unique(locs.begin(),locs.end()),locs.end());

   result_t best{container_t{},std::numeric_limits<unsigned>::max()};
   for(const std::string& loc : locs)
   {
      dist_t dummy;
      dummy.a = "";
      dummy.b = loc;
      dummy.dist = 0;

      result_t temp = route_solve1(list, container_t{}, dummy, 0, best.second);
      if(temp.second < best.second)
         best = temp;
   }

   /*
   std::cout << list << "\n\n";
   for(const auto& a : best.first)
      std::cout << " -> " << a.b;
   std::cout << "\n\n";
   */
   unsigned result = best.second;
   std::cout << sb::white << "Part 1: " << sb::reset << result << "\n";
}


void part1_threads(const container_t& list)
{
   // get all the possible starting locations
   std::vector<std::string> locs;
   for(const auto& a : list)
   {
      locs.push_back(a.a);
      locs.push_back(a.b);
   }
   std::sort(locs.begin(),locs.end());
   locs.erase(std::unique(locs.begin(),locs.end()),locs.end());


   // we need a list for return values and we need a list for threads, 1-to-1 with starting locations
   std::vector<result_t> all(locs.size());
   std::vector<std::thread> threads;
   threads.reserve(all.size());

   for(size_t i=0; i < locs.size(); ++i)
   {
      threads.emplace_back(
         [&all,list,locs,i]() {
            dist_t dummy;
            dummy.a = "";
            dummy.b = locs[i];
            dummy.dist = 0;

            all[i] = route_solve1(list, container_t{}, dummy, 0, std::numeric_limits<unsigned>::max());
         }
      );
   }

   for(auto& a : threads)
      a.join();

   std::sort(all.begin(),all.end());

   /*
   std::cout << list << "\n\n";
   for(const auto& a : all.front().first)
      std::cout << " -> " << a.b;
   std::cout << "\n\n";
   */
   unsigned result = all.front().second;
   std::cout << sb::white << "Part 1: " << sb::reset << result << "\n";
}


result_t route_solve2(container_t list, container_t route, dist_t node, unsigned dist, unsigned best)
{
   // add d to route
   route.push_back(node);

   // remove any impossible nodes
   for(container_t::iterator i=list.begin(); i != list.end(); ) // increment inside
   {
      if(i->has(node.a))
         i = list.erase(i);
      else
         ++i;
   }

   // exit condition
   if(list.empty())
      return std::make_pair(route, dist);

   // select new paths
   result_t best_result{container_t{},0};
   for(dist_t& a : list)
   {
      if(!a.has_a(node.b))
         continue;
      unsigned new_dist = dist + a.dist;
      result_t temp = route_solve2(list, route, a, new_dist, best);
      if(temp.second <= best)
         continue;
      best = temp.second;
      best_result = temp;
   }
   return best_result;
}


void part2(const container_t& list)
{
   std::vector<std::string> locs;
   for(const auto& a : list)
   {
      locs.push_back(a.a);
      locs.push_back(a.b);
   }
   std::sort(locs.begin(),locs.end());
   locs.erase(std::unique(locs.begin(),locs.end()),locs.end());

   result_t best{container_t{},0};
   for(const std::string& loc : locs)
   {
      dist_t dummy;
      dummy.a = "";
      dummy.b = loc;
      dummy.dist = 0;

      result_t temp = route_solve2(list, container_t{}, dummy, 0, best.second);
      if(temp.second > best.second)
         best = temp;
   }

   /*
   std::cout << list << "\n\n";
   for(const auto& a : best.first)
      std::cout << " -> " << a.b;
   std::cout << "\n\n";
   */
   unsigned result = best.second;

   std::cout << sb::white << "Part 2: " << sb::reset << result << "\n";
}


int main(int,char**)
{
   auto time0 = std::chrono::high_resolution_clock::now();

   container_t list = load();

   auto time1 = std::chrono::high_resolution_clock::now();

   part1(list);

   auto time2 = std::chrono::high_resolution_clock::now();

   part1_threads(list);

   auto time3  = std::chrono::high_resolution_clock::now();

   part2(list);

   auto time4  = std::chrono::high_resolution_clock::now();


   std::cout
      << sb::white
      << "\nload time:  " << std::chrono::duration_cast<std::chrono::microseconds>(time1-time0).count() << " us\n"
      << "part1 time: " << std::chrono::duration_cast<std::chrono::microseconds>(time2-time1).count() << " us\n"
      << "part1 (multi-threaded) time: " << std::chrono::duration_cast<std::chrono::microseconds>(time3-time2).count() << " us\n"
      << "part2 time: " << std::chrono::duration_cast<std::chrono::microseconds>(time4-time3).count() << " us\n"
      << std::flush
      ;
   return 0;
}
