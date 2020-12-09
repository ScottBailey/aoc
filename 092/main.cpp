#include <iostream>
#include <chrono>

#include <algorithm>
#include <stdint.h>

#include <load_vector.h>
#include <max_a.h>


const size_t preamble_size{25};

// weakness: 925549 + 2086871 = 3012420


int main(int,char**)
{
   auto time_start = std::chrono::high_resolution_clock::now();

   std::vector<uint64_t> v = load_vector<uint64_t>(std::cin);
   if(v.size() < preamble_size)
   {
      std::cerr << "premature end of data" << std::endl;
      return -1;
   }

   uint64_t critical_number=0;
   std::vector<uint64_t> t(preamble_size);  // temporary vector
   for(size_t i=preamble_size; i < v.size(); ++i)
   {
      uint64_t& curr = v[i];
      // load the temporary with diffs
      for(size_t j=0; j < preamble_size; ++j)
         t[j]=curr-v[i-j-1];
      // sort
      std::sort(t.begin(),t.end());
      // search
      bool found=false;
      for(size_t j=i-preamble_size; !found && (j < i); ++j)
      {
         auto a = std::lower_bound(t.begin(),t.end(),v[j]);
         if((a == t.end()) || (*a != v[j]) || (curr-*a == *a)) // not found or same number
            continue;
         found = true;
      }
      if(!found)
      {
         critical_number = curr;
         std::cout << "Critical number " << critical_number << " found at index " << i << "\n";
         break;
      }
   }

   // now find the encryption weakness
   // WARN: this does not sanity check and could result in an infinite loop
   uint64_t sum = v[0];
   size_t l=0;
   size_t r=1;
   for(;;)
   {
      while((r < v.size()) && (sum < critical_number))
      {
         sum += v[r];
         ++r;
      }
      if(sum == critical_number)
      {
         uint64_t min = v[l];
         uint64_t max = v[l];
         for(size_t i=l; i < r; ++i) {
            sb::min_a(min,v[i]);
            sb::max_a(max,v[i]);
         }
         std::cout << "weakness: " << min << " + " << max << " = " << min + max << "\n";
         break;
      }
      while((l<r) && (sum > critical_number))
      {
         sum -= v[l];
         ++l;
      }
   }


   auto time_end = std::chrono::high_resolution_clock::now();
   std::cout << "\ntime: " << std::chrono::duration_cast<std::chrono::microseconds>(time_end-time_start).count() << " us" << std::endl;
   return 0;
}
