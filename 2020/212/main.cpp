#include <iostream>
#include <chrono>
#include <algorithm>
#include <stdint.h>
#include <aoc/get.h>            // aoc::get_line()
#include <sb/string.h>          // sb::string::parse(), sb::string::from()
#include <map>

//const bool debug=false;

// load the data into sorted lists of pairs <english allergens list,foreign allergen list>
using list_t = std::vector<std::pair<std::vector<std::string>,std::vector<std::string>>>;
list_t load()
{
   list_t rv;

   for(std::string line = aoc::get_line(); !line.empty(); line = aoc::get_line())
   {
      std::vector<std::string> unknown = sb::string::parse<std::vector<std::string>>(line," ");
      std::vector<std::string> known;
      for(;;)
      {
         std::string temp{std::move(unknown.back())};
         unknown.pop_back();
         if(temp == "(contains")
            break;
         temp.pop_back(); // remove trailing paren or comma
         known.emplace_back(std::move(temp));
      }
      std::sort(unknown.begin(),unknown.end());
      std::sort(known.begin(),known.end());
      rv.emplace_back(std::make_pair(known,unknown));
   }

   return rv;
}


// expand into the map of single english allergen to list of lists of possible foreign allegens
using map1_t = std::map<std::string,std::vector<std::vector<std::string>>>;
map1_t expand(const list_t& list)
{
   map1_t rv;
   for(const auto& [english, foreign] : list)
      for(const std::string& e : english)
         rv[e].push_back(foreign);
   return rv;
}


// reduce the map's list of lists to a single unique list for each allergen
using map2_t = std::map<std::string,std::vector<std::string>>;
map2_t reduce(const map1_t& m)
{
   map2_t rv;
   for(auto& [known,unknown] : m)
   {
      // there's a more optimal way, here, but it's late and I'm in a rush...
      if(unknown.size() == 1)
      {
         rv[known] = unknown.front();
         continue;
      }
      else
      {
         std::vector<std::string> last = unknown.front();
         for(auto i = unknown.begin()+1; i != unknown.end(); ++i)
         {
            std::vector<std::string> temp;
            std::set_intersection(last.begin(),last.end(),i->begin(),i->end(),std::back_inserter(temp));
            temp.swap(last);
         }
         rv[known] = last;
      }
   }

   return rv;
}


// process of elimination: reduce the allergens to a one to one mapping of english to foreign allergen
using map3_t = std::map<std::string,std::string>;
map3_t reduce(map2_t m)
{
   map3_t rv;

   while(m.size())
   {
      map2_t::iterator i = m.begin();  // std::string
      for(;;)
      {
         if(i == m.end())
         {
            // we could actually, under some circumstances, but this problem isn't that complex.
            std::cerr << "can't solve: not enough uniques";
            exit(-1);
         }
         if(i->second.size() != 1)
         {
            ++i;
            continue;
         }
         // found a unique, set the return
         std::string found = i->second.front();
         rv[i->first] = found;
         m.erase(i);
         for(auto& [name,list] : m)
         {
            for(std::vector<std::string>::iterator j = list.begin(); j != list.end(); ++j)
            {
               if(*j != found)
                  continue;
               j = list.erase(j);
               break;
            }
         }
         break;
      }
   }

   return rv;
}





void dump(const map1_t& m)
{
   for(const auto& [name,llist] : m)
   {
      std::cout << name << "\n";
      for(const auto& list : llist)
      {
         for(const auto& item : list)
         {
            std::cout << "   " << item  << " ";
         }
         std::cout << "\n";
      }
   }
}


void dump(const map2_t& m)
{
   for(const auto& [name,list] : m)
   {
      std::cout << name << "\n";
      for(const auto& item : list)
      {
         std::cout << "   " << item  << " ";
      }
      std::cout << "\n";
   }
}

void dump(const map3_t& m)
{
   std::cout << "\n";
   for(auto& [name, val] : m)
      std::cout << name << " " << val << "\n";
}


size_t solve1(const list_t& list, const map3_t& m)
{
   std::vector<std::string> allergens;
   for(const auto& [english,foreign] : m)
      allergens.push_back(foreign);
   std::sort(allergens.begin(),allergens.end());

   size_t rv = 0;
   for(const auto& [dont_care, foreign_list] : list)
      for(const std::string& foreign : foreign_list)
         if(!std::binary_search(allergens.begin(),allergens.end(),foreign))
            ++rv;

   return rv;
}


int main(int,char**)
{
   auto time_start = std::chrono::high_resolution_clock::now();


   auto list = load(); // load the data into sorted lists of pairs <english allergens list,foreign allergen list>
   auto input = expand(list); // expand into the map of single english allergen to list of lists of possible foreign allegens
   //dump(input);

   auto red = reduce(input); // reduce the map's list of lists to a single unique list for each allergen
   //dump(red);

   auto fin = reduce(red);  // process of elimination: reduce the allergens to a one to one mapping of english to foreign allergen
   //dump(fin);

   // part 2:
   // since the data is already in a map sorted by english allergen, we just need to print the comma separated mapped values
   bool comma=false;
   for(const auto& [english,foreign] : fin)
   {
      if(comma)
         std::cout << ',';
      else
         comma = true;
      std::cout << foreign;
   }
   std::cout << "\n";

   auto time_end = std::chrono::high_resolution_clock::now();
   std::cout << "\ntime: " << std::chrono::duration_cast<std::chrono::microseconds>(time_end-time_start).count() << " us" << std::endl;
   return 0;
}
