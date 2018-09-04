#include <system_error>

/**
   http://blog.think-async.com/2010/04/system-error-support-in-c0x-part-4.html

   Steps that are required to provide your own errors
   1) define set of error constants of enum type that are specific you domain scope
         enum class myerrc
         {err1,err2,err3,...};
   2) define an error_category class which indicates a source of errors. 
      The category and a myerrc constant is the pair that uniquely represets your error
         class my_error_category : std::error_category
         {...}
   3) give the category a human-readable name
      const char* my_error_category::name() const override 
         {  return "my_category"; }
   4) every myerrc constant should have string representation
         std::string my_error_category::message(int code) const override {
            switch(code) 
            {
               case myerrc::err1: return "err1";
               ...
   5) uniquely identify your category. The identity of an error_category-derived object (my_error_category) is determined by its address
         static const std::error_category& my_error_category::::instance() {
            static my_error_category instance{};
            return instance;
         }
   6) construct an std::error_code from the myerrc
         inline std::error_code make_error_code(myerrc e) {
            return std::error_code(e,my_error_category::instance());
         }
   7) register for implicit conversion to std::error_code. We have to specify 'enum class myerrc' contains error constants of our domain scope
         namespace std
         {
            template <> struct is_error_code_enum<myerrc> : public std::true_type {};
         }
*/

namespace http
{
   /**
      Let's assume we're writing an HTTP library and need errors that correspond to the HTTP response codes.
   */

   enum class errc
   {
     continue_request      = 100,
     switching_protocols   = 101,
     ok                    = 200,
     // ...
     forbidden             = 403,
     gateway_timeout       = 504,
     version_not_supported = 505
   };

   class error_category : public std::error_category
   {
      error_category() = default;

   public:
      static const std::error_category& instance() noexcept {
         static error_category instance{};
         return instance;
      }

      const char*          name() const  noexcept override {
         return "http"; 
      }
      std::string          message(int code) const override {
         switch(static_cast<errc>(code)) 
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
         }
      }
      std::error_condition default_error_condition(int code) const noexcept override {
         switch (static_cast<errc>(code))
         {
            case errc::forbidden:
               return std::errc::permission_denied;
            default:
               return std::error_condition(code, *this);
         }
      }
   };

   inline std::error_code make_error_code(errc e) noexcept
   {
      return std::error_code(
         static_cast<int>(e),
         error_category::instance());
   }

}  // namespace http  

namespace std
{
   template <>
   struct is_error_code_enum<http::errc>
      : public true_type {};

}  // namespace std 

#include <future>
#include <iostream>


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
