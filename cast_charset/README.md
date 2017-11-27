# charset_cast<TO_TYPE>(FROM_TYPE)
transformation std::strings (C-strings i.e. zero terminated) of __fixed__ charsets into wstrings and vice versa  

Available signatures:
```cpp
         std::wstring to_wstring(const char*,          std::locale);
         std::wstring to_wstring(const std::string&,   std::locale);
            // Shorter versions which use the global C++ locale object 
            // or a copy of std::locale::classic if no call to std::locale::global has been made.
         std::wstring charset_cast<std::wstring>(const char*)
         std::wstring charset_cast<std::wstring>(const std::string&)
         std::wstring wstring_cast(const char*)
         std::wstring wstring_cast(const std::string&)

         std::string to_string(const wchar_t*,         std::locale);
         std::string to_string(const std::wstring&,    std::locale);
            // Shorter versions which use the global C++ locale object 
            // or a copy of std::locale::classic if no call to std::locale::global has been made.
         std::string charset_cast<std::string>(const wchar_t*)
         std::string charset_cast<std::string>(const std::wstring&)
         std::string string_cast(const wchar_t*)
         std::string string_cast(const std::wstring&)
```
where
std::string can be used to represent the following character sets:
* `US-ASCII` (7-bit character set)
* `ISO-LAtin-1` or `ISO-8859-1`  (8-bit character set)
* `ISO-LAtin-9` or `ISO-8859-15` (8-bit character set)

std::wtring can be used to represent the following character sets:
* `UCS-2` (16-bit fixed-sized character set)
* `UCS-4` or `UTF-32` (32-bit fixed-sized character set)                      

> NOTE: multibyte character-sets UTF-8 and UTF-16 are out of the specification described above			 

## Supported Compilers
* TDM-GCC 4.9.2 integrated with [improved fork of Bloodshed Dev-C++](https://sourceforge.net/projects/orwelldevcpp/?source=typ_redirect)
* Microsoft (R) C/C++ Optimizing Compiler Version 19.00.xyz ([Visual Studio 2015](https://www.visualstudio.com/vs/visual-studio-express/))

# Related links
* [The Standard C++ Library](https://books.google.com.ua/books?id=gUhE8po4jgAC&pg=PA889&lpg=PA889&dq=The+Standard+C%2B%2B+Library+by+Nicolai+M.Josuttis.+16.4.4+Character+Classification+and+Conversion&source=bl&ots=nePJgBLRUy&sig=WzW8JcRX2NrfBW_xkpLGPIVBuHw&hl=en&sa=X&ved=0ahUKEwia65e9yuzRAhVliFQKHeByD-cQ6AEIJDAB#v=onepage&q=The%20Standard%20C%2B%2B%20Library%20by%20Nicolai%20M.Josuttis.%2016.4.4%20Character%20Classification%20and%20Conversion&f=false) by Nicolai M.Josuttis. 16.4.4 Character Classification and Conversion
