# URL Reader (Windows only)
C++ programmers offten [ask](https://stackoverflow.com/questions/1011339/how-do-you-make-a-http-request-with-c) 
- Is there any way to _easily_ make a HTTP request with C++?
- I want to download the contents of a WEB page. How can it be done with minimum effort?
- Is it possible to download the content into a string?
- ...

Unfortunately, C++ does not provide any way to do it directly. The Committee is currently still working on [__Networking TS__](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/n4771.pdf) (a library for socket-based networking and asynchronous operations).  
Over the years some portable libraries have been developed. The most widely used are
- [libcurl](https://curl.haxx.se/libcurl/) is a free and easy-to-use client-side URL transfer library, supporting a lot of protocols.
- [Qt Network](https://doc.qt.io/qt-5/qtnetwork-index.html) is a part of well-known Qt library.
- [boost::asio](https://think-async.com/Asio/) library for network and low-level I/O programming that provides developers with a consistent asynchronous model using a modern C++ approach
- [and many others](https://curl.haxx.se/libcurl/competitors.html)

If you are unwilling to spend your time learning the mentioned approaches, if your purpose is as simple as simple questions above, if your target platform is Windows then an implementation described here may be the most preferable for you.  
All you need to do that is
- add _ms_url_moniker.cpp_ file into your Visual Studio project
- and use `ms_urlmon::blocking_stream` class by including _ms_url_moniker.h_ 
```cpp
std::stringstream ss;
ss << urlstream{"https://isocpp.org/about"};
std::cout << ss.str() << std::endl;

std::fstream fs{"japanese.html", ios_base::out|ios_base::binary};
f << urlstream{ "https://ja.cppreference.com/" };
```
File _ms_url_moniker.cpp_ uses Microsoft URL Monikers Library (urlmon.lib) which is fully hidden with the interface provided by _urlstream_ class. 
`#pragma comment(lib, "urlmon.lib")` inside _ms_url_moniker.cpp_ does routine work for you. 
You don't need inform the linker to search external  urlmon.lib by configuring the project.

## Further informations
* [How do you make a HTTP request with C++?](https://stackoverflow.com/questions/1011339/how-do-you-make-a-http-request-with-c) on stackoverflow
* [Creating and Using URL Monikers](https://msdn.microsoft.com/en-us/ie/aa741006(v=vs.94))

## Related links
* TBD

## Compilers
* Microsoft (R) C/C++ Compiler 19.16 
