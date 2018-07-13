
/**
   Parallelized Direct Acyclic Graph
   ---------------------------------
   A tiny automatic parallelization library with std::future

   \see C++17 STL Cookbook by Jacek Galowicz, Ch 9. (https://www.amazon.com/gp/product/178712049X/ref=as_li_ss_tl?ie=UTF8&fpl=fresh&pd_rd_i=178712049X&pd_rd_r=78JC25KCTX86BK1T2QX1&pd_rd_w=O0On9&pd_rd_wg=CSldc&pf_rd_m=ATVPDKIKX0DER&pf_rd_s=&pf_rd_r=HNN13KHDYJSQT8Y7BFC7&pf_rd_t=36701&pf_rd_p=1cf9d009-399c-49e1-901a-7b8786e59436&pf_rd_i=desktop&linkCode=sl1&tag=bfilipek-20&linkId=e82310b0a2b3cb9fcb98312eb1cea33f)
   \see https://en.wikipedia.org/wiki/Directed_acyclic_graph
*/

#include <future>

namespace pdag
{
   /**
      Usage Example:
         An ordinary function with signature: TR f(TA1,TA2,TA3)
         can be transformed into an asynchronous function object and called as

         auto result = asynchronize(f)(a1,a2,a3)();   
              ^4                    ^1    ^2       ^3
         where
            ^1 - asynchronous version of 'f'. It can be called with the same arguments like 'f'. 
            ^2 - a collable object which stores 'f', 'a1', 'a2', 'a3'. It does not call anything yet.
            ^3 - a direct std::async invocation, i.e. semantic meaning of ^3 is "Take the captured function and the arguments, and throw them together into std::async.".
            ^4 - std::future<TR> object, the result of 'f' can be obtained by calling method get().
   */

   template <typename F>
   auto asynchronize(F f) noexcept {
      return [f](auto... prms) {
         return [=]() {
            return std::async(std::launch::async, f, prms...);
         };
      };
   }

   /**
      It's a collabe object which based on captured 'f', 
      ^1) i.e it just transform a function 'f' into a function object that accepts a range of agruments.
      It supposes that all input arguments 'ftrs...' are std::future<...> objects.
      ^2) This function object does then call .get() on all of the arguments and then finally forwards them to 'f'
      ^3) returns f(...);

      Usage Example:
         auto result = future_unwrap(f)(future1,future2,...);   
              ^3                     ^1 ^2
   */

   template <typename F>
   auto future_unwrap(F f) noexcept {
      return [f](auto... ftrs) {
         return f(ftrs.get()...);
      };
   }

   /**
      \param afs...  argument list of callable objects without input arguments like asynchronize(f)(a1,a2,a3) mentioned above

      Usage Example:
         Let's suppose there are two asynchronous function objects
            auto af1 = asynchronize(func1)(a1,a2,a3);
            auto af2 = asynchronize(func2)(a1);
         and function 'TR fun3(...)' that expects results from both of them 
         then the invocation below means

         auto result = async_adapter(func3)(af1,af2)();   
              ^4                    ^1     ^2       ^3
         where
            ^1 - asynchronous version of 'func3' that mimics 'func3' accepting the same arguments
            ^2 - another function object that captures 'func3', 'af1', 'af2'
            ^3 - a direct invocations of 'af1', 'af2' and then std::async  
                 'af1()', 'af2()' produce future<> values on wich we can call .get by means future_unwrap 
            ^4 - std::future<TR> object, the result of 'func3' can be obtained by calling method get().
   */

   template <typename F>
   auto async_adapter(F f) noexcept {
      return [f](auto... afs) {
         return [=]() {
            return std::async(std::launch::async, future_unwrap(f), afs()...);
         };
      };
   }


}  // namespace pdag


#include <iostream>
#include <string>
#include <string_view>
#include <chrono>

using namespace std;
using namespace chrono_literals;
using namespace chrono;

/**
   Set of computation-heavy functions
   just to simulate time-expensive resources
*/

string create(string_view s)     // <--- costs 3 secs 
{
   this_thread::sleep_for(3s);
   return {cbegin(s),cend(s)};
}

string concat(const string& s1, const string& s2)  // <--- costs 5 secs
{
   this_thread::sleep_for(5s);
   return s1+s2;
}

string twice(const string& s)    // <--- costs 3 secs
{
   this_thread::sleep_for(3s);
   return s+s;
}

class stopwatch
{
   system_clock::time_point start_;
public:
   stopwatch()  noexcept : start_{
      system_clock::now()
   } {}
   void start() noexcept {
      start_=system_clock::now();
   }
   auto secs()  const noexcept {
      return duration_cast<seconds>(system_clock::now()-start_).count();
   }
};

string serial_version()
{
/*
   auto a   = create("foo ");
   auto b   = create("bar ");
   auto c   = concat(a, b);
   auto d   = twice(c);
   auto e   = create("this ");
   auto f   = create("that ");
   auto g   = concat(e, f);
   auto res = concat(d, g);
*/
   const auto res = 
      concat(
         twice(
            concat(
               create("foo "),
               create("bar ")
            )
         ),
         concat(
            create("this "),
            create("that ")
         )
      );
   return res;
}

string parallelized_version()
{
   using namespace pdag;

   auto pcreate = asynchronize(create); 
   auto pconcat = async_adapter(concat); 
   auto ptwice  = async_adapter(twice); 

   const auto res = 
      pconcat(
         ptwice(
            pconcat(
               pcreate("foo "),
               pcreate("bar ")
            )
         ),
         pconcat(
            pcreate("this "),
            pcreate("that ")
         )
      );
   return res().get();
}

int main()
{
   stopwatch st;
   cout << serial_version() << endl
        << "*** time elapsed: " << st.secs() << " seconds" << endl;

   st.start();
   cout << parallelized_version() << endl
        << "*** time elapsed: " << st.secs() << " seconds" << endl;
}
