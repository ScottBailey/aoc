#include <iostream>
#include <chrono>
#include <algorithm>
#include <stdint.h>
#include <aoc/get.h>            // aoc::get_line()
#include <sb/string.h>          // sb::string::parse(), sb::string::from()
#include <deque>

//const bool debug=false;

using list_t = std::deque<unsigned>;

list_t load(std::istream& is=std::cin)
{
   list_t rv;
   // read the player line - we really should validate it
   aoc::get_line(is);
   // read all the player input, exit on a blank line
   for(std::string temp = aoc::get_line(is); !temp.empty(); temp = aoc::get_line(is))
   {
      unsigned val;
      if(sb::string::from(temp,val))
         rv.push_front(val);
   }

   return rv;
}


list_t play1(list_t l, list_t r)
{
   for(;;)
   {
      if(l.empty())
         return r;
      if(r.empty())
         return l;

      unsigned i=l.back();
      unsigned j=r.back();

      l.pop_back();
      r.pop_back();

      if(i == j)
      {
         std::cerr << "tie!\n";
         exit(-1);
      }
      if(i > j)
      {
         l.push_front(i);
         l.push_front(j);
      }
      else // (j > i)
      {
         r.push_front(j);
         r.push_front(i);
      }
   }
}


void dump(const list_t& l)
{
   for(auto a : l)
      std::cout << a << "\n";
}


int main(int,char**)
{
   auto time_start = std::chrono::high_resolution_clock::now();

   list_t deck = play1(load(),load());

   uint64_t sum=0;
   for(size_t i=0; i <deck.size(); ++i)
      sum += (deck[i] * (i+1));
   std::cout << sum << "\n";


   auto time_end = std::chrono::high_resolution_clock::now();
   std::cout << "\ntime: " << std::chrono::duration_cast<std::chrono::microseconds>(time_end-time_start).count() << " us" << std::endl;
   return 0;
}
