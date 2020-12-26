#include <iostream>
#include <algorithm>
#include <unistd.h> // STDIN_FILENO
#include <vector>

#include <algorithm.h>


// answer is 6310

int main(int,char**)
{
   size_t total = 0;            // sum of all groups answeers
   std::vector<char> answers;   // answers per group
   char last = 0;               // used to find end of entry and end of group
   char ch;                     // temp for input
   while(read(STDIN_FILENO, &ch, 1) > 0)
   {
      if(ch != '\n')
         sb::store_u(answers,ch); // unique sorted store
      else if(last == '\n')
      {
         // update the tally and clear the entry
         total += answers.size();
         answers.clear();
      }

      last = ch;
   }

   // in case termination was not "\n\n" we need to update the tally
   total += answers.size();

   std::cout << total << std::endl;

   return 0;
}
