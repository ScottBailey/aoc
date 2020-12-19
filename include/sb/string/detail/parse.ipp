#ifndef sb_string_parse_h
# error "direct inclusion error"
#endif

namespace sb {
namespace string {

template<typename T = std::vector<std::string_view>>
inline T parse(const std::string_view& s, const std::string_view& d)
{
   T rv;
   std::string_view::const_pointer first = s.data();
   auto end = first + s.size();

   for(auto last = first; first < end; ++last)
   {
      if(last == end)
      {
         rv.emplace_back( std::string_view{first, static_cast<size_t>(last-first)} );
         break;
      }
      for(auto a : d)
      {
         if(a != *last)
            continue;
         if(last > first)
            rv.emplace_back( std::string_view{first, static_cast<size_t>(last-first)} );
         first = last+1;
      }
   }
   return rv;
}

} // namespace string
} // namespace sb
