#include <iostream>
#include "pointer_cast.h"

using namespace std;

struct MyObject {} instance;

int main()
{

    void*             ptr {nullptr};
    MyObject*         obj {&instance};
    const MyObject*   obj2{&instance};

    auto v1 = pointer_cast<int>(ptr);       // <-- Error if target platform is 64bit
    auto v2 = pointer_cast<long>(obj);      // <-- Error if target platform is 64bit
    auto v3 = pointer_cast<long long>(obj2);

    ptr     = pointer_cast<void*>(v1);
    obj     = pointer_cast<MyObject*>(v2);
    obj2    = pointer_cast<const MyObject*>(v3); // <-- Error if target platform is 32bit
}
