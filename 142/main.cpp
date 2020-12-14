#include <iostream>
#include <chrono>
#include <algorithm>
#include <stdint.h>
#include <unistd.h> // STDIN_FILENO
#include <map>

// it's not 2650717285842

const bool debug=false;

inline void strip3()
{
   char ch;
   read(STDIN_FILENO, &ch, 1);
   read(STDIN_FILENO, &ch, 1);
   read(STDIN_FILENO, &ch, 1);
}


inline void load_mask(uint64_t& mask, std::vector<uint64_t>& sets)
{
   // comes to us with 4 bytes stripped already
   // mask = XXXXXXXXXXXXXXXXXXXXXXXXXXXXX1XXXX0X
   strip3(); // remove " = "

   sets.clear();
   sets.push_back(0); // preload with zero value

   mask = 0;
   char ch;
   while((read(STDIN_FILENO, &ch, 1) > 0) && (ch != '\n')) // read until eol or eof
   {
      mask <<= 1;
      for(auto& a : sets)
         a <<= 1;
      switch(ch)
      {
         case '0':
            mask += 1;
            break;
         case '1':
            for(auto& a : sets)
               a += 1;
            break;
         case 'X':
            {
               // the zero value is already added to the set, so we just need to push back the one..
               size_t sz = sets.size();
               sets.reserve(sz*2);
               for(size_t i=0; i < sz; ++i)
                  sets.push_back(sets[i] + 1);
            }
            break;
      }
   }
   //if(sets.size() == 1)
   // std::cerr << "empty-ish..." << std::endl;
}


inline void load_mem(uint64_t& location, uint64_t& value)
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
   std::vector<uint64_t> sets;
   std::map<unsigned,uint64_t> mem;
   for(;;)
   {
      // read 3 chars
      strip3();
      char ch;
      if(read(STDIN_FILENO, &ch, 1) <= 0)
         break;

      if(ch == 'k')             // mask
         load_mask(mask,sets);
      else if(ch == '[')        // mem[
      {
         uint64_t loc;
         uint64_t val;
         load_mem(loc,val);
         for(auto s : sets)
            mem[ (loc & mask) | s ] = val;

         if(debug)
         {
            for(auto [l,v] : mem)
               std::cout << l << " : " << v << "\n";
            std::cout << "\n";
         }
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
