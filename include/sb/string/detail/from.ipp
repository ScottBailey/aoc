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


template<typename T>
inline bool from_hex(const std::string_view& s, T& t)
{
   t = 0;
   for(const char c : s)
   {
      unsigned val = c - '0';
      if(val < 10)
      {
         t <<= 4; // multiply by 16
         t += static_cast<T>(val);
         continue;
      }

      val = c - 'a';
      if(val < 6)
      {
         t <<= 4; // multiply by 16
         t += 10;
         t += static_cast<T>(val);
         continue;
      }

      val = c - 'A';
      if(val < 6)
      {
         t <<= 4; // multiply by 16
         t += 10;
         t += static_cast<T>(val);
         continue;
      }

      return false;
   }

   return true;
}

} // namespace string
} // namespace sb
