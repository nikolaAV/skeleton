# XOR cipher
XOR encryption (or Exclusive-OR encryption) is a common method of encrypting data into a format that cannot be trivially cracked by the _average_ person but it can trivially be broken if a person knows cryptanalysis. The XOR operator is extremely common as a component in more complex ciphers. The current implementation of XOR cipher is STL-like algorithm that provides enormous variability of function calls by means overloaded function templates. 
### Basics
'Exclisive OR' is sometimes called modulus 2 addition (or subtraction, which is identical). With this logic, a string of text can be encrypted by applying the bitwise XOR operator to every character using a given key. To decrypt the output, merely reapplying the XOR function with the key will remove the cipher.   
For example, the string "Wiki" (01010111 01101001 01101011 01101001 in 8-bit ASCII) can be encrypted with the repeating key 11110011 as follows:  
![example](./example.gif)  
That is an equivalent to code:
```cpp
using bits8 = std::bitset<8>;

inline bits8 operator"" _bs ( const char* s ) {
    return bits8{s};
}

std::vector<bits8>  src = {01010111_bs, 01101001_bs, 01101011_bs, 01101001_bs};
const bits8    key = {11110011_bs};

simple_xor(src,key);
assert(src[0]==10100100_bs);
assert(src[1]==10011010_bs);
assert(src[2]==10011000_bs);
assert(src[3]==10011010_bs);

simple_xor(src,key);
assert(src[0]==01010111_bs);
assert(src[1]==01101001_bs);
assert(src[2]==01101011_bs);
assert(src[3]==01101001_bs);
```

## Further informations
* [XOR cipher](https://en.wikipedia.org/wiki/XOR_cipher)
* [Exclusive or](https://en.wikipedia.org/wiki/Exclusive_or)

## Related links
* [compile time checks](../../iterator_traits2) for categories of iterators
* [Overloading by means std::enable_if](https://github.com/nikolaAV/Modern-Cpp/tree/master/concept%20(pceudo)/enable_if_overloading)
[back to algorithm](../)

## Compilers
* [GCC 8.2.0](https://wandbox.org/)
* [clang 7.0.0](https://wandbox.org/)
* Microsoft (R) C/C++ Compiler 19.15 
