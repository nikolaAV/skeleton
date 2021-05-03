# output_transform_iterator
The standard way to apply a function to each element and have the results added to a collection is to combine the std::transform algorithm with an output iterator such as std::back_inserter:
```cpp
// f is a function to apply to each element of the collection
int f(std::string const& s);

std::vector<std::string> strings = { "So", "long", "and", "thanks", "for", "all", "the", "fish" };
std::vector<int> results;

transform(strings.begin(), strings.end(), std::back_inserter(results), f);
```
A more modern way, which logic we saw in [Ranges: the STL to the Next Level](https://www.fluentcpp.com/2017/01/12/ranges-stl-to-the-next-level/), is to use ranges and range adaptors:
```cpp
// f is a function to apply to each element of the collection
int f(std::string const& s);

std::vector<std::string> strings = { "So", "long", "and", "thanks", "for", "all", "the", "fish" };
std::vector<int> results;

ranges::copy(strings | ranges::view::transform(f), std::back_inserter(results));
```
One interesting thing to note here is that the main action of the whole operation, which is applying the function `f`, has been transferred to the input range: `strings | ranges::view::transform`, taking this responsibility away from the algorithm. The algorithm then becomes simpler, becoming `copy` instead of `transform`.
When we see it from this perspective, we can see another way of structuring the operation. One that gets less publicity than the other ones is to shifting the logic to the output iterator. 
```cpp
// f is a function to apply to each element of the collection
int f(std::string const& s);

std::vector<std::string> strings = { "So", "long", "and", "thanks", "for", "all", "the", "fish" };
std::vector<int> results;

ranges::copy(strings, transform_f(std::back_inserter(results)));
```
where `transform_f` is an output iterator that applies `f` and forwards this result to the `std::back_inserter`.
A reason to consider smart output iterators is that they are a different way to go about structuring the call to an algorithm. And just for that reason, they can expand our view and give us more perspective on the topic of applying algorithms!

## Further informations
* [how to make copyable object assignable](https://www.fluentcpp.com/2020/11/06/how-to-make-a-copyable-object-assignable-in-cpp/)
* [Smart Output Iterators](https://www.fluentcpp.com/2017/11/28/output-iterator-adaptors-symmetry-range-adaptors/) by Jonathan Boccara

## Related links
* [zip_iterator](../zip_iterator)
* [transform_if=std::accumulate+lambdas](https://github.com/nikolaAV/Modern-Cpp/tree/master/lambda/lambda_currying2)
* [transform_if](../transform_if)

[back to algorithm](../)

## Compilers
* [GCC 7.1.0](https://wandbox.org/)
* [clang 5.0.0](https://wandbox.org/)
* Microsoft (R) C/C++ Compiler 19.16 
