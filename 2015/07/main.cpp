#include <iostream>
#include <chrono>
#include <algorithm>
#include <stdint.h>
#include <aoc/get.h>            // aoc::get_line()
#include <sb/string.h>          // sb::string::parse(), sb::string::from()
#include <sb/ansi.h>            // ansi colors: sb::red, sb::yellow, etc
#include <sb/ignore.h>          // sb::string::parse(), sb::string::from()
#include <vector>
#include <map>
#include <numeric>

//const bool debug=false;

struct operation
{
   enum op_t
   {
      literal,
      variable,
      not_,
      and_,
      or_,
      lshift,
      rshift,
   };

   using var_t = std::string;

   var_t output;

   op_t op = literal;

   var_t left;
   var_t right;

   bool done = false;
   uint16_t value;
};

/*
std::string var_string(operation::var_t v)
{
   if(!v)
      return "empty";
   std::string rv;
   while(v)
   {
      rv.insert(rv.begin(),static_cast<char>(v & 0xFF));
      v >>= 8;
   }
   return rv;
}
*/

std::ostream& operator<<(std::ostream& os, const operation& o)
{
   os << o.output << " : ";
   if(!o.left.empty())
      os << o.left << " ";
   switch(o.op)
   {
      case operation::literal: os << "literal "; break;
      case operation::variable: os << "variable "; break;
      case operation::not_: os << "not_ "; break;
      case operation::and_: os << "and_ "; break;
      case operation::or_: os << "or_ "; break;
      case operation::lshift: os << "lshift "; break;
      case operation::rshift: os << "rshift "; break;
      default: os << "bad op!\n" << std::flush; throw -1;
   }
   if(!o.right.empty())
      os << o.right << " ";
   if(o.done)
      os << " result: " << o.value;
   return os;
}

using container_t = std::map<operation::var_t,operation>;

std::ostream& operator<<(std::ostream& os, const container_t& list)
{
   for(const auto& [val, a] : list)
      os << a << "\n";
   return os;
}


operation::op_t op_from_string(const std::string_view& in)
{
   if(in == "NOT") return operation::not_;
   if(in == "AND") return operation::and_;
   if(in == "OR") return operation::or_;
   if(in == "LSHIFT") return operation::lshift;
   if(in == "RSHIFT") return operation::rshift;
   throw std::invalid_argument("bad operation");
}


container_t load(std::istream& is=std::cin)
{
   container_t rv;
   for(std::string line = aoc::get_line(is); !line.empty(); line = aoc::get_line(is))
   {
      operation o;
      auto tokens = sb::string::parse(line," ");
      // sanity check
      if(tokens.size() < 3)
         throw std::invalid_argument("bad input");

      o.output = tokens.back();
      tokens.pop_back();

      if(tokens.back() != "->")
         throw std::invalid_argument("bad input");
      tokens.pop_back();

      if(tokens.size() == 1)
      {
         o.right = tokens[0];
         if(sb::string::from(tokens[0],o.value))
         {
            o.op = operation::literal;
            o.done = true;
         }
         else
            o.op = operation::variable;
      }
      else if(tokens.size() == 2)
      {
         o.op = op_from_string(tokens[0]);
         o.right = tokens[1];
      }
      else if(tokens.size() == 3)
      {
         o.left = tokens[0];
         o.op = op_from_string(tokens[1]);
         o.right = tokens[2];
      }
      else
         throw std::invalid_argument("excessive tokens in line");

      rv[o.output] = o;
   }
   return rv;
}


uint16_t solve_1(container_t& m, const operation::var_t& var)
{
   if(var[0] >= '0' && var[0] <= '9')
   {
      uint16_t temp=0;
      sb::string::from(var,temp);
      return temp;
   }

   operation& entry = m.at(var);
   if(entry.done)
      return entry.value;

   uint16_t value = 0;
   switch(entry.op)
   {
      case operation::literal:
         value = entry.value;
         break;
      case operation::variable:
         value = solve_1(m,entry.right);
         break;
      case operation::not_:
         value = ~solve_1(m,entry.right);
         break;
      case operation::and_:
         value = solve_1(m,entry.left) & solve_1(m,entry.right);
         break;
      case operation::or_:
         value = solve_1(m,entry.left) | solve_1(m,entry.right);
         break;
      case operation::lshift:
         value = (solve_1(m,entry.left) << solve_1(m,entry.right));
         break;
      case operation::rshift:
         value = (solve_1(m,entry.left) >> solve_1(m,entry.right));
         break;
      default: std::cerr << "bad op!\n" << std::flush; throw -1;
   }

   entry.done = true;
   entry.value = value;

   return value;
}


void part1(const container_t& list)
{
   container_t temp = list;
   std::cout << sb::white << "Part 1: " << sb::reset << solve_1(temp,"a") << "\n";
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
