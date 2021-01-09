#ifndef aoc_life_h
# error "direct inclusion error"
#endif

namespace aoc {
namespace life {


template<typename T>
inline void play(typename T::list_t& list)
{
   typename T::list_t rv;
   rv.reserve(2*list.size());

   typename T::list_t maybe;
   maybe.reserve(8*list.size());

   // deal with currently on nodes
   for(const auto& a : list)
   {
      size_t adjacent_on = 0;
      typename T::list_t temp = a.adjacent();
      for(const auto& b : temp)
      {
         adjacent_on += std::binary_search(list.begin(), list.end(), b);
         maybe.push_back(b);
      }
      if(T::state(true,adjacent_on))
         rv.push_back(a);
   }

   // now deal with maybe nodes
   std::sort(maybe.begin(), maybe.end());
   maybe.erase( std::unique(maybe.begin(), maybe.end()), maybe.end());

   // remove any maybe's that are already resolved
   {
      auto i = list.begin();
      auto j = maybe.begin();
      while((i != list.end()) && (j != maybe.end()))
      {
        if(*i == *j)
        {
           ++i;
           j = maybe.erase(j);
        }
        else if(*i < *j)
           ++i;
        else // if(*j < *i)
           ++j;
      }
   }

   // deal with maybe nodes
   for(const auto& a : maybe)
   {
      size_t adjacent_on = 0;
      typename T::list_t temp = a.adjacent();
      for(const auto& b : temp)
         adjacent_on += std::binary_search(list.begin(), list.end(), b);
      if(T::state(false,adjacent_on))
         rv.push_back(a);
   }

   std::sort(rv.begin(),rv.end());
   rv.swap(list);
}


} // namespace life
} // namespace aoc
