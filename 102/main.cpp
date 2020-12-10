#include <iostream>
#include <chrono>

#include <algorithm>
#include <stdint.h>

#include <load_vector.h>
#include <dump.h>

// 259172170858496

// works for differences of 1, 2, and 3 although input is ONLY 1 and 3.

const bool debug=false;

int main(int,char**)
{
   auto time_start = std::chrono::high_resolution_clock::now();

   auto v = load_vector<int>(std::cin);

   v.push_back(0);               // add in the first value, for the charging outle
   std::sort(v.begin(),v.end()); // sort the list
   v.push_back(v.back()+3);      // add the final value for the device

   dump(debug, v, " ", "\n");   // debug input

   // transform the vector with the differences between elements
   int last =-3;                // dummy value to ensure first value has 3 diff so we never consider it
   for(int& a : v)
   {
      int n = a;
      a = n-last;
      last = n;
   }

   dump(debug, v, " ", "\n");   // debug the transformed vector

   size_t d=1; // distinct arrangements

   // find all the instances where elements differ by a single value, l and r bracket the differences and
   // will be values of 2 or 3
   for(size_t l=0; l < v.size(); ++l)
   {
      // find the left bracketing value
      //   (this works at the end of the vector also, as it NEVER terminates in a 1)
      if(v[l+1] != 1)
         continue;
      // find the right bracketing value
      size_t r=l+2;
      while(v[r] == 1)
         ++r;

      // find the number of ones in a row and sum the bracketing values
      size_t one_count = r-l-1;
      int end_sum = v[l] + v[r];  // valid values are 3+3, 3+2, 2+3, and 2+2
      l=r-1;                      // reset l for the next search. DO NOT USE IT BELOW THIS LINE!!!

      // sanity check
      if(one_count < 1)
         continue;

      // I feel like the following is a mathematical pattern and would be better represented that way, but I've had enough fun for
      // now!

      // [32]1[32]
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
      // [32]11[32]
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
      // [32]111[32]
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
      // [32]1111+[32]
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
