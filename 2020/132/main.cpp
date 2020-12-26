#include <iostream>
#include <chrono>
#include <algorithm>
#include <stdint.h>
#include <unistd.h> // STDIN_FILENO
#include <thread>

// 530015546283687

const bool debug=false;

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


uint64_t brute_slow(const std::vector<info>& v_in)
{
   std::vector<info> v = v_in;

   // brute force approach
   std::sort(v.begin(), v.end());

   // get the largest route
   auto largest = v.back();
   v.pop_back();

   if(largest.index > largest.route)
   {
      std::cerr << "problem with expected input" << std::endl;
      exit(-1);
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
         return when;

      when += largest.route;
   }
}


uint64_t brute_faster(const std::vector<info>& v_in)
{
   std::vector<info> v = v_in;

   // sanity check
   if(v.front().index != 0)
   {
      std::cerr << "expected zero index" << std::endl;
      exit(-1);
   }

   // get r0 and remove it from the list
   uint64_t r0 = v.front().route;
   v.erase(v.begin());

   // Now we are going to find a bus that leaves bus0's route minutes after r0.
   uint64_t re = 0;
   for(auto i=v.begin(); i != v.end(); ++i)
   {
      if(i->index % r0)
         continue;
      re = i->route;
      v.erase(i);
      break;
   }
   if(!re)
   {
      std::cerr << "failed to find multiplier" << std::endl;
      exit(-1);
   }

   // the additive value (mult) is r0 times the found route.
   uint64_t mult = r0*re;
   uint64_t when = mult-r0; // The initial value is actually r0*(re-1) - the first instance where time%r0 = 0 and (time+re.index)%re = 0!

   // iterate until you find it
   for(;;)
   {
      bool found = true;
      for(const auto& a : v)
      {
         if(((when+a.index) % a.route) == 0)
            continue;
         found = false;
         break;
      }
      if(found)
         return when;

      when += mult;
   }
}


void brute_thread_i(const std::chrono::time_point<std::chrono::high_resolution_clock>& time_start,
      const std::vector<info>& v, uint64_t when, uint64_t add)
{
   for(;;)
   {
      bool found = true;
      for(const auto& a : v)
      {
         if(((when+a.index) % a.route) == 0)
            continue;
         found = false;
         break;
      }
      if(found)
      {
         std::cout << "Found: " << when << "\n";

         auto time_end = std::chrono::high_resolution_clock::now();
         std::cout << "\ntime: " << std::chrono::duration_cast<std::chrono::microseconds>(time_end-time_start).count() << " us\n";
         exit(0);
      }

      when += add;
   }
}


void brute_thread(const std::chrono::time_point<std::chrono::high_resolution_clock>& time_start, const std::vector<info>& v_in)
{
   std::vector<info> v = v_in;

   // sanity check
   if(v.front().index != 0)
   {
      std::cerr << "expected zero index" << std::endl;
      exit(-1);
   }

   // get r0 and remove it from the list
   uint64_t r0 = v.front().route;
   v.erase(v.begin());

   uint64_t re = 0;
   for(auto i=v.begin(); i != v.end(); ++i)
   {
      if(i->index % r0)
         continue;
      re = i->route;
      v.erase(i);
      break;
   }
   if(!re)
   {
      std::cerr << "failed to find multiplier" << std::endl;
      exit(-1);
   }

   uint64_t mult = r0*re;
   uint64_t when = mult-r0;

   size_t tc=8; // thread count
   std::vector<std::thread> threads;
   threads.reserve(tc);

   for(size_t i=0; i < tc; ++i)
      threads.emplace_back( [&](){ brute_thread_i(time_start, v, when + (mult*i), mult*tc); } );

   threads[0].join();
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

   //uint64_t when = brute_faster(v);
   //std::cout << when << std::endl;

   brute_thread(time_start,v);

   auto time_end = std::chrono::high_resolution_clock::now();
   std::cout << "\ntime: " << std::chrono::duration_cast<std::chrono::microseconds>(time_end-time_start).count() << " us" << std::endl;
   return 0;
}
