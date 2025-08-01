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

struct parse_error_t
{
    std::vector<std::u8string> errors;
    size_t                     index_of_failure;
};

template <typename T>
using parser_result_t = std::expected<T, parse_error_t>;

template <typename T>
__constexpr parser_t<typename T::value_type_t> mk_parser(T) noexcept;
template <typename T>
__constexpr_imp parser_result_t<T>
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
__constexpr_imp parser_result_t<T>
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
        auto              _l_unexpected_func
            = [&](const std::u8string& _a_str) -> parser_result_t<T>
        {
            return parser_result_t<T>(unexpected(parse_error_t{
                .errors = {_a_str}, .index_of_failure = _l_pit.index()
            }));
        };
        if (_l_inner_parser_result.has_value())
        {
            if (not _l_pit.at_end())
            {
                return _l_unexpected_func(
                    u8"Parser exited successfully, however it did not "
                    u8"finished parsing the input string."
                );
            }
            else
            {
                return _l_inner_parser_result.value();
            }
        }
        else
        {
            return _l_unexpected_func(_l_inner_parser_result.error());
        }
    }
    catch (const parser_could_not_match_string_t& _a_exception)
    {
        return unexpected(parse_error_t{
            .errors           = {fmt::format(
                u8"Parser threw unexpected exception: \"{0}\"",
                cast_string_to_u8string(_a_exception.what())
            )},
            .index_of_failure = _l_pit.index()
        });
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
    const parser_result_t<T> _l_parse_result{parse(_a_str, _a_parser)};
    if (_l_parse_result.has_value())
    {
        return _l_parse_result.value();
    }
    else
    {
        throw abc_test_exception_t(_l_parse_result.error().errors, false);
    }
}

_END_ABC_UTILITY_PARSER_NS