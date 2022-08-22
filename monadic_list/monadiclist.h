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
// @param a list
//
auto const size = [] (auto const& list) {
    return list([](auto const&... ts){
        return sizeof...(ts);
    });
};

//
// @brief empty : List[...] -> bool, empty or not
// @param a list
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
// @brief pop_front : List[a1, a2, a3, ...] -> List[a2, a3, ...], list without the first element
// @param a list
//
auto const pop_front = [] (auto const& list) {
    return list([](auto const& first, auto const&... rest){
        return List(rest...);
    });
};

//
// @brief push_front : List[a1, a2, a3, ...], a0 -> List[a0, a1, a2,...], insertion at the beginning
// @param a list
// @param t value to be inserted in the first position
//
auto const push_front = [] (auto const& list, auto const& t) {
    return list([&t](auto const&... ts){
        return List(t, ts...);
    });
};

//
// @brief push_front : List[a1, a2, a3], a4 -> List[a1, a2, a3, a4], insertion at the end
// @param a list
// @param t value to be inserted in the last position
//
auto const push_back = [] (auto const& list, auto const& t) {
    return list([&t](auto const&... ts){
        return List(ts..., t);
    });
};

//
// @brief concat : List[a], List[b] -> List[a+b], keeps element ordering 
// @param list_left - elements of the list to be inserted first
// @param list_right - elements of the list to be inserted second
//
auto const concat = [](auto const& list_left, auto const& list_right) {
    return list_left([&list_right](auto const&... lts){
        return list_right([&](auto const&... rts){
            return List(lts..., rts...);
        });
    });
};

//
// @brief tuple : List[a1, a2, a3, ...] -> std::tuple<...>{a1, a2, a3, ...}
// @param a list
//
auto const tuple = [] (auto const& list) {
    return list([](auto const&... ts){
        return std::make_tuple(ts...);
    });
};

//
// @brief tie : List[a1, a2, a3, ...] -> std::tuple<&...> {a1, a2, a3, ...}
// @param a list
//
auto const tie = [] (auto const& list) {
    return list([](auto const&... ts){
        return std::tie(ts...);
    });
};

//
// @brief equal : List[a], List[b] -> bool, compares two lists 
// @param two lists to be compared
//
auto const equal = [](auto const& list_left, auto const& list_right) {
    return tie(list_left) == tie(list_right);
};

//
// @brief zip : List[a], List[b] -> List[std::pair{a,b}] 
// @param two lists to be combined into a list of pairs
//
auto const zip = [](auto const& list_left, auto const& list_right) {
    return list_left([&list_right](auto const&... lts){
        return list_right([&](auto const&... rts){
            return List(std::make_pair(lts, rts)...);
        });
    });
};

////////////////////////////////////////////////

//
// fmap : List[a], (a -> b) -> List[b] 
// @param a list
//
auto const fmap = [](auto const& list, auto f){
    return list([f=std::move(f)](auto const&... ts){
        return List(f(ts)...);
    });
}; 

inline auto flatten() {
    return List();
}

//
// @brief flatten : List[a1], List[a2],... -> List[a1..., a2..., ...] 
// @param a list of lists
//
template <typename ListT, typename... ListTs>
auto flatten(ListT const& first, ListTs... others) {
    return concat(first, flatten(others...));
}

//
// @brief flatmap : List[a], (a -> List[b]) -> List[b] 
// @param a list
//
auto const flatmap = [](auto const& list, auto f){
    return list([f=std::move(f)](auto const&... ts){
        return flatten(f(ts)...);
    });
}; 

//
// transform : List[a], (a -> b) -> List[b] 
// @param a list
// @note alias of 'fmap' to make it more understandable for C++ programmers knowing STL  
// 
auto const transform = [](auto const& list, auto f){
    return fmap(list, std::move(f));
}; 


// @todo questionable?
// for_each : List[a], (a -> void) -> void 
auto const for_each = [](auto const& list, auto f){
    list([&f](auto const&... ts){
        // @todo, workaround, not needed in C++17 with fold expression
        std::initializer_list<int> {(f(ts),0)...};        
    });
};

} // namespace monadic_list
