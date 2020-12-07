#ifndef sb_algorithm_h
#define sb_algorithm_h

#include <algorithm>

namespace sb {

/// t becomes t intersection u
template <typename T>
inline void intersection_a(T& t, const T& u)
{
   T out;
   std::set_intersection(t.begin(),t.end(),u.begin(),u.end(),std::back_inserter(out));
   t.swap(out);
}


/// store unique values
template<typename T>
inline void store_u(T& t, const typename T::value_type& v)
{
   // unique, sorted store into vector
   auto a = std::lower_bound(t.begin(),t.end(),v);
   if(a == t.end())
      t.push_back(v);
   else if(*a != v)
      t.insert(a,v);
}


} // namespace sb

#endif
