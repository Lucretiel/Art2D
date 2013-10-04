#pragma once

#include <string>
#include <memory>
#include <unordered_map>

template<class Base>
class Registry
{
private:
	typedef Base* (*BaseMaker)();

	typedef std::unordered_map<std::string, BaseMaker> RegistryTable;

	RegistryTable registry;

public:
	template<class Derived>
	static Base* make()
	{
		return new Derived;
	}

	typedef std::unique_ptr<Base> unique_baseptr;

	template<class Derived>
	void addEntry(const std::string& name)
	{
		registry[name] = BaseMaker(&Registry<Base>::make<Derived>);
	}

	void removeEntry(const std::string& name)
	{
		registry.erase(name);
	}

	unique_baseptr make(const std::string& name)
	{
		auto found_entry = registry.find(name);
		return unique_baseptr(found_entry != registry.end() ? found_entry->second() : nullptr);
	}
};

template<class Base>
class GlobalRegistry
{
private:
	typedef Registry<Base> RegistryType;
	static Registry<Base>& get_registry()
	{
		static Registry<Base> registry;
		return registry;
	}

public:
	typedef typename RegistryType::unique_baseptr unique_baseptr;

	//You know, boost probably has some witchcraft that can do all this statically.
	template<class Derived>
	static int registerType(const std::string& name)
	{
		get_registry().addEntry<Derived>(name);
		return 0;
	}

	static unique_baseptr make(const std::string& name)
	{
		return get_registry().make(name);
	}

};

#define REGISTER_TYPE(BASE, DERIVED, NAME) const static int reg(GlobalRegistry<BASE>::registerType<DERIVED>(NAME))
#define MAKE_TYPE(BASE, NAME) GlobalRegistry<BASE>::make(NAME)
/*
 *  Example Use:
 *
 *	    //Derrived.cpp:
 *
 *	    #include "TypeRegistry.h"
 *	    REGISTER_TYPE(Base, Derived, "derived")
 *
 *		//SomewhereElse.cpp:
 *		#include "TypeRegistry.h"
 *		auto derived = MAKE_TYPE(Base, "derived") //Pointer to a new Derived object.
 *
 */