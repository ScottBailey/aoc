#ifndef aoc_common_load_vector_h
#define aoc_common_load_vector_h

#include <vector>
#include <iostream>


template<typename T>
std::vector<T> load_vector(std::istream& ios)
{
   std::vector<T> rv;
   T temp;
   while(ios >> temp)
      rv.push_back(temp);
   return rv;
}

#endif
