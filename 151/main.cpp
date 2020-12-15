#include <iostream>
#include <chrono>
#include <algorithm>
#include <stdint.h>
#include <unistd.h> // STDIN_FILENO
#include <map>

//const bool debug=false;

using map_t = std::map<unsigned,unsigned>;

map_t load_initial()
{
   map_t rv;

   unsigned count = 0;
   unsigned value = 0;
   bool valid = false;  // is value valid?

   char ch;
   while(read(STDIN_FILENO, &ch, 1) > 0) // read until eof
   {
      unsigned temp = ch -'0';
      if(temp < 10)
      {
         value *= 10;
         value += temp;
         valid = true;
      }
      else if(valid)
      {
         rv[value] = count;
         ++count;
         value = 0;
         valid = false;
      }
   }
   if(valid)
      rv[value] = count;
   return rv;
}


int main(int,char**)
{
   auto time_start = std::chrono::high_resolution_clock::now();

   map_t m = load_initial();
   unsigned count = m.size();

   /*
   unsigned n = 0;
   for( auto [c,v] : m )
   {
     std::cout << n+1 << " (" << n << ") :  " << c << " " << v << "\n";
     ++n;
   }
   */

   unsigned last = 0;

   while(count < 2019)
   {
      unsigned delta;
      auto i = m.find(last);
      if(i == m.end()) // new value, so 0
         delta = 0;
      else
         delta = count - i->second;
      m[last] = count;
      ++count;
      last = delta;
   }

   std::cout << last << "\n";

   auto time_end = std::chrono::high_resolution_clock::now();
   std::cout << "\ntime: " << std::chrono::duration_cast<std::chrono::microseconds>(time_end-time_start).count() << " us" << std::endl;
   return 0;
}
