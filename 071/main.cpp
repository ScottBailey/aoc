#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>

// 179

// this is a container for the linkage info
struct node {
   using list_t = std::vector<node>;
   std::string outer;
   std::string inner;
   bool operator<(const node& rhs) {
      if(inner == rhs.inner)
         return outer < rhs.outer;
      return inner < rhs.inner;
   }
   bool operator==(const node& rhs) { return (outer == rhs.outer) && (inner == rhs.inner); }
   bool operator<(const std::string& rhs_inner) const { return inner < rhs_inner; }
   bool operator==(const std::string& rhs_inner) const { return inner == rhs_inner; }
   bool operator!=(const std::string& rhs_inner) const { return inner != rhs_inner; }

};
std::ostream& operator<<(std::ostream& os, const node& n) { os << "'" << n.outer << "' -> '" << n.inner << "'"; return os; }


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
         rv.emplace_back( node{outer,"no other"} );
         return rv;
      }
      // temp can be converted to a number here, if needed

      // read the inner color
      std::string inner;
      if(!(std::cin >> inner >> temp))
         return rv;
      inner += ' ';
      inner += temp;

      rv.emplace_back( node{outer,inner} );

      // read terminator. This will be "bag" optionally followed by "s" then either "," or "."
      // if it's "," we continue to the next inner color, if "." the entry is complete.
      if(!(std::cin >> temp))
         return rv;
      if(temp.empty() || (temp.back() == '.'))
         break;
   }

   return rv;
}


// find the outer bag information for a given inner color
node::list_t find_outer(const node::list_t& list, const std::string& color)
{
   node::list_t rv;
   for(auto a = std::lower_bound(list.begin(),list.end(),color); (a != list.end()) && (a->inner == color); ++a)
      rv.push_back(*a);
   return rv;
}
// find the outer bag information for the given node
node::list_t find_outer(const node::list_t& list, const node& n) { return find_outer(list,n.outer); }


int main(int,char**)
{
   auto time_start = std::chrono::high_resolution_clock::now();

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
   // sort so find is efficient
   std::sort(list.begin(),list.end());

   node::list_t result = find_outer(list,"shiny gold");
   node::list_t old_data = result;
   while(!old_data.empty())
   {
      node::list_t new_data;
      for(auto& a : old_data)
      {
         //std::cout << "testing: " << a << std::endl;
         node::list_t temp = find_outer(list,a);
         for(auto& b : temp)
         {
            bool found = false;
            for(auto& c : result)
            {
               if(b.outer == c.outer)
               {
                  found = true;
                  break;
               }
            }
            if(!found)
            {
               result.push_back(b);
               new_data.push_back(b);
               //std::cout << "  adding: " << b << std::endl;
            }
         }
      }
      old_data.swap(new_data);
   }

   //for(auto& a : result)
   //std::cout << a << std::endl;

   std::cout << "unique outer choices: " << result.size() << std::endl;

   auto time_end = std::chrono::high_resolution_clock::now();
   std::cout << std::chrono::duration_cast<std::chrono::microseconds>(time_end-time_start).count() << std::endl;
   return 0;
}
