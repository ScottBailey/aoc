#include <iostream>
#include <chrono>
#include <algorithm>
#include <stdint.h>
#include <vector>
#include <unistd.h> // STDIN_FILENO

// 23009

const bool debug=false;

class rule_pair
{
public:
   using value_type = unsigned;
   using self_type = rule_pair;
   using list_t=std::vector<rule_pair>;

   rule_pair() : m_begin{1}, m_last{0} {}  // invalid
   rule_pair(value_type f, value_type l) : m_begin{f}, m_last{l} {}

   bool valid() const { return m_last >= m_begin; }

   value_type begin() const { return m_begin; }
   value_type last() const { return m_last; }
   value_type end() const { return m_last+1; }


   /*
   // *this must intersect with rhs
   //rule_pair union(const rule_pair& rhs) const
   rule_pair combine(const rule_pair& rhs) const
   {
      return rule_pair(std::min(m_begin,rhs.m_begin), std::max(m_last,rhs.m_last));
   }
   */

   bool contains(value_type n) const
   {
      return (m_begin <= n) && (n <=m_last);
   }

   bool operator<(const self_type& rhs) const
   {
      return m_begin < rhs.m_begin;
   }

   void print(std::ostream& os) const
   {
      if(!valid())
         os << "invalid";
      else
         os << m_begin << "-" << m_last;
   }

   static void compress(list_t& v)
   {
      if(v.empty())
         return;
      if(debug)
         std::cout << "\n";
      dump(v);
      std::sort(v.begin(), v.end());
      dump(v);
      auto l = v.begin();
      while(l != v.end())
      {
         auto r = l+1;
         while((r != v.end()) && (l->end() >= r->begin()))
            ++r;
         if(std::distance(l,r) <= 1)
            l = r;
         else
         {
            l->m_last = std::max(l->m_last,(r-1)->m_last);
            l = v.erase(l+1,r);
         }
      }
      dump(v);
   }

   static self_type load()
   {
      // xxx-yyy
      char ch;
      value_type l=0;
      while(read(STDIN_FILENO, &ch, 1) > 0)
      {
         unsigned temp = ch - '0';
         if(temp > 9)
            break;
         l *= 10;
         l += temp;
      }

      value_type r=0;
      while(read(STDIN_FILENO, &ch, 1) > 0)
      {
         unsigned temp = ch - '0';
         if(temp > 9)
            break;
         r *= 10;
         r += temp;
      }
      return self_type(l,r);
   }

   static bool contains(const list_t& list, value_type n)
   {
      for(const auto& a : list)
      {
         if(a.contains(n))
            return true;
      }
      return false;
   }

   static void dump(const list_t& l)
   {
      if(!debug)
         return;
      for(const auto& a : l)
      {
         a.print(std::cout);
         std::cout << " ";
      }
      std::cout << "\n";
   }
private:
   value_type m_begin;
   value_type m_last;
};
std::ostream& operator<<(std::ostream& os, const rule_pair& o) { o.print(os); return os; }

struct rule
{
   std::string name;
   rule_pair left;
   rule_pair right;
};


inline void dump(const rule_pair& l, const rule_pair& r)
{
   if(!debug)
      return;

   std::cout << l << " " << r << "\n";
}


inline void dump(const rule_pair::list_t& l)
{
   rule_pair::dump(l);
}



std::vector<rule_pair> load_rules()
{
   // assumes well formed input

   // read each rule and push it into the return vector.
   // compress (optimize) the rules before returning

   rule_pair::list_t rv;

   for(;;)
   {
      // "name: x1-x2 or y1-y2"
      std::string name;
      char ch;

      // load name
      while(read(STDIN_FILENO, &ch, 1) > 0)
      {
         if(ch == ':')
            break;
         if(ch == '\n')
         {
            rule_pair::compress(rv);
            return rv;          // <-------------- return point
         }
         name.push_back(ch);
      }

      // skip trailing space in "name: "
      read(STDIN_FILENO, &ch, 1);

      // read first rule pair
      rule_pair first = rule_pair::load();

      // skip trailing "or "
      char cha[3];
      read(STDIN_FILENO, cha, 3);

      // read second rule pair
      rule_pair second = rule_pair::load();

      dump(first,second);

      rv.push_back(first);
      rv.push_back(second);
   }
}


inline void skip_line()
{
   char ch;
   while((read(STDIN_FILENO, &ch, 1) > 0) && (ch != '\n')) // read until eof or eol
      ;
}


std::vector<unsigned> load_ticket()
{
   std::vector<unsigned> rv;
   rv.reserve(20);

   char ch;
   unsigned value=0;
   while(read(STDIN_FILENO, &ch, 1) > 0) // read until eof
   {
      unsigned temp = ch - '0';
      if(temp < 10)
      {
         value *= 10;
         value += temp;
      }
      else
      {
         rv.push_back(value);
         if(ch == '\n')
            break;
         value = 0;
      }
   }
   return rv;
}


int main(int,char**)
{
   auto time_start = std::chrono::high_resolution_clock::now();

   auto rules = load_rules();

   // skip "your ticket:\n"
   skip_line();
   load_ticket();  // skip value for part1

   // skip empty line and "nearby tickets:\n"
   skip_line();
   skip_line();

   uint64_t error_rate = 0;
   for(;;)
   {
      auto v = load_ticket();
      if(v.empty())
         break;
      for(auto n : v)
      {
         if(rule_pair::contains(rules,n))
            continue;
         // could not find n in the rule, that's an "error"
         error_rate += n;
      }
   }

   std::cout << error_rate << "\n";

   auto time_end = std::chrono::high_resolution_clock::now();
   std::cout << "\ntime: " << std::chrono::duration_cast<std::chrono::microseconds>(time_end-time_start).count() << " us" << std::endl;
   return 0;
}
