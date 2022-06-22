/**
   std::accumulate with moving support of accumulated value(until C++20).   
   \see https://en.cppreference.com/w/cpp/algorithm/accumulate
*/

#include <iterator>
#include <utility>

template<typename InputIt, typename T, typename BinaryOperation>
T accumulate(InputIt first, InputIt last, T init, BinaryOperation op)
{
    for (; first != last; ++first) {
        init = op(std::move(init), *first);
    }
    return init;
}

template<typename Range, typename T, typename BinaryOperation>
T accumulate(Range const& range, T init, BinaryOperation op)
{
   return accumulate(std::begin(range), std::end(range), std::move(init), std::move(op) );
}

/////////////  tests //// 

#include <cassert>
#include <vector>

auto const even_number = [](auto const& range) {
   return accumulate(range, std::size_t{}, [](auto const& accum, auto const& value){
      return value%2 ? accum : accum+1; 
   });         
};

// generic copy_if implemented by means accumulate
template<typename Range, typename OutputIt, typename UnaryPredicate >
OutputIt copy_if(Range const& range, OutputIt d_first, UnaryPredicate pred) 
{
   return accumulate(range, d_first, [&pred](auto iter, auto const& value){
      return pred(value)? (*iter = value, ++iter) : iter; 
   });
}

int main()
{
   auto range = {0,1,2,3,4,5,6,7,8,9,10};
   assert (6 == even_number(range) );

   std::vector<int> selected{};
   copy_if(range, std::back_inserter(selected), [](int value) {
      return !(value%2); 
   });
   assert ((std::vector<int>{0,2,4,6,8,10}) == selected );
}
