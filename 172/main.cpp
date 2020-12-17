#include <iostream>
#include <chrono>
#include <algorithm>
#include <stdint.h>
#include <array>
#include <unistd.h> // STDIN_FILENO


const bool debug=true;
constexpr size_t expected_size{52'000};


class cube
{
public:
   using self_type = cube;
   using list_t = std::vector<cube>;

   cube() : m_x{0}, m_y{0}, m_z{0}, m_w{0} {}
   cube(int x, int y, int z, int w) : m_x{x}, m_y{y}, m_z{z}, m_w{w} {}

   bool operator==(const self_type& rhs) const
   {
      return (m_x == rhs.m_x) && (m_y == rhs.m_y) && (m_z == rhs.m_z) && (m_w == rhs.m_w);
   }
   bool operator<(const self_type& rhs) const
   {
      if(m_x < rhs.m_x) return true;
      if(m_x > rhs.m_x) return false;
      if(m_y < rhs.m_y) return true;
      if(m_y > rhs.m_y) return false;
      if(m_z < rhs.m_z) return true;
      if(m_z > rhs.m_z) return false;
      return m_w < rhs.m_w;
   }

   void inc_x() { m_x += 1; }
   void inc_y() { m_y += 1; }
   void inc_z() { m_z += 1; }
   void inc_w() { m_w += 1; }


   int x() const { return m_x; }
   int y() const { return m_y; }
   int z() const { return m_z; }
   int w() const { return m_w; }

   void x(int i) { m_x = i; }


   list_t search() const
   {
      list_t rv;
      rv.reserve(26);

      for(int w0=-1; w0 <= 1; ++w0)
         for(int z0=-1; z0 <= 1; ++z0)
            for(int y0=-1; y0 <= 1; ++y0)
               for(int x0=-1; x0 <= 1; ++x0)
               {
                  if(!x0 && !y0 && !z0 && !w0)
                     continue;
                  rv.push_back( cube(x()+x0, y()+y0, z()+z0, w()+w0));
               }
      return rv;
   }

   void print(std::ostream& os) const { os << x() << ',' << y() << ',' << z(); }
private:
      int m_x, m_y, m_z, m_w;
};
std::ostream& operator<<(std::ostream& os, const cube& c) { c.print(os); return os; }


inline void dump(const cube::list_t& v)
{
   if(!debug)
      return;
   for(const auto& a : v)
      std::cout << a << '\n';
}

template<typename T>
inline void max_a(T& l, const T& r) { if(l < r) l = r; }
template<typename T>
inline void min_a(T& l, const T& r) { if(l > r) l = r; }


inline void dump_extents(const cube::list_t& v)
{
   if(!debug)
      return;
   int x0=std::numeric_limits<int>::max();
   int y0=std::numeric_limits<int>::max();
   int z0=std::numeric_limits<int>::max();
   int w0=std::numeric_limits<int>::max();
   int x1=std::numeric_limits<int>::min();
   int y1=std::numeric_limits<int>::min();
   int z1=std::numeric_limits<int>::min();
   int w1=std::numeric_limits<int>::min();
   for(const auto& a : v)
   {
      min_a(x0,a.x());
      min_a(y0,a.y());
      min_a(z0,a.z());
      min_a(w0,a.w());
      max_a(x1,a.x());
      max_a(y1,a.y());
      max_a(z1,a.z());
      max_a(w1,a.w());
   }
   std::cout
      << " x=[" << x0 << "," << x1 << "]\n"
      << " y=[" << y0 << "," << y1 << "]\n"
      << " z=[" << z0 << "," << z1 << "]\n"
      << " w=[" << w0 << "," << w1 << "]\n"
      ;
}

inline void dump2(const cube::list_t& v)
{
   if(!debug)
      return;
   int x0=std::numeric_limits<int>::max();
   int y0=std::numeric_limits<int>::max();
   int z0=std::numeric_limits<int>::max();
   int w0=std::numeric_limits<int>::max();
   int x1=std::numeric_limits<int>::min();
   int y1=std::numeric_limits<int>::min();
   int z1=std::numeric_limits<int>::min();
   int w1=std::numeric_limits<int>::min();
   for(const auto& a : v)
   {
      min_a(x0,a.x());
      min_a(y0,a.y());
      min_a(z0,a.z());
      min_a(w0,a.w());
      max_a(x1,a.x());
      max_a(y1,a.y());
      max_a(z1,a.z());
      max_a(w1,a.w());
   }

   for(int w=w0; w <= w1; ++w)
   {
      for(int z=z0; z <= z1; ++z)
      {
         std::cout << "z=" << z << ", w=" << w << "\n";
         for(int y=z0; y <= y1; ++y)
         {
            for(int x=z0; x <= x1; ++x)
            {
               if(std::binary_search(v.begin(),v.end(),cube(x,y,z,w)))
                  std::cout << "#";
               else
                  std::cout << ".";
            }
            std::cout << "\n";
         }
         std::cout << "\n";
      }
   }
}


cube::list_t load_initial()
{
   cube::list_t rv;
   rv.reserve(expected_size);

   cube pos; // initial position

   char ch;
   while(read(STDIN_FILENO, &ch, 1) > 0)
   {
      switch(ch)
      {
         case '#':
            // store cube at current position
            rv.push_back(pos);
            [[fallthrough]];
         case '.':
            pos.inc_x();
            break;
         case '\n':
            pos.x(0);
            pos.inc_y();
            break;
         default:
            std::cerr << "unexpected input\n";
            exit(-1);
            break;
      }
   }

   std::sort(rv.begin(),rv.end());

   return rv;
}

cube::list_t v2(expected_size);
cube::list_t v3(expected_size);
cube::list_t v4(expected_size);

void update(cube::list_t& v1)
{
   // ensure the scratch list is zero'd out
   v2.resize(0); // future check
   v3.resize(0); // results

   // check the on cubes
   for(const cube& c : v1)
   {
      unsigned sum = 0;
      auto va = c.search();
      for(const cube& ca : va)
      {
         v2.push_back(ca);
         if(sum < 4)
            sum += std::binary_search(v1.begin(),v1.end(),ca);
      }
      if((sum == 2) || (sum == 3))
         v3.push_back(c);
   }

   // prepare to check the off cubes
   {
      std::sort(v2.begin(),v2.end());
      auto temp =std::unique(v2.begin(),v2.end());
      v2.resize(std::distance(v2.begin(),temp));

      // is this a std::algorithm?
      auto i = v1.begin();
      auto j = v2.begin();
      while((i != v1.end()) && (j != v2.end()))
      {
         if(*i < *j)
            ++i;
         else if(*j < *i)
            ++j;
         else // (*i == *j)
         {
            ++i;
            j = v2.erase(j);
         }
      }
   }
   // check the off cubes
   for(const cube& c : v2)
   {
      unsigned sum = 0;
      auto va = c.search();
      for(const cube& ca : va)
      {
         sum += std::binary_search(v1.begin(),v1.end(),ca);
         if(sum > 3)
            break;
      }
      if(sum == 3)
         v3.push_back(c);
   }

   std::sort(v3.begin(),v3.end());

   // swap scratch with active
   v1.swap(v3);
}



int main(int,char**)
{
   auto time_start = std::chrono::high_resolution_clock::now();

   v2.reserve(expected_size);
   v3.reserve(expected_size);

   // v1 is always the active list while v2 is the scratch
   cube::list_t v1 = load_initial();

   for(size_t i=0; i < 6; ++i)
      update(v1);

   std::cout << v1.size() << "\n";

   dump_extents(v1);

   auto time_end = std::chrono::high_resolution_clock::now();
   std::cout << "\ntime: " << std::chrono::duration_cast<std::chrono::microseconds>(time_end-time_start).count() << " us" << std::endl;
   return 0;
}
