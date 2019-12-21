#include <type_traits>

/**
 *    A typelist is a type that represents a list of types and can be manipulated by a
 *    template metaprogram. It provides the operations typically associated with a list:
 *    iterating over the elements (types) in the list, adding elements, or removing elements.
 *    However, typelists differ from most run-time data structures, such as std::list,
 *    in that they don’t allow mutation. Adding an element to a typelist, on the
 *    other hand, does not change the original typelist: Rather, adding an element to an
 *    existing typelist creates a new typelist without modifying the original.
 *
 * @see https://www.codeproject.com/Articles/1077852/TypeLists-and-a-TypeList-Toolbox-via-Variadic-Temp
 *      https://blog.galowicz.de/2016/05/08/compile_time_type_lists/
 *      http://www.tmplbook.com/, Chapter 24
 *      https://www.drdobbs.com/generic-programmingtypelists-and-applica/184403813
 */

namespace tl // <-- typelist
{

///
/// A direct implementation of a typelist encodes the elements in a parameter pack
/// The elements of a list<...> are written directly as its template arguments.
/// @code
///     using empty_list = list<>;
///     using integral_types = list<boo, char, short, int, long, long long>;
/// @endcode
///

    template<typename... Ts> struct list {};

///
/// @brief checks whether T is an list<...> type.
/// @param  T requested type to be tested
/// @return the member boolean constant 'value' equal to `true` if the T is list<...>. Otherwise value is `false`.
/// @code
///     static_assert(is_list<list<std::string>>::value==true);
///     static_assert(is_list<std::string>::value==false);
/// @endcode
///
    template <typename T>
    struct is_list : std::false_type {
    }; 

    template <typename... Ts>
    struct is_list<list<Ts...>> : std::true_type {
    }; 

    template <typename T>
    inline constexpr bool is_list_v = is_list<T>::value;

///
/// @brief `is_empty` determines whether the given typelist has no elements
/// @param TList a typelis, tl::list<type1,type2,...>
/// @return the member boolean constant 'value' equal to `true` if the TList is empty. Otherwise value is `false`.
/// @code
///     static_assert(is_empty<list<int,double,void*>>::value==false);
///     static_assert(is_empty<list<>>::value==true);
/// @endcode
///

    template<typename TList>
    struct is_empty : std::false_type {
    };

    template<>
    struct is_empty<list<>> : std::true_type {
    };

    template <typename TList>
    inline constexpr bool is_empty_v = is_empty<TList>::value;

///
/// @brief `size` returns number of elements in the list
/// @param TList a typelis, tl::list<type1,type2,...>
/// @return the member constant 'value' equal to number of elements in TList.
/// @code
///     static_assert(size<list<int,double,void*>>::value==3);
/// @endcode
///

    template<typename TList> struct size;

    template<typename... Ts>
    struct size<list<Ts...>> {
        static constexpr unsigned value = sizeof...(Ts);
    };

    template <typename TList>
    inline constexpr unsigned size_v = size<TList>::value;

///
/// @brief `any_of` checks if specified type T exists in a list of types
/// @param  T requested task to be tested
/// @param  TList variadic template list<T1,T2,...> representing a list of types
/// @return the member constant 'value' equal to `true` if at least one element of type T is in the TList. Otherwise value is `false`.
/// @code
///     any_of<list<int,char,double>,char>::value == true
/// @endcode
///

    template<typename TList, typename T> 
    struct any_of : std::false_type {
    };

    template<typename Head, typename... Tail> 
    struct any_of<list<Head,Tail...>,Head> : std::true_type {
    };

    template<typename Head, typename... Tail, typename T> 
    struct any_of<list<Head,Tail...>,T> :  any_of<list<Tail...>,T> {
    };

    template<typename TList, typename T>
    inline constexpr bool any_of_v = any_of<TList,T>::value;

///
/// @brief none_of checks if specified type T does not exist in a list of types
/// @param  T requested task to be tested
/// @param  TList variadic template list<T1,T2,...> representing a list of types
/// @return the member constant 'value' equal to `true` if there is no element of type T in the TList. Otherwise value is `false`.
/// @code
///     none_of<list<int,char,double>,float>::value == true
/// @endcode
///

    template<typename TList, typename T> 
    struct none_of : std::conditional_t<!any_of<TList,T>::value,std::true_type,std::false_type> {
    };

    template<typename TList, typename T>
    inline constexpr bool none_of_v = none_of<TList,T>::value;

///
/// @brief all_of checks if all types in the list of types are the same requested type 
/// @param  TList variadic template list<T1,T2,...> representing a list of types
/// @return the member constant 'value' equal to `true` if all types are of the requested type. Otherwise value is `false`.
/// @code
///     all_of<list<int,int,int>,int>::value == true
/// @endcode
///

    template<typename TList, typename T> 
    struct all_of : std::true_type {
    };

    template<typename Head, typename... Tail, typename T> 
    struct all_of<list<Head,Tail...>,T> :  std::conditional_t<
            std::is_same<Head,T>::value
            ,all_of<list<Tail...>,T>
            ,std::false_type
        > {
    };

    template<typename TList, typename T>
    inline constexpr bool all_of_v = all_of<TList,T>::value;

///
/// @brief the `front` metafunction extracts the first element from the typelist
/// @param  TList a typelis, tl::list<type1,type2,...>
/// @return the member typename 'type' representing the first element in TList
/// @code
///     using my_list = list<double,float,long>;
///     using first1 = front_t<my_list>;        // double, syntax 1
///     using first2 = front<my_list>::type;    // double, syntax 2
///     static_assert(std::is_same_v<first1,first2>);
///     static_assert(std::is_same_v<first1,double>);
/// @endcode
///

    template<typename TList> struct front;

    template<typename Head, typename... Tail>
    struct front<list<Head, Tail...>> {
        using type = Head;
    };

    template<typename TList>
    using front_t = typename front<TList>::type;

///
/// @brief the `back` metafunction extracts the last element from the typelist:
/// @param  TList a typelis, tl::list<type1,type2,...>
/// @return the member typename 'type' representing the last element in TList
/// @code
///     using my_list = list<double,float,long>;
///     using last = back_t<my_list>;    // long
/// @endcode
///

    template<typename TList> struct back;

    template<typename Head>
    struct back<list<Head>> : front<list<Head>> {
    };

    template<typename Head, typename... Tail>
    struct back<list<Head, Tail...>> : back<list<Tail...>> {
    };

    template<typename TList>
    using back_t = typename back<TList>::type;


///
/// @brief the `pop_front` removes the first element from the typelis
/// @param  TList a typelis, tl::list<type1,type2,...>
/// @return the member typename 'type' representing the original TList without the first element
/// @code
///     using original_list = list<A,B,C>;
///     using other_list = pop_front_t<original_list>;    // list<B,C>
/// @endcode
///

    template<typename TList> struct pop_front;

    template<typename Head, typename... Tail>
    struct pop_front<list<Head, Tail...>> {
        using type = list<Tail...>;
    };

    template<typename TList>
    using pop_front_t = typename pop_front<TList>::type;

///
/// @brief the `push_front` inserts an element onto the front of the typelist
/// @param TList a typelis, tl::list<type1,type2,...>
/// @param T type to be inserted. T can be TList
/// @return the member typename 'type' representing the typelist as a concatenation T + original TList 
/// @code
///     using original_list = list<A,B,C>;
///     using other_list1 = push_front_t<original_list,X>;    // list<X,A,B,C>
///     using other_list2 = push_front_t<original_list,original_list>;    // list<A,B,C,A,B,C>
/// @endcode
///

    template<typename TList, typename T> struct push_front;

    template<typename... Ts, typename T>
    struct push_front<list<Ts...>,T> {
        using type = list<T,Ts...>;
    };

    template<typename... Ts, typename... Us>
    struct push_front<list<Ts...>,list<Us...>> {
        using type = list<Us...,Ts...>;
    };

    template<typename TList, typename T>
    using push_front_t = typename push_front<TList,T>::type;


///
/// @brief the `push_back` inserts an element onto the end of the typelist
/// @param TList a typelis, tl::list<type1,type2,...>
/// @param T type to be inserted. T can be typelist, i.e. other TList
/// @return the member typename 'type' representing the typelist as a concatenation the original TList + T
/// @code
///     using my_list1 = push_back_t<list<signed>,unsigned>;   // list<signed,unsigned>
///     using my_list2 = push_back_t<list<A,B>,list<C,D>>;    // list<A,B,C,D>
/// @endcode
///

    template<typename TList, typename T> struct push_back;

    template<typename... Ts, typename T>
    struct push_back<list<Ts...>,T> {
        using type = list<Ts...,T>;
    };

    template<typename... Ts, typename... Us>
    struct push_back<list<Ts...>,list<Us...>> {
        using type = list<Ts...,Us...>;
    };

    template<typename TList, typename T>
    using push_back_t = typename push_back<TList,T>::type;


///
/// @brief the `push_front_unique` inserts an element T onto the front of the typelist if the element of type T does not exist in the given typelist.
/// @param TList a typelis, tl::list<type1,type2,...>
/// @param T type to be inserted
/// @return the member typename 'type' representing the typelist as a concatenation T + original TList. The resulting list has no duplication. 
/// @code
///     using original_list = list<A,B,C>;
///     using other_list = push_front_unique_t<original_list,A>;    // list<A,B,C>
/// @endcode
///
    template<typename TList, typename T> 
    struct push_front_unique {
        using type = std::conditional_t<any_of<TList,T>::value,TList,push_front_t<TList,T>>;
    };

    template<typename TList, typename T>
    using push_front_unique_t = typename push_front_unique<TList,T>::type;

///
/// @brief the `push_back_unique` inserts an element T onto the end of the typelist if the element of type T does not exist in the given typelist.
/// @param TList a typelis, tl::list<type1,type2,...>
/// @param T type to be inserted
/// @return the member typename 'type' representing the typelist as a concatenation the original TList + T. The resulting list has no duplication. 
/// @code
///     using original_list = list<A,B,C>;
///     using other_list = push_back_unique_t<original_list,A>;    // list<A,B,C>
/// @endcode
///

    template<typename TList, typename T> 
    struct push_back_unique {
        using type = std::conditional_t<any_of<TList,T>::value,TList,push_back_t<TList,T>>;
    };

    template<typename TList, typename T>
    using push_back_unique_t = typename push_back_unique<TList,T>::type;

///
/// @brief `replace_front` replaces the first element in a typelist by applying new one
/// @param TList a typelis, tl::list<type1,type2,...>
/// @param T type to be applied
/// @return the member typename 'type' representing the the original TList with replaced the first element of requested T
/// @code
///     using list1 = list<int,char,long,unsigned>;
///     using list2 = replace_front_t<list1,void>;  // list<void,char,long,unsigned>
/// @endcode
///

    template<typename TList, typename T>
    struct replace_front : push_front<pop_front_t<TList>, T> {
    };

    template<typename TList, typename T>
    using replace_front_t = typename replace_front<TList,T>::type;

///
/// @brief `nth_element` extracts a specific element from the list (zero-based indexing)
/// @param TList a typelis, tl::list<type1,type2,...>
/// @param N the integral constant representing an element position in the TList
/// @return the member typename 'type' representing element type in TList by requested index N
/// @code
///     using my_list = list<int,char,long,unsigned>;
///     using first = nth_element_t<my_list,0>; // int
///     using last = nth_element_t<my_list,3>; // unsigned
/// @endcode
///

    template<typename TList, unsigned N>
    struct nth_element : nth_element<pop_front_t<TList>,N-1> {
    };

    template<typename TList>
    struct nth_element<TList,0> : front<TList> {
    };

    template<typename TList, unsigned N>
    using nth_element_t = typename nth_element<TList,N>::type;

///
/// @brief is_unique checks if there is not a duplication in the list of types
/// @param  TList variadic template list<T1,T2,...> representing a list of types
/// @return the member constant 'value' equal to `true` if no duplication is detected. Otherwise value is `false`.
/// @code
///     is_unique<list<int,char,short,void>>::value == true
///     is_unique<list<int,char,int>>::value == false
/// @endcode
///
    template <typename TList> 
    struct is_unique : std::true_type {
    };

    template <typename Head, typename... Tail>
    struct is_unique <list<Head,Tail...>> : std::conditional_t<
             any_of<list<Tail...>,Head>::value    
            ,std::false_type
            ,is_unique<list<Tail...>>
        > {
    };    

    template <typename TList>
    inline constexpr bool is_unique_v = is_unique<TList>::value;

///  
/// @brief is_same checks if all types in the typelist are the same
/// @param  TList variadic template list<T1,T2,...> representing a list of types
/// @return the member constant 'value' equal to `true` if all types are the same. Otherwise value is `false`.
/// @code
///     is_same<list<int,int,int,int>>::value == true
///     is_same<list<int,char,int>>::value == false
/// @endcode
///

    template <typename TList> struct is_same;

    template <typename Head, typename... Tail>
    struct is_same <list<Head,Tail...>> : all_of<list<Tail...>, Head> { 
    };    

    template <typename TList>
    inline constexpr bool is_same_v = is_same<TList>::value;


///  
/// @brief checks if the typelist contains other list as an element
/// @param  TList variadic template list<T1,T2,...> representing a list of types
/// @return the member constant 'value' equal to `true` if at least one element is of list<...> type. Otherwise value is `false`.
/// @code
///     has_nested_list<list<int,int,int,int>>::value == false
///     has_nested_list<list<int,char,list<void>,int>>::value == true
/// @endcode
///

    template <typename TList>
    struct has_nested_list : std::false_type {
    };

    template <typename Head, typename... Tail>
    struct has_nested_list<list<Head, Tail...>> : std::conditional_t <
        is_list<Head>::value
       ,std::true_type
       ,has_nested_list<list<Tail...>>
    > {
    };

    template <typename TList>
    inline constexpr bool has_nested_list_v = has_nested_list<TList>::value;

/// 
/// @brief reverse returns typelist with element in reverse order
/// @param  TList variadic template list<T1,T2,...> representing a list of types
/// @return the member typename 'type' representing typelist of elements in reverse order against the original TList
/// @code
///     using right_to_left = reverse_t<list<A,B,C,D>>;
///     static_assert(std::is_same_v<right_to_left,list<D,C,B,A>>);
/// @endcode
///

    template<typename TList, bool Empty = is_empty<TList>::value>
    struct reverse;

    template<typename TList>
    using reverse_t = typename reverse<TList>::type;

    template<typename TList>
    struct reverse<TList, false> : push_back<reverse_t<pop_front_t<TList>>, front_t<TList>> { 
    };

    template<typename TList>
    struct reverse<TList, true> {
        using type = TList;
    };


/// 
/// @brief enumerates all types in the typelist one by another and perform any operations on the elements within the typlelist in a particular way.
/// @param  TList variadic template list<T1,T2,...> representing a list of types
/// @param  MetaFun describes transformation rule. It must meet the following defintion: template <typename> MetaFun { using type = some_operation<T> };
/// @return the member typename 'type' representing typelist with "transformed" elements from the original TList
/// @code
///     template <typename T> make_pointer {
///         using type = T*;    
///     };    
///     using list = transform_t<list<A,B,C>,make_pointer>;  // output: list<A*,B*,C*> 
/// @endcode
///

    template<typename TList, template<typename T> class MetaFun, bool Empty = is_empty<TList>::value>
    struct transform;

    template<typename TList, template<typename T> class MetaFun>
    using transform_t = typename transform<TList, MetaFun>::type;

    template<typename TList, template<typename T> class MetaFun>
    struct transform<TList, MetaFun, false> : push_front <
                 transform_t<pop_front_t<TList>,MetaFun>
                ,typename MetaFun<front_t<TList>>::type
            >
    {
    };

    template<typename TList, template<typename T> class MetaFun>
    struct transform<TList, MetaFun, true> {
        using type = TList;
    };
    
/// 
/// @brief combines all of the elements in the typelist into a single resulting value.
///     The `accumulate` algorithm takes a typelist TList with elements T1 ,T2 , ..., TN
///     an initial type I, and a metafunction MetaFun , which accepts two types and returns a type. 
///     It returns MetaFun(MetaFun (MetaFun (...MetaFun(I, T1), T2), ...), TN),
///     where at the i-th step in the accumulation MetaFun is applied to the result of the previous i−1 steps and Ti .
/// @param  TList variadic template list<T1,T2,...> representing a list of types
/// @param  MetaFun describes accumulator rule. 
///         It must meet the following defintion: template <typename ResultType, typename T> MetaFun { using type = some_operation<ResultType,T> };
/// @param  I initial type. 
/// @return the member typename 'type' representing accumulated type MetaFun<...>::type
/// @see run-time analog: https://en.cppreference.com/w/cpp/algorithm/accumulate    
/// @code
///         template <typename T1, typename T2>
///         struct filter { // selects only integral types
///             using type = std::conditional_t<std::is_integral<T2>::value, push_back_t<T1,T2>, T1>;
///         };
///         using input = list<unsigned, double, long, float, void, char*, short>;
///         using output = accumulate_t<input, filter, list<>>;   // list<unsigned, long, short>
/// @endcode
///

   template<
        typename TList
       ,template<typename ResultT, typename T> class MetaFun
       ,typename I
       ,bool = is_empty<TList>::value
       >
   struct accumulate;

   template<typename TList, template<typename ResultT, typename T> class MetaFun, typename I>
   struct accumulate<TList, MetaFun, I, false> : accumulate<
                                                        pop_front_t<TList>
                                                       ,MetaFun
                                                       ,typename MetaFun<I, front_t<TList>>::type
                                                   > {
   };

   template<typename TList, template<typename ResultT, typename T> class MetaFun, typename I>
   struct accumulate<TList, MetaFun, I, true> {
       using type = I;
   };

   template<typename TList, template<typename ResultT, typename T> class MetaFun, typename I>
   using accumulate_t = typename accumulate<TList, MetaFun, I>::type;


/// 
/// @brief eliminates all but the first element of type T from the typelist 
/// @param  TList variadic template list<T1,T2,...> representing a list of types
/// @return the member typename 'type' representing typelist of elements with no duplications
/// @code
///     using output = unique_t<list<A,B,C,A,D,B>>;
///     static_assert(std::is_same_v<output,list<A,B,C,D>>);
/// @endcode
///

   template <typename TList>
   struct unique : accumulate<TList, push_back_unique, list<>> {
   };

   template<typename TList>
   using unique_t = typename unique<TList>::type;

/// 
/// @brief eliminates all but the first element of type T from the input typelist and populates the output typelist of elements in reverse order 
/// @param  TList variadic template list<T1,T2,...> representing a list of types
/// @return the member typename 'type' representing typelist of elements with no duplications. Resulting typelist has elements in reverse order.
/// @code
///     using output = unique_reverse_t<list<A,B,C,A,D,B>>;
///     static_assert(std::is_same_v<output,list<D,C,B,A>>);
/// @endcode
///

   template <typename TList>
   struct unique_reverse : accumulate<TList,push_front_unique,list<>> {
   };

   template<typename TList>
   using unique_reverse_t = typename unique_reverse<TList>::type;


///
/// @brief inserts linearly all elements from one typelist onto the end of the other one
/// @param TList1 a destination typelis, tl::list<type1,type2,...>
/// @param TList2 a source typelis, tl::list<type1,type2,...tl::list<type3,type4,...>>
///         TList2 is to be inserted. If it contains nested lists than their elements are copied linearly (nested lists are expanded) 
/// @return the member typename 'type' representing the typelist as a concatenation TList1 + TList2
/// @code
///     using destination = list<void>;
///     using source = list<int,list<char,bool>,list<float,double>>;
///     using composition = copy_linearly_t<destination,source>; // list<void,int,char,bool,float,double>
/// @endcode
///

   template<typename TList, typename T>
   struct copy_linearly : push_back<TList, T> {
   };

   template <typename TList1, typename... Ts>
   struct copy_linearly<TList1, list<Ts...>> : accumulate<list<Ts...>, copy_linearly, TList1> {
   };

   template <typename TList1>
   struct copy_linearly<TList1, list<>> {
      using type = TList1;
   };

   template<typename TList1, typename TList2>
   using copy_linearly_t = typename copy_linearly<TList1, TList2>::type;


///
/// @brief expands input typelist and makes it linear
/// @param TList a typelis, tl::list<type1,type2,...>
///         If it contains nested lists than they will be expanded
/// @return the member typename 'type' representing the linear typelist (no nested typelists)
/// @code
///      using my_list = linear_list_t<list<list<List<A,B>>>,C>;  // list<A,B,C>
/// @endcode
///

   template<typename TList>
   struct linear_list : copy_linearly<list<>, TList> {
   };

   template<typename TList>
   using  linear_list_t = typename  linear_list<TList>::type;


////////////////////// TBD
// remove_first<TList,T>
// remove_last<TList,T>
// remove_all<TList,T>
// for_each(TList,Visitor<T>)
//
// ... a list of algorithms is incomplete


}   // namespace tl, tl = typelist
