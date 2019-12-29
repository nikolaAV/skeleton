#include <type_traits>
#include <utility>
#include <cstdlib>

/*
   Literal operator templates for strings
   --------------------------------------
   https://habr.com/ru/post/243581/
   http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0424r2.pdf
   https://stackoverflow.com/a/28209546
   http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3599.html
   https://groups.google.com/a/isocpp.org/forum/#!topic/std-proposals/X4rnPAOB0as


    g++ main.cpp -std=c++14 -o exe
*/

namespace traits
{
   template <char ch>
   struct is_sign : std::conditional_t< 
         ch == '-' || ch == '+'
         ,std::true_type
         ,std::false_type
      >{
   };

   template <char ch>
   struct is_floating_point_sign : std::conditional_t< 
         ch == '.' || ch == 'e' || ch == 'E' || ch == 'p' || ch == 'P'
         ,std::true_type
         ,std::false_type
      >{
   };

   template <char ch>
   struct is_digit : std::conditional_t< 
         ch == '0' || ch == '1' || ch == '2' || ch == '3' || ch == '4' || ch == '5'
                  || ch == '6' || ch == '7' || ch == '8' || ch == '9'
                  || is_sign<ch>::value
         ,std::true_type
         ,std::false_type
      >{
   };

   template <char ch>
   struct is_xdigit : std::conditional_t< 
         is_digit<ch>::value ||
         ch == 'A' || ch == 'B' || ch == 'C' || ch == 'D' || ch == 'E' || ch == 'F' ||
         ch == 'a' || ch == 'b' || ch == 'c' || ch == 'd' || ch == 'e' || ch == 'f' || ch == 'x' || ch == 'X'
         ,std::true_type
         ,std::false_type
      >{
   };

   template <char ch, char... chars>
   struct is_digits {
      static constexpr bool value = is_digits<ch>::value && is_digits<chars...>::value;
   };

   template <char ch>
   struct is_digits<ch> : is_digit<ch> {
   };

   template <char ch, char... chars>
   struct is_xdigits {
      static constexpr bool value = is_xdigits<ch>::value && is_xdigits<chars...>::value;
   };

   template <char ch>
   struct is_xdigits<ch> : is_xdigit<ch> {
   };

   template <char ch, char... chars>
   struct is_floating_point_digits {
      static constexpr bool value = is_floating_point_digits<ch>::value && is_floating_point_digits<chars...>::value;
   };

   template <char ch>
   struct is_floating_point_digits<ch> {
      static constexpr bool value = is_xdigit<ch>::value || is_floating_point_sign<ch>::value;
   };

} // traits namespace


template <char... chars>
struct char_sequence : std::integer_sequence<char, chars...> { 
    static const char* value() {
        static constexpr char str[sizeof...(chars) + 1] = { chars..., '\0' };
        return str;
    }
};

template <typename T, T... chars>
constexpr char_sequence<chars...> operator""_cs() { return { }; }


template <char... chars>
struct digit_sequence : char_sequence<chars...> { 
    static_assert(traits::is_digits<chars...>::value,"char is not a digit");
    static long value() {
        char* end{};
        return std::strtol(char_sequence<chars...>::value(), &end, 10);
    }
};

template <typename T, T... chars>
constexpr digit_sequence<chars...> operator""_ds() { return { }; }

template <char... chars>
struct xdigit_sequence : char_sequence<chars...> { 
    static_assert(traits::is_xdigits<chars...>::value,"char is not a xdigit");
    static long value() {
        char* end{};
        return std::strtol(char_sequence<chars...>::value(), &end, 16);
    }
};

template <typename T, T... chars>
constexpr xdigit_sequence<chars...> operator""_xs() { return { }; }


template <char... chars>
struct fdigit_sequence : char_sequence<chars...> { 
    static_assert(traits::is_floating_point_digits<chars...>::value,"char is not a floating-point digit sign");
    static double value() {
        char* end{};
        return std::strtod(char_sequence<chars...>::value(), &end);
    }
};

template <typename T, T... chars>
constexpr fdigit_sequence<chars...> operator""_fs() { return { }; }


// Example of usage
#include <string>
#include <iostream>

using type = decltype("Nikola AV"_cs);
#define STYPE(s) decltype("" s ""_cs)
#define DTYPE(s) decltype("" s ""_ds)
#define XTYPE(s) decltype("" s ""_xs)
#define FTYPE(s) decltype("" s ""_fs)

using namespace std;

int main()
{
   cout << STYPE("another string")::value() << endl;
   cout << DTYPE("12345")::value() << endl;
   cout << XTYPE("0ABcdEf")::value() << endl;
   cout << FTYPE("0X1.BC70A3D70A3D7P+6")::value() << endl;
   cin.get();
}
