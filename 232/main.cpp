#include <iostream>
#include <chrono>
#include <algorithm>
#include <stdint.h>
#include <aoc/get.h>            // aoc::get_line()
#include <sb/string.h>          // sb::string::parse(), sb::string::from()
#include <vector>
#include <array>
#include <cassert>
#include <map>
//const bool debug=false;

constexpr size_t max{10}; // zero is a dummy, so add 1
//constexpr size_t max{1'000'001}; // zero is a dummy, so add 1

template<typename T>
struct node_type
{
   node_type<T>* left;
   node_type<T>* right;
   T val;
};

using node_t = node_type<unsigned>;
using list_t = std::array<node_t,max>; // the zero-ith value is a dummy and will point to the firost valid


// cut the 3 nodes after node i
node_t* cut3(node_t* i)
{
   // temp->rv->r1->r2->other
   node_t* temp = i;
   node_t* rv = temp->right;
   node_t* other = rv->right->right->right;
   temp->right = other;
   other->left = temp;
   return rv;
}


// add the 3 nodes after index i
void add3(list_t& list, unsigned i, node_t* n1)
{
   // temp->rv->r1->r2->other
   node_t* temp = &(list[i]);
   node_t* other = temp->right;

   // fix left side of the splice
   temp->right = n1;
   n1->left = temp;

   // fix right side of the splice
   node_t* n3 = n1->right->right;
   other->left = n3;
   n3->right = other;
}


// indicate if a splice contains the value i
bool has3(node_t* n1, unsigned i)
{
   return (n1->val == i) || (n1->right->val == i) || (n1->right->right->val == i);
}


list_t load(std::istream& is=std::cin)
{
   list_t rv;

   // zero is a dummy
   node_t* last=&(rv[0]);
   last->left = nullptr;
   last->val = 0;

   size_t i=1;
   char ch;
   while(is.get(ch))
   {
      unsigned val = ch -'0';
      if( val > 9 )
         break;

      node_t* curr = &(rv[val]);

      // update last
      last->right = curr;

      // update this
      curr->val = val;
      curr->left = last;
      //curr->right = nullptr;

      last = curr;
      ++i;
   }

   node_t* curr = &(rv[i]);
   while(i < max)
   {
      // update last
      last->right = curr;

      // update this
      curr->val = i;
      curr->left = last;
      //curr->right = nullptr;

      last = curr;
      ++curr;
      ++i;
   }

   last->right = rv[0].right; // update the first member of the list
   rv[0].right->left = last;  // update the last member of the list

   return rv;
}


inline void dump(const list_t& v)
{
   unsigned start = v[0].right->val;

   node_t* end = v[0].right;
   if(end->val == start)
      std::cout << "(" << end->val << ") ";
   else
      std::cout << end->val << " ";

   node_t* pos = end->right;
   while(pos != end)
   {
      if(pos->val == start)
         std::cout << "(" << pos->val << ") ";
      else
         std::cout << pos->val << " ";
      pos = pos->right;
   }
   std::cout << "\n";

   /*
   for(auto a : v)
   {
      if(a.val == start)
         std::cout << "(" << a.val << ") ";
      else
         std::cout << a.val << " ";
   }
   std::cout << std::endl;
   */
   /*
   node_t* end = v[0].right;
   std::cout << end->val;
   node_t* pos = end->right;
   while(pos != end)
   {
      std::cout << pos->val;
      pos = pos->right;
   }
   std::cout << "\n";
   */

   /*
   for(auto a : v)
   {
      std::cout << a.val << ": ";
      if(a.left)
         std::cout << a.left->val << " ";
      else
         std::cout << "X ";
      if(a.right)
         std::cout << a.right->val << " ";
      else
         std::cout << "X ";
      std::cout << std::endl;
   }
   std::cout << std::endl;
   */
}


inline void dump1(const list_t& v)
{
   node_t* pos = v[1].right;
   while(pos->val != 1)
   {
      std::cout << pos->val;
      pos = pos->right;
   }
   std::cout << "\n";
}


inline void dump2(const list_t& v)
{
   unsigned a = v[1].right->val;
   unsigned b = v[1].right->right->val;
   std::cout << a << " x " << b << " = " << a*b << "\n";
}


inline void move(list_t& v, size_t iterations)
{
   // ____Sabc__________
   // ____Sxxx__________ abc
   // ____Sxxx-----N____ abc
   // ____S-----Nxxx____ abc
   // ____S-----Nabc____

   // zero index node contains the start node's pointer

   node_t* start = v[0].right;
   for(size_t i=0; i < iterations; ++i)
   {
      v[0].right = start;
      std::cout << i+1 << ": ";
      dump(v);

      node_t* splice = cut3(start);
      unsigned insert_pos = start->val;
      for(;;)
      {
         --insert_pos;
         if(!insert_pos)
            insert_pos = v.size()-1;
         if(!has3(splice,insert_pos))
            break;
      }
      add3(v,insert_pos,splice);
      start = start->right;
   }
   v[0].right = start;
}


int main(int,char**)
{
   auto time_start = std::chrono::high_resolution_clock::now();

   list_t v = load();
   move(v,10);
   dump1(v);



   auto time_end = std::chrono::high_resolution_clock::now();
   std::cout << "\ntime: " << std::chrono::duration_cast<std::chrono::microseconds>(time_end-time_start).count() << " us" << std::endl;
   return 0;
}
