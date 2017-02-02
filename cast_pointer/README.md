# TO_TYPE pointer_cast(FROM_TYPE)
Safe & Portable converting a pointer of any type into integer representation.
In case of dimension shortage one of them (integer,pointer) compiler error will be raised 

```cpp
    void*       ptr;
    MyObject*   obj;
    const MyObject*   obj2;
    ....

    auto v1 = pointer_cast<int>(ptr);       // <-- Error if target platform is 64bit
    auto v2 = pointer_cast<long>(obj);      // <-- Error if target platform is 64bit
    auto v3 = pointer_cast<long long>(obj2);

    // vice versa
    ptr     = pointer_cast<void*>(v1);
    obj     = pointer_cast<MyObject*>(v2);
    obj2    = pointer_cast<const MyObject*>(v3); // <-- Error if target platform is 32bit
```

## Supported Compilers
* TDM-GCC 4.9.2 integrated with [improved fork of Bloodshed Dev-C++](https://sourceforge.net/projects/orwelldevcpp/?source=typ_redirect)
* Microsoft (R) C/C++ Optimizing Compiler Version 19.00.xyz ([Visual Studio 2015](https://www.visualstudio.com/vs/visual-studio-express/))
