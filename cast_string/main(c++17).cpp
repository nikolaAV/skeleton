#include <iostream>
#include <string>
#include <limits>
#include <charconv>
#include <array>
#include <iterator>
#include <cassert>

/**
   C++17 higher-performance locale-independent alternative conversion based on
   pair to_chars/from_chars (integers only)

   !!! Under c onstruction !!!
*/

template <typename value_type>
inline std::string to_chars(const value_type& v)
{
   constexpr size_t size = std::numeric_limits<value_type>::digits10+1;
   std::array<char, size> str{};
   auto const res = std::to_chars(str.data(), str.data()+str.size(), v);
   assert( 0<=std::distance(str.data(),res.ptr) );
   const size_t real_length = std::distance(str.data(),res.ptr);
   assert( real_length <= size);
   return std::string(str.data(), str.data()+real_length);
}

inline std::string to_chars(const char ch)
{
   return std::string(1,ch);
}

inline std::string to_chars(const unsigned char ch)
{
   return to_chars(static_cast<char>(ch));
}

template <typename value_type>
inline bool from_chars(const std::string& s, value_type& value)
{
   std::from_chars(s.data(), s.data()+s.size(), value);
   return true;
}


// string_cast<>
// ---------------------------
// integer -> (w)string -> ingetger conversion is only supported 
// std::to_chars(float)/std::from_chars(float) are not implemented yet.

template <typename TO_TYPE, typename FROM_TYPE>
inline TO_TYPE string_cast(const FROM_TYPE&  v)
{
   return to_chars(v);
}

template <typename TO_TYPE>
inline TO_TYPE string_cast(const std::string&  s)
{
   TO_TYPE result{};
   from_chars(s,result);
   return result;
}

using namespace std;


int main()
{
   auto const s1 = string_cast<string>('5');
   auto const s2 = string_cast<string>(55);
   auto const s3 = string_cast<string>(4);
   auto const s4 = string_cast<string>(100);
//   auto const s5 = string_cast<wstring>(5.123); now not supported

   cout  << s1 << endl
         << s2 << endl
         << s3 << endl
         << s4 << endl;

   cout  //<< s1 << endl
         << string_cast<int>(s2) << endl
         << string_cast<long>(s3) << endl
         << string_cast<size_t>(s4) << endl;
}

