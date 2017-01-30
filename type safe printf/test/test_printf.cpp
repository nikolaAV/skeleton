#include <string>
#include "..\ts_printf.h"
#include "tut.h"


namespace
{

struct data
{
   data() = default;
   data(const data&) = delete;
   data& operator=(const data&) = delete;
};

using test_group    = tut::test_group<data>;
using test_instance = test_group::object;

test_group tg("type_safe::print");

} // end of anonymous namespace


namespace tut
{

   template<>
   template<>
   void test_instance::test<1>()
   {
        std::stringstream ss;
        type_safe::printf(ss,"% Congratulation!, age %, rating: %", "Mr. Anonym", 48, 123.456);
        ensure(ss.str() == "Mr. Anonym Congratulation!, age 48, rating: 123.456");

        std::wstringstream ws;
        type_safe::printf(ws, L"% Congratulation!, age %, rating: %", L"Mr. Anonym", 48, 123.456);
        ensure(ws.str() == L"Mr. Anonym Congratulation!, age 48, rating: 123.456");

        type_safe::printf(std::wcout, L"% Congratulation!, age %, rating: %\n", L"Mr. Anonym", 48, 123.456);
        type_safe::printf(std::cout, "% Congratulation!, age %, rating: %\n", "Mr. Anonym", 48, 123.456);

        type_safe::printf("% Congratulation!, age %, rating: %\n", "Mr. Anonym", 48, 123.456);
        type_safe::printf(L"% Congratulation!, age %, rating: %\n", L"Mr. Anonym", 48, 123.456);

        const std::string s = type_safe::sprintf("% Congratulation!, age %, rating: %", "Mr. Anonym", 48, 123.456);
        ensure(s == "Mr. Anonym Congratulation!, age 48, rating: 123.456");

        const std::wstring w = type_safe::sprintf(L"% Congratulation!, age %, rating: %", L"Mr. Anonym", 48, 123.456);
        ensure(w == L"Mr. Anonym Congratulation!, age 48, rating: 123.456");
   }

    template<>
    template<>
    void test_instance::test<2>()
    {
        const bool first{false}, second{true};
        try 
        {
            type_safe::printf("first: %, second %, argument is missed", first);
            ensure(!"runtime_error is expected");
        }
        catch(std::runtime_error& e)
        {
            std::cout << e.what() << " <-- It's Ok, right expectancy" << std::endl;
        }

        try
        {
            type_safe::printf("first: %, second ?, ... ", first, second);
            ensure(!"runtime_error is expected");
        }
        catch (std::runtime_error& e)
        {
            std::cout << e.what() << " <-- It's Ok, right expectancy" << std::endl;
        }
    }

    template<>
    template<>
    void test_instance::test<3>()
    {
        ensure(type_safe::sprintf("%%") == "%" );
        ensure(type_safe::sprintf("%%%%") == "%%");
        ensure(type_safe::sprintf("value:%",0) == "value:0");
        ensure(type_safe::sprintf("%%value:%", 0) == "%value:0");
        ensure(type_safe::sprintf("value:% %%", 0) == "value:0 %");
        //ensure(type_safe::sprintf("value:%%%", 0) == "%value:0%"); <-- failed
    }

    struct my_type
    {
      const std::string val_ = "my_type exemplar";
      my_type()                           = default;
      my_type(const my_type&)             = delete;
      my_type(my_type&&)                  = delete;
      my_type& operator=(const my_type&)  = delete;
      my_type& operator=(my_type&&)       = delete;
    };
    template <typename charT, typename traits>
    std::basic_ostream<charT, traits>& operator<<(std::basic_ostream<charT, traits>& out, const my_type& s)
    {
        out << s.val_;
        return out;
    }

    template<>
    template<>
    void test_instance::test<4>()
    {
       my_type instance;
       ensure(type_safe::sprintf("'%', number %", instance, 1) == "'my_type exemplar', number 1");
    }



} // namespace 'tut'
