#include <stdwarnings_suppress_on.h>
#include <string>
#include <stdexcept>
#include <utility>
#include <ostream>
#include <string_view>
#include <stdwarnings_suppress_off.h>

namespace ascii
{

template <typename CharT>
constexpr bool is_ascii(const CharT& ch) {
   // https://en.cppreference.com/w/cpp/language/ascii
   return 0x0 <= ch && ch <= 0x7F;
}

/**
   \see https://stackoverflow.com/questions/26072709/how-to-use-static-assert-for-constexpr-function-arguments-in-c
   \see https://akrzemi1.wordpress.com/2017/05/18/asserts-in-constexpr-functions/
   \see https://github.com/nikolaAV/Modern-Cpp/tree/master/constexpr/invocation_context
*/
template <typename CharT>
constexpr decltype(auto) sieve(CharT&& ch) {
   return is_ascii(ch)? 
         std::forward<CharT>(ch) 
      :  throw std::invalid_argument{"no ascii character"}
   ;
}

template <typename CharT>
struct char_traits : std::char_traits<CharT> {
   static constexpr void assign( CharT& r, const CharT& a ) {
      std::char_traits<CharT>::assign(r,sieve(a));
   }
   static constexpr CharT* copy( CharT* dest, const CharT* src, std::size_t count ) {
      for (std::size_t i{0}; i < count; ++i)
         *dest++ = sieve(src[i]);
      return dest;
   }
   static constexpr CharT* move( CharT* dest, const CharT* src, std::size_t count ) {
      for (std::size_t i{0}; i < count; ++i)
         *dest++ = sieve(std::move(src[i]));
      return dest;
   }
};

template <typename CharT, typename Alloc = std::allocator<CharT>>
using basic_string = std::basic_string<CharT,char_traits<CharT>,Alloc>;

template <typename CharT, typename Traits, typename Alloc>
std::basic_ostream<CharT, Traits>& 
operator<<(std::basic_ostream<CharT, Traits>& os, const basic_string<CharT, Alloc>& str ) {
   return os << std::basic_string_view{str.data(),str.size()};
}

using string  = basic_string<char>;
using wstring = basic_string<wchar_t>;

}  // end namespace ascii

///////////  Example Of Usage

#include <stdwarnings_suppress_on.h>
#include <iostream>
#include <cassert>
#include <sstream>
#include <stdwarnings_suppress_off.h>

using namespace std; 

void test_01() {
   const ascii::string s {"Hello, World!"};
   assert(s=="Hello, World!");
   const ascii::wstring s2 {L"Hello, World!"};
   assert(s2==L"Hello, World!");

   try {
      ascii::string wrong {"Hello, World!\217"};
      cout << wrong.c_str() << endl;   
   }
   catch(const exception& e) {
      assert(string{"no ascii character"}==e.what());   
      return;
   }
   assert(false);
}

void test_02() {
   const ascii::string s1 {"Hello"};
   const ascii::string s2 {"World!"};
   assert(s1 + ", " + s2 =="Hello, World!");
}

void test_03() {
   const ascii::wstring s1 {L"Hello"};
   const ascii::wstring s2 {L"World!"};
   assert(s1 + L", " + s2 ==L"Hello, World!");

   try {
      assert(s1 + L", " + s2 + wchar_t('\0x8F') ==L"Hello, World!");
   }
   catch(const exception& e) {
      assert(string{"no ascii character"}==e.what());   
      return;
   }
   assert(false);
}

void test_04() {
   const ascii::string s {"Hello, World!"};
   stringstream ss;
   ss << s;
   assert(ss.str() =="Hello, World!");
}

int main()
{
   test_01();
   test_02();
   test_03();
   test_04();
}

