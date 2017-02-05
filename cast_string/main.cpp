#include <iostream>
#include "string_cast.h"

using namespace std;

struct point
{
   size_t x;
   size_t y;
};

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
   auto const s4 = string_cast<string>(point{3,4});

   cout << "char: "     << string_cast<char>(s1) << endl
        << "int: "      << string_cast<int>(s2) << endl
        << "float: "    << string_cast<float>(s3) << endl
        << "my point: " << string_cast<point>(s4) << endl;

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
