#ifndef aoc_life_h
#define aoc_life_h

namespace aoc {
namespace life {


// T is a class or struct with the following functions:
//   using list_t=std::list<self_type>;
//   list_t adjacent() const; // return the neighbors of this node
//   static bool state(bool current_state, size_t adjacent_on_nodes);
//   bool operator<(const self_type& rhs) const;
//   bool operator==(const self_type& rhs) const;

template <typename T>
void play(typename T::list_t& list);


} // namespcae life
} // namespace aoc

#include <aoc/detail/life.ipp>

#endif
