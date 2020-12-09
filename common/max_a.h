#ifndef aoc_common_max_a_h
#define aoc_common_max_a_h

#include <string_view>

namespace sb {

template<typename T>
inline void max_a(T& l, const T& r)
{
   if(l < r)
      l = r;
}

template<typename T>
inline void min_a(T& l, const T& r)
{
   if(l > r)
      l = r;
}

}

#endif
