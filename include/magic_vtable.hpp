#pragma once

#ifndef MAGIC_VTABLE_LIB
#define MAGIC_VTABLE_LIB

#if defined(_MSC_VER)
	// Prior to Clang 19.1.0, mangling of functions with NTTP member pointers was incompatible with MSVC 19.20+:
	// https://releases.llvm.org/19.1.0/tools/clang/docs/ReleaseNotes.html#abi-changes-in-this-version
	#if (_MSC_VER < 1920) || (defined(__clang__) && __clang_major__ < 19)
		#define MAGIC_VTABLE_PREFIX std::string_view{"@@$"}
		#define MAGIC_VTABLE_SUFFIX std::string_view{"@@"}
	#else
		#define MAGIC_VTABLE_PREFIX std::string_view{"1@$"}
		#define MAGIC_VTABLE_SUFFIX std::string_view{"@@"}
	#endif
#else
	#error "Unsupported compiler"
#endif

#if defined(__cpp_consteval)
	#define MAGIC_VTABLE_CONSTEVAL consteval
#elif defined(__cpp_constexpr) && __cpp_constexpr >= 201304L
	#define MAGIC_VTABLE_CONSTEVAL constexpr
#else
	#define MAGIC_VTABLE_CONSTEVAL inline
#endif

#include <stdexcept>
#include <string_view>
#include <utility>

namespace magic_vft
{
	namespace detail
	{
		MAGIC_VTABLE_CONSTEVAL uint8_t parse_alphabet_encoded_hex(const char ch)
		{
			if (ch < 'A' || ch >= 'A' + 16)
			{
				throw std::invalid_argument{"invalid hex digit"};
			}
			return static_cast<uint8_t>(ch - 'A');
		}

		MAGIC_VTABLE_CONSTEVAL size_t decode_microsoft_value(std::string_view str)
		{
			using namespace std::literals::string_view_literals;

			// weird cases that i'm not sure how to handle
			if (str.substr(0, 3) == "B3A"sv)
				return 4;
			if (str.substr(0, 3) == "B7A"sv)
				return 8;

			// Skip the leading 'B'
			if (str.front() != 'B')
			{
				throw std::invalid_argument{"unexpected character"};
			}
			str.remove_prefix(1);

			size_t value{};
			while (!str.empty() && str.front() != '@')
			{
				value *= 16;
				value += parse_alphabet_encoded_hex(str.front());
				str.remove_prefix(1);
			}
			return value;
		}
	}

	template<auto>
	MAGIC_VTABLE_CONSTEVAL size_t vtable_index()
	{
		constexpr std::string_view mangled{__FUNCDNAME__};
		constexpr auto first = mangled.find(MAGIC_VTABLE_PREFIX) + MAGIC_VTABLE_PREFIX.size();
		constexpr auto last = mangled.find(MAGIC_VTABLE_SUFFIX, first);
		constexpr auto value = detail::decode_microsoft_value(mangled.substr(first, last - first));
		return value / sizeof(size_t);
	}
}

#endif // MAGIC_VTABLE_LIB
