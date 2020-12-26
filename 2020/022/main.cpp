#include <iostream>
#include <string>
#include <sb/string/from.h>

int main(int,char**)
{

   size_t good = 0;
   for(;;)
   {
      std::string temp;
      if(!(std::cin >> temp) || temp.empty())
         break;

      size_t pos = temp.find("-");
      if(pos == std::string::npos)
         break;

      size_t p=0;
      size_t q=0;
      sb::string::from(temp.substr(0,pos),p);
      sb::string::from(temp.substr(pos+1),q);
      --p;
      --q;

      if(!(std::cin >> temp) || (temp.size() != 2) || (temp[1] != ':'))
         break;

      char search = temp[0];

      if(!(std::cin >> temp) || temp.empty())
         break;

      size_t count = 0;
      if((p < temp.size()) && (temp[p] == search))
         ++count;
      if((q < temp.size()) && (temp[q] == search))
         ++count;
      if( count == 1)
         ++good;
   }

   std::cout << good << std::endl;
   return 0;

}
