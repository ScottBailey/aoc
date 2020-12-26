#include <iostream>
#include <charconv>
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

      size_t min=0;
      size_t max=0;
      sb::string::from(temp.substr(0,pos),min);
      sb::string::from(temp.substr(pos+1),max);

      if(!(std::cin >> temp) || (temp.size() != 2) || (temp[1] != ':'))
         break;

      char search = temp[0];

      if(!(std::cin >> temp) || temp.empty())
         break;

      size_t count=0;
      for(char c : temp)
      {
         if(c == search)
            ++count;
      }

      if((count >= min) && (count <= max))
         ++good;
   }

   std::cout << good << std::endl;
   return 0;

}
