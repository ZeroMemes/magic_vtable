#include <magic_vtable.hpp>

#include <iostream>

struct TestClass
{
	virtual void a() = 0;
	virtual void b() = 0;
	virtual void c() = 0;

	struct Inner
	{
		virtual void a() = 0;
		virtual void b() = 0;
		virtual void c() = 0;
	};
};

namespace Test
{
	struct Inner
	{
		virtual void a() = 0;
		virtual void b() = 0;
		virtual void c() = 0;
	};
}

static_assert(magic_vft::vtable_index<&TestClass::a>() == 0);
static_assert(magic_vft::vtable_index<&TestClass::b>() == 1);
static_assert(magic_vft::vtable_index<&TestClass::c>() == 2);

static_assert(magic_vft::vtable_index<&TestClass::Inner::a>() == 0);
static_assert(magic_vft::vtable_index<&TestClass::Inner::b>() == 1);
static_assert(magic_vft::vtable_index<&TestClass::Inner::c>() == 2);

static_assert(magic_vft::vtable_index<&Test::Inner::a>() == 0);
static_assert(magic_vft::vtable_index<&Test::Inner::b>() == 1);
static_assert(magic_vft::vtable_index<&Test::Inner::c>() == 2);

int main()
{
	// Find the index at compile time
	constexpr auto index = magic_vft::vtable_index<&TestClass::c>();
	std::cout << index << std::endl;

	// Retrieve the index at runtime
	std::cout << magic_vft::vtable_index<&TestClass::b>() << std::endl;
}
