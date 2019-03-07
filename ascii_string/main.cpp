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
constexpr bool is_ascii(const CharT& ch) noexcept {
   return 0x0 <= ch && ch <= 0x7F;  // https://en.cppreference.com/w/cpp/language/ascii
}

template <typename CharT>
struct sieve_exception {
   template <typename CharU>
   constexpr bool operator()(CharT& l, CharU&& r) const {
      return is_ascii(r)?
            l=std::forward<CharU>(r), true
         :  throw std::invalid_argument{"no ascii character"}, false
      ;
   } 
};

template <typename CharT>
struct sieve_skip {
   template <typename CharU>
   constexpr bool operator()(CharT& l, CharU&& r) const {
      return is_ascii(r)?
            l=std::forward<CharU>(r), true
         :  false
      ;
   } 
};

template <typename CharT>
struct sieve_question_mark {
   template <typename CharU>
   constexpr bool operator()(CharT& l, CharU&& r) const {
      return is_ascii(r)?
            l=std::forward<CharU>(r), true
         :  l='?', true // <--- yes, it's correct. the assignment has taken a place
      ;
   } 
};

template <typename CharT, typename SieveT>
struct char_traits : std::char_traits<CharT> {
   static constexpr void assign( CharT& l, const CharT& r ) {
      SieveT{}(l,r);
   }
   static constexpr CharT* copy( CharT* dest, const CharT* src, std::size_t count ) {
      for (std::size_t i{0}; i < count; ++i)
         if(SieveT{}(*dest,src[i]))
            ++dest;   
      return dest;
   }
   static constexpr CharT* move( CharT* dest, const CharT* src, std::size_t count ) {
      for (std::size_t i{0}; i < count; ++i)
         if(SieveT{}(*dest,std::move(src[i])))
            ++dest;   
      return dest;
   }
};

template <typename CharT, typename SieveT=sieve_skip<CharT>, typename AllocT=std::allocator<CharT>>
struct basic_string : std::basic_string<CharT,char_traits<CharT,SieveT>,AllocT> {
   using base_type = std::basic_string<CharT,char_traits<CharT,SieveT>,AllocT>;
   using base_type::base_type;

   template <typename TraitsU, typename AllocU>
   basic_string(const std::basic_string<CharT,TraitsU,AllocU>& other) : basic_string(other.c_str(),other.size()) {}

   template <typename TraitsU, typename AllocU>
   operator std::basic_string<CharT,TraitsU,AllocU>() const {
      return std::basic_string<CharT,TraitsU,AllocU>{data(),size()};
   } 
};

template<typename CharT, typename SieveT, typename AllocT, typename TraitsU, typename AllocU>
constexpr bool operator==(const ascii::basic_string<CharT,SieveT,AllocT>& l, const std::basic_string<CharT,TraitsU,AllocU>& r) noexcept {
   return l.size()==r.size()? std::equal(l.data(),l.data()+l.size(),r.data()) : false;
}

template<typename CharT, typename SieveT, typename AllocT, typename TraitsU, typename AllocU>
constexpr bool operator==(const std::basic_string<CharT,TraitsU,AllocU>& l, const ascii::basic_string<CharT,SieveT,AllocT>& r) noexcept {
   return r==l;
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
   const ascii::string  s1 {"Hello, World!"};
   assert(s1=="Hello, World!");
   assert("Hello, World!"==s1);
   const ascii::wstring s2 {L"Hello, World!"};
   assert(s2==L"Hello, World!");
   assert(L"Hello, World!"==s2);

   const std::string  stds {"Hello, World!"};
   const std::wstring stdw {L"Hello, World!"};
   assert(stds == static_cast<std::string>(ascii::string{stds}) );
   assert(static_cast<std::wstring>(ascii::wstring{stdw}) == stdw );
   assert(stds == ascii::string{stds} );
   assert(ascii::wstring{stdw} == stdw );
}


int main()
{
   test_01();
}

