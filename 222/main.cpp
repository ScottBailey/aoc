#include <iostream>
#include <chrono>
#include <algorithm>
#include <stdint.h>
#include <aoc/get.h>            // aoc::get_line()
#include <sb/string.h>          // sb::string::parse(), sb::string::from()

//#define sb_use_deque
//#define sb_use_vector
#define sb_use_circular_buffer

#if defined(sb_use_deque)
# include <deque>
using list_t = std::deque<unsigned>;
#endif

#if defined(sb_use_vector)
# include <vector>
using list_t = std::vector<unsigned>;
#endif

#if defined(sb_use_circular_buffer)
#include <boost/circular_buffer.hpp>
using list_t = boost::circular_buffer<unsigned>;
#endif

#include <boost/container_hash/hash.hpp>

const bool debug=false;



list_t load(std::istream& is=std::cin)
{
   list_t rv;
   // read the player line - we really should validate it
   aoc::get_line(is);
   // read all the player input, exit on a blank line
   for(std::string temp = aoc::get_line(is); !temp.empty(); temp = aoc::get_line(is))
   {
      unsigned val;
      if(sb::string::from(temp,val))
      {
#if defined(sb_use_circular_buffer)
         if(rv.capacity() < (rv.size()+1))
            rv.set_capacity(rv.capacity()+10);
#endif
         rv.insert(rv.begin(),val); // rv.push_front()
      }
   }

   return rv;
}



std::pair<bool,list_t> play1(list_t::iterator lb, list_t::iterator le, list_t::iterator rb, list_t::iterator re)
{
   list_t l;
   list_t r;

#if defined(sb_use_vector)
   size_t max = std::distance(lb,le) + std::distance(rb,re);
   l.reserve(max);
   r.reserve(max);
#endif
#if defined(sb_use_circular_buffer)
   size_t max = std::distance(lb,le) + std::distance(rb,re);
   l.set_capacity(max);
   r.set_capacity(max);
#endif

   while(lb < le)
   {
      l.push_back(*lb);
      ++lb;
   }
   while(rb < re)
   {
      r.push_back(*rb);
      ++rb;
   }

   for(;;)
   {
      if(l.empty())
         return std::make_pair(false,r);
      if(r.empty())
         return std::make_pair(true,l);

      unsigned i=l.back();
      unsigned j=r.back();

      l.pop_back();
      r.pop_back();

      if(i == j)
      {
         std::cerr << "tie!\n\n";
         exit(-1);
      }
      if(i > j)
      {
         l.insert(l.begin(),i); // rv.push_front()
         l.insert(l.begin(),j); // rv.push_front()
      }
      else // (j > i)
      {
         r.insert(r.begin(),j); // rv.push_front()
         r.insert(r.begin(),i); // rv.push_front()
      }
   }
}


// true if player left wins
size_t global_depth=1;
std::pair<bool,list_t> play2(list_t::iterator lb, list_t::iterator le, list_t::iterator rb, list_t::iterator re)
{
   // need to add a hash list here?
   size_t depth = global_depth;
   if(debug)
      ++global_depth;

   size_t round=0;
   if(debug)
      std::clog << "=== Game " << depth << " ===\n";

   list_t l;
   list_t r;

#if defined(sb_use_vector)
   size_t max = std::distance(lb,le) + std::distance(rb,re);
   l.reserve(max);
   r.reserve(max);
#endif

#if defined(sb_use_circular_buffer)
   size_t max = std::distance(lb,le) + std::distance(rb,re);
   l.set_capacity(max);
   r.set_capacity(max);
#endif

   while(lb < le)
   {
      l.push_back(*lb);
      ++lb;
   }
   while(rb < re)
   {
      r.push_back(*rb);
      ++rb;
   }

   std::vector<size_t> hashes;
   for(;;)
   {
      if(r.empty())
      {
         if(debug)
            std::clog << "The winner of game " << depth <<" is player 1!\n";
         return std::make_pair(true,l);
      }
      if(l.empty())
      {
         if(debug)
            std::clog << "The winner of game " << depth <<" is player 2!\n";
         return std::make_pair(false,r);
      }

      // this comes before hashes for debug
      unsigned i=l.back();
      unsigned j=r.back();

      if(debug)
      {
         std::clog
            << "\n-- Round " << ++round << " (Game " << depth <<") --\n"
            << "Player 1's deck: "
            ;
         for(size_t a = l.size()-1; a < l.size(); --a) // rolls over after zero
         {
            std::clog << l[a];
            if(a)
               std::clog << ", ";
         }
         std::clog << "\nPlayer 2's deck: ";
         for(size_t a = r.size()-1; a < r.size(); --a) // rolls over after zero
         {
            std::clog << r[a];
            if(a)
               std::clog << ", ";
         }
         std::clog
            << "\nPlayer 1 plays: " << i
            << "\nPlayer 2 plays: " << j
            << std::endl
            ;
      }


      // deal with hashes here
      {
         size_t temp = 0;
         for(auto a : l) // we must hash it ourselves as hash_value doesn't support circular buffer
            boost::hash_combine(temp,a);
         boost::hash_combine(temp,99); // use 99 as a deck separator
         for(auto a : r) // we must hash it ourselves as hash_value doesn't support circular buffer
            boost::hash_combine(temp,a);
         auto a = std::lower_bound(hashes.begin(),hashes.end(),temp);
         if(a != hashes.end() && *a == temp)
            return std::make_pair(true,l);
         hashes.insert(a,temp);
      }


      l.pop_back();
      r.pop_back();

      if(i <= l.size() && j <= r.size())
      {
         if(debug)
            std::clog << "Playing a sub-game to determine the winner...\n\n";
         auto [left_wins2,deck2] = play2(l.end()-i,l.end(),r.end()-j,r.end());
         if(left_wins2)
         {
            if(debug)
               std::clog
                  << "\n...anyway, back to game " << depth << ".\n"
                  << "Player 1 wins round " << round << " of game " << depth << "!\n"
                  ;
            l.insert(l.begin(),i); // rv.push_front()
            l.insert(l.begin(),j); // rv.push_front()
         }
         else
         {
            if(debug)
               std::clog
                  << "\n...anyway, back to game " << depth << ".\n"
                  << "Player 2 wins round " << round << " of game " << depth << "!\n"
                  ;
            r.insert(r.begin(),j); // rv.push_front()
            r.insert(r.begin(),i); // rv.push_front()
         }
      }
      else if(i == j)
      {
         std::cerr << "tie!\n\n";
         exit(-1);
      }
      else if(i > j)
      {
         if(debug)
            std::clog << "Player 1 wins round " << round << " of game " << depth << "!\n";
         l.insert(l.begin(),i); // rv.push_front()
         l.insert(l.begin(),j); // rv.push_front()
      }
      else // (j > i)
      {
         if(debug)
            std::clog << "Player 2 wins round " << round << " of game " << depth << "!\n";
         r.insert(r.begin(),j); // rv.push_front()
         r.insert(r.begin(),i); // rv.push_front()
      }
   }
}


inline void dump(const list_t& l, std::ostream& os=std::cout)
{
   for(auto a : l)
      os << a << "\n";
}


inline size_t score(const list_t& deck)
{
   uint64_t rv=0;
   for(size_t i=0; i < deck.size(); ++i)
      rv += (deck[i] * (i+1));
   return rv;
}


int main(int,char**)
{
   auto time_start = std::chrono::high_resolution_clock::now();

   auto l = load();
   auto r = load();

   //auto [left_wins1,deck1] = play1(l.begin(),l.end(),r.begin(),r.end());
   //std::cout << score(deck1) << "\n";

   auto [left_wins2,deck2] = play2(l.begin(),l.end(),r.begin(),r.end());

   if(debug)
   {
      l.clear();
      r.clear();
      if(left_wins2)
         l=deck2;
      else
         r=deck2;

      std::clog << "\n\n== Post-game results ==\nPlayer 1's deck:";
      for(size_t a = l.size()-1; a < l.size(); --a) // rolls over after zero
      {
         std::clog << " " << l[a];
         if(a)
            std::clog << ",";
      }
      std::clog << "\nPlayer 2's deck:";
      for(size_t a = r.size()-1; a < r.size(); --a) // rolls over after zero
      {
         std::clog << " " << r[a];
         if(a)
            std::clog << ",";
      }
   }

   std::cout << score(deck2) << "\n";



   auto time_end = std::chrono::high_resolution_clock::now();
   std::cout << "\ntime: " << std::chrono::duration_cast<std::chrono::microseconds>(time_end-time_start).count() << " us" << std::endl;
   return 0;
}
