/**
   \see https://github.com/nikolaAV/skeleton/tree/master/ascii_string 
*/

#include <stdwarnings_suppress_on.h>
#include <string>
#include <stdexcept>
#include <utility>
#include <iterator>
#include <ostream>
#include <istream>
#include <type_traits>
#include <algorithm>
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
   constexpr CharT operator()(const CharU&) const {
      // https://github.com/nikolaAV/Modern-Cpp/tree/master/constexpr/invocation_context
      throw ExceptionT{"no ascii character"}; //, CharT{}; <-- expression result unused [-Wunused-value]
   } 
};

template <typename CharT, char Char>
struct replacement {
   template <typename CharU>
   constexpr CharT operator()(const CharU&) const noexcept {
      return CharT{Char};
   } 
};

template <typename CharT, typename ViolationHandlerT>
struct basic_sieve {
   template <typename CharU>
   constexpr void operator()(CharT& left, CharU&& right) const {
      if(is_ascii(right))
         if constexpr (std::is_same_v<std::decay_t<CharT>,std::decay_t<CharU>>)
            left=std::forward<CharU>(right);
         else
            left=static_cast<CharT>(right);
      else
         left=ViolationHandlerT{}(right);
   } 
};

template <typename CharT, typename SieveT>
struct char_traits : std::char_traits<CharT> {
   template <typename CharU>
   static constexpr void assign( CharT& left, const CharU& right ) {
      sieve_(left,right);
   }
   template <typename CharU>
   static constexpr void assign( CharT* dest, std::size_t count, const CharU& right ) {
      std::for_each(dest,std::next(dest,count),[&](CharT& left){
         sieve_(left,right);   
      });
   }
   template <typename CharU>
   static constexpr CharT* copy( CharT* dest, const CharU* src, std::size_t count ) {
      std::for_each(dest,std::next(dest,count),[&](CharT& left){
         sieve_(left,*src++);   
      });
      return dest;
   }
   template <typename CharU>
   static constexpr CharT* move( CharT* dest, const CharU* src, std::size_t count ) {
      std::for_each(dest,std::next(dest,count),[&](CharT& left){
         sieve_(left,std::move(*src++));   
      });
      return dest;
   }
private:
   static constexpr SieveT sieve_{};
};

/**

*/

template <typename StrToT, typename StrFtomT>
StrToT cast(const StrFtomT& s) {
   using traits_type = typename StrToT::traits_type;
   StrToT out(s.size(), ' ');
   traits_type::copy(out.data(),s.data(),s.size());
   return out;
} 

template <typename CharT, typename ExceptionT = std::invalid_argument>
using sieve_exception   = basic_sieve<CharT,throw_exception<CharT,ExceptionT>>;
template <typename CharT, char Char = '?'>
using sieve_replacement = basic_sieve<CharT,replacement<CharT,Char>>;

template <
   typename CharT, 
   typename SieveT = sieve_replacement<CharT>, 
   typename AllocT = std::allocator<CharT>>
using basic_string = std::basic_string<CharT,char_traits<CharT,SieveT>,AllocT>;

/**

*/

template <typename CharT, typename SieveT, typename AllocT, typename TraitsT>
void swap(ascii::basic_string<CharT,SieveT,AllocT>& left, std::basic_string<CharT,TraitsT,AllocT>& right) {
   static_assert(sizeof(left)==sizeof(right));  // <-- just in case
   auto p = (std::basic_string<CharT,TraitsT,AllocT>*)(void*)&left;
   right.swap(*p);
   using traits_type = typename std::decay_t<decltype(left)>::traits_type;
   try {
      traits_type::move(left.data(),left.data(),left.size());
   } catch (...) {
      right.swap(*p);
      throw;
   }
}

template <typename CharT, typename SieveT, typename AllocT, typename TraitsT>
void swap(std::basic_string<CharT,TraitsT,AllocT>& left, ascii::basic_string<CharT,SieveT,AllocT>& right) {
   swap(right,left);
}

template<typename CharT, typename TraitsT, typename SieveT, typename AllocT>
std::basic_ostream<CharT,TraitsT>& operator<<(std::basic_ostream<CharT,TraitsT>& os, const basic_string<CharT,SieveT,AllocT>& s) {
   return os.write(s.data(),s.size());
}

template<typename CharT, typename TraitsT, typename SieveT, typename AllocT>
std::basic_istream<CharT,TraitsT>& operator>>(std::basic_istream<CharT,TraitsT>& is, basic_string<CharT,SieveT,AllocT>& s) {
   std::basic_string<CharT,std::char_traits<CharT>,AllocT> out;
   is >> out;
   swap(s,out);
   return is;
}

using string  = basic_string<char>;
using wstring = basic_string<wchar_t>;

namespace literals
{
inline string operator "" _ascii(const char *str, std::size_t len) {
   return string(str,len);
}
   
inline wstring operator "" _ascii(const wchar_t *str, std::size_t len) {
   return wstring(str,len);
}

}  // end namespace literals

}  // end namespace ascii

///////////  Example Of Usage

#include <stdwarnings_suppress_on.h>
#include <iostream>
#include <cassert>
#include <sstream>
#include <stdwarnings_suppress_off.h>

using namespace std; 

void test_01() {
   assert(ascii::string { "Hello, World!"}== "Hello, World!");
   assert(ascii::wstring{L"Hello, World!"}==L"Hello, World!");
   assert( "Hello, World!"==ascii::string { "Hello, World!"});
   assert(L"Hello, World!"==ascii::wstring{L"Hello, World!"});

   assert(ascii::string{"characters: '\xFF' and '\xAA'"} == "characters: '?' and '?'" );
   assert(ascii::string(10,char(0xFF))=="??????????");
}

void test_01a() {
   using namespace ascii::literals;

   assert( "Hello, World!"_ascii  == "Hello, World!");
   assert(L"Hello, World!"_ascii  ==L"Hello, World!");
   assert( "Hello, World!"== "Hello, World!"_ascii);
   assert(L"Hello, World!"==L"Hello, World!"_ascii);
}

void test_02() {

   try {
      using ascii_string = ascii::basic_string<char,ascii::sieve_exception<char>>;
      const ascii_string s = R"(characters: 'ÿ' and 'ª')";
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

void test_02b() {
   using ascii_string = ascii::basic_string<char,ascii::sieve_replacement<char,'#'>>;
   assert(ascii_string{"characters: '\xFF' and '\xAA'"} == "characters: '#' and '#'" );
   assert(ascii_string(10,char(0xFF))=="##########");
}

void test_03() {
   using namespace ascii::literals;

   const auto s1 = "Hello"_ascii;
   const auto s2 = "World!"_ascii;
   const auto s3 = s1 + ", " + s2;
   assert(s3 =="Hello, World!");
   static_assert(is_same_v<decltype(s1),decltype(s3)>);
}

void test_04() {
   using ascii::cast;
   assert(cast<ascii::string> ( "Hello, World!"s)  ==  "Hello, World!");
   assert(cast<ascii::wstring>( "Hello, World!"s)  == L"Hello, World!");
   assert(cast<ascii::wstring>(L"Hello, World!"s)  == L"Hello, World!");
   assert(cast<ascii::string> (L"Hello, World!"s)  ==  "Hello, World!");

   assert(cast<ascii::string> ( "Copyright symbol: '\xB8'"s)         == "Copyright symbol: '?'");
   assert(cast<ascii::string> (L"Copyright symbol: '\xA9', Utf-16"s) == "Copyright symbol: '?', Utf-16");
}

void test_05() {
   using namespace ascii::literals;
   {
      stringstream ss;
      ss << "Hello, World!"_ascii;
      assert(ss.str() =="Hello, World!");
   }
   {
      wstringstream ss;
      ss << L"Hello, World!"_ascii;
      assert(ss.str() == L"Hello, World!");
   }
   {
      stringstream ss;
      ss << "characters: '\xFF' and '\xAA'"_ascii;
      assert(ss.str() == "characters: '?' and '?'");
   }
}

void test_06() {
   ascii::string  s1 {"C++ protects against accident, not against fraud"};
   std::string    s2 {"Bjarne Stroustrup"};

   swap(s1,s2);
   assert("Bjarne Stroustrup"==s1);
   assert("C++ protects against accident, not against fraud"==s2);

   swap(s2,s1);
   assert("Bjarne Stroustrup"==s2);
   assert("C++ protects against accident, not against fraud"==s1);
}

void test_06a() {
   ascii::string  s1;
   std::string    s2 {"characters: '\xFF' and '\xAA'"};
   swap(s1,s2);
   assert(s1=="characters: '?' and '?'");
   assert(s2.empty());
}

void test_07() {
   const std::string    original_message {"The C++ Programming Language"};
                                       //   0   1     2          3
   std::istringstream   is {original_message};
   std::ostringstream   os;
   ascii::string        s0, s1, s2, s3;

   is >> s0 >> s1 >> s2 >> s3;
   assert(s0 +" "+ s1 +" "+ s2+ " " +s3 ==original_message.c_str());
   os << s0 << " " << s1 << " " << s2 << " " << s3;
   assert(original_message == os.str());
}

void test_07a() {
   ascii::string        s;
//   std::istringstream   is {R"(German-umlaut:'ä'-and-euro-symbol:'¤')"};  // ISO Latin-15 encoding
   std::istringstream   is {"German-umlaut:'\xE4'-and-euro-symbol:'\xA4'"};  // ISO Latin-15 encoding
   is >> s;
   assert(s=="German-umlaut:'?'-and-euro-symbol:'?'");
}

int main()
{
   test_01();
   test_01a();
   test_02();
   test_02a();
   test_02b();
   test_03();
   test_04();
   test_05();
   test_06();
   test_06a();
   test_07();
   test_07a();
}
