#include <iostream>
#include <chrono>
#include <algorithm>
#include <stdint.h>
#include <aoc/get.h>            // aoc::get_line()
#include <sb/string.h>          // sb::string::parse(), sb::string::from()
#include <vector>
#include <array>
#include <cassert>
//const bool debug=false;

using list_t = std::vector<unsigned>;


list_t load(std::istream& is=std::cin)
{
   list_t rv;

   char ch;
   while(is.get(ch))
   {
      unsigned val = ch -'0';
      if( val > 9 )
         break;
      rv.push_back(val);
   }
   return rv;
}


inline void dump(const list_t& v)
{
   for(auto a : v)
      std::cout << a << " ";
   std::cout << "\n";
}


inline bool newstart(list_t& v, unsigned n)
{
   list_t::iterator a = std::find(v.begin(),v.end(),n);
   if(a == v.end())
      return false;
   list_t w;
   w.reserve(v.capacity());
   /*
   std::for_each(a+1, v.end(), std::back_inserter(w));
   std::for_each(v.begin(), a, std::back_inserter(w));
   */
   std::for_each(a, v.end(), [&](unsigned u){ w.push_back(u); });
   std::for_each(v.begin(), a, [&](unsigned u){ w.push_back(u); });
   v.swap(w);
   return true;
}


inline void move(list_t& v)
{
   assert(v.size() > 4);

   std::array<list_t::value_type,3> w{ v[1], v[2], v[3] };
   v.erase(v.begin()+1, v.begin()+4);
   unsigned search = v[0];
   for(;;)
   {
      search -= 1;
      if( !search )
         search = 9;
      auto a = std::find(v.begin(),v.end(),search);
      if(a == v.end())
         continue;
      v.insert(a+1, w.begin(), w.end());
      break;
   }
   v.push_back(v.front());
   v.erase(v.begin());
}


int main(int,char**)
{
   auto time_start = std::chrono::high_resolution_clock::now();


   list_t v = load();
   for(size_t i =0; i < 100; ++i)
      move(v);
   newstart(v, 1);

   for(size_t i=1 ; i < v.size(); ++i)
      std::cout << v[i];
   std::cout << "\n";


   auto time_end = std::chrono::high_resolution_clock::now();
   std::cout << "\ntime: " << std::chrono::duration_cast<std::chrono::microseconds>(time_end-time_start).count() << " us" << std::endl;
   return 0;
}
