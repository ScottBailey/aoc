#ifndef sb_md5_h
#define sb_md5_h

namespace sb {
namespace md5 {

using hash_t = std::array<uint8_t,16>;

hash_t sum(const void* v, size_t sz);

/// T must support data() and size() functions
template<typename T>
hash_t sum(const T& t);

std::string to_string(const hash_t& h);

} // namespace md5
} // namespace sb

#include <sb/detail/md5.ipp>
#endif
