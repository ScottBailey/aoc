#include <iostream>
#include <chrono>
#include <string>
#include <unistd.h> // STDIN_FILENO
#include <sb/string/from.h>

#if 0
# include <map>
using map_t = std::map<std::string,std::string>;
#else
# include <unordered_map>
using map_t = std::unordered_map<std::string,std::string>;
#endif

const std::string byr{"byr"};
const std::string iyr{"iyr"};
const std::string eyr{"eyr"};
const std::string hgt{"hgt"};
const std::string hcl{"hcl"};
const std::string ecl{"ecl"};
const std::string pid{"pid"};
const std::string cid{"cid"};

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
   if( !m.count(byr)
         || !m.count(iyr)
         || !m.count(eyr)
         || !m.count(hgt)
         || !m.count(hcl)
         || !m.count(ecl)
         || !m.count(pid)
       )
      return false;

   { // byr
      int n =0;
      const std::string& temp = m.at(byr);
      if((temp.size() != 4) || !sb::string::from(temp,n) || (n < 1920) || (n > 2002))
         return false;
   }
   { // iyr
      int n =0;
      const std::string& temp = m.at(iyr);
      if((temp.size() != 4) || !sb::string::from(temp,n) || (n < 2010) || (n > 2020))
         return false;
   }
   { // eyr
      int n =0;
      const std::string& temp = m.at(eyr);
      if((temp.size() != 4) || !sb::string::from(temp,n) || (n < 2020) || (n > 2030))
         return false;
   }
   { // hgt
      int n =0;
      const std::string& temp = m.at(hgt);
      if((temp.size() < 4) || (temp.size() > 5) || !sb::string::from(temp.substr(-0,temp.size()-2),n))
         return false;
      std::string type = temp.substr(temp.size()-2);
      if(type == "cm")
      {
         if((n < 150) || (n > 193))
            return false;
      }
      else if(type == "in")
      {
         if((n < 59) || (n > 76))
            return false;
      }
      else
         return false;
   }
   { // hcl
      const std::string& temp = m.at(hcl);
      if((temp.size() != 7) || (temp[0] != '#'))
         return false;
      for(size_t i=1; i < temp.size(); ++i)
      {
         char c = temp[i];
         if( ! (((c >= '0') && (c <= '9')) || ((c >= 'a') && (c <= 'f'))) )
            return false;
      }
   }
   { // ecl
      const std::string& temp = m.at(ecl);
      if(temp.size() != 3)
         return false;
      if((temp != "amb") && (temp != "blu") && (temp != "brn") && (temp != "gry") && (temp != "grn") && (temp != "hzl") && (temp != "oth"))
         return false;
   }
   { // pid
      const std::string& temp = m.at(pid);
      if(temp.size() != 9)
         return false;
      for(size_t i=1; i < temp.size(); ++i)
      {
         char c = temp[i];
         if((c < '0') || (c > '9'))
            return false;
      }
   }
   // cid is don't care
   size_t expect = 7 + m.count("cid");
   return(expect == m.size());
}


int main(int,char**)
{
   auto time_start = std::chrono::high_resolution_clock::now();

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


   auto time_end = std::chrono::high_resolution_clock::now();
   std::cout << "\ntime: " << std::chrono::duration_cast<std::chrono::microseconds>(time_end-time_start).count() << " us" << std::endl;
   return 0;
}
