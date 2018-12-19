/**
   The greatest common divisor (gcd in short) of two or more non-zero integers,
   also known as the greatest common factor (gcf),
   highest common factor (hcf),
   greatest common measure (gcm),
   or highest common divisor,
   is the greatest positive integer that divides all of them.
   \see https://en.wikipedia.org/wiki/Greatest_common_divisor

   To compute the gcd for more than two integers, we could use the std::accumulate algorithm from the header <numeric>
*/

#include <numeric>

template< class InputIt>
auto gcd(InputIt first, InputIt last)
{
   return std::accumulate(first,last,*first,[](auto const& a, auto const&b){
      return std::gcd(a,b);
   });
}

template< class InputRange>
auto gcd(const InputRange& range)
{
   return gcd(std::begin(range), std::end(range));
}

/////////////

#include <initializer_list>
#include <list>
#include <vector>
#include <cassert>

int main()
{
   using namespace std;
   
   assert(8==gcd(initializer_list<int>{48,16,24,96}));
   assert(1==gcd(vector<unsigned>{1,2,3}));
   assert(2==gcd(list<long>{8,6,4,2,10,12,100}));
}
