#pragma once
#include "abc_test/utility/parsers/parser_input.hpp"
#include "abc_test/utility/types.hpp"
#include <memory>


_BEGIN_ABC_UTILITY_PARSER_NS

template <typename T>
struct parser_base_t
{
public:
    using value_type_t                   = T;
    __constexpr virtual ~parser_base_t() = default;
    __constexpr virtual result_t<T>
        run_parser(parser_input_t& _a_parse_input) const = 0;
};
template <typename T>
struct default_parser_t;
template <typename T>
using parser_t = std::shared_ptr<parser_base_t<T>>;
template <typename T>
__constexpr parser_t<typename T::value_type_t> mk_parser(T) noexcept;
template <typename T>
__constexpr_imp result_t<T>
                parse(
                    const std::u8string_view _a_str,
                    const parser_t<T>&       _a_parser = mk_parser(default_parser_t<T>())
                ) noexcept;
template <typename T>
__constexpr_imp T
    parse_with_exception(
        const std::u8string_view            _a_str,
        const utility::parser::parser_t<T>& _a_parser
        = mk_parser(default_parser_t<T>())
    );

template <typename T>
__constexpr parser_t<typename T::value_type_t>
            mk_parser(
                T _a_parser_base
            ) noexcept
{
    using namespace std;
    return make_shared<T>(_a_parser_base);
}

_END_ABC_UTILITY_PARSER_NS

_BEGIN_ABC_UTILITY_PARSER_NS
template <typename T>
__constexpr_imp result_t<T>
                parse(
                    const std::u8string_view            _a_str,
                    const utility::parser::parser_t<T>& _a_parser
                ) noexcept
{
    using namespace std;
    using namespace utility::parser;
    using namespace _ABC_NS_UTILITY_STR;
    parser_input_t _l_pit(_a_str);
    try
    {
        const result_t<T> _l_inner_parser_result{_a_parser->run_parser(_l_pit)};
        if (_l_inner_parser_result.has_value())
        {
            if (not _l_pit.at_end())
            {
                return result_t<T>{
                    unexpected(u8"Parser okay but not at end of string")
                };
            }
        }
        return _l_inner_parser_result;
    }
    catch (const parser_could_not_match_string_t& _a_exception)
    {
        return unexpected(fmt::format(
            u8"Parser threw unexpected exception: \"{0}\"",
            cast_string_to_u8string(_a_exception.what())
        ));
    }
}

template <typename T>
__constexpr_imp T
    parse_with_exception(
        const std::u8string_view            _a_str,
        const utility::parser::parser_t<T>& _a_parser
    )
{
    using namespace std;
    using namespace _ABC_NS_ERRORS;
    using namespace utility::parser;
    const result_t<T> _l_parse_result{parse(_a_str, _a_parser)};
    if (_l_parse_result.has_value())
    {
        return _l_parse_result.value();
    }
    else
    {
        throw abc_test_exception_t({_l_parse_result.error()}, false);
    }
}

_END_ABC_UTILITY_PARSER_NS