#include <iostream>
#include <chrono>
#include <algorithm>
#include <stdint.h>
#include <vector>
#include <unistd.h> // STDIN_FILENO

const bool debug=false;

using ticket_t = std::vector<unsigned>;
using ticket_list_t = std::vector<ticket_t>;

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
      std::sort(v.begin(), v.end());
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

private:
   value_type m_begin;
   value_type m_last;
};
std::ostream& operator<<(std::ostream& os, const rule_pair& o) { o.print(os); return os; }

class rule
{
public:
   using self_type = rule;
   using value_type = rule_pair::value_type;
   using list_t = std::vector<self_type>;

   std::string name;
   rule_pair left;
   rule_pair right;

   bool contains(value_type n) const { return left.contains(n) || right.contains(n); }
   void print(std::ostream& os) const { os << name << ": " << left << " or " << right; }
   bool valid() const { return !name.empty(); }
   bool operator==(const self_type& rhs) const { return name == rhs.name; }
   bool operator!=(const self_type& rhs) const { return name != rhs.name; }
};
std::ostream& operator<<(std::ostream& os, const rule& r) { r.print(os); return os; }


rule::list_t load_rules()
{
   // assumes well formed input

   // read each rule and push it into the return vector.
   // compress (optimize) the rules before returning

   rule::list_t rv;

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
            return rv;          // <-------------- return point
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

      rv.emplace_back( rule{name,first,second});
   }
}


rule_pair::list_t make_rule_pair_list(const rule::list_t& l)
{
   rule_pair::list_t rv;
   for(const auto& a : l)
   {
      rv.push_back(a.left);
      rv.push_back(a.right);
   }
   rule_pair::compress(rv);
   return rv;
}


inline void skip_line()
{
   char ch;
   while((read(STDIN_FILENO, &ch, 1) > 0) && (ch != '\n')) // read until eof or eol
      ;
}


ticket_t load_ticket()
{
   ticket_t rv;
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

   // skip "your ticket:\n", but load it
   skip_line();
   ticket_t my_ticket = load_ticket();

   // skip empty line and "nearby tickets:\n", we will look at each one and save the valid ones
   skip_line();
   skip_line();
   ticket_list_t valid_tickets;
   {
      auto quick_rules = make_rule_pair_list(rules);

      for(;;)
      {
         auto v = load_ticket();
         if(v.empty())
            break;
         bool good = true;
         for(auto n : v)
         {
            if(rule_pair::contains(quick_rules,n))
               continue;
            // could not find n in the rule, that's an "error"
            good = false;
            break;
         }
         if(good)
            valid_tickets.push_back(v);
      }
   }

   // now, by process of elimination, determine which column must correspond with which label
   {
      if(rules.size() != valid_tickets[0].size())
      {
         std::cerr << "bad assumption: rules.size() " << rules.size() << "  valid_tickets[0].size() " << valid_tickets[0].size() << "\n";
         for(auto a : valid_tickets)
         {
            for(auto b : a)
               std::cout << b << " ";
            std::cout << "\n";
         }
         return -1;
      }
      size_t sz = rules.size();

      std::vector<std::vector<rule>> rule_columns(sz,rules); // copy all the rules into a vector for every column in the ticket

      for(size_t i=0; i < sz; ++i) // for every column
      {
         // we will test all the possible rules in column i against all the tickets values for column i
         std::vector<rule>& possible_list = rule_columns[i]; // for clarity

         for(const ticket_t& t : valid_tickets) // test every single ticket!
         {
            unsigned n = t[i];  // the value in the ith column for this ticket
            for(rule::list_t::iterator j = possible_list.begin(); j != possible_list.end(); ) // j updated inside loop
            {
               if( j->contains(n) )
                  ++j;
               else
                  j = possible_list.erase(j);
            }
         }
      }

      // at this time we are left with a set of possible rules for each column


      // for each rule, see if it can exist in only one column

      // create a list of invalid rules and swap it with the result list
      rule::list_t temp(sz,rule{});
      rules.swap(temp);

      while(!temp.empty())
      {
         size_t column = sz;  // invalid value, used to indicate found state
         rule r;
         // if there exists a column with only a single rule
         for(size_t i=0; (i < sz); ++i)
         {
            if(rule_columns[i].size() != 1)
               continue;
            column = i;
            r = rule_columns[i].front();
            rule_columns[i].clear();
            break;
         }

         // or a rule with only a single column
         if(column == sz)
         {
            for(const rule& a : temp) // for every remaining rule...
            {
               size_t temp_column = sz;
               for(size_t i = 0; i < sz; ++i) // ...iterate over every column...
               {
                  // ...to see this rule is found in that column...
                  for(const rule& b :rule_columns[i])
                  {
                     if(a != b)
                        continue;
                     // found it for the first time
                     if(temp_column == sz)
                     {
                        temp_column = i;
                        break;
                     }
                     // found it for the second time, a is not a valid rule
                     // set the exit continue on i
                     // clear the temp
                     // and break
                     i = sz;
                     temp_column = sz;
                     break;
                  }
               }
               if(temp_column < sz)
               {
                  column = temp_column;
                  r = a;
                  break;
               }
            }
         }
         if((column >= sz) || !r.valid())
         {
            std::cerr << "failed to find a solution\n";
            return -1;
         }
         // store the rule in the final location
         rules[column] = r;
         // and remove it from the temp and rule_column
         std::erase(temp,r);
         for(auto& a : rule_columns)
            std::erase(a,r);
      }
   }

   if(debug)
   {
      for(size_t i = 0; i < rules.size(); ++i)
         std::cout << "column " << i << ": " << rules[i].name << "\n";
      std::cout << "\n";
   }

   uint64_t product = 1;
   for(size_t i = 0; i < rules.size(); ++i)
   {
      if(rules[i].name.find("departure") == std::string::npos)
         continue;
      product *= my_ticket[i];
      if(debug)
         std::cout << rules[i].name << " " << my_ticket[i] << " " << product << "\n";

   }

   std::cout << product << "\n";

   auto time_end = std::chrono::high_resolution_clock::now();
   std::cout << "\ntime: " << std::chrono::duration_cast<std::chrono::microseconds>(time_end-time_start).count() << " us" << std::endl;
   return 0;
}
