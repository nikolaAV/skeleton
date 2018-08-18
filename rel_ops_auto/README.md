# Auto generation of comparison operators
Auto generation of comparison operators __!=,>,<=,>=__ for any object of type T with given a user-defined `operator==` and `operator<`. 
```cpp
struct Foo : comparable::type<Foo> { // <-- rel_ops generation is here
    int n;
    Foo(int v) : n(v) {}
};
 
bool operator==(const Foo& lhs, const Foo& rhs)
{
    return lhs.n == rhs.n;
}
 
bool operator<(const Foo& lhs, const Foo& rhs)
{
    return lhs.n < rhs.n;
}
 
int main()
{
    const Foo f1{1}, f2{2};

    cout << boolalpha;
    cout << "not equal?     : " << (f1 != f2) << '\n';
    cout << "greater?       : " << (f1 > f2) << '\n';
    cout << "less equal?    : " << (f1 <= f2) << '\n';
    cout << "greater equal? : " << (f1 >= f2) << '\n';
}
```
As of C++20, this implementation is deprecated in favor of [`operator<=>`](https://en.cppreference.com/w/cpp/language/default_comparisons).

## Further informations
* [Barton–Nackman trick](https://en.wikipedia.org/wiki/Barton%E2%80%93Nackman_trick)
* [`std`::rel_ops](https://en.cppreference.com/w/cpp/utility/rel_ops/operator_cmp)
* [`operator<=>`](https://en.cppreference.com/w/cpp/language/default_comparisons)
## Related links
* [ADL and friend function defenition](https://github.com/nikolaAV/Storehouse-Of-Knowledge/blob/master/questions/README.md#argument-dependent-lookup-friend-function-definition)

## Compilers
* [GCC 8.1.0](https://wandbox.org/)
* [clang 6.0.0](https://wandbox.org/)
* Visual C++ 19.14 
