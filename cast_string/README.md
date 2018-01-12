# How do I convert an integer to a string and vice versa?
The simplest way is to use a stringstream.
this technique works for converting any type that you can output using << to a string and there is operator >> from input stream to your type

```cpp
  auto const s1 = string_cast<string>('5');
  auto const s2 = string_cast<string>(55);
  auto const s3 = string_cast<wstring>(5.123);

  cout  << "char: "  << string_cast<char>(s1) << endl
		<< "int: "   << string_cast<int>(s2) << endl
		<< "float: " << string_cast<float>(s3) << endl;
```

# Related links
* [C++ Style and Technique FAQ](http://www.stroustrup.com/bs_faq2.html#int-to-string) by Bjarne Stroustrup
* [C++17, Elementary string conversions](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0067r5.html)
