#ifndef _TYPE_SAFE_PRINTF_INCLUDED_
#define _TYPE_SAFE_PRINTF_INCLUDED_

#include <stdexcept>
#include <iostream>
#include <type_traits>

namespace type_safe
{

/**
   \brief type safe analogue of C run time function int printf( const char* format, ... ); http://en.cppreference.com/w/cpp/io/c/fprintf 

   Signatures:
      void         printf(ostream& out, const char* formant, ...);
      void         printf(wostream& out, const wchar_t* formant, ...);
      void         printf (const char* formant, ...);    // to std::cout 
      void         printf (const wchar_t* formant, ...); // to std::wcout
      std::string  sprintf(const char* formant, ...);
      std::wstring sprintf(const wchar_t* formant, ...);
   
   \return        void
   \exception     std::runtime_error in case of plain '%' number does not match number of values to be printed  
   \remark        idea comes from http://www.stroustrup.com/C++11FAQ.html#variadic-templates 
*/


template <typename charT, typename traits>
void printf(std::basic_ostream<charT,traits>& out, const charT* s)
{
    while (s && *s)
    {
        // make sure that there wasn't meant to be more arguments
        // %% represents plain % in a format string
        if (*s == '%' && *++s != '%')	
            throw std::runtime_error("invalid format: missing arguments");
        out << *s++;
    }
}

template<typename charT, typename traits, typename valT, typename... Args>
void printf(std::basic_ostream<charT, traits>& out, const charT* s, const valT& value, const Args&... args)
{
    while (s && *s) 
    {
        if (*s == '%' && *++s != '%') // %% represents plain % in a format string
        {
                out << value;
                return printf(out, s, args...); // recursion: ``peel off'' first argument
        }
        out << *s++;
    }
    throw std::runtime_error("extra arguments provided to printf");
}

namespace details_
{
    template <typename T> struct selector;
    template<> struct selector<char>
    {
        using string_t          = std::string;
        using stringstream_t    = std::stringstream;
        using consolestream_t   = std::ostream;
        static consolestream_t& console() { return std::cout; }
    };
    template<> struct selector<wchar_t>
    {
        using string_t          = std::wstring;
        using stringstream_t    = std::wstringstream;
        using consolestream_t   = std::wostream;
        static consolestream_t& console() { return std::wcout; }
    };

    template <typename charT1, typename charT2> struct is_one_of;
    /**
        is_one_of<charT1,charT1> 
        equivalent to
        std::enable_if<std::is_same<charT1,char>::value || std::is_same<charT2, wchar_t>::value>
        where
            charT1 = char, charT2 = wchar_t  
            charT1 = wchar_t, charT2 = char
    */
    template <> struct is_one_of<char,wchar_t> { using type = void; };
    template <> struct is_one_of<wchar_t,char> { using type = void; };

}  // namespace details_

template
<   
      typename charT, typename... Args 
     ,typename = typename details_::is_one_of<char,wchar_t>::type
>
void printf(const charT* s, const Args&... args)
{
    printf(details_::selector<charT>::console(), s, args...);
}

template
<
     typename charT, typename... Args
   , typename = typename details_::is_one_of<char, wchar_t>::type
>
auto sprintf(const charT* s, const Args&... args) -> typename details_::selector<charT>::string_t
{
    typename details_::selector<charT>::stringstream_t ss;
    printf(ss, s, args...);
    return ss.str();
}

} // namespace type_safe

#endif // _TYPE_SAFE_PRINTF_INCLUDED_

