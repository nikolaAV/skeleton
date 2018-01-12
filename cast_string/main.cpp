#include <iostream>
#include <cassert>
#include "string_cast.h"

using namespace std;

struct point
{
   size_t x;
   size_t y;
};

inline bool operator==(const point& left, const point& right)
{
   return left.x==right.x && left.y==right.y;
}

template <typename charT, typename traits>
inline std::basic_ostream<charT,traits>& operator<<(std::basic_ostream<charT, traits>& out, const point p)
{
   return out << p.x << " " << p.y;
}

template <typename charT, typename traits>
inline std::basic_istream<charT, traits>& operator>>(std::basic_istream<charT, traits>& from, point& p)
{
   return from >> p.x >> p.y;
}

int main()
{
   auto const s1 = string_cast<string>('5');
   auto const s2 = string_cast<string>(55);
   auto const s3 = string_cast<wstring>(5.123);
   auto const s4 = string_cast<wstring>((double)100000000000.1236);
   auto const s5 = string_cast<string>(point{3,4});

   cout << "char: "     << string_cast<char>(s1) << endl
        << "int: "      << string_cast<int>(s2) << endl
        << "float: "    << string_cast<float>(s3) << endl
        << "double: "   << string_cast<double>(s4) << endl
        << "my point: " << string_cast<point>(s5) << endl;

   assert('5'           == string_cast<char> (s1));
   assert( 55           == string_cast<int>  (s2));
//   assert( 5.123        == string_cast<float>(s3)); <-- issue with precision for floating point types!
   assert((point{3,4})  == string_cast<point>(s5));

   try
   {
      cout << string_cast<long>("l34"); // 'l' - is not digit
   }
   catch(const std::exception& e)
   {
      cout << e.what();
   }

   cin.get();
}
