#include <iostream>
#include <chrono>
#include <algorithm>
#include <deque>
#include <stdint.h>


//const bool debug=false;

class token {
public:
   using list_t=std::deque<token>;
   using value_type = uint64_t;

   enum op_t {
      paren_left,
      paren_right,
      add,
      mult,
      val,
   };

   token(op_t o) : m_op{o}, m_val{0} {}
   token(value_type n) : m_op{val}, m_val{n} {}

   bool is_val() const { return m_op == val; }
   bool is_paren() const { return m_op == paren_left || m_op == paren_right; }
   op_t op() const { return m_op; }
   value_type value() const { return m_val; }

   token operator*(const token& rhs) const { return m_val * rhs.m_val; }
   token& operator*=(const token& rhs) { m_val *= rhs.m_val; return *this;}
   token operator+(const token& rhs) const { return m_val + rhs.m_val; }
   token& operator+=(const token& rhs) { m_val += rhs.m_val; return *this;}

   bool operator==(const token& rhs) const {return m_op == rhs.m_op && m_val == rhs.m_val; }

   void print(std::ostream& os) const {
      switch(m_op) {
         case paren_left: os << '('; return;
         case paren_right: os << ')'; return;
         case add: os << '+'; return;
         case mult: os << '*'; return;
         case val: os << m_val; return;
      }
      os << "error";
   }

   static list_t load(std::istream& is);
   static value_type solve(const list_t& l);


private:

   void op_i(const token& m, const token& r);

   static void solve_i(list_t& v, list_t::iterator first, list_t::iterator last);  // solve and erase for [first,last)

   op_t m_op;
   value_type m_val;
};

std::ostream& operator<<(std::ostream& os, const token& t) { t.print(os); return os;}
std::ostream& operator<<(std::ostream& os, const token::list_t&l ) {
   for(const auto& t : l )
   {
      t.print(os);
      os << ' ';
   }
   return os;
}

token::list_t token::load(std::istream& is)
{
   list_t rv;

   // there is no zero or negative value in the input, optimize accordingly...

   unsigned val=0;
   char ch;
   while(is.get(ch))
   {
      unsigned temp = ch - '0';
      if(temp < 10)
      {
         val *= 10;
         val += temp;
      }
      else
      {
         if(val)
         {
            rv.push_back(token(val));
            val = 0;
         }

         switch(ch) {
            case '(': rv.push_back(token(paren_left)); break;
            case ')': rv.push_back(token(paren_right)); break;
            case '+': rv.push_back(token(add)); break;
            case '*': rv.push_back(token(mult)); break;
            case ' ': continue;
            case '\n': return rv;
         }
      }
   }

   if(val)
   {
      rv.push_back(token(val));
      val = 0;
   }

   return rv;
}


void token::op_i(const token& m, const token& r)
{
   if(m.op() == add)
      *this += r;
   else if(m.op() == mult)
      *this *= r;
   else
   {
      std::cerr << "logic error: " << *this << " " << m << " " << r << "\n";
      exit(-1);
   }
}


void token::solve_i(list_t& v, list_t::iterator fin, list_t::iterator lin) // solve for [first,last), erase [first,last)
{
   // next: an operation
   // next +1: a value
   token result(0);
   auto next = fin+1;
   auto last = lin;
   if(!fin->is_paren())
      result = *fin;
   else
   {
      ++next;
      result = *(fin+1);
      --last;
   }
   while(next != last) // ignoring the error state: first + 2 == last (i.e. no operation)
   {
      result.op_i(*next,*(next+1));
      next += 2;
   }
   *fin = result;
   v.erase(fin+1,lin);
}


token::value_type token::solve(const token::list_t& lin)
{
   token::list_t v = lin;
   // do parens
   for(;;)
   {
      auto l1 = find(v.begin(),v.end(),token(token::paren_left));
      if(l1 == v.end())
         break;
      for(;;)
      {
         auto l2 = find(l1+1,v.end(),token(token::paren_left));
         if(l2 == v.end())
            break;
         l1 = l2;
      }
      auto r1 = find(l1+1,v.end(),token(token::paren_right));
      if(r1 == v.end())
      {
         std::cerr << "bad input: mismatched parens\n" << lin << "\n";
         return -1;
      }
      solve_i(v,l1,r1+1);
   }

   solve_i(v,v.begin(),v.end());

   return v.front().value();
}


int main(int,char**)
{
   auto time_start = std::chrono::high_resolution_clock::now();

   uint64_t sum=0;
   for(;;)
   {
      token::list_t t = token::load(std::cin);
      if(!t.size())
         break;
      sum += token::solve(t);
   }

   std::cout << "sum: " << sum << '\n';

   auto time_end = std::chrono::high_resolution_clock::now();
   std::cout << "\ntime: " << std::chrono::duration_cast<std::chrono::microseconds>(time_end-time_start).count() << " us" << std::endl;
   return 0;
}
