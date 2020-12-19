#ifndef sb_string_from_h
#define sb_string_from_h

#include <string_view>

namespace sb {
namespace string {

/// convert a string to a value
/// @todo include hex
/// @param s  string to convert (may contain leading '+' or '-')
/// @param t  return storage for T - partial if return is false
/// @return true if the whole string was converted to a T and stored in t
template<typename T>
bool from(const std::string_view& s, T& t);

//template<typename T>
//bool from_hex(const std::string_view& s, T& t);


} // namespace string
} // namespace sb

#include <sb/string/detail/from.ipp>

#endif
