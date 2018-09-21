#include <algorithm>
#include <iterator>
#include <type_traits>
#include <cassert>

/**
   \brief simple_xor applies the XOR function (exclusive disjunction operation) to a range [src_first, src_last) and stores the result in another range, beginning at d_first.

   \param src_first, src_last - the source range of elements to transform
   \param key_first, key_last - the XOR encryption key
   \param d_first - the beginning of the destination range, may be equal to first1 or first2
   \return Output iterator to the element past the last element transformed.

   \see  https://en.wikipedia.org/wiki/Exclusive_or
         https://en.wikipedia.org/wiki/XOR_cipher
*/


template<typename InputIt, typename ForwardIt, typename OutputIt>
   // requires InputIterator<InputIt>,       https://en.cppreference.com/w/cpp/named_req/InputIterator
   // requires ForwardIterator<ForwardIt>,   https://en.cppreference.com/w/cpp/named_req/ForwardIterator
   // requires OutputIterator<OutputIt>,     https://en.cppreference.com/w/cpp/named_req/OutputIterator
OutputIt simple_xor(InputIt src_first, InputIt src_last, ForwardIt key_first, ForwardIt key_last, OutputIt d_first)
{
   assert(key_first!=key_last);

   class cyclic_iterator
   {
      const ForwardIt f_ ;
      const ForwardIt l_ ;
      ForwardIt c_   {f_};

      cyclic_iterator& operator=(const cyclic_iterator&) = delete;

   public:
      cyclic_iterator(ForwardIt first, ForwardIt last)
         : f_(first), l_(last) {
      }
      auto operator++() {
         return ++c_!=l_? c_: (c_=f_,c_);
      }
      decltype(auto) operator*() const {
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


template<typename InputIt, typename KeyRange, typename OutputIt>
OutputIt simple_xor(InputIt src_first, InputIt src_last, const KeyRange& key, OutputIt d_first)
{
   return simple_xor(
       src_first,src_last
      ,std::begin(key),std::end(key)
      ,d_first
   );
}

template<typename InputRange, typename KeyRange, typename OutputIt>
OutputIt simple_xor(const InputRange& source, const KeyRange& key, OutputIt d_first)
{
   return simple_xor(
       std::begin(source),std::end(source)
      ,std::begin(key),std::end(key)
      ,d_first
   );
}

template<typename InputOutputRange, typename KeyRange>
auto simple_xor(InputOutputRange& source, const KeyRange& key)
{
   return simple_xor(source,key,std::begin(source));
}

///////////////////////////////////////////////////

#include <string>
#include <vector>
#include <list>
#include <bitset>
#include <array>
#include <sstream>
#include <iostream>

using namespace std;

using bits8 = bitset<8>;

bits8 operator"" _bs ( const char* s )
{
    return bits8{s};
}

void test1()
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

void test2()
{
   string         origin  {"Wiki"};
   const string   key     {"key"};

   simple_xor(origin,key);
   assert(origin!="Wiki");

   simple_xor(origin,key,&origin[0]);  // an equivalent to simple_xor(origin,key);
   assert(origin=="Wiki");
}


template <typename input_stream, typename output_stream>
void transfer(input_stream& in, output_stream& out)
{
   using input_iterator    = istreambuf_iterator<typename input_stream::char_type>;
   using output_iterator   = ostreambuf_iterator<typename output_stream::char_type>;

   simple_xor(
       input_iterator(in),input_iterator{}
      ,"Bjarne Stroustrup"
      ,output_iterator(out)
   );
}

void test3()
{
   stringstream ss, o1, o2;
   ss << "C++ is designed to allow you to express ideas, but if you don't have ideas or don't have any clue about how to express them, C++ doesn't offer much help.";

   transfer(ss,o1);
   transfer(o1,o2);
   cout << o2.str() << endl;
}


int main()
{
   test1();
   test2();
   test3();
}
