#include <iostream>
#include <chrono>
#include <algorithm>
#include <stdint.h>

//const bool debug=false;

enum : long
{
   north =0,
   north_east =45,
   east =90,
   south_east =135,
   south =180,
   south_west =225,
   west =270,
   north_west =315,
};


inline long norm(const long& dir)
{
   return (dir + 360) % 360;
}


inline void move(long facing, long dist, long& e, long& n)
{
   switch(facing)
   {
      case north: n += dist; return;
      case south: n -= dist; return;
      case east:  e += dist; return;
      case west:  e -= dist; return;
   }
   std::cout << "bad move" << std::endl;
   exit(-1);
}


int main(int,char**)
{
   auto time_start = std::chrono::high_resolution_clock::now();


   long n =0;
   long e =0;
   long facing = east;


   char cmd;
   long val;
   while( std::cin >> cmd >> val)
   {
      switch(cmd)
      {
         case 'N': n += val; break;
         case 'S': n -= val; break;
         case 'E': e += val; break;
         case 'W': e -= val; break;
         case 'L': facing = norm(facing - val); break;
         case 'R': facing = norm(facing + val); break;
         case 'F': move(facing, val, e, n); break;
         default: std::cerr << "Bad cmd? " << cmd << std::endl; return -1;
      }
   }

   std::cout << std::abs(e) + std::abs(n) << std::endl;

   auto time_end = std::chrono::high_resolution_clock::now();
   std::cout << "\ntime: " << std::chrono::duration_cast<std::chrono::microseconds>(time_end-time_start).count() << " us" << std::endl;
   return 0;
}
