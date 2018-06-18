In some situations, the existing STL algorithms are not enough. But nothing hinders us from implementing our own.

* [quick_remove_at & quick_remove_if](./quick_remove) 
These algorithms are dedicated for the particular type of STL containers: vectors. You can remove an element by the index (quick_remove_at) or all elements (quick_remove_if) satisfying criteria with the amortized constant complexity __O(1)__. But these algorithms do not guarantee element ordering after the operation completion.

