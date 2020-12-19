#ifndef sb_string_parse_h
#define sb_string_parse_h

#include <string_view>
#include <vector>

namespace sb {
namespace string {

/// parse a string s with delimiters d
/// @param s  string to parse
/// @param d  string of terminators
/// @return container of parsed values
template<typename T = std::vector<std::string_view>>
T parse(const std::string_view& s, const std::string_view& d);

//template<typename T>
//bool from_hex(const std::string_view& s, T& t);


} // namespace string
} // namespace sb

#include <sb/string/detail/parse.ipp>

#endif
