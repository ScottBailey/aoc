#include <iostream>
#include <string>
#include <map>
#include <unistd.h> // STDIN_FILENO

std::array<char,65536> buffer;


using map_t = std::map<std::string,std::string>;


/// #return false for eof
bool load(map_t& m)
{
   std::string type;
   std::string key;
   std::string val;
   bool on_key = true;;
   char ch;
   while(read(STDIN_FILENO, &ch, 1) > 0)
   {
      switch(ch)
      {
         case '\n':
            if(key.empty())
               return true;
            m[key] = val;
            key.clear();
            val.clear();
            on_key = true;
            break;

         case ' ':
            if(!key.empty())
               m[key] = val;
            key.clear();
            val.clear();
            on_key = true;
            break;

         case ':':
            on_key = false;
            break;

         default:
            if(on_key)
               key.push_back(ch);
            else
               val.push_back(ch);
      }
   }
   // we get here only on EOF
   if(!key.empty() && !val.empty())
      m[key] = val;
   return false;
}


void dump(const map_t& m)
{
   for(map_t::const_iterator i = m.begin(); i != m.end(); ++i)
      std::cout << i->first << ":" << i->second << "\n";
   std::cout << std::endl;
}


bool validate(const map_t& m)
{
   if( !m.count("byr")
         || !m.count("iyr")
         || !m.count("eyr")
         || !m.count("hgt")
         || !m.count("hcl")
         || !m.count("ecl")
         || !m.count("pid")
       )
      return false;
   size_t expect = 7 + m.count("cid");
   return(expect == m.size());
}


int main(int,char**)
{
   size_t count = 0;
   size_t bad = 0;
   for(;;)
   {
      map_t m;
      bool con = load(m);
      if(validate(m))
         ++count;
      else
         ++bad;
      if(!con)
         break;
   }

   std::cout << "valid: " << count << std::endl;
   std::cout << "bad: " << bad << std::endl;

   return 0;
}
