// g++ main.cpp -std=c++14 -o exe -g
// g++ main.cpp -std=c++14 -Wextra -Wall -pedantic-errors -o exe
// g++ main.cpp -std=c++17 -Wextra -Wall -pedantic-errors -o exe
// g++ main.cpp -std=c++23 -Wextra -Wall -pedantic-errors -o exe
// core dump file --> /var/lib/apport/coredump (before, $ ulimit -c unlimited)
// g++ -fsanitize=address -g main.cpp -o exe
//
// valgrind --leak-check=full --show-reachable=yes ./exe
// valgrind --tool=massif ./exe  --time-unit=B
// ms_print massif.out.XXX
// massif-visualizer massif.out.XXX
//
// to change compile release, type 
// sudo update-alternatives --config gcc
//

#include <iostream>
#include <string>
#include <type_traits>
#include <utility>
#include <cstdint>
#include <tuple>

#include "cartesian_product.hpp" // cartesian_product::exec

struct _1
{
    friend std::ostream& operator<<(std::ostream& out, const _1&)
    {
        return out << "1";
    }
};

struct _2
{
    friend std::ostream& operator<<(std::ostream& out, const _2&)
    {
        return out << "2";
    }
};

struct _3
{
    friend std::ostream& operator<<(std::ostream& out, const _3&)
    {
        return out << "3";
    }
};

struct up
{
    friend std::ostream& operator<<(std::ostream& out, const up&)
    {
        return out << "up";
    }
};

struct down
{
    friend std::ostream& operator<<(std::ostream& out, const down&)
    {
        return out << "down";
    }
};

struct blue
{
    friend std::ostream& operator<<(std::ostream& out, const blue&)
    {
        return out << "blue";
    }
};

struct red
{
    friend std::ostream& operator<<(std::ostream& out, const red&)
    {
        return out << "red";
    }
};


// Example: usage of the cartesian product

auto const t1 = std::make_tuple(_1{}, _2{}, _3{});
auto const t2 = std::make_tuple(up{}, down{});
auto const t3 = std::make_tuple(blue{}, red{});

int main()
{
    std::cout << "----- variant 1 -----" << std::endl;
    cartesian_product::exec(
         [](auto&& v1, auto&& v2, auto&& v3) { std::cout << v1 << " " << v2 << " " << v3 << std::endl; }
        ,t1
        ,t2
        ,t3
    );

    std::cout << "\n----- variant 2 -----" << std::endl;
    cartesian_product::exec(
         [](auto&&... args) { ((std::cout << args << "-"), ...) << std::endl; }
        ,std::make_tuple(_1{}, _2{}, _3{})
        ,std::make_tuple(up{}, down{})
        ,std::make_tuple(blue{}, red{})
    );

    std::cout << "\n----- variant 3 -----" << std::endl;
    cartesian_product::exec(
         [](auto&&... args) { ((std::cout << args << "-"), ...) << std::endl; }
        ,std::make_tuple(1, 2, 3)
        ,std::make_tuple("up", "down")
        ,std::make_tuple("blue", "red")
    );

    return 0;
}






