# How to enable/disable particular warning come from std?
As far as we are pragmatic and humble programmers we always ask the compiler to point out gaps inside our code.
But that is not concerned to the Library written (and tested) by the gurus.
The technique which proposed below allow to suppress warnings come from the Standard Library and do nothing concerning your own code.

```cpp
#include "stdwarnings_suppress_on.h" // <-- to suppress/hide every well-known warning message comes from C++ std
	#include <iostream>
	#include <vector>
	...
#include "stdwarnings_suppress_off.h" //<-- to rollback the severities of the messages 

#include "my_bicycle.h"

int main()
{
	...
```

## Further informations
* [How to Disable a Warning in C++](https://www.fluentcpp.com/2019/08/30/how-to-disable-a-warning-in-cpp/) by Jonathan Boccara

## Related links
* TBD

## Compilers
* Microsoft (R) C/C++ Compiler
* GCC
* Clang
