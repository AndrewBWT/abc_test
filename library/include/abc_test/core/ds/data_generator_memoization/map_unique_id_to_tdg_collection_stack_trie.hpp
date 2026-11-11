#pragma once
#include "abc_test/core/ds/data_generator_memoization/typeless_data_generator_collection_stack_trie.hpp"
#include "abc_test/utility/parsers/default_parser.hpp"
#include "abc_test/utility/printers/default_printer.hpp"
#include "abc_test/utility/types.hpp"

#include <cuchar>
#include <pnp/core.hpp>

_BEGIN_ABC_DS_NS
class map_unique_id_to_tdg_collection_stack_trie_t;
using parse_map_unique_id_to_tdg_collection_stack_trie_result_t
    = result_t<map_unique_id_to_tdg_collection_stack_trie_t>;
using key_t = std::u8string;

class map_unique_id_to_tdg_collection_stack_trie_t
{
public:
    __no_constexpr
        map_unique_id_to_tdg_collection_stack_trie_t() noexcept
        = default;
    __no_constexpr bool
        insert(const key_t& _a_id, const tdg_collection_stack_trie_t& _a_trie)
            noexcept;
    __no_constexpr const std::map<key_t, tdg_collection_stack_trie_t>&
                         map() const noexcept;
    friend __no_constexpr
        parse_map_unique_id_to_tdg_collection_stack_trie_result_t
        parse_compressed_map_of_unique_ids_to_tdg_collection_stack_tries(
            const std::u8string_view _a_str
        ) noexcept;
    template <bool To_Hex = true>
    friend __no_constexpr std::u8string
                          print_map_of_unique_ids_to_tdg_collection_stack_tries(
                              const map_unique_id_to_tdg_collection_stack_trie_t& _a_map
                          ) noexcept;
    friend __no_constexpr std::u8string
        print_compressed_map_of_unique_ids_to_tdg_collection_stack_tries(
            const map_unique_id_to_tdg_collection_stack_trie_t& _a_map
        ) noexcept;
    __no_constexpr std::size_t
                   size() const noexcept;
private:
    std::map<key_t, tdg_collection_stack_trie_t> _m_internal_map;
};

struct map_unique_id_to_tdg_collection_stack_trie_printer
{
    inline void
        run_printer(
            const map_unique_id_to_tdg_collection_stack_trie_t& object_arg,
            pnp::text_sink&                                     sink_arg
        ) const
    {}
};

template <bool Hex = true>
struct map_unique_id_to_tdg_collection_stack_trie_pnp
{
    inline void
        run_printer(
            const map_unique_id_to_tdg_collection_stack_trie_t& object_arg,
            pnp::text_sink&                                     sink_arg
        ) const
    {
        bool first{true};
        for (const auto& [key, trie] : object_arg.map())
        {
            if (not first)
            {
                sink_arg.write(u8":");
            }
            first = false;
            if constexpr (Hex)
            {
                sink_arg.write(abc::utility::str::to_hex(key));
            }
            else
            {
                sink_arg.write(key);
            }
            sink_arg.write(u8":");
            compressed_typless_data_generator_collection_stack_trie_pnp<
                Hex,
                true>{}
                .run_printer(trie, sink_arg);
        }
    }

    inline pnp::basic_parser_return_value
        run_parser(
            pnp::parser_input&                                     input_arg,
            abc::ds::map_unique_id_to_tdg_collection_stack_trie_t& object_arg,
            pnp::parser_run_key                                    run_key_arg
        ) const
    {
        using namespace std;
        const auto element_str = [](const size_t element_arg)
        {
            const string str{to_string(element_arg)};
            return u8string(str.begin(), str.end());
        };
        map_unique_id_to_tdg_collection_stack_trie_t result;
        // Empty text is an empty map.
        if (input_arg.at_end())
        {
            object_arg = std::move(result);
            return {};
        }
        size_t element{0};
        do
        {
            ++element;
            // The key. Ends at the first non-hex character (Hex), or at the
            // next
            // ':' (plain).
            const size_t key_start{input_arg.position()};
            u8string     key_text;
            {
                const pnp::input_text text{input_arg.peek_while(
                    [](const char8_t c)
                    {
                        if constexpr (Hex)
                        {
                            return (c >= u8'0' && c <= u8'9')
                                   || (c >= u8'a' && c <= u8'f')
                                   || (c >= u8'A' && c <= u8'F');
                        }
                        else
                        {
                            return c != u8':';
                        }
                    }
                )};
                key_text = text.get();
            }
            if (key_text.empty())
            {
                return input_arg.fail(
                    u8"Expected the unique ID of element "
                        + element_str(element) + u8", but found none.",
                    key_start
                );
            }
            input_arg.advance(key_text.size());
            u8string key;
            if constexpr (Hex)
            {
                result_t<u8string> decoded{abc::utility::str::from_hex(key_text)
                };
                if (not decoded.has_value())
                {
                    return input_arg.fail(
                        u8"Could not decode the hex unique ID of element "
                            + element_str(element) + u8": " + decoded.error(),
                        key_start
                    );
                }
                key = std::move(decoded.value());
            }
            else
            {
                key = std::move(key_text);
            }
            if (not input_arg.advance_if_starts_with(u8":"))
            {
                return input_arg.fail(
                    u8"Expected ':' after the unique ID of element "
                    + element_str(element) + u8"."
                );
            }
            // The trie. Its own parser knows where it ends, so a ':' inside it
            // (e.g. in a plain-text string) can't confuse the split.
            tdg_collection_stack_trie_t trie;
            if (auto parse_result{input_arg.run_parser(
                    compressed_typless_data_generator_collection_stack_trie_pnp<
                        Hex,
                        true>{},
                    trie
                )};
                parse_result.failed())
            {
                parse_result.add_msg(
                    u8"while parsing the trie of element "
                    + element_str(element)
                );
                return parse_result;
            }
            if (not result.insert(key, trie))
            {
                return input_arg.fail(
                    u8"Element " + element_str(element)
                        + u8" has the unique ID \"" + key
                        + u8"\", which an earlier element already has.",
                    key_start
                );
            }
        }
        while (input_arg.advance_if_starts_with(u8":"));
        // Only stored once everything has parsed.
        object_arg = std::move(result);
        return {};
    }
};
#if 0
_BEGIN_ABC_UTILITY_PARSER_NS

template <>
struct default_parser_t<abc::ds::map_unique_id_to_tdg_collection_stack_trie_t>
    : public parser_base_t<
          abc::ds::map_unique_id_to_tdg_collection_stack_trie_t>
{
    __no_constexpr virtual result_t<
        abc::ds::map_unique_id_to_tdg_collection_stack_trie_t>
        run_parser(parser_input_t& _a_parse_input) const;
    // using namespace abc::ds;
    // return parse_compressed_map_of_unique_ids_to_tdg_collection_stack_tries(
    //   _a_parse_input.sv()
    // );
};

_END_ABC_UTILITY_PARSER_NS
_BEGIN_ABC_UTILITY_PRINTER_NS
template <>
struct default_printer_t<abc::ds::map_unique_id_to_tdg_collection_stack_trie_t>
    : public printer_base_t<
          abc::ds::map_unique_id_to_tdg_collection_stack_trie_t>
{
    static constexpr bool is_specialized{true};

    __no_constexpr virtual std::u8string
        run_printer(const abc::ds::map_unique_id_to_tdg_collection_stack_trie_t&
                        _a_parse_input) const;
};
_END_ABC_UTILITY_PRINTER_NS
#endif
_END_ABC_DS_NS

template <>
struct fmt::formatter<_ABC_NS_DS::map_unique_id_to_tdg_collection_stack_trie_t>
    : formatter<string_view>
{
    // parse is inherited from formatter<string_view>.
    // Can'tbe constexpr due to use of fmt::format
    __no_constexpr auto
        format(
            _ABC_NS_DS::map_unique_id_to_tdg_collection_stack_trie_t _a_rtd,
            format_context&                                          _a_ctx
        ) const -> format_context::iterator;
};

_BEGIN_ABC_DS_NS
__no_constexpr_imp bool
    map_unique_id_to_tdg_collection_stack_trie_t::insert(
        const key_t&                       _a_id,
        const tdg_collection_stack_trie_t& _a_trie
    ) noexcept
{
    if (_m_internal_map.contains(_a_id))
    {
        return false;
    }
    else
    {
        _m_internal_map.insert({_a_id, _a_trie});
        return true;
    }
}

__no_constexpr_imp const std::map<key_t, tdg_collection_stack_trie_t>&
    map_unique_id_to_tdg_collection_stack_trie_t::map() const noexcept
{
    return _m_internal_map;
}

__no_constexpr_imp std::size_t
    map_unique_id_to_tdg_collection_stack_trie_t::size() const noexcept
{
    return _m_internal_map.size();
}

__no_constexpr_imp parse_map_unique_id_to_tdg_collection_stack_trie_result_t
    parse_compressed_map_of_unique_ids_to_tdg_collection_stack_tries(
        const std::u8string_view str_arg
    ) noexcept
{
    const auto parse_result{pnp::try_parse<
        map_unique_id_to_tdg_collection_stack_trie_t,
        map_unique_id_to_tdg_collection_stack_trie_pnp<true>>(str_arg)};
    if (parse_result.outcome.succeeded())
    {
        return parse_result.value;
    }
    // Messages are lowest-level first; show the outermost first.
    std::u8string message{
        u8"Could not parse map_unique_id_to_tdg_collection_stack_trie_t"
    };
    const auto& messages{parse_result.outcome.parser_error_messages};
    for (auto itt = messages.rbegin(); itt != messages.rend(); ++itt)
    {
        message.append(u8": ").append(*itt);
    }
    return std::unexpected(message);
}

template <bool To_Hex>
__no_constexpr_imp std::u8string
                   print_map_of_unique_ids_to_tdg_collection_stack_tries(
                       const map_unique_id_to_tdg_collection_stack_trie_t& map_arg
                   ) noexcept
{
    return pnp::to_u8string(
        map_unique_id_to_tdg_collection_stack_trie_pnp<To_Hex>{}, map_arg
    );
}

__no_constexpr_imp std::u8string
    print_compressed_map_of_unique_ids_to_tdg_collection_stack_tries(
        const map_unique_id_to_tdg_collection_stack_trie_t& map_arg
    ) noexcept
{
    return pnp::to_u8string<map_unique_id_to_tdg_collection_stack_trie_pnp<true>>(map_arg);
}

_END_ABC_DS_NS
#if 0
_BEGIN_ABC_UTILITY_PRINTER_NS
__no_constexpr_imp std::u8string
    default_printer_t<abc::ds::map_unique_id_to_tdg_collection_stack_trie_t>::
        run_printer(
            const abc::ds::map_unique_id_to_tdg_collection_stack_trie_t&
                _a_parse_input
        ) const
{
    using namespace std;
    using namespace ds;
    return default_printer<map<key_t, tdg_collection_stack_trie_t>>()
        ->run_printer(_a_parse_input.map());
}

_END_ABC_UTILITY_PRINTER_NS
#endif
_BEGIN_ABC_UTILITY_PARSER_NS
__no_constexpr_imp
    result_t<abc::ds::map_unique_id_to_tdg_collection_stack_trie_t>
    default_parser_t<abc::ds::map_unique_id_to_tdg_collection_stack_trie_t>::
        run_parser(
            parser_input_t& _a_parse_input
        ) const
{
    using namespace abc::ds;
    return parse_compressed_map_of_unique_ids_to_tdg_collection_stack_tries(
        _a_parse_input.get_u8string()
    );
}

_END_ABC_UTILITY_PARSER_NS
__no_constexpr_imp auto
    fmt::formatter<_ABC_NS_DS::map_unique_id_to_tdg_collection_stack_trie_t>::
        format(
            _ABC_NS_DS::map_unique_id_to_tdg_collection_stack_trie_t _a_rtd,
            format_context&                                          _a_ctx
        ) const -> format_context::iterator
{
    using namespace std;
    const string _l_rv{fmt::format(
        ""
        // "{0}{{{1} = {2}}}",
        // typeid(_a_rtd).name(),
        // "_m_internal_map",
        // abc::u8string_to_string(fmt::format(u8"{}", _a_rtd.map()))
    )};
    return formatter<string_view>::format(_l_rv, _a_ctx);
}