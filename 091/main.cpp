#include <iostream>
#include <chrono>

#include <algorithm>

#include <load_vector.h>
#include <stdint.h>

const size_t preamble_size{25};



int main(int,char**)
{
   auto time_start = std::chrono::high_resolution_clock::now();

   std::vector<uint64_t> v = load_vector<uint64_t>(std::cin);
   if(v.size() < preamble_size)
   {
      std::cerr << "premature end of data" << std::endl;
      return -1;
   }


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
         std::cout << "Critical number " << curr << " found at index " << i << "\n";
         break;
      }
   }



   auto time_end = std::chrono::high_resolution_clock::now();
   std::cout << "\ntime: " << std::chrono::duration_cast<std::chrono::microseconds>(time_end-time_start).count() << std::endl;
   return 0;
}
