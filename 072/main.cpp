#include <iostream>
#include <algorithm>
#include <vector>
#include <from_string.h>

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
   bool operator==(const node& rhs) { return (outer == rhs.outer) && (inner == rhs.inner); }
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
      // read <n> <color> <color> bag[s,.]
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


node::list_t find_inner(const node::list_t& list, const std::string& color)
{
   node::list_t rv;
   for(auto a = std::lower_bound(list.begin(),list.end(),color); (a != list.end()) && (a->outer == color); ++a)
      rv.push_back(*a);
   return rv;
}
node::list_t find_inner(const node::list_t& list, const node& n) { return find_inner(list,n.outer); }

//size_t depth=0;
//std::string indent() { return std::string(depth,' '); }

uint64_t count_inner(const node::list_t& list, const node& n)
{
   if(n.inner == "no other")
      return 0;

   //depth += 2;
   //std::cout << indent() << "counting for " << n.inner << std::endl;

   uint64_t rv = 1;
   node::list_t sub = find_inner(list,n.inner);
   for(auto& a : sub)
   {
      if(a.count)
         rv += count_inner(list,a) * a.count;
   }

   //std::cout << indent() << "counted for " << n.inner << " "<< rv << std::endl;
   //depth -=2;

   return rv;
}
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
   node::list_t list;
   size_t line =0;
   for(;;)
   {
      node::list_t temp = parse_line();
      if(temp.empty())
         break;
      ++line;
      for(auto& a : temp)
         list.push_back(a);
   }
   std::sort(list.begin(),list.end());


   //for(auto& a : list)
   //std::cout << a << std::endl;

   std::cout << count_inner(list,"shiny gold") << std::endl;

   return 0;
}
