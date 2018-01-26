# 'weak_this' idiom
The technique gives you a guarantee that a target object still exists or has been already destroyed when an asynchronous callback is invoked.
The trick is to bind a `weak_ptr` into asynchronous trigger which invokes the callback, using lambda captures.
__Note:__ A pending asynchronous trigger will __not__ prolong the lifetime of the target object, yet is still safe if the target object is deleted.
```cpp
class worker : public std::enable_shared_from_this<worker>
{
public:
   void do_update()
   {
      // ... some computational burden
   }
   void async_update()
   {
	/**
		gives a task execution (do_update) away for other thread
		and ... and forgets about it
	*/
   thread{[wp = weak_from_this()]() {  // <-- std::weak_ptr is captured here
            if(auto sp=wp.lock())
               sp->do_update();  // <-- the guarantee the instance of worker is not destroyed yet 
         }
      }.detach();
   }
}; 

int main()
{
	auto sp = std::make_shared<worker>();
	sp->async_update(); // will it have had time to execute?
}
```

## Further informations
* [When is std::weak_ptr useful?](https://stackoverflow.com/questions/12030650/when-is-stdweak-ptr-useful) on stackoverflow
* [auto self(shared_from_this())](http://www.boost.org/doc/libs/1_54_0/doc/html/boost_asio/example/cpp11/http/server/connection.cpp) from boost.asio
* [BBB](https://www.youtube.com/watch?v=QhwPSRiPmUA&index=3&list=PLZN9ZGiWZoZoOoswTzuz8nqDwuxXwuThV) by AAA (start from the frame 14:33)

## Related links
* TBD

## Compilers
* [GCC 7.2.0](https://wandbox.org/)
* [clang 5.0.0](https://wandbox.org/)
* Visual C++ 19.12 
