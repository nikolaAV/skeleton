#include <algorithm>
#include <vector>
#include <iterator>
#include <utility>

namespace permutation
{
   /**
      \brief Applies the given function object 'f' to each permutation transformed on range [first,last)   

      \param [in] first, last - the range to apply the function to. 
                  InputIt must meet the requirements of InputIterator. https://en.cppreference.com/w/cpp/named_req/InputIterator
      \param [in] f - function object, to be applied to a permutation based on the range [first,last).
                  The signature of the function should be equivalent to the following:
                  void fun(InputIt, InputIt);
      \retval function object 'f'.
   */
   template< class InputIt, class RangeFunction >
   RangeFunction for_each( InputIt first, InputIt last, RangeFunction f) {
      using value_type = typename std::iterator_traits<InputIt>::value_type;
      std::vector<value_type> copy(first,last);
      std::sort(copy.begin(), copy.end());
      do {
         f(copy.begin(), copy.end());
      } while(std::next_permutation(copy.begin(), copy.end()));
      return std::move(f);
   }
   
}  // end of namespace permutation


///////////////////////////////////////////////////


#include <iostream>
#include <iomanip>
#include <ios>

int main()
{
   using namespace std;

   const auto seq = {'a','b','c'};

   size_t line_no {0};
   permutation::for_each(begin(seq), end(seq), [&line_no](auto first, auto last) {
      cout << setw(2) << setfill('0') << right << line_no++ << " -> ";
      copy(first,last,ostream_iterator<char>{cout,", "});
      cout << endl;
   });
}
