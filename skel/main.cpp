#include <iostream>
#include <chrono>
#include <algorithm>
#include <stdint.h>

//const bool debug=false;

int main(int,char**)
{
   auto time_start = std::chrono::high_resolution_clock::now();


   auto time_end = std::chrono::high_resolution_clock::now();
   std::cout << "\ntime: " << std::chrono::duration_cast<std::chrono::microseconds>(time_end-time_start).count() << " us" << std::endl;
   return 0;
}
