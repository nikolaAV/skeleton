//#include "stdwarnings_suppress_on.h"
#include <sstream>
#include <iostream>
#include "ms_url_moniker.h"
//#include "stdwarnings_suppress_off.h"


class web_page_stream {
   ms_urlmon::blocking_stream wrap_;

   template <size_t N>
   unsigned long read(char(& buff)[N]) {
      unsigned long bytes_read{};
      return wrap_.read(buff, N, bytes_read);
   }

public:
   explicit 
   web_page_stream(std::string_view url) : wrap_(url) {}

   template <size_t buffer_size, typename OtherStream>
   void read(OtherStream& out) {
      char buff[buffer_size] = {};   
      for(auto bytes_read = read(buff); bytes_read > 0U; bytes_read = read(buff))
         out.write(buff, static_cast<std::streamsize>(bytes_read));
   }
};

using namespace std; 

int main()
{
   try
   {
      web_page_stream wps {"https://isocpp.org/about"};

      stringstream ss;
      wps.read<100>(ss);

      cout << ss.str() << endl;
   }
   catch(const exception& e)
   {
      cout << e.what() << endl;
   }
   cin.get();
}
