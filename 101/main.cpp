#include <iostream>
#include <chrono>

#include <algorithm>
#include <stdint.h>

#include <load_vector.h>



int main(int,char**)
{
   auto time_start = std::chrono::high_resolution_clock::now();

   auto v = load_vector<int>(std::cin);

   std::sort(v.begin(),v.end());

   int d1=0;
   int d3=1; // for the diff from the last to device
   int last=0;
   for(auto a : v)
   {
      int diff = a - last;
      if(diff == 1)
         ++d1;
      else if(diff == 3)
         ++d3;
      last = a;
   }

   std::cout << d1 << " * " << d3 << " = " << d1 * d3 << std::endl;

   auto time_end = std::chrono::high_resolution_clock::now();
   std::cout << "\ntime: " << std::chrono::duration_cast<std::chrono::microseconds>(time_end-time_start).count() << " us" << std::endl;
   return 0;
}
