#include <sstream>
#include <iostream>
#include <fstream>
#include "ms_url_moniker.h"

class urlstream {
   ms_urlmon::blocking_stream impl_;

   template <size_t N>
   unsigned long read(char(& buff)[N]) {
      unsigned long bytes_read{};
      return impl_.read(buff, N, bytes_read);
   }

public:
   explicit 
   urlstream(std::string_view url) : impl_(url) {} // can throw std::runtime_error

   /**
      \param 'buffer_size' data from the Internet is read by predefined portions where capacity is specified in the number of bytes
      \OtherOutputStream any type which is compliant to std::basic_ostream
   */
   template <size_t buffer_size, typename OtherOutputStream>
   OtherOutputStream& read(OtherOutputStream& out) {
      char buff[buffer_size] = {};   
      for(auto bytes_read = read(buff); bytes_read > 0U; bytes_read = read(buff))
         out.write(buff, static_cast<std::streamsize>(bytes_read));
      return out;
   }
};

template <typename CharT, typename Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& out, urlstream&& in) {
   return in.read<1024>(out);
}

using namespace std; 

int main()
{
   try
   {
      {
         stringstream ss;
         ss << urlstream{"https://isocpp.org/about"};
         cout << ss.str() << endl;
      }
      {
         fstream f{"japanese_web_page.html", ios_base::out|ios_base::binary|ios_base::trunc};
         f << urlstream{ "https://ja.cppreference.com/" };
      }
   }
   catch(const exception& e)
   {
      cout << e.what() << endl;
   }
}
