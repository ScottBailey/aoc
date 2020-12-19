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

   static void solve_i(list_t& v, const size_t first, size_t end);

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


void token::solve_i(list_t& v, const size_t first, size_t end)
{
   // there are guaranteed to be no parenthesis here
   while((end-first) > 1)
   {
      // first do multiplication
      size_t cur = first;
      while(cur+2 < end)
      {
         if(v[cur+1].op() == mult)  // mult is lower precedent in the rules
            cur += 2;
         else
         {
            v[cur] += v[cur+2];
            v.erase(v.begin()+cur+1,v.begin()+cur+3);
            end -= 2;
         }
      }
      // now do addition
      cur = first;
      while(cur+2 < end)
      {
         v[cur] *= v[cur+2];
         v.erase(v.begin()+cur+1,v.begin()+cur+3);
         end -= 2;
      }
   }
}


token::value_type token::solve(const token::list_t& lin)
{
   if(lin.empty())
      return 0;

   token::list_t v = lin;

   // do parens first
   {
      size_t lp = v.size()-1;
      while(lp < v.size()) // let it roll over for the exit condition
      {
         if(v[lp].op() != paren_left)
         {
            --lp;
            continue;
         }
         // found
         size_t rp = lp+1;
         while(v[rp] != paren_right) // not sanity checked for end()
            ++rp;

         solve_i(v,lp+1,rp);

         v[lp] = v[lp+1];
         v.erase(v.begin()+lp+1,v.begin()+lp+3);
         --lp;
      }
   }

   solve_i(v,0,v.size());

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
