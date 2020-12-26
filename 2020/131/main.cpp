#include <iostream>
#include <chrono>
#include <algorithm>
#include <stdint.h>
#include <unistd.h> // STDIN_FILENO

//const bool debug=false;

static_assert(sizeof(unsigned) >= 4, "type to small");


inline unsigned from_ch(char ch)
{
   unsigned temp = ch - '0';
   if(temp > 9)
   {
      std::cerr << "current_time() - unexpected input: " << ch << std::endl;
      exit(-1);
   }
   return temp;
}


inline std::vector<unsigned> load_routes()
{
   std::vector<unsigned> rv;

   unsigned val = 0;
   char ch;
   while((read(STDIN_FILENO, &ch, 1) > 0) && (ch != '\n')) // read until eol or eof
   {
      switch(ch)
      {
         case ',':
            if(val)
            {
               rv.push_back(val);
               val = 0;
            }
            break;
         case 'x':
            // I think we will do something here i part 2
            break;
         default:
            val *= 10;
            val += from_ch(ch);
            break;
      }
   }

   // if val is !0, load it
   if(val)
      rv.push_back(val);
   return rv;
}

inline unsigned load_time()
{
   unsigned rv = 0;
   char ch;
   while((read(STDIN_FILENO, &ch, 1) > 0) && (ch != '\n'))
   {
      rv *= 10;
      rv += from_ch(ch);
   }
   return rv;
}


unsigned next_departure(unsigned now, unsigned route)
{
   unsigned mod = now % route;
   if(!mod)
      return now;
   return route - mod;
}


int main(int,char**)
{
   auto time_start = std::chrono::high_resolution_clock::now();

   unsigned now = load_time();
   std::vector<unsigned> v = load_routes();
   if(v.empty())
   {
      std::cerr << "premature end of input";
      return -1;
   }

   unsigned wait = 1000; // some large number
   unsigned route = 0;

   for(unsigned r: v)
   {
      unsigned n = next_departure(now,r);
      if(n >= wait)
         continue;
      wait = n;
      route = r;
   }

   std::cout
      << "route " << route << " has " << wait << " minute wait.\n"
      << "solution: " << route << " * " << wait << " = " << route*wait << "\n"
      ;

   auto time_end = std::chrono::high_resolution_clock::now();
   std::cout << "\ntime: " << std::chrono::duration_cast<std::chrono::microseconds>(time_end-time_start).count() << " us" << std::endl;
   return 0;
}
