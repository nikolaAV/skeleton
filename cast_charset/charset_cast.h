#ifndef _CHARSET_WIDEN_NARROW_CAST_INCLUDED_
#define _CHARSET_WIDEN_NARROW_CAST_INCLUDED_

#include <string>
#include <locale>
#include <cassert>
#include <cstring>
#include <cwchar>

#ifdef _MSC_VER
    #pragma warning( push )
    #pragma warning( disable: 4996 )  // warning C4996: '<name>': Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'
    #pragma warning( disable: 26481 ) // warning C26481: Don't use pointer arithmetic. Use span instead. (bounds.1: http://go.microsoft.com/fwlink/p/?LinkID=620413)
#endif


/**
   \brief transformation strings of fixed charsets into wstrings and vice versa

      Signatures:
         std::wstring to_wstring(const char*,          std::locale);
         std::wstring to_wstring(const std::string&,   std::locale);
            // Shorter versions which use the global C++ locale object 
            // or a copy of std::locale::classic if no call to std::locale::global has been made.
         std::wstring charset_cast<std::wstring>(const char*)
         std::wstring charset_cast<std::wstring>(const std::string&)
         std::wstring wstring_cast(const char*)
         std::wstring wstring_cast(const std::string&)

         std::string to_string(const wchar_t*,         std::locale);
         std::string to_string(const std::wstring&,    std::locale);
            // Shorter versions which use the global C++ locale object 
            // or a copy of std::locale::classic if no call to std::locale::global has been made.
         std::string charset_cast<std::string>(const wchar_t*)
         std::string charset_cast<std::string>(const std::wstring&)
         std::string string_cast(const wchar_t*)
         std::string string_cast(const std::wstring&)

      where 
         std::string can be used to represent the following character sets:
                     US-ASCII (7-bit character set)
                     ISO-LAtin-1 or ISO-8859-1  (8-bit character set)
                     ISO-LAtin-9 or ISO-8859-15 (8-bit character set)
         std::wtring can be used to represent the following character sets:
                     UCS-2 (16-bit fixed-sized character set)
                     UCS-4 or UTF-32 (32-bit fixed-sized character set)                      


   \exception     std::bad_cast if std::has_facet<Facet>(loc) == false.     
   \remark        "The Standard C++ Library" by Nicolai M.Josuttis. 16.4.4 Character Classification and Conversion
*/

namespace details_
{
   inline
   std::wstring to_wstring(const char* begin, const size_t len, const std::locale& loc)
   {
      using namespace std;
      assert(begin);
      if(!len)
         return wstring();
      wstring out(static_cast<wstring::size_type>(len),' ');
      use_facet<ctype<wchar_t>>(loc).widen(begin,begin+len,&out[0]);
         return out;
   } 

   inline
   std::string to_string(const wchar_t* begin, const size_t len, const std::locale& loc)
   {
      using namespace std;
      assert(begin);
      if(!len)
         return string();
      string out(static_cast<string::size_type>(len), ' ');
      use_facet<ctype<wchar_t>>(loc).narrow(begin, begin+len, '?', &out[0]);
         return out;
   }

} // details_


/**
   Function to_wstring can be used to convert a character sequence of type 'char' from the native character set
   to the corresponding wide character sequence in the character set used by a locale.

   \param[in] s   input character sequence of type 'char' or std::string (overload version)
   \param[in] loc requested locale
   \retval        character sequence of 'wchar_t' holded by std::wstring

   \example: convert German string with umlaut and euro symbol into wide-character string
      std::string in   = "nj: ¨a + \xA4 1";
      std::wstring out = to_wstring(in,std::locale("de_DE.ISO-8859-15"));
*/

inline
std::wstring to_wstring(const char ch, const std::locale& loc)
{
   return details_::to_wstring(&ch,1,loc);
} 

template <size_t N> 
inline
std::wstring to_wstring(const char (&arr)[N], const std::locale& loc)
{
    return details_::to_wstring(arr, N-1, loc);
}

inline
std::wstring to_wstring(const char* s, const std::locale& loc)
{
   assert(s);
   return details_::to_wstring(s, std::strlen(s), loc);
}

inline
std::wstring to_wstring(const std::string& s, const std::locale& loc)
{
   return details_::to_wstring(s.c_str(), s.size(), loc);
}

template <typename OUT_STRING_TYPE> inline OUT_STRING_TYPE charset_cast(const char*);
template <typename OUT_STRING_TYPE> inline OUT_STRING_TYPE charset_cast(const char);
template <typename OUT_STRING_TYPE> inline OUT_STRING_TYPE charset_cast(const std::string&);

/**
   "lightwight" version of to_wstring which uses global classic locate i.e. according to user defined environment   

   \param[in] s   input character sequence of type 'char' or std::string (overload version)
   \retval        character sequence of 'wchar_t' holded by std::wstring

   \example: convert German string (default user locale environment) with umlaut and euro symbol into wide-string 
      auto out = charset_cast<std::wstring>("nj: ¨a + \xA4 1");
*/

template<>
inline
std::wstring charset_cast<std::wstring>(const char* s)
{
   assert(s);
   return to_wstring(s,std::locale());
} 

template<>
inline
std::wstring charset_cast<std::wstring>(const char ch)
{
   return to_wstring(ch,std::locale());
} 

template<>
inline
std::wstring charset_cast<std::wstring>(const std::string& s)
{
   return to_wstring(s, std::locale());
}

inline std::wstring wstring_cast(const char* s)         { return charset_cast<std::wstring>(s); }
inline std::wstring wstring_cast(const char ch)         { return charset_cast<std::wstring>(ch); }
inline std::wstring wstring_cast(const std::string& s)  { return charset_cast<std::wstring>(s); }

/**
   The opposite direction (wchar_t -> char), 
   Function to_string can be used to convert a character sequence from the wide-character set 
   to a corresponding char sequence in the native character set (locale), provided that there is such a char.

   \param[in] s   input character sequence of type 'wchar_t' or std::wstring (overload version)
   \param[in] loc requested locale
   \retval        character sequence of 'char' holded by std::string
   \note          symbol '?' represents not existing wide-character for requested locale 

   \example: conver German string (wstring) with umlaut and euro symbol into 'char' string
   std::wstring in = L"nj: ¨a + \xA4 1";
   std::string out = to_string(in,std::locale("de_DE.ISO-8859-15"));
*/

inline
std::string to_string(const wchar_t ch, const std::locale& loc)
{
   return details_::to_string(&ch,1,loc);
}

template <size_t N>
inline
std::string to_string(const wchar_t (&arr)[N], const std::locale& loc)
{
    return details_::to_string(arr, N-1, loc);
}

inline
std::string to_string(const wchar_t* s, const std::locale& loc)
{
   assert(s);
   return details_::to_string(s, std::wcslen(s), loc);
}

inline
std::string to_string(const std::wstring& s, const std::locale& loc)
{
   return details_::to_string(s.c_str(), s.size(), loc);
}

template <typename OUT_STRING_TYPE> inline OUT_STRING_TYPE charset_cast(const wchar_t*);
template <typename OUT_STRING_TYPE> inline OUT_STRING_TYPE charset_cast(const wchar_t);
template <typename OUT_STRING_TYPE> inline OUT_STRING_TYPE charset_cast(const std::wstring&);

/**
   "lightwight" version of to_string which uses global classic locate i.e. according to user defined environment

   \param[in] s   input character sequence of type 'wchar_t' or std::wstring (overload version)
   \retval        character sequence of 'char' holded by std::string
   \note          symbol '?' represents not existing wide-character for requested locale

   \example: convert German string (wide char literal) with umlaut and euro symbol into single char string, using default user locale environment
      auto out = charset_cast<std::string>(L"nj: ¨a + \xA4 1");
*/

template<>
inline
std::string charset_cast<std::string>(const wchar_t* s)
{
   assert(s);
   return to_string(s, std::locale());
}

template<>
inline
std::string charset_cast<std::string>(const wchar_t ch)
{
   return to_string(ch, std::locale());
}

template<>
inline
std::string charset_cast<std::string>(const std::wstring& s)
{
   return to_string(s, std::locale());
}

inline std::string string_cast(const wchar_t* s)        { return charset_cast<std::string>(s); }
inline std::string string_cast(const wchar_t ch)        { return charset_cast<std::string>(ch); }
inline std::string string_cast(const std::wstring& s)   { return charset_cast<std::string>(s); }

#ifdef _MSC_VER
    #pragma warning( pop )
#endif

#endif // _CHARSET_WIDEN_NARROW_CAST_INCLUDED_

