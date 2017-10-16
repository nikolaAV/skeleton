#include <iostream>
#include <utility>
#include <cassert>

/**
   Original idea:
   https://akrzemi1.wordpress.com/2017/06/28/compile-time-string-concatenation/
*/

using namespace std;

template <size_t N>
class string_literal
{
   const char* first_;

public:
   constexpr string_literal(const char (&l)[N+1])
      : first_(l)
   {
      assert(first_[N]=='\0' && "Non-Zero terminating literal");
   } 

   constexpr char operator[](int Index) const
   {
      assert(0<=Index && (size_t)Index<N && "Index: out of range");
      return first_[Index];
   }
   constexpr size_t size() const
   {
      return N;
   }
   constexpr const char* c_str() const
   {
      return first_;   
   }
   constexpr operator const char*() const
   {
      return c_str();
   }
};

template <int N_ZERO>
constexpr decltype(auto) literal(const char (&l)[N_ZERO])
{
   return string_literal<N_ZERO-1>(l);
}

template <size_t N>
class string_holder
{
   char arr_[N+1];

   template <typename ANY_STRING_LITERAL1, typename ANY_STRING_LITERAL2, std::size_t... I1, std::size_t... I2>
   constexpr string_holder(
             const ANY_STRING_LITERAL1& l1
            ,const ANY_STRING_LITERAL2& l2
            ,std::index_sequence<I1...>
            ,std::index_sequence<I2...>
      )
      : arr_ {l1[I1]...,l2[I2]...,'\0'}
   {}

public:
   template
      <   size_t N1
         ,size_t N2
         ,template <size_t> typename L1
         ,template <size_t> typename L2
         ,typename Indices1=std::make_index_sequence<N1>
         ,typename Indices2=std::make_index_sequence<N2>
      >
   constexpr 
   string_holder(const L1<N1>& l1,const L2<N2>& l2)
      :  string_holder{l1,l2,Indices1{},Indices2{}}
   {}

   constexpr char operator[](int Index) const
   {
      assert(0<=Index && (size_t)Index<N && "Index: out of range");
      return arr_[Index];
   }
   constexpr size_t size() const
   {
      return N;
   }
   constexpr const char* c_str() const
   {
      return &arr_[0];   
   }
   constexpr operator const char*() const
   {
      return c_str();
   }
};

template <size_t N1, size_t N2>
constexpr decltype(auto) operator+(const string_literal<N1>& l1, const string_literal<N2>& l2)
{
   return string_holder<N1+N2>(l1,l2);
}

template <size_t N1, size_t N2>
constexpr decltype(auto) operator+(const char(&l1)[N1], const string_literal<N2>& l2)
{
   return string_holder<N1+N2-1>(literal(l1),l2);
}

template <size_t N1, size_t N2>
constexpr decltype(auto) operator+(const string_literal<N1>& l1, const char(&l2)[N2])
{
   return string_holder<N1+N2-1>(l1,literal(l2));
}

template <size_t N1, size_t N2>
constexpr decltype(auto) operator+(const string_holder<N1>& h1, const string_holder<N2>& h2)
{
   return string_holder<N1+N2>(h1,h2);
}

template <size_t N1, size_t N2>
constexpr decltype(auto) operator+(const char(&l1)[N1], const string_holder<N2>& h)
{
   return string_holder<N1+N2-1>(literal(l1),h);
}

template <size_t N1, size_t N2>
constexpr decltype(auto) operator+(const string_holder<N1>& h, const char(&l2)[N2])
{
   return string_holder<N1+N2-1>(h,literal(l2));
}

template <size_t N1, size_t N2>
constexpr decltype(auto) operator+(const string_holder<N1>& h, const string_literal<N2>& l2)
{
   return string_holder<N1+N2>(h,l2);
}

template <size_t N1, size_t N2>
constexpr decltype(auto) operator+(const string_literal<N1>& l1, const string_holder<N2>& h)
{
   return string_holder<N1+N2>(l1,h);
}

int main()
{
   constexpr auto std      = literal("std::");
   constexpr auto out      = literal(" << ");
   constexpr auto quote    = literal("\"");
   constexpr auto phrase   = literal("Hello") + ", " + "World" + "!";
   constexpr auto phrase2  = quote+phrase+quote;
   constexpr auto expr     = std+"cout" +out+ phrase2 +out+ std+"endl";

   // std::cout << "Hello, World!" << std::endl
   // 012       ^         ^         ^         ^
   static_assert(41==expr.size());
   cout << expr << endl;

   cin.get();
} 

        