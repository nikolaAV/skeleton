In some situations, the existing STL algorithms are not enough. But nothing hinders us from implementing our own.

* [quick_remove_at & quick_remove_if](./quick_remove) \
These algorithms are dedicated for the particular type of STL containers: vectors. You can remove an element by the index (quick_remove_at)  with the amortized constant complexity _O(1)_. Note: the algorithm does not guarantee element ordering after the operation completion. There is also version (quick_remove_if) that removes all elements satisfying criteria with complexity _O(n)_.
* [split & split_if](./split) \
To split __any__ (not only strings) range of items at each occurrence of a specific value, and it copies the chunks that result from that into an output range.
* [trim, trim_left, trim_right & trim_if](./trim) \
Trim- family functions to eliminate elements by a requested criteria from STL containers on both ends. Note: it's not restricted for only std::string & std::wstring.
* [find_and_replace](./find_and_replace) \
Find and Replace All Occurrences in a C++ String: `string`, `wstring`, `u16string`, `u32string`.

