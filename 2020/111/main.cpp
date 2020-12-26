#include <iostream>
#include <chrono>
#include <algorithm>
#include <stdint.h>
#include <unistd.h> // STDIN_FILENO

// 2265

const bool debug=false;

using row_t = std::vector<unsigned>;
using seats_t = std::vector<row_t>;


enum : unsigned {
   empty = 0,
   occupied = 1,
   floor = 2,
};


seats_t load()
{
   seats_t rv;

   size_t sz = 0;
   // get the first input row
   {
      row_t r;
      r.push_back(floor);   // dummy value in position 0
      char ch;
      while((read(STDIN_FILENO, &ch, 1) > 0) && (ch != '\n'))
      {
         switch(ch)
         {
            case '.': r.push_back(floor); break;
            case 'L': r.push_back(empty); break;
         }
      }
      r.push_back(floor); //dummy value in final position
      sz = r.size();

      // store in a dummy row first
      rv.emplace_back( row_t(sz,empty) );

      // then store the valid row
      rv.push_back(r);
   }

   // get and store all the rest of the input rows
   for(;;)
   {
      row_t r;
      r.reserve(sz);
      r.push_back(floor);   // dummy value in column 0
      char ch;
      while((read(STDIN_FILENO, &ch, 1) > 0) && (ch != '\n'))
      {
         switch(ch)
         {
            case '.': r.push_back(floor); break;
            case 'L': r.push_back(empty); break;
         }
      }
      r.push_back(floor); //dummy value in final column

      if(r.size() != sz)
      {
         if(r.size() != 2) // 2 occurs at empected end of data, so it's not an error
            std::cerr << "Bad size for input" << std::endl;
         break;
      }

      rv.push_back(r);
   }

   // store in a final dummy row
   rv.emplace_back( row_t(sz,empty) );

   return rv;
}


inline unsigned sum_adjacent(const seats_t& in, const size_t r, const size_t c)
{
   return
      (in[r-1][c-1] & occupied)
      + (in[r-1][c] & occupied)
      + (in[r-1][c+1] & occupied)
      + (in[r][c-1] & occupied)
      + (in[r][c+1] & occupied)
      + (in[r+1][c-1] & occupied)
      + (in[r+1][c] & occupied)
      + (in[r+1][c+1] & occupied)
      ;
}

void update(const seats_t& in, seats_t& out)
{
   // Remember to ignore the dummy columns and rows!
   size_t szr = in.size() - 1;
   size_t szc = in[0].size() - 1;
   for(size_t r = 1; r < szr; ++r)
   {
      for(size_t c = 1; c < szc; ++c)
      {
         unsigned v = in[r][c];
         if(v == floor)
            continue;
         unsigned sum = sum_adjacent(in,r,c);
         if(v == empty)
         {
            // becomes occupied if all adjacent seats are empty
            if(!sum)
               out[r][c] = occupied;
            else
               out[r][c] = empty;
         }
         else // if(v == occupied)
         {
            // 4 or more adjacent seats will switch to empty
            if(sum < 4)
               out[r][c] = occupied;
            else
               out[r][c] = empty;
         }
      }
   }
}


inline void dump(bool d, const seats_t& in)
{
   if(!d)
      return;
   // Remember to ignore the dummy columns and rows!
   size_t szr = in.size() - 1;
   size_t szc = in[0].size() - 1;
   for(size_t r = 1; r < szr; ++r)
   {
      for(size_t c = 1; c < szc; ++c)
      {
         switch(in[r][c])
         {
            case floor:    std::cout << '.'; break;
            case empty:    std::cout << 'L'; break;
            case occupied: std::cout << '#'; break;
            default:       std::cout << 'X'; break;
         }
      }
      std::cout << "\n";
   }
   std::cout << "\n";
}


unsigned count(const seats_t& in)
{
   unsigned rv = 0;
   // Remember to ignore the dummy columns and rows!
   size_t szr = in.size() - 1;
   size_t szc = in[0].size() - 1;
   for(size_t r = 1; r < szr; ++r)
   {
      for(size_t c = 1; c < szc; ++c)
         rv += in[r][c] & occupied;
   }
   return rv;
}



int main(int,char**)
{
   auto time_start = std::chrono::high_resolution_clock::now();

   seats_t s1 = load();
   seats_t s2 = s1;

   size_t n=0;
   for(;;)
   {
      dump(debug,s1);
      // s1 transforms into s2
      update(s1,s2);
      dump(debug,s2);
      ++n;
      if(s1 == s2)
         break;
      // s2 transforms into s1
      update(s2,s1);
      ++n;
      if(s1 == s2)
         break;
   }

   std::cout << "Stabilized at " << count(s1) << " occupied seats after " << n << " iterations..\n";


   auto time_end = std::chrono::high_resolution_clock::now();
   std::cout << "\ntime: " << std::chrono::duration_cast<std::chrono::microseconds>(time_end-time_start).count() << " us" << std::endl;
   return 0;
}
