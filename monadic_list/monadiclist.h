#include <utility>
#include <tuple>
#include <initializer_list> // @todo, workaround, not needed in C++17 with fold expression

/**
 *      Description ....
 *
 * @see https://youtu.be/B3COkcqwAec?list=PL3BR09unfgcgJPQZKaacwzGmcXMtEA-19&t=1378
 *      http://cpptruths.blogspot.com/2014/08/fun-with-lambdas-c14-style-part-3.html 
 *      
 */

namespace monadic_list {

// @see https://youtu.be/B3COkcqwAec?list=PL3BR09unfgcgJPQZKaacwzGmcXMtEA-19&t=1378
// @see http://cpptruths.blogspot.com/2014/08/fun-with-lambdas-c14-style-part-3.html    

//
// @brief construct : a1, a2, a3, ... -> List[a1, a2, a3, ...]
// @param any number of values of any type which can be copyed
// @code 
//  auto const list = List(1,"2",3.14);
// @endcode 
// 
auto const List = [](auto... ts) {
    return [=](auto f) -> decltype(f(ts...)) {
        return f(ts...);
    };
};

//
// @brief size : List[a1, a2, a3, ...] -> size_t, number of elements 
//
auto const size = [] (auto const& list) {
    return list([](auto const&... ts){
        return sizeof...(ts);
    });
};

//
// @brief empty : List[a1, a2, a3, ...] -> bool, empty or not
//
auto const empty = [] (auto const& list) {
    return list([](auto const&... ts){
        return sizeof...(ts) == 0;
    });
};

//
// @brief front : List[a1, a2, a3, ...] -> a1, returns the first element
// @param a list
//
auto const front = [] (auto const& list) {
    return list([](auto const& first, auto const&... rest){
        return first;
    });
};

//
// pop_front : List[a1, a2, a3, ...] -> List[a2, a3, ...], list without the first element
// @param a list
//
auto const pop_front = [] (auto const& list) {
    return list([](auto const& first, auto const&... rest){
        return List(rest...);
    });
};

//
// push_front : List[a1, a2, a3, ...], a0 -> List[a0, a1, a2,...], insertion at the beginning
// @param a list
// @param t value to be inserted in the first position
//
auto const push_front = [] (auto const& list, auto const& t) {
    return list([&t](auto const&... ts){
        return List(t, ts...);
    });
};

//
// push_front : List[a1, a2, a3], a4 -> List[a1, a2, a3, a4], insertion at the end
// @param a list
// @param t value to be inserted in the last position
//
auto const push_back = [] (auto const& list, auto const& t) {
    return list([&t](auto const&... ts){
        return List(ts..., t);
    });
};

// tuple : List[a1, a2, a3, ...] -> std::tuple<...>{a1, a2, a3, ...}
auto const tuple = [] (auto const& list) {
    return list([](auto const&... ts){
        return std::make_tuple(ts...);
    });
};

// tie : List[a1, a2, a3, ...] -> std::tuple<&...> {a1, a2, a3, ...}
auto const tie = [] (auto const& list) {
    return list([](auto const&... ts){
        return std::tie(ts...);
    });
};

////////////////////////////////////////////////

// fmap :
// transform : List[a], (a -> b) -> List[b] 
auto const transform = [](auto const& list, auto f){
    return list([f=std::move(f)](auto const&... ts){
        return List(f(ts)...);
    });
}; 

// for_each : List[a], (a -> void) -> void 
auto const for_each = [](auto const& list, auto f){
    list([&f](auto const&... ts){
        // @todo, workaround, not needed in C++17 with fold expression
        std::initializer_list<int> {(f(ts),0)...};        
    });
};

// equal : List[a], List[b] -> bool 
auto const equal = [](auto const& list_left, auto const& list_right) {
    return tie(list_left) == tie(list_right);
};

// concat : List[a], List[b] -> List[a+b] 
auto const concat = [](auto const& list_left, auto const& list_right) {
    return list_left([&list_right](auto const&... lts){
        return list_right([&](auto const&... rts){
            return List(lts..., rts...);
        });
    });
};

// zip : List[a], List[b] -> List[std::pair{a,b}] 
auto const zip = [](auto const& list_left, auto const& list_right) {
    return list_left([&list_right](auto const&... lts){
        return list_right([&](auto const&... rts){
            return List(std::make_pair(lts, rts)...);
        });
    });
};

} // namespace monadic_list
