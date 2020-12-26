#include <iostream>
#include <chrono>
#include <algorithm>
#include <stdint.h>
#include <unistd.h> // STDIN_FILENO
#include <map>

//const bool debug=false;

using map_t = std::map<uint64_t,uint64_t>;

map_t load_initial()
{
   map_t rv;

   uint64_t count = 0;
   uint64_t value = 0;
   bool valid = false;  // is value valid?

   char ch;
   while(read(STDIN_FILENO, &ch, 1) > 0) // read until eof
   {
      uint64_t temp = ch -'0';
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
   uint64_t count = m.size();

   /*
   uint64_t n = 0;
   for( auto [c,v] : m )
   {
     std::cout << n+1 << " (" << n << ") :  " << c << " " << v << "\n";
     ++n;
   }
   */

   uint64_t last = 0;

   uint64_t search = 30000000-1;
   while(count < search)
   {
      uint64_t delta;
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
