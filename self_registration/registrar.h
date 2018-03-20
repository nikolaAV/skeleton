#ifndef _REGISTRAR_INCLUDED_
#define _REGISTRAR_INCLUDED_

/**
   Original idea: "Factory With Self-Registering Types"
   http://www.bfilipek.com/2018/02/factory-selfregister.html
*/

#include <string_view>
#include <memory>
#include <map>
#include <string>
#include <cassert>

template <typename inteface_type>
struct registrar
{
   using interface_ptr  = std::unique_ptr<inteface_type>;
   using factory_method = interface_ptr(*)();  

   static bool          reg(std::string_view,factory_method);
   static interface_ptr get(std::string_view);

private:
   using factory_map_type = std::map<std::string,factory_method>;
   static factory_map_type& factory();
   ~registrar() = delete;  // no exemplar of the registrar is required
};

// concept registrable
// {
//      static const char*    name();
//      static factory_method create();
// };

template <typename interface_type, typename registrable>
struct auto_enrolment : interface_type
{
protected:   
   static bool registered_;   
   /**
      'bool registered_' is a data member of class template
      If the variable is not used anywhere the compiler can remove it
      and ... do_reg will be never called. 
      https://stackoverflow.com/questions/19630570/what-does-it-mean-to-odr-use-something
   */
   ~auto_enrolment() { (void)registered_; } // <-- just to prevent the optimization

   using interface_ptr  = typename registrar<interface_type>::interface_ptr;
   using factory_method = typename registrar<interface_type>::factory_method;
};

template <typename interface_type, typename registrable>
bool auto_enrolment<interface_type,registrable>::registered_ = registrar<interface_type>::reg(registrable::name(),registrable::create); 

template <typename inteface_type>
typename registrar<inteface_type>::factory_map_type& 
registrar<inteface_type>::factory()
{
   static factory_map_type the_instance;
   return the_instance;
}

template <typename inteface_type>
bool 
registrar<inteface_type>::reg(std::string_view name, factory_method fm)
{
   auto it = factory().insert(std::make_pair(name.data(),fm));
   assert(it.second && "[reg]: duplication is not allowed");   
   return it.second;
}

template <typename inteface_type>
typename registrar<inteface_type>::interface_ptr 
registrar<inteface_type>::get(std::string_view name)
{
   auto it = factory().find(name.data());
   return it!=factory().end()? it->second() : interface_ptr{};
} 

#endif // _REGISTRAR_INCLUDED_

