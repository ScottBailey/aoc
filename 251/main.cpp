#include <iostream>
#include <chrono>
#include <algorithm>
#include <stdint.h>
#include <aoc/get.h>            // aoc::get_line()
#include <sb/string.h>          // sb::string::parse(), sb::string::from()
#include <array>

constexpr bool debug=false;

/*
  Notes:

  Given public keys x and y


*/


using list_t=std::array<uint64_t,2>;
std::ostream& operator<<(std::ostream& os, const list_t& l)
{
   for(size_t i=0; i < l.size(); ++i)
   {
      if(i)
         os << ", ";
      os << l[i];
   }
   return os;
}


list_t load(std::istream& is=std::cin)
{
   list_t rv;
   list_t::value_type temp;
   size_t i=0;
   while(is >> temp)
   {
      if(i >= rv.size())
         throw std::invalid_argument("exceeded 2 input values");
      rv[i] = temp;
      ++i;
   }
   if(i != rv.size())
      throw std::invalid_argument("failed to receive 2 input values");

   return rv;
}


inline uint64_t find_loop(uint64_t subject_num, const uint64_t public_key, const uint64_t mod_num = 20201227)
{
   uint64_t loop=0;
   uint64_t val=1;
   for(;;)
   {
      ++loop;
      val *= subject_num;
      val %= mod_num;
      if(val == public_key)
         return loop;

      // There should be an exit criteria, I believe, but I'm unsure what. cclearly, it's not this: 'if(val > public_key) break;'
   }

   return 0;
}


inline uint64_t find_key(uint64_t loop1, const uint64_t public_key2, const uint64_t mod_num = 20201227)
{
   uint64_t val = 1;
   for( ;loop1; --loop1)
   {
      val *= public_key2; // subject_num!!!
      val %= mod_num;
   }

   return val;
}


uint64_t part1(const list_t& public_keys)
{
   // We have the public keys:
   if(debug)
      std::clog << "public keys: " << public_keys << "\n";

   // And the subject numbers:
   constexpr list_t subject_nums{7,7};
   if(debug)
      std::clog << "subject nums: " << subject_nums << "\n";

   // Lets just find the loop value (keep in mind we really only need to find the loop value for the first number)
   /*
   // this code (unnecessarily) finds both loop values:
   list_t loops;
   for(size_t i=0; i < public_keys.size(); ++i)
      loops[i] = find_loop(subject_nums[i],public_keys[i]);
   if(debug)
      std::clog << "loop values: " << loops << "\n";
   */

   uint64_t loop0 = find_loop(subject_nums[0],public_keys[0]);
   if(debug)
      std::clog << "loop0 value: " << loop0 << "\n";

   // And now the shared...
   uint64_t encryption_key = find_key(loop0,public_keys[1]);
   if(debug)
      std::clog << "shared encryption key: " << encryption_key << "\n";

   return encryption_key;
}


int main(int,char**)
{
   auto time_start = std::chrono::high_resolution_clock::now();

   auto in = load();
   std::cout << "Part 1 solution: "<< part1(in) << "\n";

   auto time_end = std::chrono::high_resolution_clock::now();
   std::cout << "\ntime: " << std::chrono::duration_cast<std::chrono::microseconds>(time_end-time_start).count() << " us" << std::endl;
   return 0;
}
