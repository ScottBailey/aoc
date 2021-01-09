#include <iostream>
#include <chrono>
#include <algorithm>
#include <stdint.h>
#include <aoc/get.h>            // aoc::get_line()
#include <sb/string.h>          // sb::string::parse(), sb::string::from()
#include <sb/ansi.h>            // ansi colors: sb::red, sb::yellow, etc
#include <sb/ignore.h>          // sb::string::parse(), sb::string::from()
#include <vector>
#include <numeric>
#include <aoc/life.h>            // aoc::life::play()

//const bool debug=false;

const size_t max{100};

class node_t
{
public:
   using list_t=std::vector<node_t>;

   node_t(unsigned x, unsigned y)
      : m_x{x}, m_y{y}
   {}

   bool is_valid() const
   {
      return m_x < max && m_y < max;
   }

   list_t adjacent() const // return the neighbors of this node
   {
      list_t temp
      {
         node_t(m_x-1, m_y+1),
         node_t(m_x,   m_y+1),
         node_t(m_x+1, m_y+1),
         node_t(m_x+1, m_y),
         node_t(m_x+1, m_y-1),
         node_t(m_x,   m_y-1),
         node_t(m_x-1, m_y-1),
         node_t(m_x-1, m_y)
      };

      list_t rv;
      rv.reserve(temp.size());
      for(const auto& a : temp)
      {
         if(a.is_valid())
            rv.push_back(a);
      }
      return rv;
   }

   static bool state(bool current_state, size_t adjacent_on_nodes)
   {
      if(current_state)
         return adjacent_on_nodes == 2 || adjacent_on_nodes == 3;
      return adjacent_on_nodes == 3;
   }

   bool operator<(const node_t& rhs) const
   {
      if(m_y == rhs.m_y)
         return m_x < rhs.m_x;
      return m_y < rhs.m_y;
   }

   bool operator==(const node_t& rhs) const
   {
      return m_x == rhs.m_x && m_y == rhs.m_y;
   }

private:
   unsigned m_x = std::numeric_limits<unsigned>::max();
   unsigned m_y = std::numeric_limits<unsigned>::max();
};



using container_t = node_t::list_t;

std::ostream& operator<<(std::ostream& os, const container_t& list)
{
   for(size_t y=0; y < max; ++y)
   {
      for(size_t x=0; x < max; ++x)
      {
         if(std::binary_search(list.begin(),list.end(),node_t(x,y)))
            os << "#";
         else
            os << ".";
      }
      os << "\n";
   }

   return os;
}


container_t load(std::istream& is=std::cin)
{
   container_t rv;

   size_t x=0;
   size_t y=0;
   char ch;
   while(is.get(ch))
   {
      switch(ch)
      {
         case '\n': ++y; x = 0; break;
         case '.': ++x; break;
         case '#': rv.push_back( node_t(x,y) ); ++x; break;
         default:
            throw std::invalid_argument("bad input");
      }
   }

   return rv;
}


void part1(const container_t& list)
{
   container_t result = list;
   for(size_t i=0; i < 100; ++i)
      aoc::life::play<node_t>(result);
   std::cout << sb::white << "Part 1: " << sb::reset << result.size() << "\n";
}


void part2(const container_t& list)
{
   int result = list.size();

   std::cout << sb::white << "Part 2: " << sb::reset << result << "\n";
}


int main(int,char**)
{
   auto time0 = std::chrono::high_resolution_clock::now();

   container_t list = load();

   auto time1 = std::chrono::high_resolution_clock::now();

   part1(list);

   auto time2  = std::chrono::high_resolution_clock::now();

   part2(list);

   auto time3  = std::chrono::high_resolution_clock::now();


   std::cout
      << sb::white
      << "\nload time:  " << std::chrono::duration_cast<std::chrono::microseconds>(time1-time0).count() << " us\n"
      << "part1 time: " << std::chrono::duration_cast<std::chrono::microseconds>(time2-time1).count() << " us\n"
      << "part2 time: " << std::chrono::duration_cast<std::chrono::microseconds>(time3-time2).count() << " us\n"
      << std::flush
      ;
   return 0;
}
