#include <iostream>
#include <algorithm>
#include <vector>
#include <from_string.h>
#include <chrono>

// 18925

// this is a container for the linkage info
struct node {
   using list_t = std::vector<node>;
   std::string outer;
   unsigned count;
   std::string inner;
   bool operator<(const node& rhs) {
      if(outer == rhs.outer)
         return inner < rhs.inner;
      return outer < rhs.outer;
   }
   bool operator==(const node& rhs) { return (outer == rhs.outer) && (inner == rhs.inner) && (count == rhs.count); }
   bool operator<(const std::string& rhs_outer) const { return outer < rhs_outer; }
   bool operator==(const std::string& rhs_outer) const { return outer == rhs_outer; }
   bool operator!=(const std::string& rhs_outer) const { return outer != rhs_outer; }

};
std::ostream& operator<<(std::ostream& os, const node& n) { os << "'" << n.outer << "' -> '"  << n.count << " " << n.inner << "'"; return os; }


// WARN: this function does not sanity check input very well!!!
node::list_t parse_line()
{
   node::list_t rv;
   std::string temp; // temp variable

   // read the outer color
   std::string outer;
   if(!(std::cin >> outer >> temp))
      return rv;
   outer += ' ';
   outer += temp;

   // read "bags contain"
   if(!(std::cin >> temp >> temp))
      return rv;

   // now read the list of bags it contains
   for(;;)
   {
      // read the inner count: "<n> <color> <color> bag[s,.]" OR "no other bags."
      // start with the bag count, either an int or the word "no"
      if(!(std::cin >> temp))
         return rv;
      if(temp == "no") // "no other bags."
      {
         std::cin >> temp >> temp; // remove "other bags" from the input stream
         rv.emplace_back( node{outer,0,"no other"} );
         return rv;
      }
      // temp is converted to a number here
      unsigned count;
      sb::string::from(temp,count);

      std::string inner;
      if(!(std::cin >> inner >> temp))
         return rv;
      inner += ' ';
      inner += temp;

      rv.emplace_back( node{outer,count,inner} );

      // read terminator. This will be "bag" optionally followed by "s" then either "," or "."
      // if it's "," we continue to the next inner color, if "." the entry is complete.
      if(!(std::cin >> temp))
         return rv;
      if(temp.empty() || (temp.back() == '.'))
         break;
   }

   return rv;
}


// find the inner bag information for a given outer color
node::list_t find_inner(const node::list_t& list, const std::string& color)
{
   node::list_t rv;
   for(auto a = std::lower_bound(list.begin(),list.end(),color); (a != list.end()) && (a->outer == color); ++a)
      rv.push_back(*a);
   return rv;
}
// find the inner bag information for the given node
node::list_t find_inner(const node::list_t& list, const node& n) { return find_inner(list,n.outer); }


uint64_t count_inner(const node::list_t& list, const node& n)
{
   // is it a terminator? return 0
   if(n.inner == "no other")
      return 0;

   uint64_t rv = 1; // there is this bag, at minimum
   node::list_t sub = find_inner(list,n.inner);
   for(auto& a : sub)
   {
      // then all the minimums
      rv += count_inner(list,a) * a.count;
   }

   return rv;
}
// outside call, so the first outer isn't included in the sum of bags
uint64_t count_inner(const node::list_t& list, const std::string& color)
{
   //std::cout << indent() << "counting for " << color << std::endl;
   if(color == "no other")
      return 0;

   uint64_t rv = 0;
   node::list_t sub = find_inner(list,color);
   for(auto& a : sub)
      rv += count_inner(list,a) * a.count;
   return rv;
}


int main(int,char**)
{
   auto time_start = std::chrono::high_resolution_clock::now();

   node::list_t list;
   for(;;)
   {
      node::list_t temp = parse_line();
      if(temp.empty())
         break;
      for(auto& a : temp)
         list.push_back(a);
   }
   std::sort(list.begin(),list.end());


   //for(auto& a : list)
   //std::cout << a << std::endl;

   std::cout << count_inner(list,"shiny gold") << std::endl;

   auto time_end = std::chrono::high_resolution_clock::now();
   std::cout << std::chrono::duration_cast<std::chrono::microseconds>(time_end-time_start).count() << std::endl;
   return 0;
}
