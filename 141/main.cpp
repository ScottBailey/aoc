#include <iostream>
#include <chrono>
#include <algorithm>
#include <stdint.h>
#include <unistd.h> // STDIN_FILENO
#include <map>

//const bool debug=false;

inline void strip3()
{
   char ch;
   read(STDIN_FILENO, &ch, 1);
   read(STDIN_FILENO, &ch, 1);
   read(STDIN_FILENO, &ch, 1);
}


inline void load_mask(uint64_t& mask, uint64_t& set)
{
   // comes to us with 4 bytes stripped already
   // mask = XXXXXXXXXXXXXXXXXXXXXXXXXXXXX1XXXX0X
   strip3(); // remove " = "

   char ch;
   mask = 0;
   set = 0;
   while((read(STDIN_FILENO, &ch, 1) > 0) && (ch != '\n')) // read until eol or eof
   {
      mask <<= 1;
      set  <<= 1;
      if(ch == 'X')
         mask += 1;
      else
         set += ch & 1; // get the low bit from ch
   }
}


inline void load_mem(unsigned& location, uint64_t& value)
{
   // comes to us with 4 bytes stripped already
   // mem[8] = 11
   char ch;
   location = 0;
   while((read(STDIN_FILENO, &ch, 1) > 0) && (ch != ']')) // read until eol or eof
   {
      location *= 10;
      location += ch - '0';
   }

   strip3(); // remove " = "

   value = 0;
   while((read(STDIN_FILENO, &ch, 1) > 0) && (ch != '\n')) // read until eol or eof
   {
      value *= 10;
      value += ch - '0';
   }
}


int main(int,char**)
{
   auto time_start = std::chrono::high_resolution_clock::now();

   uint64_t mask=0;
   uint64_t set=0;
   std::map<unsigned,uint64_t> mem;
   for(;;)
   {
      // read 3 chars
      strip3();
      char ch;
      if(read(STDIN_FILENO, &ch, 1) <= 0)
         break;

      if(ch == 'k')             // mask
         load_mask(mask,set);
      else if(ch == '[')        // mem[
      {
         unsigned loc;
         uint64_t val;
         load_mem(loc,val);
         val = (val & mask) | set;
         mem[loc] = val;
      }
      else
      {
         std::cout << "unexpected characters" << std::endl;
         return 0;
      }
   }

   uint64_t mem_total = 0;
   for(auto [loc, val] : mem)
      mem_total += val;

   std::cout << mem_total << "\n";

   auto time_end = std::chrono::high_resolution_clock::now();
   std::cout << "\ntime: " << std::chrono::duration_cast<std::chrono::microseconds>(time_end-time_start).count() << " us" << std::endl;
   return 0;
}
