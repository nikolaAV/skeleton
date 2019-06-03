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

      void release() noexcept;

      blocking_stream(const blocking_stream&)            = delete;
      blocking_stream& operator=(const blocking_stream&) = delete;

   public:
      /**
         Creates a blocking type stream object from a URL and downloads the data from the Internet.
         When the data is downloaded, the client application or control can read it by using the blocking_stream::read method.

         \exception std::runtime_error
      */
      explicit
      blocking_stream(std::string_view url);
      blocking_stream(blocking_stream&&) noexcept;
      blocking_stream() = default;           // <-- "empty" stream
      ~blocking_stream();

      blocking_stream& operator=(blocking_stream&&) noexcept;

      void swap(blocking_stream& other) noexcept;
      /**
         \brief reads a specified number of bytes ('out_size') from the MS moniker stream object into memory, starting at the current seek pointer.

         \param [out] 'out' - a pointer to the buffer which the stream data is read into
         \param [in]  'out_size' - The number of bytes of data to read i.e. capacity of buffer 'out'
         \param [out] 'bytes_read' - reference to unsigned long variable that receives the actual number of bytes read. The number of bytes read may be zero.
         \retval      the same as 'bytes_read'
         \see https://docs.microsoft.com/en-us/windows/desktop/api/objidl/nf-objidl-isequentialstream-read
      */
      unsigned long read(void* out, unsigned long out_size, unsigned long& bytes_read);
   };

} // end of namespace ms_urlmon

#endif // MICROSOFT_URL_MONIKER_INCLUDED_
