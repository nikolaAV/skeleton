#include <stdexcept>
#include <utility>
#include <cassert>

#if defined(_WIN32) || (_WIN64)

#include <urlmon.h>
#include <comdef.h>
#include "ms_url_moniker.h"

#pragma comment(lib, "urlmon.lib")

namespace ms_urlmon {

using namespace std; 

blocking_stream::blocking_stream(string_view url)
{
   // https://docs.microsoft.com/en-us/previous-versions/windows/internet-explorer/ie-developer/platform-apis/ms775127(v=vs.85)
   if(HRESULT res = ::URLOpenBlockingStream(0, url.data(), &native_, 0, 0); res!=S_OK)
      throw runtime_error{_com_error{res}.ErrorMessage()};
}

blocking_stream::blocking_stream(blocking_stream&& other) noexcept
{
   swap(other);
}

blocking_stream& blocking_stream::operator=(blocking_stream&& other) noexcept
{
   swap(other);
   other.release();
   return *this;
}

blocking_stream::~blocking_stream()
{
   release();
}

void blocking_stream::release() noexcept
{
   if(native_) {
      IStream* copy{};
      std::swap(copy, native_);
      try {
         copy->Release();
      }
      catch (...) {
         assert(!"[blocking_stream::release] something is extremely wrong.");
      }
   }
}

unsigned long blocking_stream::read(void* out, unsigned long out_size, unsigned long& bytes_read) 
{
   // https://docs.microsoft.com/en-us/windows/desktop/api/objidl/nn-objidl-istream
   assert(out && out_size);
   native_->Read(out, out_size, &bytes_read);
   return bytes_read;
}

void blocking_stream::swap(blocking_stream& other) noexcept
{
   std::swap(native_,other.native_);
}

} // end of namespace ms_urlmon

#else // !((_WIN32) || (_WIN64))
   #error Windows platform is only supported.
#endif
