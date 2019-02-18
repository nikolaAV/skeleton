/**
   Friendship is the strongest coupling we can express in C++, even stronger than inheritance.
   So we’d better be careful and avoid it if possible. But sometimes we just don’t get around giving a class more access than another.
   Passkey Idiom is a technique which allow you make restricted friendship with one-method granularity.

   \see https://stackoverflow.com/questions/3217390/clean-c-granular-friend-equivalent-answer-attorney-client-idiom/3217430#3217430
   \see https://arne-mertz.de/2016/10/passkey-idiom/
*/

template <typename T>
class passkey {
   friend T;
   passkey()                           = default;
   passkey(const passkey&)             = default;
   passkey& operator=(const passkey&)  = delete;
};

////// Example of Usage //////

#include <iostream>
#include <string>

using namespace std;

struct government;
struct playmate;

class person {
   string name_;
   string favotite_food_;
   string favotite_drink_;
   string social_security_no_;

public:
   string name() const               { return name_; }
   string favotite_food(passkey<playmate>) const        { return favotite_food_; }
   string favotite_drink(passkey<playmate>) const       { return favotite_drink_; }
   string social_security_no(passkey<government>) const { return social_security_no_; }
};

struct government {
   static void info(const person& p) {
      cout << p.name() 
           << p.social_security_no({})
           << endl;
   }
};

struct playmate {
   static void info(const person& p) {
      cout << p.name() 
           << p.favotite_food({})
           << p.favotite_drink({})
           << endl;
   }
};

int main()
{
}

