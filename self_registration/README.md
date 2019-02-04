# Self-Registering types

One of most common way of working with polymorphic types is to use a factory which produces objects of terminal types hidden by an particular interface for an external code (controllers). Based on the [classic "Shape Hierarchy"](https://docs.microsoft.com/en-us/dotnet/csharp/programming-guide/classes-and-structs/polymorphism) an usage can be like that:
```cpp
#include "shape_factory.h"

void usage(const string& key)
{
    // where 'key' is an identifier of terminal type/class 
    // it can be "circle","rectangle","triangle",etc.
    shape* s = factoty.get(key); 
    s->draw();
}
```
But what has to be done before?
### 1) Interface for an abstract 'shape' figure 
```cpp
// shape.h
class shape
{
public:
   virtual ~shape() {}
   virtual void draw() = 0;
};
```
### 2) Concrete types which are derived from the 'shape'
```cpp
// circle.h,cpp
class circle : public shape
{
   void draw() override {/*...*/}
};

// rectangle.h,cpp
class rectangle : public shape
{
   void draw() override {/*...*/}
};

...
```
### 3) The factory
```cpp
// factory.h,cpp
// a possible implementation
#include "circle.h"
#include "rectangle.h"
...
shape* factoty::get(const string& key)
{
    if(key=="circle")
        return new circle{...};
    if(key=="rectangle")
        return new rectangle{...};
```
The self-registering types allow you to forget the step 3 and focus on domain specific features. You do not have to keep in mind  modifying your 'factory' code every time you need to add new shape: 'triangle', 'square', etc.

Example how to make the terminal classes which implement the 'shape' abstraction self-registering types. 
```cpp
// translation unit 1 : circle.cpp
#include "shape.h"
#include "registrar.h"
class circle : public auto_enrolment<shape,circle> 
{
   void draw() override {/*...*/}
   // concept registrable
   static auto name()   { return "circle"; }
   static auto create() { return new circle(); }
};

// translation unit 2 : rectangle.cpp
#include "shape.h"
#include "registrar.h"
class rectangle : public auto_enrolment<shape,rectangle> 
{
   void draw() override {/*...*/}
   // concept registrable
   static auto name()   { return "rectangle"; }
   static auto create() { return new rectangle(); }
};

...

// translation unit N : program.cpp
#include "shape.h"
#include "registrar.h"
void usage(const string& key)
{
    using shapes   = registrar<shape>;
    auto s = shapes::get(key)
    s->draw();
}
```

## Further informations
* [Factory With Self-Registering Types](http://www.bfilipek.com/2018/02/factory-selfregister.html) by Bartlomiej Filipek

## Related links
* TBD

## Compilers
* [GCC 7.2.0](https://wandbox.org/)
* [clang 5.0.0](https://wandbox.org/)
* Visual C++ 19.12 
 
