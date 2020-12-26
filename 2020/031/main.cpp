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


int main(int,char**)
{
   std::vector<std::vector<int>> vv = load();
   //dump(vv);
   if(vv.empty())
   {
      std::cerr << "no data!" << std::endl;
      return -1;
   }

   size_t x=0; // across
   size_t y=0; // down

   size_t slope_x = 3;
   size_t slope_y = 1;

   int trees = 0;
   while(y < vv.size())
   {
      const std::vector<int>& v = vv[y];
      //std::vector<int>& v = vv[y];
      trees += v[x%v.size()];
      //v[x%v.size()]+=2;

      x += slope_x;
      y += slope_y;
   }

   //dump(vv);
   std::cout << "\n" << trees << std::endl;

   return 0;
}
