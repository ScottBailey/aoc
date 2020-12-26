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


int main(int,char**)
{
   auto time_start = std::chrono::high_resolution_clock::now();


   long n =0;
   long e =0;

   long wn =1;
   long we =10;


   char cmd;
   long val;
   while( std::cin >> cmd >> val)
   {
      switch(cmd)
      {
         case 'N': wn += val; break;
         case 'S': wn -= val; break;
         case 'E': we += val; break;
         case 'W': we -= val; break;
         case 'L':
            val = norm(-val);
            // fallthrough
         case 'R':
            if(val == 180)
            {
               we = -we;
               wn = -wn;
            }
            else if(val == 90)
            {
               long temp = wn;
               wn = -we;
               we = temp;
            }
            else if(val == 270)
            {
               long temp = wn;
               wn = we;
               we = -temp;
            }
            else
            {
               std::cerr << "bad rotation" << std::endl;
               return -1;
            }
            break;
         case 'F':
            {
               e += we * val;
               n += wn * val;
            }
            break;
         default: std::cerr << "Bad cmd? " << cmd << std::endl; return -1;
      }
   }

   std::cout << std::abs(e) + std::abs(n) << std::endl;

   auto time_end = std::chrono::high_resolution_clock::now();
   std::cout << "\ntime: " << std::chrono::duration_cast<std::chrono::microseconds>(time_end-time_start).count() << " us" << std::endl;
   return 0;
}
