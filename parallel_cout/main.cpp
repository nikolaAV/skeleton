#include <sstream>
#include <iostream>
#include <mutex>

namespace parallel
{
   class cout : public std::stringstream {
      inline static std::mutex m_;
   public:
      ~cout() {
         std::lock_guard<std::mutex> lock{m_};
         std::cout << rdbuf();
      }
   };
}  // end namespace parallel

////// Example of Usage //////

#include <thread>
#include <vector>

using namespace std;

void print_cout(string s, size_t n) {
   cout << "[" << n << "]: " << s << endl;
}

void print_pcout(string s, size_t n) {
   parallel::cout{} << "[" << n << "]: " << s << endl;
}

template <typename F>
void run10threads(F f, string greeting)
{
   vector<thread> v;
   for(size_t i{0}; i<10; ++i)
      v.emplace_back(f,greeting,i);
   for(auto& t:v) t.join();
}

int main()
{
   run10threads(print_cout, "Hello std::cout");
   run10threads(print_pcout,"Hello parallel::cout");
}

