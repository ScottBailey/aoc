#ifndef aoc_common_from_string_h
#define aoc_common_from_string_h

#include <string_view>

namespace sb {
namespace string {

template<typename T>
bool from(const std::string_view& s, T& t)
{
   t = 0;
   for(const char c : s)
   {
      unsigned val = c - '0';
      if(val > 9)
         return false;
      t *= 10;
      t += static_cast<T>(val);
   }
   return true;
}

}
}

#endif
