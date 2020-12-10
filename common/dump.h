#ifndef aoc_common_dump_h
#define aoc_common_dump_h

template<typename T>
inline void dump(const T& t)
{
   for(const auto& a : t)
      std::cout << a << " ";
}

template<typename T>
inline void dump(bool debug, const T& t, const std::string& separator = " ", const std::string& terminator = "")
{
   if(!debug)
      return;
   for(const auto& a : t)
      std::cout << a << separator;
   if(!terminator.empty())
      std::cout << terminator;
}

#endif
