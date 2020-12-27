#ifndef sb_md5_h
# error "direct inclusion error"
#endif

#include <exception>
#include <openssl/md5.h>

namespace sb {
namespace md5 {

inline hash_t sum(const void* v, size_t sz)
{
   hash_t digest{0}; // return value

   MD5_CTX ctx;
   if(!MD5_Init(&ctx))
      throw std::runtime_error{"Failed to initialize MD5_CTX"};

   MD5_Update(&ctx, v, sz);

   MD5_Final(digest.data(), &ctx);
   return digest;
}


/// T must support data() and size() functions
template<typename T>
inline hash_t sum(const T& t)
{
   return sum(t.data(),t.size());
}


inline std::string to_string(const hash_t& h)
{
   std::string rv;
   rv.reserve(16);
   for(uint8_t n : h)
   {
      switch(n>>4)
      {
         case 0x0: rv.push_back('0'); break;
         case 0x1: rv.push_back('1'); break;
         case 0x2: rv.push_back('2'); break;
         case 0x3: rv.push_back('3'); break;
         case 0x4: rv.push_back('4'); break;
         case 0x5: rv.push_back('5'); break;
         case 0x6: rv.push_back('6'); break;
         case 0x7: rv.push_back('7'); break;
         case 0x8: rv.push_back('8'); break;
         case 0x9: rv.push_back('9'); break;
         case 0xa: rv.push_back('a'); break;
         case 0xb: rv.push_back('b'); break;
         case 0xc: rv.push_back('c'); break;
         case 0xd: rv.push_back('d'); break;
         case 0xe: rv.push_back('e'); break;
         case 0xf: rv.push_back('f'); break;
      }
      switch(n&0xF)
      {
         case 0x0: rv.push_back('0'); break;
         case 0x1: rv.push_back('1'); break;
         case 0x2: rv.push_back('2'); break;
         case 0x3: rv.push_back('3'); break;
         case 0x4: rv.push_back('4'); break;
         case 0x5: rv.push_back('5'); break;
         case 0x6: rv.push_back('6'); break;
         case 0x7: rv.push_back('7'); break;
         case 0x8: rv.push_back('8'); break;
         case 0x9: rv.push_back('9'); break;
         case 0xA: rv.push_back('a'); break;
         case 0xB: rv.push_back('b'); break;
         case 0xC: rv.push_back('c'); break;
         case 0xD: rv.push_back('d'); break;
         case 0xE: rv.push_back('e'); break;
         case 0xF: rv.push_back('f'); break;
      }
   }
   return rv;
}


} // namespace md5
} // namespace sb
