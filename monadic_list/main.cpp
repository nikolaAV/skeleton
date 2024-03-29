/*
    g++ main.cpp -std=c++14 -o exe -g
   // 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
*/


#include <cassert>
#include <iostream>
#include <vector>

#include "monadiclist.h"

template <typename T, typename U>
std::ostream& operator<<(std::ostream& out, std::pair<T,U> const& value) {
    return out << "{" << value.first << "," << value.second << "}";
}

auto const print = [](auto const& list){
    monadic_list::for_each(list, [](auto const& t){
        std::cout << t << ",";
    });
    std::cout << std::endl;
};


void test00()
{
    using namespace monadic_list;

    auto const list = List(1,2,3,4,5);
    print(list);
    assert(1 == front(list));
    assert(5 == size(list));
    assert(false == empty(list));

    auto const list2 = pop_front(list);
    assert(2 == front(list2));
    assert(4 == size(list2));
    assert(false == empty(list2));

    auto const list3 = pop_front(list2);
    assert(3 == front(list3));
    assert(3 == size(list3));
    assert(false == empty(list3));

    auto const list4 = pop_front(list3);
    assert(4 == front(list4));
    assert(2 == size(list4));
    assert(false == empty(list4));

    auto const list5 = pop_front(list4);
    assert(5 == front(list5));
    assert(1 == size(list5));
    assert(false == empty(list5));

    auto const list6 = pop_front(list5);
    assert(0 == size(list6));
    assert(empty(list6));
}

void test01()
{
    using namespace monadic_list;

    auto const list = List(1,"string-23",3.14);
    auto const expected = std::make_tuple(1,"string-23",3.14);
    assert(expected == tuple(list));
    assert(expected == tie(list));
}

void test02()
{
   using namespace monadic_list;

   auto const list1 = push_front(List(), 'A');
   auto const list2 = push_front(list1, "C++");
   auto const list3 = push_front(list2, 123);
   auto const list4 = push_front(list3, -.456);
   assert(equal(list4, List(-.456, 123, "C++", 'A')));

   auto const list5 = push_back(list4, '\0');
   assert(equal(list5, List(-.456, 123, "C++", 'A', '\0')));
}

void test03()
{
   using namespace monadic_list;

   auto const list = flatten(List(1,2,3), List(4,5), List(6,7,8,9)); 
   assert(equal(list, List(1,2,3,4,5,6,7,8,9)));
}

void test04()
{
   using namespace monadic_list;

   auto const input = List(1,2,3,4,5,6,7,8,9); 
   auto const propagate = [](auto&& element){
        return List(element*-1, element*2);
   }; 

   auto const output = flatmap(input, propagate);
   assert(equal(output, List(-1,2,-2,4,-3,6,-4,8,-5,10,-6,12,-7,14,-8,16,-9,18))); 
}

void test11()
{
    using namespace monadic_list;

    auto const list = List(1,2,3,4,5);
    print(list);

    auto const list2 = transform(list, [](auto&& t){ return t*2; });
    print(list2);

    std::cout << "equal: " << equal(list2, List(2,4,6,8,10)) << std::endl;

    auto const list3 = concat(list2, list);
    print(list3);

    auto const list4 = zip(list2, list);
    print(list4);

}

void test12()
{
    // lazy/deferred invocation

    using namespace monadic_list;

    auto const arg1 = List(1,2,3);
    auto const arg2 = List("one","two","three");
    auto const args = List(arg1, arg2); 
    auto const lazy_zip = [&args] {
        return args([](auto const& arg1, auto const& arg2){
            return zip(arg1, arg2);
        });
    };
    // ...

    auto const result = lazy_zip();
    print(result);
}

void test13()
{
    // TBD
}

int main()
{
    test00();
    test01();
    test02();
    test03();
    test04();

    test11();
    test12();
    test13();

}
