#ifndef aoc_common_dump_h
#define aoc_common_dump_h

template<typename T>
void dump(const T& t)
{
   for(const auto& a : t)
      std::cout << a << " ";
}

#endif
