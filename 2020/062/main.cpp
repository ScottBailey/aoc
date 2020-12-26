#include <iostream>
#include <algorithm>
#include <unistd.h> // STDIN_FILENO
#include <vector>

#include <algorithm.h>

// answer is 3193

int main(int,char**)
{
   size_t total = 0;            // sum of each groups yes answers
   bool first = true;           // indicates the next entry is the first in a group
   std::vector<char> answers;   // the yes answers per group
   std::vector<char> temp;      // the yes answers per entry
   char last = 0;               // last input, used to find group terminators
   char ch;                     // temp for input
   while(read(STDIN_FILENO, &ch, 1) > 0) // read in a charachter
   {
      // handle end of line (termination of an entry or group)
      if(ch != '\n')
      {
         // unique, sorted store into vector
         sb::store_u(temp,ch);
      }
      else
      {
         // handle empty line (termination of a group)
         if(last == '\n')
         {
            // simply increment the total count by the current count clear the current count
            total += answers.size();
            answers.clear();
            first = true; // indicate the next entry is the first in a group
         }
         else
         {
            // This is termination of an entry, if it's the first entry, just take it. if it's the 2nd+ entry, intersect it with
            // the current answers so far. Either way, clear the entry and indicate the next entry is the first in the group
            if(first)
               answers = temp;
            else
               sb::intersection_a(answers,temp);
            temp.clear();
            first = false;
         }
      }
      last = ch;
   }

   // if termination wasn't "\n\n", we may need to update answers and total
   if(first)
      answers = temp;
   else if(!temp.empty())
      sb::intersection_a(answers,temp);
   total += answers.size();

   // print total
   std::cout << total << std::endl;

   return 0;
}
