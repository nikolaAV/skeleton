#include <algorithm>
#include <iterator>
#include <type_traits>
#include <cassert>

namespace concepts
{
   template< typename T, typename = std::void_t<> >
   struct is_iterator : std::false_type { };

   template< typename T >
   struct is_iterator<T, std::void_t<typename T::iterator_category>> : std::true_type { };

   template< typename T, typename = std::void_t<>, typename = std::void_t<> >
   struct is_range : std::false_type { };

   template< typename T >
   struct is_range<T, std::void_t<decltype(std::begin(std::declval<T>()))>, std::void_t<decltype(std::end(std::declval<T>()))>> 
                   : std::true_type { };

   template< typename T, typename = std::void_t<> >
   struct is_stream : std::false_type { };

   template< typename T >
   struct is_stream<T, std::void_t<typename T::iostate>> : std::true_type { };

   template< typename T, typename U >
   struct is_value_type_of : std::conditional_t<
                           std::is_same_v<T,typename U::value_type>,
                           std::true_type,std::false_type> {};

   template< typename T >
   inline constexpr bool is_iterator_v = is_iterator<T>::value || std::is_pointer_v<T>;

   template< typename T >
   inline constexpr bool is_range_v = is_range<T>::value || std::is_array_v<T>;

   template< typename T, typename U >
   inline constexpr bool is_value_type_of_v = is_value_type_of<T,U>::value;

   template< typename T >
   inline constexpr bool is_stream_v = is_stream<T>::value;

} // end of namespace concepts

// https://en.wikipedia.org/wiki/Variadic_macro
#define Requires(...) \
  typename std::enable_if_t<__VA_ARGS__>* = nullptr

/**
   \brief simple_xor applies the XOR function (exclusive disjunction operation) to a range [src_first, src_last) and stores the result in another range, beginning at d_first.

   \param [src_first, src_last) - a pair of iterators specifying the source range of elements to transform
   \param [key_first, key_last) - a pair of iterators specifying the XOR encryption key
   \param d_first - an iterator to the beginning of the destination range, may be equal to src_first
   \return Output iterator to the element past the last element transformed, i.e. destination.end()

   \see  https://en.wikipedia.org/wiki/Exclusive_or
         https://en.wikipedia.org/wiki/XOR_cipher
*/


template<typename InputIt, typename ForwardIt, typename OutputIt
   // requires InputIterator<InputIt>,       https://en.cppreference.com/w/cpp/named_req/InputIterator
   // requires ForwardIterator<ForwardIt>,   https://en.cppreference.com/w/cpp/named_req/ForwardIterator
   // requires OutputIterator<OutputIt>,     https://en.cppreference.com/w/cpp/named_req/OutputIterator
   ,Requires(concepts::is_iterator_v<InputIt>)
   ,Requires(concepts::is_iterator_v<ForwardIt>)
   ,Requires(concepts::is_iterator_v<OutputIt>)
   >
OutputIt simple_xor(InputIt src_first, InputIt src_last, ForwardIt key_first, ForwardIt key_last, OutputIt d_first)
{
   assert(key_first!=key_last); // the key must not be empty!

   class cyclic_iterator
   {
      const ForwardIt f_ ;
      const ForwardIt l_ ;
      ForwardIt c_   {f_};

      cyclic_iterator& operator=(const cyclic_iterator&) = delete;

   public:
      cyclic_iterator(ForwardIt first, ForwardIt last) noexcept
         : f_(first), l_(last) {
      }
      auto operator++() noexcept(noexcept(++c_) && noexcept(c_!=l_)) {
         return ++c_!=l_? c_: (c_=f_,c_);
      }
      decltype(auto) operator*() const noexcept(noexcept(*c_)) {
         return *c_;
      }
   };

   return std::transform(   
                src_first,src_last
               ,cyclic_iterator{key_first,key_last}
               ,d_first
               ,[](auto const& left, auto const& right) -> std::common_type_t<decltype(left),decltype(right)> {
                  return left ^ right;
   });
}

template<typename InputIt, typename ValueType, typename OutputIt
   ,Requires(concepts::is_iterator_v<InputIt>)
   ,Requires(concepts::is_value_type_of_v<ValueType,InputIt>)
   ,Requires(concepts::is_iterator_v<OutputIt>)
   >
OutputIt simple_xor(InputIt src_first, InputIt src_last, const ValueType& key, OutputIt d_first)
{
   const ValueType arr[] = {key};
   return simple_xor(
       src_first,src_last
      ,std::begin(arr),std::end(arr)
      ,d_first
   );
}

template<typename InputRange, typename ValueType, typename OutputIt
   ,Requires(concepts::is_range_v<InputRange>)
   ,Requires(concepts::is_value_type_of_v<ValueType,InputRange>)
   ,Requires(concepts::is_iterator_v<OutputIt>)
   >
OutputIt simple_xor(const InputRange& source, const ValueType& key, OutputIt d_first)
{
   return simple_xor(
       std::begin(source),std::end(source)
      ,key
      ,d_first
   );
}

template<typename InputRange, typename ValueType, typename OutputRange
   ,Requires(concepts::is_range_v<InputRange>)
   ,Requires(concepts::is_value_type_of_v<ValueType,InputRange>)
   ,Requires(concepts::is_range_v<OutputRange>)
   >
auto simple_xor(const InputRange& source, const ValueType& key, OutputRange& destination)
{
   return simple_xor(
       source
      ,key
      ,std::begin(destination)
   );
}

template<typename InputOutputRange, typename ValueType
   ,Requires(concepts::is_range_v<InputOutputRange>)
   ,Requires(concepts::is_value_type_of_v<ValueType,InputOutputRange>)
   >
auto simple_xor(InputOutputRange& source, const ValueType& key)
{
   return simple_xor(
       source
      ,key
      ,source
   );
}

template<typename InputIt, typename KeyRange, typename OutputIt
   ,Requires(concepts::is_iterator_v<InputIt>)
   ,Requires(concepts::is_range_v<KeyRange>)
   ,Requires(concepts::is_iterator_v<OutputIt>)
   >
OutputIt simple_xor(InputIt src_first, InputIt src_last, const KeyRange& key, OutputIt d_first)
{
   return simple_xor(
       src_first,src_last
      ,std::begin(key),std::end(key)
      ,d_first
   );
}

template<typename InputRange, typename KeyRange, typename OutputIt
   ,Requires(concepts::is_range_v<InputRange>)
   ,Requires(concepts::is_range_v<KeyRange>)
   ,Requires(concepts::is_iterator_v<OutputIt>)
   >
OutputIt simple_xor(const InputRange& source, const KeyRange& key, OutputIt d_first)
{
   return simple_xor(
       std::begin(source),std::end(source)
      ,key
      ,d_first
   );
}

template<typename InputRange, typename KeyRange, typename OutputRange
   ,Requires(concepts::is_range_v<InputRange>)
   ,Requires(concepts::is_range_v<KeyRange>)
   ,Requires(concepts::is_range_v<OutputRange>)
   >
auto simple_xor(const InputRange& source, const KeyRange& key, OutputRange& destination)
{
   return simple_xor(
       source
      ,key
      ,std::begin(destination)
   );
}

template<typename InputOutputRange, typename KeyRange
   ,Requires(concepts::is_range_v<InputOutputRange>)
   ,Requires(concepts::is_range_v<KeyRange>)
   >
auto simple_xor(InputOutputRange& source, const KeyRange& key)
{
   return simple_xor(source,key,source);
}

template<typename InputIt, typename KeyRange, typename OutputStm
   ,Requires(concepts::is_iterator_v<InputIt>)
   ,Requires(concepts::is_range_v<KeyRange>)
   ,Requires(concepts::is_stream_v<OutputStm>)
   >
auto simple_xor(InputIt src_first, InputIt src_last, const KeyRange& key, OutputStm& destination)
{
   using output_iterator = std::ostreambuf_iterator<typename OutputStm::char_type>;
   return simple_xor(
       src_first,src_last
      ,key
      ,output_iterator{destination}
   );
}

template<typename InputRange, typename KeyRange, typename OutputStm
   ,Requires(concepts::is_range_v<InputRange>)
   ,Requires(concepts::is_range_v<KeyRange>)
   ,Requires(concepts::is_stream_v<OutputStm>)
   >
auto simple_xor(const InputRange& source, const KeyRange& key, OutputStm& destination)
{
   return simple_xor(
       std::begin(source),std::end(source)
      ,key
      ,destination
   );
}

template<typename InputStm, typename KeyRange, typename OutputStm
   ,Requires(concepts::is_stream_v<InputStm>)
   ,Requires(concepts::is_range_v<KeyRange>)
   ,Requires(concepts::is_stream_v<OutputStm>)
   >
auto simple_xor(InputStm& source, const KeyRange& key, OutputStm& destination)
{
   using input_iterator   = std::istreambuf_iterator<typename InputStm::char_type>;
   return simple_xor(
       input_iterator{source},input_iterator{}
      ,key
      ,destination
   );
}

template<typename InputStm, typename KeyRange, typename OutputIt
   ,Requires(concepts::is_stream_v<InputStm>)
   ,Requires(concepts::is_range_v<KeyRange>)
   ,Requires(concepts::is_iterator_v<OutputIt>)
   >
auto simple_xor(InputStm& source, const KeyRange& key, OutputIt& d_first)
{
   using input_iterator   = std::istreambuf_iterator<typename InputStm::char_type>;
   return simple_xor(
       input_iterator{source},input_iterator{}
      ,key
      ,d_first
   );
}

template<typename InputStm, typename KeyRange, typename OutputRange
   ,Requires(concepts::is_stream_v<InputStm>)
   ,Requires(concepts::is_range_v<KeyRange>)
   ,Requires(concepts::is_range_v<OutputRange>)
   >
auto simple_xor(InputStm& source, const KeyRange& key, OutputRange& destination)
{
   return simple_xor(
       source
      ,key
      ,std::begin(destination)
   );
}

#undef Requires

///////////////////////////////////////////////////

#include <string>
#include <vector>
#include <list>
#include <bitset>
#include <array>
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;

using bits8 = bitset<8>;

inline bits8 operator"" _bs ( const char* s )
{
    return bits8{s};
}


void test1_1()
{
   vector<bits8>  src = {01010111_bs, 01101001_bs, 01101011_bs, 01101001_bs};
   const bits8    key = {11110011_bs};

   simple_xor(src,key);
   assert(src[0]==10100100_bs);
   assert(src[1]==10011010_bs);
   assert(src[2]==10011000_bs);
   assert(src[3]==10011010_bs);

   simple_xor(src,key);
   assert(src[0]==01010111_bs);
   assert(src[1]==01101001_bs);
   assert(src[2]==01101011_bs);
   assert(src[3]==01101001_bs);
}

void test1_2()
{
   vector<bits8>         src = {01010111_bs, 01101001_bs, 01101011_bs, 01101001_bs};
   const array<bits8,1>  key = {11110011_bs};

   simple_xor(src,key);
   assert(src[0]==10100100_bs);
   assert(src[1]==10011010_bs);
   assert(src[2]==10011000_bs);
   assert(src[3]==10011010_bs);

   simple_xor(src,key);
   assert(src[0]==01010111_bs);
   assert(src[1]==01101001_bs);
   assert(src[2]==01101011_bs);
   assert(src[3]==01101001_bs);
}

void test1_3()
{
   const list<bits8> src = {01010111_bs, 01101001_bs, 01101011_bs, 01101001_bs};
   const bits8       key = {11110011_bs};
   vector<bits8>     out;

   simple_xor(src.begin(),src.end(),key,std::back_inserter(out));
   assert(out[0]==10100100_bs);
   assert(out[1]==10011010_bs);
   assert(out[2]==10011000_bs);
   assert(out[3]==10011010_bs);

   simple_xor(out,key);
   assert(out[0]==01010111_bs);
   assert(out[1]==01101001_bs);
   assert(out[2]==01101011_bs);
   assert(out[3]==01101001_bs);
}

void test2_1()
{
   string         origin  {"Wiki"};
   const string   key     {"key"};

   simple_xor(origin,key);
   assert(origin!="Wiki");

   simple_xor(origin,key,origin);        // an equivalent to simple_xor(origin,key);
   assert(origin=="Wiki");
}

void test2_2()
{
   string         origin  {"Wiki"};
   const string   key     {"key"};

   simple_xor(origin,key);
   assert(origin!="Wiki");

   simple_xor(origin,key,&origin[0]);  // an equivalent to simple_xor(origin,key);
   assert(origin=="Wiki");
}

void test2_3()
{
   //                       0        9        19        29        39        49        59       68
   const string   origin  {"A program that has not been tested does not work. --Bjarne Stroustrup"};
   list<unsigned> out;
   simple_xor(origin,origin,back_inserter(out));
   assert(69==out.size());
   assert(all_of(out.begin(),out.end(),[](unsigned v){return 0==v;}));
}

void test3_1()
{
   stringstream ss, o1, o2;
   ss << "C++ is designed to allow you to express ideas, but if you don't have ideas or don't have any clue about how to express them, C++ doesn't offer much help.";
   const string origin = ss.str();

   simple_xor(ss,"Bjarne Stroustrup",o1);
   assert(origin!=o1.str());
   simple_xor(o1,"Bjarne Stroustrup",o2);
   assert(origin==o2.str());
}

void test3_2()
{
   {  ifstream input("main.cpp",ios::binary);  
      ofstream output("main.encoded",ios::binary | ios::trunc);  
      assert(input && output);
      simple_xor(input,"my secret word",output);
   }
   {  ifstream input("main.encoded",ios::binary);  
      ofstream output("main.decoded",ios::binary | ios::trunc);  
      assert(input && output);
      simple_xor(input,"my secret word",output);
   }
   {  using input_iterator = istreambuf_iterator<typename ifstream::char_type>;
      ifstream input1("main.cpp",ios::binary);  
      ifstream input2("main.decoded",ios::binary);  
      assert(input1 && input2);
      assert(equal(input_iterator{input1},input_iterator{},input_iterator{input2}));
   }
}

int main()
{
   test1_1();
   test1_2();
   test1_3();
   test2_1();
   test2_2();
   test2_3();
   test3_1();
   test3_2();
}
