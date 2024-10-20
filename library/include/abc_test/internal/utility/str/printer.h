#pragma once
#include <string>
#include <fmt/format.h>
#include <vector>
#include <expected>
#include <functional>
#include "abc_test/internal/utility/internal/macros.h"
#include <variant>
#include <scn/scan.h>
#include "abc_test/internal/errors/test_library_exception.h"

_BEGIN_ABC_UTILITY_STR_NS
	template<
		typename T
	>
	using printer_fp_t = std::string(*)(const T&);
	template<
		typename T
	>
	using printer_fo_t = std::function<std::string(const T&)>;
	template<
		typename T
	>
	struct printer_t
	{
	public:
		__constexpr
			printer_t(
			) noexcept;
		__constexpr
			printer_t(
				const printer_fp_t<T> _a_func
			) noexcept;
		__constexpr
			printer_t(
				const printer_fo_t<T> _a_func
			) noexcept;
		__constexpr
			std::string
			run_printer(
				const T& _a_str
			) const noexcept;
	private:
		using inner_variant = std::variant<printer_fp_t<T>, printer_fo_t<T>>;
		__constexpr
			printer_t(
				const inner_variant& _a_func
			) noexcept;
		inner_variant _m_func;
	};
	namespace
	{
		template<
			typename T
		>
		__constexpr
			printer_fo_t<T>
			default_printer(
			) noexcept;
	}
	_END_ABC_UTILITY_STR_NS

_BEGIN_ABC_UTILITY_STR_NS
	template<
		typename T
	>
	__constexpr_imp
		printer_t<T>::printer_t(
		) noexcept
		: printer_t<T>(default_printer<T>())
	{

	}
	template<
		typename T
	>
	__constexpr_imp
		printer_t<T>::printer_t(
			const printer_fp_t<T> _a_func
		) noexcept
		: printer_t<T>(inner_variant(_a_func))
	{

	}
	template<
		typename T
	>
	__constexpr_imp
		printer_t<T>::printer_t(
			const printer_fo_t<T> _a_func
		) noexcept
		: printer_t<T>(inner_variant(_a_func))
	{

	}
	template<
		typename T
	>
	__constexpr_imp
		std::string
		printer_t<T>::run_printer(
			const T& _a_obj
		) const noexcept
	{
		using namespace std;
		if (holds_alternative<printer_fp_t<T>>(_m_func))
		{
			printer_fp_t<T> _l_func{ get<printer_fp_t<T>>(_m_func) };
			return _l_func(_a_obj);
		}
		else if (holds_alternative<printer_fo_t<T>>(_m_func))
		{
			printer_fo_t<T> _l_func{ get<printer_fo_t<T>>(_m_func) };
			return _l_func(_a_obj);
		}
		else
		{
			return "Malformed run_parser element";
		}
	}
	template<
		typename T
	>
	__constexpr_imp
		printer_t<T>::printer_t(
			const inner_variant& _a_func
		) noexcept
		: _m_func(_a_func)
	{

	}
	namespace
	{
		template<
			typename T
		>
		__constexpr_imp
			printer_fo_t<T>
			default_printer(
			) noexcept
		{
			return printer_fo_t<T>([](const T& _a_obj)
				{
					return fmt::format("{}", _a_obj);
				});
		}
	}
	_END_ABC_UTILITY_STR_NS