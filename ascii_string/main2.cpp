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

template <typename CharT>
struct err_exception {
   CharT operator()() const {
      throw std::invalid_argument{"no ascii character"};
   }
};

template <typename CharT>
struct err_question_mark {
   constexpr CharT operator()() const noexcept {
      return static_cast<CharT>('?');
   }
};

/**
   \see https://stackoverflow.com/questions/26072709/how-to-use-static-assert-for-constexpr-function-arguments-in-c
   \see https://akrzemi1.wordpress.com/2017/05/18/asserts-in-constexpr-functions/
   \see https://github.com/nikolaAV/Modern-Cpp/tree/master/constexpr/invocation_context
*/
template <typename ErrPolicy,typename CharT>
constexpr decltype(auto) sieve(CharT&& ch) {
   return is_ascii(ch)? 
         std::forward<CharT>(ch) 
      :  ErrPolicy{}()
   ;
}

template <typename CharT, typename ErrPolicy>
struct char_traits : std::char_traits<CharT> {
   static constexpr void assign( CharT& r, const CharT& a ) {
      std::char_traits<CharT>::assign(r,sieve<ErrPolicy>(a));
   }
   static constexpr CharT* copy( CharT* dest, const CharT* src, std::size_t count ) {
      for (std::size_t i{0}; i < count; ++i)
         *dest++ = sieve<ErrPolicy>(src[i]);
      return dest;
   }
   static constexpr CharT* move( CharT* dest, const CharT* src, std::size_t count ) {
      for (std::size_t i{0}; i < count; ++i)
         *dest++ = sieve<ErrPolicy>(std::move(src[i]));
      return dest;
   }
};

template <typename CharT, typename ErrPolicy, typename Alloc = std::allocator<CharT>>
struct basic_string : std::basic_string<CharT,char_traits<CharT,ErrPolicy>,Alloc> {
   using base_type = std::basic_string<CharT,char_traits<CharT,ErrPolicy>,Alloc>;
   using base_type::base_type;

   template <typename TraitsU, typename AllocU>
   basic_string(const std::basic_string<CharT,TraitsU,AllocU>& other) : basic_string(other.c_str(),other.size()) {}

   template <typename TraitsU, typename AllocU>
   operator std::basic_string<CharT,TraitsU,AllocU>() const {
      return std::basic_string<CharT,TraitsU,AllocU>{data(),size()};
   } 
};

template <typename CharT, typename Traits, typename ErrPolicy, typename Alloc>
std::basic_ostream<CharT, Traits>& 
operator<<(std::basic_ostream<CharT, Traits>& os, const basic_string<CharT,ErrPolicy,Alloc>& str ) {
   return os << std::basic_string_view{str.data(),str.size()};
}

template<typename CharT, typename ErrPolicy, typename AllocT, typename TraitsU, typename AllocU>
constexpr bool operator==(const ascii::basic_string<CharT,ErrPolicy,AllocT>& l, const std::basic_string<CharT,TraitsU,AllocU>& r) noexcept {
   return l.size()==r.size()? std::equal(l.data(),l.data()+l.size(),r.data()) : false;
}

template<typename CharT, typename ErrPolicy, typename AllocT, typename TraitsU, typename AllocU>
constexpr bool operator==(const std::basic_string<CharT,TraitsU,AllocU>& l, const ascii::basic_string<CharT,ErrPolicy,AllocT>& r) noexcept {
   return r==l;
}

using string  = basic_string<char,err_exception<char>>;
using wstring = basic_string<wchar_t,err_exception<wchar_t>>;

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
}

void test_01_err() {
   using my_string = ascii::basic_string<char,ascii::err_question_mark<char>>;
   const my_string s{"Hello, World!\217"};
   assert(s=="Hello, World!?");

   try {
      const ascii::string wrong {"Hello, World!\217"};
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

string foo() {
   return "nikola";
}


void test_05() {
   const std::string s1 = foo();
   const ascii::string s2 = s1;
   const std::string s3 = s2;

   assert(static_cast<std::string>(s2) == foo() );
   assert(s2 == foo() );
   assert(foo() == s2 );
}


int main()
{
   test_01();
   test_01_err();
   test_02();
   test_03();
   test_04();
   test_05();
}

