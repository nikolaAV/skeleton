#include <array>
#include <algorithm>
#include "tut.h"

/**
Example, how to test std::transform by means http://mrzechonek.github.io/tut-framework/
*/


namespace
{

struct Data
{
};

typedef tut::test_group<Data>   test_group_t;
typedef test_group_t::object    test_intance_t;

test_group_t test_group("algorithm basic");

} // end of anonymous namespace


namespace tut
{

template<>
template<>
void test_intance_t::test<1>()
{
    using array_t = std::array<int,3>;

    array_t a1 = {1,2,3};
    array_t a2;
    std::transform(a1.begin(),a1.end(),a2.begin(),
                    [](int i){ return i*i; }
    );
    ensure(a2 == array_t{1,4,9} );
}


} // namespace tut

