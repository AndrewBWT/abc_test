#pragma once
#include "abc_test/internal/utility/internal/macros.hpp"

#include <scn/scan.h>
#include <string>

_BEGIN_ABC_UTILITY_NS

struct str_t
{
public:
    str_t()
    {}

    std::string _m_str;

    str_t(
        const char* val
    )
    {
        _m_str = std::string(val);
    }

    str_t(
        const std::string_view _a_str
    )
    {
        _m_str = std::string(_a_str);
    }
};

auto
    format_as(
        abc::utility::str_t _a_val
    )
{
    return _a_val._m_str;
}
constexpr bool
    operator==(
        const abc::utility::str_t& _a_lhs,
        const abc::utility::str_t& _a_rhs
    ) noexcept
{
    return _a_lhs._m_str == _a_rhs._m_str;
}
constexpr bool
operator!=(
    const abc::utility::str_t& _a_lhs,
    const abc::utility::str_t& _a_rhs
    ) noexcept
{
    return _a_lhs._m_str != _a_rhs._m_str;
}
_END_ABC_UTILITY_NS
template <>
struct scn::scanner<abc::utility::str_t>
{
    template <typename ParseContext>
    auto
        parse(
            ParseContext& pctx
        ) -> scan_expected<typename ParseContext::iterator>
    {
        return pctx.begin();
    }

    template <typename Context>
    auto
        scan(
            abc::utility::str_t& val,
            Context&             ctx
        ) const -> scan_expected<typename Context::iterator>
    {
        // constexpr std::string _l_str;
        auto _l_begin = ctx.begin();
        while (_l_begin != ctx.end() && *_l_begin != '"')
        {
            ++_l_begin;
        }
        if (_l_begin != ctx.end() && *_l_begin == '"')
        {
            ++_l_begin;
            auto _l_end = _l_begin;
            while (*_l_end != '"')
            {
                _l_end++;
            }
            if (_l_end == _l_begin)
            {
                val._m_str = "";
                _l_end++;
                return _l_end;
            }
            auto result = scn::scan<std::string>(
                scn::ranges::subrange(_l_begin, _l_end), "{}"
            );
            if (! result)
            {
                return unexpected(result.error());
            }
            val._m_str = result->value();
            auto x     = result->begin();
            ++x;
            return x;
        }
        else
        {
            auto result = scn::scan<std::string>(ctx.range(), "{}");
            val._m_str = result->value();
            return result->begin();
        }
    }
};