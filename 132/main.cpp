#include <iostream>
#include <chrono>
#include <algorithm>
#include <stdint.h>
#include <unistd.h> // STDIN_FILENO

const bool debug=true;

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

struct info
{
   unsigned index;
   unsigned route;
};
bool operator<(const info& l, const info& r) { return l.route < r.route; }


inline std::vector<info> load_routes()
{
   std::vector<info> rv;

   unsigned i=0;
   unsigned val = 0;
   char ch;

   // strip time
   while((read(STDIN_FILENO, &ch, 1) > 0) && (ch != '\n'))
      ;

   // read routes
   while((read(STDIN_FILENO, &ch, 1) > 0) && (ch != '\n')) // read until eol or eof
   {
      switch(ch)
      {
         case ',':
            if(val)
            {
               rv.push_back( info{i,val});
               ++i;
               val = 0;
            }
            break;
         case 'x':
            ++i;
            break;
         default:
            val *= 10;
            val += from_ch(ch);
            break;
      }
   }

   // if val is !0, load it
   if(val)
      rv.push_back( info{i,val});
   return rv;
}


inline void dump(bool d, std::vector<info> v)
{
   if(!d)
      return;
   for(const auto& a : v)
      std::cout << a.index << " :  " << a.route << std::endl;
}


int main(int,char**)
{
   auto time_start = std::chrono::high_resolution_clock::now();

   std::vector<info> v = load_routes();
   if(v.empty())
   {
      std::cerr << "premature end of input";
      return -1;
   }

   dump(debug,v);

   // brute force approach
   std::sort(v.begin(), v.end());

   // get the largest route
   auto largest = v.back();
   v.pop_back();

   if(largest.index > largest.route)
   {
      std::cerr << "problem with expected input" << std::endl;
      return -1;
   }

   uint64_t when = largest.route - largest.index;
   for(;;)
   {
      bool found = true;
      for(const auto& a : v)
      {
         uint64_t mod = (when+a.index)%a.route;
         if(!mod)
            continue;
         found = false;
         break;
      }

      if(found)
         break;

      uint64_t last_when = when;
      when += largest.route;
      if(when < last_when)
      {
         std::cerr << "overflow" << std::endl;
         return -1;
      }
   }

   std::cout << when << std::endl;

   auto time_end = std::chrono::high_resolution_clock::now();
   std::cout << "\ntime: " << std::chrono::duration_cast<std::chrono::microseconds>(time_end-time_start).count() << " us" << std::endl;
   return 0;
}
