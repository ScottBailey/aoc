#include <iostream>
#include <algorithm>
#include <unistd.h> // STDIN_FILENO
#include <vector>
#include <max_a.h>

int main(int,char**)
{
   size_t total = 0;
   std::vector<char> answers;
   char last = 0;
   char ch;
   while(read(STDIN_FILENO, &ch, 1) > 0)
   {
      if(ch == '\n')
      {
         if(last == '\n')
         {
            total += answers.size();
            answers.clear();
         }
         last = ch;
         continue;
      }

      auto a = std::lower_bound(answers.begin(),answers.end(),ch);
      if(a == answers.end())
         answers.push_back(ch);
      else if(*a != ch)
         answers.insert(a,ch);
      last = ch;
   }

   total += answers.size();
   answers.clear();

   std::cout << total << std::endl;

   return 0;
}
