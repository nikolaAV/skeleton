// g++ main.cpp -std=c++14 -o exe -g
// g++ main.cpp -std=c++14 -Wextra -Wall -pedantic-errors -o exe
// g++ main.cpp -std=c++17 -Wextra -Wall -pedantic-errors -o exe
// g++ main.cpp -std=c++23 -Wextra -Wall -pedantic-errors -o exe
// core dump file --> /var/lib/apport/coredump (before, $ ulimit -c unlimited)
// g++ -fsanitize=address -g main.cpp -o exe
//
// valgrind --leak-check=full --show-reachable=yes ./exe
// valgrind --tool=massif ./exe  --time-unit=B
// ms_print massif.out.XXX
// massif-visualizer massif.out.XXX
//
// to change compile release, type 
// sudo update-alternatives --config gcc
//

#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

// ============================================================
// FRAMEWORK: strong type wrapper
// ============================================================
//
// Usage:
//   using Age    = StrongType<int,    struct AgeTag>;
//   using Weight = StrongType<double, struct WeightTag>;
//   using Name   = StrongType<std::string, struct NameTag>;
//
// Rules:
//   - Explicit construction only:   Age age{42};
//   - No implicit conversion between different strong types
//   - .value() to access underlying value
//   - Arithmetic (+,-,*,/) returns the same strong type
//   - Full comparison (==,!=,<,>,<=,>=)
//   - Stream output (operator<<)
// ============================================================

template<typename T, typename Tag>
class StrongType
{
public:
    constexpr explicit StrongType(T val)
        noexcept(std::is_nothrow_move_constructible<T>::value)
        : val_(std::move(val)) {}

    constexpr T const& value() const noexcept { return val_; }
    constexpr T&       value()       noexcept { return val_; }

    // --- comparison ---
    friend constexpr bool operator==(StrongType a, StrongType b) noexcept { return a.val_ == b.val_; }
    friend constexpr bool operator!=(StrongType a, StrongType b) noexcept { return a.val_ != b.val_; }
    friend constexpr bool operator< (StrongType a, StrongType b) noexcept { return a.val_ <  b.val_; }
    friend constexpr bool operator> (StrongType a, StrongType b) noexcept { return a.val_ >  b.val_; }
    friend constexpr bool operator<=(StrongType a, StrongType b) noexcept { return a.val_ <= b.val_; }
    friend constexpr bool operator>=(StrongType a, StrongType b) noexcept { return a.val_ >= b.val_; }

    // --- arithmetic (result is the same strong type) ---
    friend constexpr StrongType operator+(StrongType a, StrongType b) noexcept { return StrongType{a.val_ + b.val_}; }
    friend constexpr StrongType operator-(StrongType a, StrongType b) noexcept { return StrongType{a.val_ - b.val_}; }
    friend constexpr StrongType operator*(StrongType a, StrongType b) noexcept { return StrongType{a.val_ * b.val_}; }
    friend constexpr StrongType operator/(StrongType a, StrongType b) noexcept { return StrongType{a.val_ / b.val_}; }

    // --- stream output ---
    friend std::ostream& operator<<(std::ostream& os, const StrongType& a) { return os << a.val_; }

private:
    T val_;
};

// ============================================================
// CLIENT: define domain types (one line each)
// ============================================================

using Age    = StrongType<int,    struct AgeTag>;
using Weight = StrongType<double, struct WeightTag>;
using Name   = StrongType<std::string, struct NameTag>;

// ============================================================
// FRAMEWORK API: uses strong types — misuse caught at compile time
// ============================================================

void doCalculation(Age age, Weight weight, Name name)
{
    std::cout << "name="   << name
              << " age="   << age
              << " weight=" << weight << "\n";
}

// ============================================================
// DEMO
// ============================================================

int main()
{
    Age    age{30};
    Weight weight{75.5};
    Name   name{"Alice"};

    doCalculation(age, weight, name);       // OK

    // doCalculation(weight, age, name);    // Compile error: no match for 'Age' <- 'Weight'
    // doCalculation(age, age, name);       // Compile error: no match for 'Weight' <- 'Age'
    // Age wrong = weight;                  // Compile error: no implicit conversion

    // arithmetic — result is the same strong type
    Age  older  = age + Age{5};             // Age{35}
    bool isAdult = age >= Age{18};          // true

    // access underlying value when needed
    int  raw_age    = age.value();
    double raw_w    = weight.value();

    std::cout << "older="   << older   << "\n"   // 35
              << "isAdult=" << isAdult << "\n"   // 1
              << "raw_age=" << raw_age << "\n"   // 30
              << "raw_w="   << raw_w   << "\n";  // 75.5
}






