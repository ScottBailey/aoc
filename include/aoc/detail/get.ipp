#ifndef aoc_get_h
# error "direct inclusion error"
#endif

namespace aoc {

inline std::string get_line(std::istream& is)
{
   std::string s;
   s.reserve(40);
   char ch;
   while(is.get(ch))
   {
      if(ch == '\n')
         break;
      s.push_back(ch);
   }
   return s;
}

} // namespace aoc
