#pragma once
#include <pnp/core.hpp>
#include "abc_test/utility/internal/macros.hpp"
#include "abc_test/utility/str/string_utils.hpp"

_BEGIN_ABC_UTILITY_NS
template <typename Inner_Pnp>
struct hex_pnp
{
    Inner_Pnp inner{};

    template <typename T>
        requires pnp::printer_c<Inner_Pnp, T>
    constexpr void
        run_printer(const T& value_arg, pnp::text_sink& sink_arg) const
    {
        sink_arg.write(
            abc::utility::str::to_hex(pnp::to_u8string(inner, value_arg))
        );
    }

    template <typename T>
        requires pnp::parser_c<Inner_Pnp, T>
    constexpr pnp::basic_parser_return_value
        run_parser(
            pnp::parser_input& input_arg,
            T& value_arg,
            [[maybe_unused]] pnp::parser_run_key run_key_arg
        ) const
    {
        using namespace std;
        const size_t start{ input_arg.position() };
        u8string     hex_text;
        {
            const pnp::input_text text{ input_arg.peek_while(
                [](const char8_t c)
                {
                    return (c >= u8'0' && c <= u8'9')
                           || (c >= u8'a' && c <= u8'f')
                           || (c >= u8'A' && c <= u8'F');
                }
            ) };
            hex_text = text.get();
        }
        input_arg.advance(hex_text.size());

        const result_t<u8string> decoded{ abc::utility::str::from_hex(hex_text) };
        if (not decoded.has_value())
        {
            return input_arg.fail(
                u8"could not decode hex text: " + decoded.error(), start
            );
        }
        pnp::parser_input inner_input(
            unicode_bridge::cast_string_view<char>(decoded.value())
        );
        if (const auto result{ inner_input.run_parser(inner, value_arg) };
            result.failed())
        {
            return input_arg.fail(
                u8"hex-decoded text could not be parsed", start
            );
        }
        if (not inner_input.at_end())
        {
            return input_arg.fail(
                u8"hex-decoded text had unparsed text after the value", start
            );
        }
        return {};
    }
};

template <bool Hex, typename Pnp>
using maybe_hex_pnp_t = std::conditional_t<Hex, hex_pnp<Pnp>, Pnp>;
_END_ABC_UTILITY_NS