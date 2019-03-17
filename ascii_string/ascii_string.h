#ifndef ASCII_STRING_INCLUDED__
#define ASCII_STRING_INCLUDED__

/**
   Q: Can I be sure that my C++ program will only get a set of Ascii characters
   A: Yes, if a special string type holding only Ascii characters will be provided 

   \see https://github.com/nikolaAV/skeleton/tree/master/ascii_string 
*/

#include <string>
#include <string_view>
#include <stdexcept>
#include <utility>
#include <ostream>
#include <istream>
#include <type_traits>
#include <algorithm>

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

/**
   basic_sieve specifies a filter how non-ascii character must be presented in a ascii sequence
*/

template <typename CharT, typename ViolationHandlerT>
struct basic_sieve {
   using violation_type = ViolationHandlerT;
   template <typename CharU>
   constexpr void operator()(CharT& left, CharU&& right) const {
      if(is_ascii(right))
         if constexpr (std::is_same_v<std::decay_t<CharT>,std::decay_t<CharU>>)
            left=std::forward<CharU>(right);
         else
            left=static_cast<CharT>(right);
      else
         left=violation_type{}(right);
   } 
};

template <typename CharT, typename ExceptionT = std::invalid_argument>
using sieve_exception   = basic_sieve<CharT,throw_exception<CharT,ExceptionT>>;
template <typename CharT, char Char = '?'>
using sieve_replacement = basic_sieve<CharT,replacement<CharT,Char>>;

/**
   ascii::char_traits 
   Provides basic character and string operations for ASCII symbols.
   The defined operation set is such that generic algorithms almost always can be implemented in terms of it.

   The ascii::char_traits<> class template satisfies the requirements of CharTraits.
   \see https://en.cppreference.com/w/cpp/named_req/CharTraits
*/

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
   ascii::basic_string is of std::basic_string-like class template.
   The definitions of the operations are supplied via the Traits template parameter - ascii::char_traits<>
*/

template <
   typename CharT, 
   typename SieveT = sieve_replacement<CharT>, 
   typename AllocT = std::allocator<CharT>>
using basic_string = std::basic_string<CharT,char_traits<CharT,SieveT>,AllocT>;


/**
   Gives a mechanism how ascii- and std- strings can be convertible between each other.
   Provided that there is existence of elementary conversion between character types they hold. 

   \code
      str::string    s1 = ...;
      ascii::string  as = cast<ascii::string>(s1);
      ascii::wstring aw = cast<ascii::wstring>(s1);

      ascii::wstring a = ...;
      std::wstring   w = cast<std::wstring>(a);
      std::string    s = cast<std::string>(a);
   \endcode
*/

template <typename StrToT, typename StrFtomT>
StrToT cast(const StrFtomT& s) {
   using traits_type = typename StrToT::traits_type;
   StrToT out(s.size(), ' ');
   traits_type::copy(out.data(),s.data(),s.size());
   return out;
} 

/**
   swap(ascii::string&,std::string&);
   swap(std::string&,ascii::string&);

   Swaps the characters pointed to by 'compatible' strings
   where 'compatible' means
      - Both CharT are identical
      - Both AllocT are identical

   \code
      str::string    s = "world!";
      ascii::string  a = "Hello";
      swap(s,a);
   \endcode

   During swapping non-ascii characters are handled as specified in char_traits for the given ascii::string
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

/**
   writes ascii::string into compatible stream (ChatrTs are equivalent)
*/
template<typename CharT, typename TraitsT, typename SieveT, typename AllocT>
std::basic_ostream<CharT,TraitsT>& operator<<(std::basic_ostream<CharT,TraitsT>& os, const basic_string<CharT,SieveT,AllocT>& s) {
   return os.write(s.data(),s.size());
}

/**
   reads ascii::string from compatible stream (ChatrTs are equivalent)
   If it's needed non-ascii characters are handled as specified in char_traits for the given ascii::string
*/
template<typename CharT, typename TraitsT, typename SieveT, typename AllocT>
std::basic_istream<CharT,TraitsT>& operator>>(std::basic_istream<CharT,TraitsT>& is, basic_string<CharT,SieveT,AllocT>& s) {
   std::basic_string<CharT,std::char_traits<CharT>,AllocT> out;
   is >> out;
   swap(s,out);
   return is;
}

using string  = basic_string<char>;
using wstring = basic_string<wchar_t>;

template <typename StringT>
using basic_string_view = std::basic_string_view<typename StringT::value_type,typename StringT::traits_type>;

using string_view  = basic_string_view<string>;
using wstring_view = basic_string_view<wstring>;

}  // end namespace ascii

namespace ascii_literals
{

inline ascii::string operator "" _ascii(const char *str, std::size_t len) {
   return ascii::string(str,len);
}
   
inline ascii::wstring operator "" _ascii(const wchar_t *str, std::size_t len) {
   return ascii::wstring(str,len);
}

}  // end ascii_namespace literals


#endif // ASCII_STRING_INCLUDED__

