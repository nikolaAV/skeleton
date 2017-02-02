#if !defined( _SAFE_POINTER_CAST_H__)
#define _SAFE_POINTER_CAST_H__

#include <cstdint>

/**
    Safe & Portable converting a pointer of any type into integer representation
    In case of dimension shortage one of them (integer,pointer) compiler error will be raised 
    @Example of usage

    void*       ptr;
    MyObject*   obj;
    ....

    int     v1 = pointer_cast<int>(ptr);    // <-- Compile-time Error if target platform is 64bit
    long    v2 = pointer_cast<long>(obj);   // <-- Compile-time Error if target platform is 64bit

    // vice versa
    ptr = pointer_cast<void*>(v1);
    obj = pointer_cast<MyObject*>(v2);
*/

namespace private_
{

    inline intptr_t cast(const void* p)
    {
        static_assert(sizeof(void*)==sizeof(intptr_t), "Incapable integer type of holding a pointer");
        return reinterpret_cast<intptr_t>(p);
    }

    inline void* cast(intptr_t v)
    {
        static_assert(sizeof(void*) == sizeof(intptr_t), "Incapable integer type of holding a pointer");
        return reinterpret_cast<void*>(v);
    }

}   // end of private_

template <typename PTR_TYPE, typename INT_TYPE>
inline PTR_TYPE pointer_cast(INT_TYPE v)
{
    static_assert(sizeof(INT_TYPE)<=sizeof(intptr_t), "Requested integer type will have been truncated");
    return static_cast<PTR_TYPE>(private_::cast(static_cast<intptr_t>(v)));
}

template <typename INT_TYPE, typename PTR_TYPE>
inline INT_TYPE pointer_cast(PTR_TYPE* p)
{
    static_assert(sizeof(intptr_t) <= sizeof(INT_TYPE), "Value of the pointer will have been truncated");
    return static_cast<INT_TYPE>(private_::cast(p));
}



#endif // _SAFE_POINTER_CAST_H__
