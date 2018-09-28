# iterator_traits<> become to be similar to type_traits<>
Iterators are an extremely important concept in C++.
The STL aims to be as flexible and generic as possible, and iterators are a great help in that regard.
Unfortunately, they are sometimes a bit tedious to use, which is why many novices avoid them and even fall back to C-Style C++.  
There are multiple [categories of iterators](https://en.cppreference.com/w/cpp/iterator), and they have different limitations.
They are not too hard to memorize, just remember that the capabilities one category requires are inherited from the next powerful category.
The whole point of iterator categories is that if an algorithm knows what kind of iterator it is dealing with, it can be implemented in an optimized way.
This way, the programmer can lean back and express his intent, while the compiler can choose the optimal implementation for the given task.  
But another side of this story is when a programmer himself wants to apply such optimization.
Let's say he wants to implement several overloaded his own algorithms for different parametrized input agruments like that:
```cpp
    // version for iterators
    template <typename InIter, typename OutIter>
    enable_if_t<is_iterator_v<InIter> && is_iterator_v<OutIter>>
    my_algorithm(InIter src_first, OutIter d_first) {
        ...

    // version for streams
    template <typename InStream, typename OutStream>
    enable_if_t<is_stream_v<InStream> && is_stream_v<OutStream>>
    my_algorithm(InStream& src, OutStream& dest) {
        ...

    // version for containers
    template <typename InContainer, typename OutContainer>
    enable_if_t<is_container_v<InContainer> && is_container_v<OutContainer>>
    my_algorithm(const InContainer& src, OutContainer& dest) {
        ...
```
Focusing on iterators, we would like to have approach to detect iterator categories at compile time and this approach would be close to how it has been done for built-in type:
[`std::integtal<>`](https://en.cppreference.com/w/cpp/header/type_traits), `std::pointer<>`, `std::function<>`, etc.  
Thus, we wants to see code below to be compilable:
```cpp
    vector<int> v = {1,2,3,4,5};
    auto it = v.begin();
    using container_type = decltype(v);
    using iterator_type =  decltype(it);
    static_assert(!is_iterator_v<container_type>);
    static_assert(is_iterator_v<iterator_type>);
    static_assert(is_random_access_iterator_v<iterator_type>);
    ...
    list<int> v = {1,2,3,4,5};
    auto it = v.begin();
    using container_type = decltype(v);
    using iterator_type =  decltype(it);
    static_assert(!is_iterator_v<container_type>);
    static_assert(is_iterator_v<iterator_type>);
    static_assert(is_bidirectional_iterator_v<iterator_type>);
```
#### iterator_traits2
A header only library in which iterator traits are defined as compile-time template-based interface to query the properties of iterators.
```cpp
template< class T > struct is_iterator;
template< class T > struct is_random_access_iterator;
template< class T > struct is_input_iterator;
template< class T > struct is_output_iterator;
template< class T > struct is_forward_iterator;
template< class T > struct is_bidirectional_iterator;
```
where __T__ - a type to check  
Every mentioned trait above '___is_...__' after instantiation is derived from one of two possible boolean types: [`std::true_type`](https://en.cppreference.com/w/cpp/types/integral_constant) or `std::false_type`.
Moreover each trait is accompanied with an appropriate alias variable template
```cpp
template< class T > inline constexpr bool is_iterator_v = is_iterator<T>::value;
...
```
#### How does it work?
In order to detect T is an iterator, `std::void_t` metafunction is used which is based on ill-formed types in [SFINAE](https://en.cppreference.com/w/cpp/language/sfinae) context. 
```cpp
// primary template handles types that have no nested ::iterator_category member:
template< class, class = std::void_t<> >
struct is_iterator : std::false_type { };
 
// specialization recognizes types that do have a nested ::iterator_category member:
template< class T >
struct is_iterator<T, std::void_t<typename T::iterator_category>> : std::true_type { };

// specialization recognizes types that do have a nested ::iterator_category member
// and (!!!) this nested ::iterator_category is equal to std::forward_iterator_tag 
template< class T >
struct is_forward_iterator ...  
                      : std::conditional_t<
                            std::is_same<typename T::iterator_category,std::forward_iterator_tag>::value
                           ,true_type
                           ,false_type> {};
```
## Further informations
* [`std::void_t`](https://en.cppreference.com/w/cpp/types/void_t)
* [`std::conditional`](https://en.cppreference.com/w/cpp/types/conditional)
* [SFINAE](https://en.cppreference.com/w/cpp/language/sfinae)
* [Modern Template Metaprogramming](https://www.youtube.com/watch?v=a0FliKwcwXE&feature=youtu.be&t=52m50s) by Walter E.Brown on CppCon 2014  

## Related links
* [Member-function detection at the compile-time](https://github.com/nikolaAV/Modern-Cpp/tree/master/concept%20(pceudo)/has_type_member)
* [Overload resolution for template function](https://github.com/nikolaAV/Modern-Cpp/tree/master/concept%20(pceudo)/enable_if_overloading#overload-resolution-for-template-function-with-conditional-removement-by-means-stdenable_if)

## Compilers
* [GCC 8.1.0](https://wandbox.org/)
* [clang 6.0.0](https://wandbox.org/)
* Microsoft (R) C/C++ Compiler 19.14 
