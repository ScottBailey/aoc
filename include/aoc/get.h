#ifndef aoc_get_h
#define aoc_get_h

#include <string>
#include <iostream>

namespace aoc {

std::string get_line(std::istream& is);

} // namespace aoc

#include <aoc/detail/get.ipp>

#endif
