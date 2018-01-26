#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <map>
#include <mutex>
#include <memory>

using namespace std;
using namespace std::chrono_literals;

using id2id_map = map<size_t,thread::id>;

class destination : private id2id_map
{
   mutable mutex  mtx_{};
public:
   void insert(size_t v)   {lock_guard<mutex> l{mtx_}; id2id_map::insert(make_pair(v,this_thread::get_id())); }      
   auto size() const       {lock_guard<mutex> l{mtx_}; return id2id_map::size(); }      

   using id2id_map::begin;
   using id2id_map::end;
};

class worker : public enable_shared_from_this<worker>
{
   static size_t  counter_;
   const size_t   id_;     // worker unique identifier
   destination&   out_;    // place where a result of work should be put into

public:
   explicit worker(destination& out) : id_(counter_++), out_(out) {}

   void do_update()
   {
      this_thread::sleep_for(5ms);  // <-- just to simulate a computational burden
      out_.insert(id_); 
   }

   void async_update()
   {
      thread{[wp = weak_from_this()]() { // <-- gives a task execution away for other thread
            if(auto sp=wp.lock())        // the guarantee the instance of worker is not destroyed yet
               sp->do_update();   
         }
      }.detach();                        // <-- and forgets about it
   }
}; 

size_t worker::counter_{0};

int main()
{
   destination        result;
   {
      vector<shared_ptr<worker>> workers;
      for(size_t i=0; i<100; ++i)
         workers.push_back(make_shared<worker>(result));
      cout << "total workers: " << workers.size() << endl;

      // starting of the asynchronous (!!!) work ...
      for(auto&& w:workers) w->async_update();

      cout << "done before of the block: " << result.size() << endl;
   }  // <-- all workers are being destroyed here ...

   cout << "done after of the block: " << result.size() << endl;
   for(auto&& rec:result) 
      cout << rec.first << " -> "<< rec.second << endl;

   this_thread::sleep_for(10ms);  // <-- just to make sure all threads have been completed
   cout << "done at the end of main: " << result.size() << endl;
}

