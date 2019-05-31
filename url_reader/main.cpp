//#include "stdwarnings_suppress_on.h"
#include <set>
#include <string>
#include <sstream>
#include <iostream>
#include <string_view>
#include <cassert>
#include <memory>
#include <utility>
#include <stdexcept>

#include <urlmon.h>
#include <comdef.h>
//#include "stdwarnings_suppress_off.h"

#pragma comment(lib, "urlmon.lib")

/**

   https://stackoverflow.com/questions/1011339/how-do-you-make-a-http-request-with-c/12374407#12374407
   https://docs.microsoft.com/en-us/previous-versions/windows/internet-explorer/ie-developer/platform-apis/ms774965(v%3dvs.85)
   Microsoft URL Monikers
   https://msdn.microsoft.com/en-us/ie/aa741006(v=vs.94)

*/

class blocking_stream_wrapper {
   IStream* native_{};
public:
            blocking_stream_wrapper() = default;
   explicit blocking_stream_wrapper(std::string_view url) {
      if(HRESULT res = ::URLOpenBlockingStream(0, url.data(), &native_, 0, 0); res!=S_OK)
         throw std::runtime_error{_com_error{res}.ErrorMessage()};
   }
   ~blocking_stream_wrapper() {
      if(native_)
         native_->Release();
   }
   unsigned long read(void* out, unsigned long out_size, unsigned long& bytes_read) {
      assert(out && out_size);
      native_->Read(out, out_size, &bytes_read);
      return bytes_read;
   }
   void swap(blocking_stream_wrapper& other) {
      std::swap(native_,other.native_);
   }
};


class web_page_stream {
   blocking_stream_wrapper wrap_;

   template <size_t N>
   unsigned long read(char(& buff)[N]) {
      unsigned long bytes_read{};
      return wrap_.read(buff, N, bytes_read);
   }

public:
   web_page_stream()                                  = delete;
   web_page_stream(const web_page_stream&)            = delete;
   web_page_stream& operator=(const web_page_stream&) = delete;
   web_page_stream& operator=(web_page_stream&&)      = delete;

   explicit web_page_stream(std::string_view url) : wrap_(url) {}

   web_page_stream(web_page_stream&& other) {
      swap(other);
   }

   void swap(web_page_stream& other) {
      wrap_.swap(other.wrap_);
   }

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
