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

   for(size_t i=0; i < sz; ++i)
   {
      for(size_t j=0; j < sz; ++j)
      {
         if(j == i)
            continue;
         int search = 2020 - (v[i] + v[j]);
         auto a = std::lower_bound(v.begin()+i,v.end(),search);
         if((a == v.end()) || (*a != search)) // not found?
            continue;
         std::cout << v[i] << " * " << v[j] << " * " << *a << " = " << (static_cast<long>(v[i]) * v[j] * (*a)) << std::endl;
         return 0;
      }
   }

   std::cerr << "NO SOLUTION!" << std::endl;
   return -1;
}
