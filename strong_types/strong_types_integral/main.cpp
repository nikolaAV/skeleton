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


namespace strong_type {

    enum class X : std::uint32_t {};
    enum class Y : std::uint32_t {};

    template <typename ST>
    constexpr auto underlying_value(ST st)
    {
        return static_cast<std::underlying_type_t<ST>>(st);
    }

    void draw(X x, Y y)
    {
        std::cout << "draw(" << underlying_value(x) << ", " << underlying_value(y) << ")\n";
    }

}   // namespace strong_type


// !!! g++ main.cpp -std=c++17
int main()
{
    using namespace strong_type;

    draw(X{1u}, Y{2u});

    return 0;
}






