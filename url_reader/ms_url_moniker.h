#ifndef MICROSOFT_URL_MONIKER_INCLUDED_
#define MICROSOFT_URL_MONIKER_INCLUDED_

/**

   https://stackoverflow.com/questions/1011339/how-do-you-make-a-http-request-with-c/12374407#12374407
   https://docs.microsoft.com/en-us/previous-versions/windows/internet-explorer/ie-developer/platform-apis/ms774965(v%3dvs.85)
   Microsoft URL Monikers
   https://msdn.microsoft.com/en-us/ie/aa741006(v=vs.94)
*/

#include <string_view>

struct IStream;   // forward declaration

namespace ms_urlmon {

   class blocking_stream {
      IStream* native_{};
      void release();

   public:
      blocking_stream(const blocking_stream&) = delete;
      explicit
      blocking_stream(std::string_view url); // can throw std::runtime_error
      blocking_stream(blocking_stream&&);
      blocking_stream() = default;
      ~blocking_stream();

      void swap(blocking_stream& other) noexcept;
      /**
         \param out
         \param out_size
         \param bytes_read
         \retval 
      */
      unsigned long read(void* out, unsigned long out_size, unsigned long& bytes_read);
   };

} // end of namespace ms_urlmon

#endif // MICROSOFT_URL_MONIKER_INCLUDED_
