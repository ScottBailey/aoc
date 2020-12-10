#include <iostream>
#include <chrono>

#include <algorithm>
#include <stdint.h>

#include <load_vector.h>
#include <max_a.h>

// 259172170858496

const bool debug=true;

int main(int,char**)
{
   auto time_start = std::chrono::high_resolution_clock::now();

   auto v = load_vector<int>(std::cin);

   v.push_back(0);
   std::sort(v.begin(),v.end());
   v.push_back(v.back()+3);

   if(debug)
   {
      for(auto a : v)
         std::cout << a << " ";
      std::cout << std::endl;
   }



   int last =-3;
   for(int& a : v)
   {
      int n = a;
      a = n-last;
      last = n;
   }

   if(debug)
   {
      for(auto a : v)
         std::cout << a << " ";
      std::cout << std::endl;
   }

   size_t d=1;
   for(size_t l=0; l < v.size(); ++l)
   {
      // this works at the end
      if(v[l+1] != 1)
         continue;
      size_t r=l+2;
      while(v[r] == 1)
         ++r;

      // find the number of ones in a row
      size_t one_count = r-l-1;
      int end_sum = v[l] + v[r];
      l=r-1;

      if(one_count < 1)
         continue;
      if(one_count == 1)
      {
         if(end_sum == 4)
            d *= 3;
         else if(end_sum == 5)
            d *= 2;
         //else if(end_sum == 6)
         //  d *= 1;

         continue;
      }
      if(one_count == 2)
      {
         if(end_sum == 4)
            d *= 4;
         else if(end_sum == 5)
            d *= 3;
         else if(end_sum == 6)
            d *= 2;

         continue;
      }

      if(one_count == 3)
      {
         if(end_sum == 4)
            d *= 8;
         else if(end_sum == 5)
            d *= 5;
         else if(end_sum == 6)
            d *= 4;
         continue;
      }

      //if(one_count >= 4)
      if(end_sum == 4)
         d *= 16;
      else if(end_sum == 5)
         d *= 10;
      else if(end_sum == 6)
         d *= 7;

      // I believe this is correct, but the data file doesn't go that far
      d <<= (one_count -4);
   }
   std::cout << d << std::endl;

   auto time_end = std::chrono::high_resolution_clock::now();
   std::cout << "\ntime: " << std::chrono::duration_cast<std::chrono::microseconds>(time_end-time_start).count() << " us" << std::endl;
   return 0;
}
