// g++ main.cpp -std=c++14 -o exe -g
// g++ main.cpp -std=c++14 -Wextra -Wall -pedantic-errors -o exe
// g++ main.cpp -std=c++17 -Wextra -Wall -pedantic-errors -o exe
// g++ main.cpp -std=c++23 -Wextra -Wall -pedantic-errors -o exe
// core dump file --> /var/lib/apport/coredump (before, $ ulimit -c unlimited)
// g++ -fsanitize=address -g main.cpp -o exe
//

#include <iostream>
#include <string>
#include <type_traits>
#include <utility>
#include <cstdint>
#include <tuple>
#include <vector>
#include <algorithm>
#include <cassert>


/**
 * @brief Moves a half-open range to a new position in the same sequence.
 *
 * The elements in [cut_begin, cut_end) are moved so that they begin at
 * @p paste_begin. The operation is performed in place; the relative order
 * of the moved elements and of the remaining elements is preserved.
 *
 * If @p paste_begin is equal to @p cut_begin or @p cut_end, or if the cut
 * range is empty, the sequence is left unchanged.
 *
 * @tparam ForwardIt Iterator type. The iterators must refer to the same
 *         sequence and support relational comparison.
 * @tparam ForwardIts Return type, defaulting to a pair of @p ForwardIt
 *         iterators.
 * @param cut_begin Beginning of the range to move, inclusive.
 * @param cut_end End of the range to move, exclusive.
 * @param paste_begin Position before which the range is inserted.
 * @return A pair {new_begin, new_end} delimiting the moved range after the
 *         operation. For a boundary no-op, returns the original cut range.
 *
 * @note The operation uses std::rotate and requires the value type to be
 *       movable/swappable as required by that algorithm.
 */
template <typename ForwardIt, typename ForwardIts = std::pair<ForwardIt, ForwardIt>>
ForwardIts cut_and_paste(ForwardIt cut_begin, ForwardIt cut_end, ForwardIt paste_begin)
{
    return
        paste_begin < cut_begin ?
            ForwardIts {paste_begin, std::rotate(paste_begin, cut_begin, cut_end)} :
        cut_end < paste_begin ?
            ForwardIts {std::rotate(cut_begin, cut_end, paste_begin), paste_begin} : 
            ForwardIts {cut_begin, cut_end}

    ;
}

void test01_paste_left()
{
    // _____________________________________________________
    // | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11   |
    // _____________________________________________________
    // | A | B | C | D | E | F | G | H | I | J | K  | end()|
    // _____________________________________________________
    //   ↑               ↑                       ↑
    // paste_begin     cut_begin               cut_end

    char arr[] =            {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K'};
    char const expected[] = {'E', 'F', 'G', 'H', 'I', 'J', 'A', 'B', 'C', 'D', 'K'};
    //                        ↑                             ↑
    //                      new_begin                    new_end

    auto cut_begin = arr + 4;       // E
    auto cut_end = arr + 10;        // K
    auto paste_begin = arr + 0 ;    // A

    auto [new_begin, new_end] = cut_and_paste(cut_begin, cut_end, paste_begin);

    assert(std::equal(std::begin(arr), std::end(arr), std::begin(expected)));
    assert(new_begin == paste_begin && new_end == arr + 6); 
}

void test02_paste_right()
{
    // _____________________________________________________
    // | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 |
    // _____________________________________________________
    // | A | B | C | D | E | F | G | H | I | J | K  | end()|
    // _____________________________________________________
    //       ↑               ↑                   ↑
    // cut_begin          cut_end           paste_begin

    char arr[] =            {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K'};
    char const expected[] = {'A', 'F', 'G', 'H', 'I', 'J', 'B', 'C', 'D', 'E', 'K'};
    //                                                      ↑                   ↑
    //                                                   new_begin          new_end

    auto cut_begin = arr + 1;       // B
    auto cut_end = arr + 5;         // F
    auto paste_begin = arr + 10;    // K

    auto [new_begin, new_end] = cut_and_paste(cut_begin, cut_end, paste_begin);

    assert(std::equal(std::begin(arr), std::end(arr), std::begin(expected)));
    assert(new_begin == arr + 6 && new_end == paste_begin); 
}

void test03_paste_at_cut_begin_noop()
{
    char arr[] =            {'A', 'B', 'C', 'D', 'E', 'F'};
    //                                  ↑         ↑
    //                              cut_begin  cut_end
    //                            paste_begin
    char const expected[] = {'A', 'B', 'C', 'D', 'E', 'F'};
    //                                  ↑         ↑
    //                              new_begin  new_end

    auto cut_begin = arr + 2;   // C
    auto cut_end = arr + 4;     // E
    auto paste_begin = arr + 2; // C

    auto [new_begin, new_end] = cut_and_paste(cut_begin, cut_end, paste_begin);

    assert(std::equal(std::begin(arr), std::end(arr), std::begin(expected)));
    assert(new_begin == cut_begin && new_end == cut_end); 
}

void test04_paste_at_cut_end_noop()
{
    char arr[] =            {'A', 'B', 'C', 'D', 'E', 'F'};
    //                                  ↑         ↑
    //                               cut_begin  cut_end
    //                             paste_begin
    char const expected[] = {'A', 'B', 'C', 'D', 'E', 'F'};
    //                                  ↑         ↑
    //                               new_begin  new_end

    auto cut_begin = arr + 2; // C
    auto cut_end = arr + 4;   // E
    auto paste_begin = arr + 4; // E

    auto [new_begin, new_end] = cut_and_paste(cut_begin, cut_end, paste_begin);

    assert(std::equal(std::begin(arr), std::end(arr), std::begin(expected)));
    assert(new_begin == cut_begin && new_end == cut_end); 
}

void test05_empty_range_noop()
{
    char arr[] =            {'A', 'B', 'C', 'D', 'E', 'F'};
    //                             ↑         ↑
    //                        paste_begin  cut_begin
    //                                     cut_end  
    char const expected[] = {'A', 'B', 'C', 'D', 'E', 'F'};
    //                             ↑
    //                          cut_begin
    //                          cut_end  

    auto cut_begin = arr + 3;   // D
    auto cut_end = arr + 3;     // D
    auto paste_begin = arr + 1; // B

    auto [new_begin, new_end] = cut_and_paste(cut_begin, cut_end, paste_begin);

    assert(std::equal(std::begin(arr), std::end(arr), std::begin(expected)));
    assert(new_begin == paste_begin && new_end == paste_begin); 
}

void test06_paste_before_range()
{
    char arr[] =            {'A', 'B', 'C', 'D', 'E', 'F'};
    //                        ↑         ↑              ↑
    //                   paste_begin  cut_begin     cut_end
    char const expected[] = {'C', 'D', 'E', 'A', 'B', 'F'};
    //                        ↑         ↑
    //                   new_begin   new_end

    auto const cut_begin = arr + 2;     // C
    auto const cut_end = arr + 5;       // F
    auto const paste_begin = arr + 0;   // A

    auto [new_begin, new_end] = cut_and_paste(cut_begin, cut_end, paste_begin);

    assert(std::equal(std::begin(arr), std::end(arr), std::begin(expected)));
    assert(new_begin == paste_begin && new_end == arr + std::distance(cut_begin, cut_end)); 
}

void test07_paste_after_range()
{
    char arr[] =            {'A', 'B', 'C', 'D', 'E', 'F'};
    //                             ↑         ↑              ↑
    //                        cut_begin    cut_end      paste_begin
    char const expected[] = {'A', 'D', 'E', 'F', 'B', 'C'};
    //                                            ↑         ↑
    //                                       cut_begin    cut_end

    auto const cut_begin = arr + 1;           // B
    auto const cut_end = arr + 3;             // D
    auto const paste_begin = std::end(arr);   // end()

    auto [new_begin, new_end] = cut_and_paste(cut_begin, cut_end, paste_begin);

    assert(std::equal(std::begin(arr), std::end(arr), std::begin(expected)));
    assert(new_begin == arr + 4);
    assert(new_end == new_begin + std::distance(cut_begin, cut_end));
    assert(new_end == std::end(arr)); 
}

int main()
{
    test01_paste_left();
    test02_paste_right();
    test03_paste_at_cut_begin_noop();
    test04_paste_at_cut_end_noop();
    test05_empty_range_noop();
    test06_paste_before_range();
    test07_paste_after_range();
}



