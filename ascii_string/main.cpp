#include <stdwarnings_suppress_on.h>
#include <string>
#include <stdexcept>
#include <utility>
#include <ostream>
#include <string_view>
#include <type_traits>

#include <stdwarnings_suppress_off.h>

namespace ascii
{

template <typename CharT>
constexpr bool is_ascii(const CharT& ch) noexcept {
   // https://en.cppreference.com/w/cpp/language/ascii
   return 0x0 <= ch && ch <= 0x7F;
}

template <typename CharT, typename ExceptionT>
struct throw_exception {
   template <typename CharU>
   constexpr CharT operator()(const CharU& ch) const {
      // https://github.com/nikolaAV/Modern-Cpp/tree/master/constexpr/invocation_context
      throw ExceptionT{"no ascii character"}, CharT{};
   } 
};

template <typename CharT>
struct question_mark_substitution {
   template <typename CharU>
   constexpr CharT operator()(const CharU&) const noexcept {
      return CharT{'?'};
   } 
};

template <typename CharT, typename ViolationHandlerT>
struct basic_sieve {
   template <typename CharU>
   constexpr void operator()(CharT& l, CharU&& r) const {
      if(is_ascii(r))
         if constexpr (std::is_same_v<std::decay_t<CharT>,std::decay_t<CharU>>)
            l=std::forward<CharU>(r);
         else
            l=static_cast<CharT>(r);
      else
         l=ViolationHandlerT{}(r);
   } 
};

template <typename CharT, typename SieveT>
struct char_traits : std::char_traits<CharT> {
   template <typename CharU>
   static constexpr void assign( CharT& l, const CharU& r ) {
      sieve_(l,r);
   }
   template <typename CharU>
   static constexpr void assign( CharT* dest, std::size_t count, const CharU& r ) {
      for (std::size_t i{0}; i < count; ++i) 
         sieve_(*dest++,r);
   }
   template <typename CharU>
   static constexpr CharT* copy( CharT* dest, const CharU* src, std::size_t count ) {
      for (std::size_t i{0}; i < count; ++i)
         sieve_(*dest++,src[i]);
      return dest;
   }
   template <typename CharU>
   static constexpr CharT* move( CharT* dest, const CharU* src, std::size_t count ) {
      for (std::size_t i{0}; i < count; ++i)
         sieve_(*dest++,std::move(src[i]));
      return dest;
   }
private:
   static constexpr SieveT sieve_{};
};

template <typename StrToT, typename StrFtomT>
StrToT cast(const StrFtomT& s) {
   using traits_type = typename StrToT::traits_type;
   StrToT out(s.size(), ' ');
   traits_type::copy(out.data(),s.data(),s.size());
   return out;
} 

/*
template<typename CharT, typename SieveT, typename AllocT, typename TraitsU, typename AllocU>
constexpr bool operator==(const ascii::basic_string<CharT,SieveT,AllocT>& l, const std::basic_string<CharT,TraitsU,AllocU>& r) noexcept {
   return l.size()==r.size()? std::equal(l.data(),l.data()+l.size(),r.data()) : false;
}

template<typename CharT, typename SieveT, typename AllocT, typename TraitsU, typename AllocU>
constexpr bool operator==(const std::basic_string<CharT,TraitsU,AllocU>& l, const ascii::basic_string<CharT,SieveT,AllocT>& r) noexcept {
   return r==l;
}
*/

template <typename CharT, typename ExceptionT = std::invalid_argument>
using sieve_exception      = basic_sieve<CharT,throw_exception<CharT,ExceptionT>>;
template <typename CharT>
using sieve_question_mark  = basic_sieve<CharT,question_mark_substitution<CharT>>;

template <
   typename CharT, 
   typename SieveT = sieve_question_mark<CharT>, 
   typename AllocT = std::allocator<CharT>>
using basic_string = std::basic_string<CharT,char_traits<CharT,SieveT>,AllocT>;

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

/*

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
*/

void test_01() {
   assert(ascii::string { "Hello, World!"}== "Hello, World!");
   assert(ascii::wstring{L"Hello, World!"}==L"Hello, World!");
   assert( "Hello, World!"==ascii::string { "Hello, World!"});
   assert(L"Hello, World!"==ascii::wstring{L"Hello, World!"});

   assert(ascii::string{"characters: '\xFF' and '\xAA'"} == "characters: '?' and '?'" );
   assert(ascii::string(10,char(0xFF))=="??????????");
}

void test_02() {

   try {
      using ascii_string = ascii::basic_string<char,ascii::sieve_exception<char>>;
      const ascii_string s = "characters: '\xFF' and '\xAA'";
   }
   catch(const std::invalid_argument& e) {
      assert(std::string{"no ascii character"} == e.what());
      return;
   }
   assert(false);
}

void test_02a() {

   try {
      using ascii_string = ascii::basic_string<char,ascii::sieve_exception<char,std::domain_error>>;
      const ascii_string s = "characters: '\xFF' and '\xAA'";
   }
   catch(const std::domain_error& e) {
      assert(std::string{"no ascii character"} == e.what());
      return;
   }
   assert(false);
}

void test_03() {
   const ascii::string s1 {"Hello"};
   const ascii::string s2 {"World!"};
   const auto s3 = s1 + ", " + s2;
   assert(s3 =="Hello, World!");
   static_assert(is_same_v<decltype(s1),decltype(s3)>);
}

void test_04() {
   using ascii::cast;
   assert(cast<ascii::string> (std::string { "Hello, World!"})  ==  "Hello, World!");
   assert(cast<ascii::wstring>(std::string { "Hello, World!"})  == L"Hello, World!");
   assert(cast<ascii::wstring>(std::wstring{L"Hello, World!"})  == L"Hello, World!");
   assert(cast<ascii::string> (std::wstring{L"Hello, World!"})  ==  "Hello, World!");

   assert(cast<ascii::string> (std::string { "Copyright symbol: '\xB8'"})        == "Copyright symbol: '?'");
   assert(cast<ascii::string> (std::wstring{L"Copyright symbol: '\xA9', Utf-16"}) == "Copyright symbol: '?', Utf-16");
}

int main()
{
   test_01();
   test_02();
   test_02a();
   test_03();
   test_04();
}

