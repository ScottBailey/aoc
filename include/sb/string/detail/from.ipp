#ifndef sb_string_from_h
# error "direct inclusion error"
#endif

namespace sb {
namespace string {

template<typename T>
inline bool from(const std::string_view& s, T& t)
{
   bool negative=false;
   t = 0;
   for(const char c : s)
   {
      unsigned val = c - '0';
      if(val > 9)
      {
         if(t || negative)
            return false;
         if(c == '-')
            negative = true;
         else if(c != '+')
            return false;
         continue;
      }
      t *= 10;
      t += static_cast<T>(val);
   }
   if(negative)
      t = -t;
   return true;
}

} // namespace string
} // namespace sb
