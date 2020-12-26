#include <iostream>
#include <unistd.h> // STDIN_FILENO
#include <string>
#include <vector>
#include <string_view>
#include <max_a.h>

struct vals_t
{
   int hi;
   int lo;
};


void half(bool upper, vals_t& v)
{
   int reduce = (1 + v.hi - v.lo) / 2;  // add one for rounding
   if(upper)
      v.lo += reduce;
   else
      v.hi -= reduce;
}


int find_col(const std::string_view& s)
{
   vals_t v{7,0};
   for(size_t i=7; i < 10; ++i)
      half(s[i] == 'R',v);
   return v.hi;
}


int find_row(const std::string_view& s)
{
   vals_t v{127,0};
   for(size_t i=0; i < 7; ++i)
      half(s[i] == 'B',v);
   return v.hi;
}


int seat_id(const std::string_view& s)
{
   int row = find_row(s);
   int col = find_col(s);
   return (8 * row) + col;
}


int main(int,char**)
{
   //std::vector<char> v(10);
   std::string v(10,'0');

   std::vector<bool> seats(127*8+7,false);

   size_t pos = 0;
   char ch;
   while(read(STDIN_FILENO, &ch, 1) > 0)
   {
      // store data into the array
      if(pos < v.size())
         v[pos] = ch;
      ++pos;
      if(ch == '\n')
      {
         int id = seat_id(v);
         seats[id] = true;
         pos = 0;
      }
   }

   /*
   for(size_t i=0; i < seats.size(); ++i)
   {
      if(!seats[i])
         std::cout << "Empty: " << i << "\n";
   }
   */

   for(size_t i=8; i < (seats.size() - 8); ++i)
   {
      if(!seats[i] && seats[i-1] && seats[i+1])
      {
         std::cout << "My seat: " << i << std::endl;
         break;
      }
   }

   return 0;
}
