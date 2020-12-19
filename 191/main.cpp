#include <iostream>
#include <chrono>
#include <algorithm>
#include <stdint.h>
#include <map>
#include <vector>
#include <string>
#include <sb/string/from.h>
#include <sb/string/parse.h>
#include <aoc/get.h>

inline void exit_message(const std::string& s)
{
   std::cerr << "error: " << s << std::endl;
   exit(-1);
}


class rule
{
public:
   static void load(std::istream& is);
   static bool test(const std::string& s)
   {
      size_t offset = 0;
      return m_map.at(0).test(s,offset) && (offset == s.size());
   }

   static void dump()
   {
      std::ostream& os = std::cout;
      for(const auto&[rule_number,r] : m_map)
      {
         os << rule_number << ": ";

         if(r.m_char)
         {
            os << '"' << r.m_char << "\"\n";
            continue;
         }

         for(size_t i = 0; i < r.m_rules.size(); ++i)
         {
            if(i)
               os << "| ";
            for( auto n : r.m_rules[i])
               os << n << " ";
         }
         os << "\n";
      }
   }

private:
   using rule_t = std::vector<unsigned>;
   using rule_list_t = std::vector<rule_t>;

   rule(char c) : m_char{c} {}
   rule(const rule_list_t& r ) : m_char{0}, m_rules{r} {}

   bool test(const std::string& data, size_t& offset) const;

   char m_char;
   rule_list_t m_rules;

   using map_t=std::map<unsigned,rule>;
   static map_t m_map;
};

rule::map_t rule::m_map;


void rule::load(std::istream& is)
{
   for(std::string s = aoc::get_line(is); !s.empty(); s = aoc::get_line(is))
   {
      auto strings = sb::string::parse(s,":|");
      if(strings.size() < 2)
      {
         std::cerr << "unexpected input:" << s << "\n";
         exit(-1);
      }

      // get the rule number
      unsigned rule_number;
      if(!sb::string::from(strings[0], rule_number))
         exit_message("decoding rule_number");

      // test for termination
      size_t quote_loc = strings[1].find('"');
      if(quote_loc != std::string_view::npos)
      {
         ++quote_loc;
         if(quote_loc >= strings[1].size())
            exit_message("decoding rule");
         m_map.insert({rule_number, rule(strings[1][quote_loc])});
         continue;
      }

      rule_list_t rules;
      for(size_t i=1; i < strings.size(); ++i)
      {
         rules.push_back(rule_t{});
         auto rule_strings = sb::string::parse(strings[i]," ");
         for(const auto& rule_string : rule_strings)
         {
            unsigned r;
            if(!sb::string::from(rule_string,r))
            {
               std::cerr << "error decoding rule_string:" << rule_string << "\n";
               exit(-1);
            }
            rules.back().push_back(r);
         }
      }
      m_map.insert({rule_number,rules});
   }
}


bool rule::test(const std::string& data, size_t& offset) const
{
   // are we at a char termination?
   if(m_char)
   {
      if(data[offset] != m_char)
         return false;
      ++offset;
      return true;
   }

   // no, go down each of the paths we ight have
   for(const rule_t& r : m_rules)
   {
      size_t temp = offset;
      bool rv = true;
      for(auto n : r)
      {
         rv &= m_map.at(n).test(data,temp);
         if(!rv)
            break;
      }
      if(rv)
      {
         offset = temp;
         return true;
      }
   }
   return false;
}


int main(int,char**)
{
   auto time_start = std::chrono::high_resolution_clock::now();

   rule::load(std::cin);
   //rule::dump();
   size_t sum=0;
   std::string s;
   while(std::cin >> s)
   {

      //if(rule::test(s)) std::cout << s << std::endl;
      sum += rule::test(s);
   }

   std::cout << sum << "\n";

   auto time_end = std::chrono::high_resolution_clock::now();
   std::cout << "\ntime: " << std::chrono::duration_cast<std::chrono::microseconds>(time_end-time_start).count() << " us" << std::endl;
   return 0;
}
