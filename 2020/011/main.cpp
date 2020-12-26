#include <algorithm>
#include <load_vector.h>

int main(int,char**)
{
   std::vector<int> v = load_vector<int>(std::cin);
   if(v.empty())
   {
      std::cerr << "NO DATA!!!" << std::endl;
      return -1;
   }

   const size_t sz{v.size()};
   std::sort(v.begin(),v.end());

   std::vector<int> d(sz);
   for(size_t i=0; i < sz; ++i)
      d[i] = 2020-v[i];

   for(size_t i=0; i < sz; ++i)
   {
      int n = v[i];
      // lower_bound() requires sorted list, d is sorted in REVERSE order, so use reverse iterator
      auto a = std::lower_bound(d.rbegin()+i,d.rend(),n);
      if((a == d.rend()) || (*a != n)) // not found?
         continue;
      int m = 2020 - n;
      std::cout << n << " * " << m << " = " << (n * m) << std::endl;
      return 0;
   }

   std::cerr << "NO SOLUTION!" << std::endl;
   return -1;
}
