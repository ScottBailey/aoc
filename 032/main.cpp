#include <iostream>
#include <string>
#include <vector>


std::vector<std::vector<int>> load()
{
   std::vector<std::vector<int>> rv;

   std::string s;
   while((std::cin >> s) && !s.empty())
   {
      std::vector<int> v(s.size());
      for(size_t i=0; i < s.size(); ++i)
         v[i] = (s[i] == '#');
      rv.push_back(v);
   }
   return rv;
}


void dump(const std::vector<std::vector<int>>& vv)
{
   for(const std::vector<int>& v : vv)
   {
      for(int n : v)
         std::cout << n;
      std::cout << "\n";
   }
   std::cout.flush();
}


long traverse(const std::vector<std::vector<int>>& vv, size_t slope_x, size_t slope_y)
{
   size_t x=0;
   size_t y=0;

   int rv = 0;
   while(y < vv.size())
   {
      const std::vector<int>& v = vv[y];
      rv += v[x%v.size()];

      x += slope_x;
      y += slope_y;
   }

   return static_cast<long>(rv);
}


int main(int,char**)
{
   std::vector<std::vector<int>> vv = load();
   if(vv.empty())
   {
      std::cerr << "no data!" << std::endl;
      return -1;
   }
   std::cout << (
                 traverse(vv,1,1)
                 * traverse(vv,3,1)
                 * traverse(vv,5,1)
                 * traverse(vv,7,1)
                 * traverse(vv,1,2)
                 )
             << std::endl
      ;


   return 0;
}
