/*
   g++ main.cpp -std=c++14 -o exe -g
   g++ main.cpp -std=c++14 -Wextra -Wall -pedantic-errors -o exe
   g++ main.cpp -std=c++17 -Wextra -Wall -pedantic-errors -o exe
   core dump file --> /var/lib/apport/coredump (before, $ ulimit -c unlimited)
*/

#include "pipe.h"

#include <string>
#include <iostream>

enum class EducationLevel
{
    none
   ,primary
   ,secondary
   ,bachelor
   ,master
   ,doctor
};

enum class Gender
{
    male
   ,female
};

struct Person
{
   std::string _name;
   Gender _gender;
   std::size_t _age;
   EducationLevel _education;
};   

bool is_female(Person const& p) noexcept
{
   return p._gender == Gender::female;
}

std::string to_name(Person const& p)
{
   return p._name;
}

std::string concatenate(std::string const& name, std::string init)
{
   return init + ", " + name;
}

int main()
{
//   std::vector<Person> input = {
   Person input[] = {
       {"Olivia", Gender::female,  30, EducationLevel::primary}
      ,{"Jonh",   Gender::male,    25, EducationLevel::bachelor}
      ,{"Emma",   Gender::female,  16, EducationLevel::secondary}
      ,{"Alex",   Gender::male,    20, EducationLevel::master}
      ,{"Oliver", Gender::male,    22, EducationLevel::master}
      ,{"William",Gender::male,    60, EducationLevel::none}
      ,{"Anna",   Gender::female,  18, EducationLevel::secondary}
      ,{"Janett", Gender::female,  33, EducationLevel::doctor}
      ,{"Henry",  Gender::male,    43, EducationLevel::doctor}
   };

   using namespace pipe;
   auto resrult = input | filter(is_female) | transform(to_name) | fold(concatenate, std::string(""));
   std::cout << resrult << std::endl;
}

