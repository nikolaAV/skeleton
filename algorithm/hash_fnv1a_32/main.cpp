#include <cstdint>
#include <cstddef>
#include <utility>
#include <iterator>
#include <type_traits>

/**
   \brief Fowler–Noll–Vo hash function
   \see  https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
         https://create.stephan-brumme.com/fnv-hash/
         http://www.isthe.com/chongo/tech/comp/fnv/
         https://github.com/nikolaAV/skeleton/blob/master/switch_string/main.cpp
*/

namespace fnv1a32   // FNV-1a hash algorithm for 32 bits
{
   namespace details
   {
      /**
         constexpr analog of std::accumulate(...)
         \see https://en.cppreference.com/w/cpp/algorithm/accumulate
      */
      template<typename InputIt, typename T, typename BinaryOperation>
      constexpr T accumulate(InputIt first, InputIt last, T init, BinaryOperation op) 
      noexcept(noexcept(op(std::declval<T>(), std::declval<typename std::iterator_traits<InputIt>::value_type>()))) {
         while (first != last)
            init = op(init, *first++);
         return init;
      }

      template <std::size_t N> struct bytes_t {
         uint8_t _[N];
      };

      constexpr uint32_t _prime_ = 0x01000193; //   16777619
      constexpr uint32_t _seed_  = 0x811C9DC5; // 2166136261

      inline constexpr uint32_t hash(uint8_t _1byte, uint32_t prev) noexcept {
         return (_1byte^prev)*_prime_;
      }

      inline constexpr uint32_t hash(char _1byte, uint32_t prev) noexcept {
         static_assert(sizeof(uint8_t)==sizeof(char));
         return hash(static_cast<uint8_t>(_1byte), prev);
      }

      template <std::size_t N>
      constexpr uint32_t hash(bytes_t<N> bytes, uint32_t prev) noexcept {
         return accumulate(std::begin(bytes._), std::end(bytes._),prev,[](uint32_t p, uint8_t v){
            return hash(v,p); 
         });
      }

      template <typename T>
      uint32_t hash(T value, uint32_t prev) noexcept {  // <-- constexpr won't work. why? read it a couple of lines down
         constexpr auto byte_count{sizeof(T)};
         const union { 
            T value; 
            bytes_t<byte_count> bytes; 
            static_assert(byte_count == sizeof(bytes));
         } cast {value};
         return hash(cast.bytes, prev);   // <-- expression did not evaluate to a constant
                                          // failure was caused by accessing a non-active member of a union
                                          // https://stackoverflow.com/questions/11373203/accessing-inactive-union-member-and-undefined-behavior
      }

   } // end of namespace details

   namespace concepts
   {
      // https://github.com/nikolaAV/Modern-Cpp/tree/master/concept%20(pceudo)/has_type_member
      // https://github.com/nikolaAV/Modern-Cpp/tree/master/concept%20(pceudo)/detection_idiom

      template <typename T, typename = void>
      struct is_range_t : std::false_type {};

      template <typename T>
      struct is_range_t<T,std::void_t<decltype(std::begin(std::declval<T>())),decltype(std::end(std::declval<T>()))>>
                        : std::true_type {};

      template <typename T>
      inline constexpr bool is_range_v = is_range_t<T>::value;

   } // end of namespace concepts

   template<typename InputIt>
   constexpr uint32_t 
   hash(InputIt first, InputIt last) noexcept {
      using namespace details;
      return accumulate(first, last, _seed_, [](const uint32_t& prev, const auto& v) {
         return hash(v, prev);
      });
   }

   template<typename Range>
   constexpr std::enable_if_t<concepts::is_range_v<Range>,uint32_t>
   hash(const Range& r) noexcept {
      return hash(std::begin(r),std::end(r));
   }

   template<typename T>
   constexpr std::enable_if_t<!concepts::is_range_v<T>, uint32_t>
   hash(const T& t) noexcept {
      using namespace details;
      return hash(t, _seed_);
   }

} // namespace fnv1a32


// Example of usage

#include <cassert>
#include <iostream>
#include <string_view>
#include <vector>
#include <array>
#include <sstream>
#include <initializer_list>

inline constexpr uint32_t hash(std::string_view s) noexcept {
   return fnv1a32::hash(s);
}


int main()
{
   {
      constexpr char    exp[] = { "C++ language" };
      const std::string str {exp};

      static_assert(hash("C++ language") == hash(exp));
      assert(hash(str) == hash(exp));
   }
   {
      constexpr std::array arr1 = { 'F','N','V','1','a' };
      static_assert(hash("FNV1a") == fnv1a32::hash(arr1));

      const std::array arr2 {arr1};
      assert(fnv1a32::hash(arr1) == fnv1a32::hash(arr2));
   }
   {
      const int    exp[] = { 0,1,2,3,4,5,6,7,8,9 };
      assert( 0 != fnv1a32::hash(exp));
   }
   {
      const std::vector v = { .1,.2,.3,.4,.5,.6,.7,.8,.9 };
      assert(0 != fnv1a32::hash(v));
   }
   {
      std::istringstream s("Hello, World!");
      using iterator = std::istream_iterator<char>;
      std::cout << fnv1a32::hash(iterator(s),iterator())  << std::endl;
   }
   {
      struct aggregate {
         size_t m1;
         double m2;
      };

      const auto agg  = aggregate{0,.9};
      std::cout << fnv1a32::hash(agg) << std::endl;

      const auto list = { agg,agg,agg };
      std::cout << fnv1a32::hash(list) << std::endl;
   }

}
