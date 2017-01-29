#include <vector>
#include "tut.h"

/**
   Example, how to test std::vector by means http://mrzechonek.github.io/tut-framework/
*/

namespace
{

struct Data
{
    typedef std::vector<int> vector_t;
    vector_t v;
};

typedef tut::test_group<Data>   test_group_t;
typedef test_group_t::object    test_intance_t;

test_group_t test_group("std::vector basic operations");

} // end of anonymous namespace


namespace tut
{

template<>
template<>
void test_intance_t::test<1>()
{
    set_test_name("vector::check size & value");
    v.push_back(100);
    ensure(v.size() == 1);
    ensure("size=1", v.size() == 1);
    ensure("v[0]=100", v[0] == 100);
}

template<>
template<>
void test_intance_t::test<2>()
{
    set_test_name("vector::check capacity");
    v.resize(22);
    ensure_equals("capacity", v.size(), 22U);
	vector_t tmp;
    v.swap(tmp);
    ensure_equals("capacity", v.size(), 0U);
}

template<>
template<>
void test_intance_t::test<3>()
{
    set_test_name("vector::check range constructor");
    v = vector_t({ 1, 2, 3, 4 });
    ensure_equals("size", v.size(), 4U);
    ensure("v[0]", v[0] == 1);
    ensure("v[1]", v[1] == 2);
    ensure("v[2]", v[2] == 3);
    ensure("v[3]", v[3] == 4);
}

} // namespace tut

