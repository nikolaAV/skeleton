#if !defined( _SAFE_STRING_CAST_H__)
#define _SAFE_STRING_CAST_H__

#include <sstream>
#include <string>

/**
   \brief How do I convert an integer to a string?

   The simplest way is to use a stringstream.
   this technique works for converting any type that you can output using << to a string.

   \code
      auto const s1 = string_cast<string>('5');
      auto const s2 = string_cast<string>(55);
      auto const s3 = string_cast<wstring>(5.123);

      cout  << "char: "  << string_cast<char>(s1) << endl
            << "int: "   << string_cast<int>(s2) << endl
            << "float: " << string_cast<float>(s3) << endl;
   \endcode

   \remark http://www.stroustrup.com/bs_faq2.html#int-to-string,
           !!! string conversion based on stream technique does not guarantee precision for floating point types !!!
           For more details please see the discussion at https://github.com/nlohmann/json/issues/360

   \note since C++17 there is a possibility to use
      - http://en.cppreference.com/w/cpp/string/basic_string/to_string
      - http://en.cppreference.com/w/cpp/utility/to_chars as a higher-performance locale-independent alternative
*/

namespace selector
{
   template <typename>  struct stream;
   template <>  struct stream<std::string>      { using type = std::stringstream; };
   template <>  struct stream<std::wstring>     { using type = std::wstringstream; };
   template <>  struct stream<const char*>      { using type = std::stringstream; };
   template <>  struct stream<const wchar_t*>   { using type = std::wstringstream; };

   template <typename>  struct buffer;
   template <>  struct buffer<const char*>      { using type = std::string; };
   template <>  struct buffer<const wchar_t*>   { using type = std::wstring; };

}  // end namespace selector

template <typename TO_TYPE, typename FROM_TYPE>
inline TO_TYPE string_cast(const FROM_TYPE&  v)
{
   typename selector::stream<TO_TYPE>::type ss;
   ss << std::fixed 
      // << std::setprecision(...) is meaningful for all floating-point types 
      << v;
   return ss.str();
}

namespace private_
{
   template <typename T>
   inline const typename selector::buffer<T>::type buffer(const T& s) { return s; }
   inline const std::string&  buffer(const std::string& s) { return s; }
   inline const std::wstring& buffer(const std::wstring& s) { return s; }

#ifdef __GNUG__
   #pragma GCC diagnostic push
   #pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

   template <typename TO_TYPE, typename FROM_TYPE>
   inline TO_TYPE from_string(const FROM_TYPE&  s)
   {
      typename selector::stream<FROM_TYPE>::type ss{ buffer(s) };
      ss.exceptions(std::ios::failbit);
      TO_TYPE out {};
      ss >> out;
      return out;
   }

#ifdef __GNUG__
   #pragma GCC diagnostic pop
#endif


} // end of namespace private_

template <typename TO_TYPE>
inline TO_TYPE string_cast(const std::string&  s)
{
   return private_::from_string<TO_TYPE>(s);
}

template <typename TO_TYPE>
inline TO_TYPE string_cast(const char* s)
{
   return private_::from_string<TO_TYPE>(s);
}

template <typename TO_TYPE>
inline TO_TYPE string_cast(const std::wstring&  s)
{
   return private_::from_string<TO_TYPE>(s);
}

template <typename TO_TYPE>
inline TO_TYPE string_cast(const wchar_t* s)
{
   return private_::from_string<TO_TYPE>(s);
}


#endif // _SAFE_STRING_CAST_H__
