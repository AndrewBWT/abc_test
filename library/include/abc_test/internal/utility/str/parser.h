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
	using parse_result_t = std::expected<T, std::string>;
	template<
		typename T
	>
	using parser_fp_t = parse_result_t<T>(*)(const std::string_view);
	template<
		typename T
	>
	using parser_fo_t = std::function<parse_result_t<T>(const std::string_view)>;
	template<
		typename T
	>
	struct parser_t
	{
	public:
		__constexpr
			parser_t(
			) noexcept;
		__constexpr
			parser_t(
				const parser_fp_t<T> _a_func
			);
		__constexpr
			parser_t(
				const parser_fo_t<T> _a_func
			);
		__constexpr
			parse_result_t<T>
			run_parser(
				const std::string_view _a_str
			) const noexcept;
		__constexpr
			T
			run_parser_with_exception(
				const std::string_view _a_str
			) const;
	private:
		using inner_variant = std::variant<parser_fp_t<T>, parser_fo_t<T>>;
		__constexpr
			parser_t(
				const inner_variant& _a_func
			);
		inner_variant _m_func;
	};
	namespace
	{
		template<
			typename T
		>
		__constexpr
			parser_fo_t<T>
			default_parser(
			) noexcept;
	}
	_END_ABC_UTILITY_STR_NS

_BEGIN_ABC_UTILITY_STR_NS
	template<
		typename T
	>
	__constexpr_imp
		parser_t<T>::parser_t(
		) noexcept
		: parser_t<T>(default_parser<T>())
	{

	}
	template<
		typename T
	>
	__constexpr_imp
		parser_t<T>::parser_t(
			const parser_fp_t<T> _a_func
		)
		: parser_t<T>(inner_variant(_a_func))
	{

	}
	template<
		typename T
	>
	__constexpr_imp
		parser_t<T>::parser_t(
			const parser_fo_t<T> _a_func
		)
		: parser_t<T>(inner_variant(_a_func))
	{

	}
	template<
		typename T
	>
	__constexpr_imp
		parse_result_t<T>
		parser_t<T>::run_parser(
			const std::string_view _a_str
		) const noexcept
	{
		using namespace std;
		if (holds_alternative<parser_fp_t<T>>(_m_func))
		{
			parser_fp_t<T> _l_func{ get<parser_fp_t<T>>(_m_func) };
			return _l_func(_a_str);
		}
		else if (holds_alternative<parser_fo_t<T>>(_m_func))
		{
			parser_fo_t<T> _l_func{ get<parser_fo_t<T>>(_m_func) };
			return _l_func(_a_str);
		}
		else
		{
			return unexpected("Error: Malformed run_parser element");
		}
	}
	template<
		typename T
	>
	__constexpr_imp
		T
		parser_t<T>::run_parser_with_exception(
			const std::string_view _a_str
		) const
	{
		using namespace std;
		using namespace errors;
		parse_result_t<T> _l_res{};
		if (holds_alternative<parser_fp_t<T>>(_m_func))
		{
			parser_fp_t<T> _l_func{ get<parser_fp_t<T>>(_m_func) };
			_l_res = _l_func(_a_str);
		}
		else if (holds_alternative<parser_fo_t<T>>(_m_func))
		{
			parser_fo_t<T> _l_func{ get<parser_fo_t<T>>(_m_func) };
			_l_res = _l_func(_a_str);
		}
		else
		{
			_l_res = unexpected("Malformed run_parser element");
		}
		if (_l_res.has_value())
		{
			return _l_res.value();
		}
		else
		{
			throw test_library_exception_t(_l_res.error());
		}
	}
	template<
		typename T
	>
	__constexpr_imp
		parser_t<T>::parser_t(
			const inner_variant& _a_func
		)
		: _m_func(_a_func)
	{
		using namespace std;
		using namespace errors;
		if (holds_alternative<parser_fp_t<T>>(_m_func) &&
			get<parser_fp_t<T>>(_m_func) == nullptr)
		{
			throw test_library_exception_t(
				"Could not initialise parser_t as argument is a function pointer which equals nullptr");
		}
		else if (holds_alternative<parser_fo_t<T>>(_m_func) &&
			get<parser_fo_t<T>>(_m_func) == nullptr)
		{
			throw test_library_exception_t(
				"Could not initialise parser_t as argument is a std::function object which was default initialised");
		}
	}
	namespace
	{
		template<
			typename T
		>
		__constexpr_imp
			parser_fo_t<T>
			default_parser(
			) noexcept
		{
			using namespace std;
			return parser_fo_t<T>{[](const std::string_view _a_str)
				{
					if (auto _l_res = scn::scan<T>(_a_str, "{}"))
					{
						return expected<T, string>(_l_res->value());
					}
					else
					{
						return expected<T, string>(unexpected(
							fmt::format(
								"Could not parse string \"{0}\" using default scn::scan function for type {1}.",
								_a_str, typeid(T).name())
						));
					}
				}
			};
		}
	}
	_END_ABC_UTILITY_STR_NS