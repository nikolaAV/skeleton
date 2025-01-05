# lightweight pipeline builder in functional style  

TBD

Example of usage:
```cpp
#include "pipe.h"

int main()
{
//   std::vector<Person> input = {
   Person input[] = {
       {"Olivia", Gender::female,  30, EducationLevel::primary}
      ,{"Jonh",   Gender::male,    25, EducationLevel::bachelor}
      ,{"Emma",   Gender::female,  16, EducationLevel::secondary}
      ,{"Alex",   Gender::male,    20, EducationLevel::master}
      ,{"Oliver", Gender::male,    22, EducationLevel::master}
      ,{"William",Gender::male,    60, EducationLevel::none}
      ,{"Anna",   Gender::female,  18, EducationLevel::secondary}
      ,{"Janett", Gender::female,  33, EducationLevel::doctor}
      ,{"Henry",  Gender::male,    43, EducationLevel::doctor}
   };

   using namespace pipe;
   auto resrult = input | filter(is_female) | transform(to_name) | fold(concatenate, std::string(""));
   std::cout << resrult << std::endl;
}

// output 
Janett, Anna, Emma, Olivia
```


## Further informations
TBD

## Related links
* TBD

[back to algorithm](../)

## Compilers
C++14 compliant

* [GCC 5.5.0](https://wandbox.org/)
* [clang 5.0.0](https://wandbox.org/)
* Microsoft (R) C/C++ Compiler 19.14 
