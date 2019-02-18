# The passkey idiom
The `friend` keyword in C++ has always been clouded by controversy, often being accused of breaking encapsulation.
"The passkey idiom" allows you to limit the scope of the friend declaration.
Let's take the example of a person class that has some public and private data.
```cpp
class person {
   string name_;
   string favotite_food_;
   string favotite_drink_;
   string social_security_no_;

public:
   string name() const;
private:
   string favotite_food() const;
   string favotite_drink() const;
   string social_security_no() const;
...
```
Let's now imagine a government that needs to know a person's social security number
```cpp
void government::info(const person& p) const {
      cout << p.name() 
           << p.social_security_no()  // Line A
           << endl;
   }
```
and the playmate who knows everything about the person except for the social security number:
```cpp
void playmate::info(const person& p) const {
      cout << p.name() 
           << p.favotite_food()     // Line B1
           << p.favotite_drink()    // Line B2
           << endl;
   }
```
The compiler will raise an error at _Lines A-B_ because of access denied to person's private data.
What can we do to give the government and the playmate access without making the person's information public?
A successful but heavy-handed approach would be to make them friends of the person class.
```cpp
class person {
   friend government;
   friend playmate;
...
```
But, on the contrary, that is not what we have strived to achieve.
Now the playmate can read person's social security number and the government is aware of person's favourites as well.  
__The passkey idiom__ can provide separate access scope for each particular consumer such as a government or a playmate.  
To do that the following changes are required: 
* `private` function-members are to become `public`.
* each function-members should accept extra parameter: _the passkey_.
```cpp
class person {
public:
   string name() const;
   string favotite_food(passkey<playmate>) const;
   string favotite_drink(passkey<playmate>) const;
   string social_security_no(passkey<government>) const;
...
```
* invocation syntax at _lines A-B_ requires the empty-aggregate initialization for an input argument. 
```cpp
void government::info(const person& p) const {
      cout << p.name() 
           << p.social_security_no({})  // Line A
           << endl;
   }
void playmate::info(const person& p) const {
      cout << p.name() 
           << p.favotite_food({})     // Line B1
           << p.favotite_drink({})    // Line B2
           << endl;
   }
```
where
```cpp
template <typename T>
class passkey {
   friend T;
   passkey()                           = default;
   passkey(const passkey&)             = default;
   passkey& operator=(const passkey&)  = delete;
};
```

## Further informations
* [The passkey idiom](https://arne-mertz.de/2016/10/passkey-idiom/) by Arne Mertz
* [Clean C++ granular friend equivalent?](https://stackoverflow.com/questions/3217390/clean-c-granular-friend-equivalent-answer-attorney-client-idiom/3217430#3217430) on stackoverflow. 

## Related links
* TBD

## Compilers
* [GCC 8.1.0](https://wandbox.org/)
* [clang 7.0.0](https://wandbox.org/)
* Microsoft (R) C/C++ Compiler 19.16 
