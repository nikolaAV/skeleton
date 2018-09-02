#include <system_error>
#include <future>
#include <iostream>

/**
   http://blog.think-async.com/2010/04/system-error-support-in-c0x-part-4.html

*/

namespace http
{
   /**
      Let's assume we're writing an HTTP library and need errors that correspond to the HTTP response codes.
   */

   enum class errc
   {
     continue_request = 100,
     switching_protocols = 101,
     ok = 200,
     // ...
     forbidden = 403,
     gateway_timeout = 504,
     version_not_supported = 505
   };

   class error_category : public std::error_category
   {
      error_category() = default;

   public:
      static const std::error_category& instance() noexcept;

      const char*          name() const  noexcept override;
      std::string          message(int) const override;
      std::error_condition default_error_condition(int) const noexcept override;
   };

   inline const std::error_category& error_category::instance() noexcept 
   {
      static error_category instance{};
      return instance;
   }

   inline const char* error_category::name() const noexcept 
   { 
      return "http"; 
   }

   inline std::string error_category::message(int ev) const 
   {
      switch(static_cast<errc>(ev)) 
      {
         case errc::continue_request:
            return "Continue";
         case errc::switching_protocols:
            return "Switching protocols";
         case errc::ok:
            return "OK";
         //...
         case errc::forbidden:
            return "Forbidden";
         case errc::gateway_timeout:
            return "Gateway time-out";
         case errc::version_not_supported:
            return "HTTP version not supported";
         default:
            return "Unknown HTTP error";
      }
   }   

   inline std::error_condition error_category::default_error_condition( int ev ) const noexcept 
   {
      switch (static_cast<errc>(ev))
      {
         case errc::forbidden:
            return std::errc::permission_denied;
         default:
            return std::error_condition(ev, *this);
      }
   }

   inline std::error_code make_error_code(errc e) noexcept
   {
      return std::error_code(
         static_cast<int>(e),
         error_category::instance());
   }

   inline std::error_condition make_error_condition(errc e) noexcept
   {
      return std::error_condition(
         static_cast<int>(e),
         error_category::instance());
   }


}  // namespace http  

namespace std
{
  template <>
  struct is_error_code_enum<http::errc>
    : public true_type {};
}


void process(const std::error_code& ec)
{
   using namespace std;
   const auto& default_cond = ec.default_error_condition();
   cout  << "category: " << ec.category().name() << endl
         << "code: " << ec.value() << endl
         << "bool: " << static_cast<bool>(ec) << endl
         << "message: " << ec.message().c_str() << endl
         << "default category: " << default_cond.category().name() << endl  
         << "default code: " << default_cond.value() << endl  
         << "default message: " << default_cond.message().c_str() << endl;  
}


int main()
{
   using namespace std;

   process(make_error_code(errc::file_exists));
   cout << endl;
   process(make_error_code(future_errc::broken_promise));
   cout << endl;
   process(make_error_code(io_errc::stream));
   cout << endl;
   process(make_error_code(http::errc::switching_protocols));
   cout << endl;
   process(make_error_code(http::errc::forbidden));
   cout << endl;

   if(make_error_code(http::errc::forbidden)==http::errc::forbidden)
   {
   }

   if(make_error_code(http::errc::forbidden)==std::errc::permission_denied)
   {
   }
}