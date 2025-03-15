#pragma once

#include "abc_test/core/errors/test_library_exception.hpp"
#include "abc_test/utility/internal/macros.hpp"
#include "abc_test/utility/parsers/default_parser.hpp"
#include "abc_test/utility/printers/default_printer.hpp"

#include <random>
#include <scn/scan.h>


_BEGIN_ABC_UTILITY_NS
using seed_t = std::vector<uint32_t>;

class complete_global_seed_t
{
public:
    complete_global_seed_t() = default;

    complete_global_seed_t(
        const unsigned int _a_seed
    ) noexcept
        : _m_inner_seed(_a_seed)
    {}

    complete_global_seed_t(
        const std::variant<unsigned int, std::vector<uint32_t>>& _a_seed
    ) noexcept
        : _m_inner_seed(_a_seed)
    {}

    complete_global_seed_t(
        const std::vector<uint32_t>& _a_seed
    ) noexcept
        : _m_inner_seed(_a_seed)
    {}

    __constexpr std::optional<unsigned int>
                integer() const noexcept
    {
        using namespace std;
        if (holds_alternative<unsigned int>(_m_inner_seed))
        {
            return make_optional(get<unsigned int>(_m_inner_seed));
        }
        else
        {
            return nullopt;
        }
    }

    __constexpr std::optional<std::vector<uint32_t>>
                vector_of_integers() const noexcept
    {
        using namespace std;
        if (holds_alternative<vector<uint32_t>>(_m_inner_seed))
        {
            return make_optional(get<vector<uint32_t>>(_m_inner_seed));
        }
        else
        {
            return nullopt;
        }
    }

    __constexpr utility::seed_t
                get_seed(
                    const std::size_t _a_seed_to_create_size
                ) const
    {
        utility::seed_t _l_rv;
        if (auto _l_ptr{get_if<unsigned int>(&_m_inner_seed)};
            _l_ptr != nullptr)
        {
            _l_rv.resize(_a_seed_to_create_size);
            std::srand(*_l_ptr);
            for (size_t _l_idx{0}; _l_idx < _a_seed_to_create_size; ++_l_idx)
            {
                _l_rv[_l_idx] = std::rand();
            }
        }
        else if (auto _l_ptr{get_if<std::vector<uint32_t>>(&_m_inner_seed)};
                 _l_ptr != nullptr)
        {
            _l_rv = *_l_ptr;
        }
        else
        {
            throw errors::unaccounted_for_variant_exception(_m_inner_seed);
        }
        return _l_rv;
    }

    __constexpr std::string
                print_in_hex() const
    {
        std::variant<unsigned int, std::vector<uint32_t>>;
        using namespace std;
        string _l_rv;
        if (auto _l_ptr{get_if<unsigned int>(&_m_inner_seed)};
            _l_ptr != nullptr)
        {
            _l_rv.append("0");
            _l_rv.append(fmt::format("{:x}", *_l_ptr));
        }
        else if (auto _l_ptr{get_if<vector<uint32_t>>(&_m_inner_seed)};
                 _l_ptr != nullptr)
        {
            _l_rv.append("1");
            for (const uint32_t& _l_seed_val : *_l_ptr)
            {
                _l_rv.append(fmt::format("{:x}", _l_seed_val));
            }
        }
        else
        {
            throw errors::unaccounted_for_variant_exception(_m_inner_seed);
        }
        return _l_rv;
    }
private:
    std::variant<unsigned int, std::vector<uint32_t>> _m_inner_seed
        = std::vector<uint32_t>();
};

__constexpr result_t<complete_global_seed_t>
            parse_complete_global_string_in_hex(
                const std::string_view _a_str
            )
{
    using namespace std;
    if (_a_str.size() == 0)
    {
        return unexpected(fmt::format(
            "Size of string representing seed must not be empty. String = "
            "{0}",
            _a_str
        ));
    }
    else
    {
        char              _l_mode{_a_str[0]};
        const string_view _l_rest_of_str{_a_str.substr(1)};
        if (_l_mode == '0')
        {
            // Parse rest of string as hex.
            //  interpret the parsed number as hex
            auto _l_result{scn::scan<unsigned int>(_l_rest_of_str, "{:x}")};
            if (_l_result.has_value())
            {
                return complete_global_seed_t(_l_result->value());
            }
            else
            {
                return unexpected(fmt::format(
                    "Couldn't parse substring {0} in string {1}.",
                    _l_rest_of_str,
                    _a_str
                ));
            }
        }
        else if (_l_mode == '1')
        {
            // Every 8 characters is a uint32_t.
            size_t _l_str_size_mod_8{_l_rest_of_str.size() / 8};
            if ((_l_rest_of_str.size() % 8) != 0)
            {
                return unexpected(fmt::format(
                    "Reading a hex string of 32 bit integers. Each integer "
                    "is 8 characters. The string's size must be a multiple "
                    "of 8, but is not. String size = {0}, string = {1}",
                    _l_rest_of_str.size(),
                    _l_rest_of_str
                ));
            }
            else
            {
                vector<uint32_t> _l_vector;
                size_t           _l_str_size_mod_8{_l_rest_of_str.size() / 8};
                for (size_t _l_idx{0}; _l_idx < _l_str_size_mod_8; ++_l_idx)
                {
                    string_view _l_integer_str{
                        _l_rest_of_str.substr(_l_idx * 8, 8)
                    };
                    auto _l_result{scn::scan<uint32_t>(_l_integer_str, "{:x}")};
                    if (_l_result.has_value())
                    {
                        _l_vector.push_back(_l_result->value());
                    }
                    else
                    {
                        return unexpected(fmt::format(
                            "Couldn't parse substring {0} in string {1}.",
                            _l_integer_str,
                            _a_str
                        ));
                    }
                }
                return complete_global_seed_t(_l_vector);
            }
        }
        else
        {
            return unexpected(fmt::format(
                "String representing seed's first character denotes mode. "
                "mode "
                "\"{0}\" not recognised. String = {1}",
                _a_str[0],
                _a_str
            ));
        }
    }
}

class global_seed_t
{
public:
    __constexpr
    global_seed_t()
        = default;

    __constexpr
    global_seed_t(
        const complete_global_seed_t& _a_seed
    ) noexcept
        : _m_inner_seed(_a_seed)
    {}

    __constexpr void
        set_complete_seed(
            complete_global_seed_t& _a_seed
        ) const
    {
        using namespace std;
        if (auto _l_ptr{get_if<monostate>(&_m_inner_seed)}; _l_ptr != nullptr)
        {
            _a_seed = static_cast<unsigned int>(std::time(0));
        }
        else if (auto _l_ptr{get_if<complete_global_seed_t>(&_m_inner_seed)};
                 _l_ptr != nullptr)
        {
            _a_seed = *_l_ptr;
        }
        else
        {
            throw errors::unaccounted_for_variant_exception(_m_inner_seed);
        }
    }

    __constexpr bool
        is_not_set() const noexcept
    {
        using namespace std;
        return holds_alternative<monostate>(_m_inner_seed);
    }

    __constexpr std::optional<complete_global_seed_t>
                complete_global_seed() const noexcept
    {
        using namespace std;
        if (holds_alternative<complete_global_seed_t>(_m_inner_seed))
        {
            return get<complete_global_seed_t>(_m_inner_seed);
        }
        else
        {
            return nullopt;
        }
    }
private:
    std::variant<std::monostate, complete_global_seed_t> _m_inner_seed
        = std::monostate{};
};

class inner_rng_t
{
public:
    virtual void progress(const std::size_t _a_n_to_progress) noexcept = 0;
    virtual void
        set_seed(const seed_t& _a_seed) noexcept
        = 0;
    virtual std::mt19937_64::result_type
        operator()() noexcept
        = 0;
};

class inner_rng_mt19937_64_t : public inner_rng_t
{
public:
    inline virtual void progress(const std::size_t _a_n_to_progress) noexcept
    {
        _m_rng.discard(_a_n_to_progress);
    }
    inline inner_rng_mt19937_64_t(
        const utility::seed_t& _a_seed
    )
    {
        std::seed_seq _l_seed_seq(_a_seed.begin(), _a_seed.end());
        _m_rng.seed(_l_seed_seq);
    }

    virtual void
        set_seed(
            const seed_t& _a_seed
        ) noexcept
    {
        std::seed_seq _l_seed_seq(_a_seed.begin(), _a_seed.end());
        _m_rng.seed(_l_seed_seq);
    }

    virtual std::mt19937_64::result_type
        operator()() noexcept
    {
        return _m_rng();
    }
private:
    std::mt19937_64 _m_rng;
};

struct rng
{
public:
    using result_type = std::mt19937_64::result_type;

    static result_type
        min() noexcept
    {
        return std::mt19937_64::min();
    }

    static result_type
        max() noexcept
    {
        return std::mt19937_64::max();
    }

    inline rng
        make_rng(
            const std::size_t _a_n_elements_to_take
        )
    {
        seed_t _l_seed(_a_n_elements_to_take);
        for (size_t _l_idx{0}; _l_idx < _a_n_elements_to_take; ++_l_idx)
        {
            _l_seed[_l_idx] = _m_rng.get()->operator()();
        }
        std::shared_ptr<inner_rng_t> _l_rng_cpy{_m_rng};
        _l_rng_cpy->set_seed(_l_seed);
        return utility::rng(_l_rng_cpy, _l_seed);
    }

    inline void
        progress(
            const size_t _a_expected_calls
        ) noexcept
    {
        if (_m_calls > _a_expected_calls)
        {
            _m_rng->set_seed(_m_seed);
            _m_calls = 0;
            _m_calls += _a_expected_calls;
            _m_rng.get()->progress(_a_expected_calls);
        }
        _m_calls += _a_expected_calls;
        _m_rng.get()->progress(_a_expected_calls);
    }

    inline result_type
        operator()()
    {
        ++_m_calls;
        return _m_rng.get()->operator()();
    }

    template <typename Rng>
    static rng
        make_rng(
            const complete_global_seed_t& _a_global_seed,
            const std::size_t             _a_seed_size
        ) noexcept
    {
        using namespace std;
        const utility::seed_t _l_seed{ _a_global_seed.get_seed(_a_seed_size) };
        return rng(make_shared<Rng>(_l_seed), _l_seed);
    }

    template <typename Rng>
    static rng
        make_rng(
            const seed_t& _a_seed
        ) noexcept
    {
        return rng(make_shared<Rng>(_a_seed), _a_seed);
    }

    inline rng() noexcept = delete;

    /*inline rng(
        const complete_global_seed_t& _a_global_seed,
        const std::size_t             _a_seed_to_create_size
    )
        : _m_rnd()
        , _m_seed(_a_global_seed.get_seed(_a_seed_to_create_size))
        , _m_calls(0)
    {
        using namespace std;
        std::seed_seq _l_seed_seq(_m_seed.begin(), _m_seed.end());
        _m_rnd = std::mt19937_64(_l_seed_seq);
    }*/

    // inline rng(
    // const seed_t& _a_seed
    // ) noexcept
    // : _m_rnd(), _m_seed(_a_seed), _m_calls(0)
    //{
    // std::seed_seq _l_seed_seq(_m_seed.begin(), _m_seed.end());
    // _m_rnd = std::mt19937_64(_l_seed_seq);
    // }

    constexpr size_t
        calls() const noexcept
    {
        return _m_calls;
    }
private:
    std::shared_ptr<inner_rng_t> _m_rng;
    // std::mt19937_64 _m_rnd;
    utility::seed_t _m_seed;
    size_t          _m_calls;

    inline rng(
        const std::shared_ptr<inner_rng_t>& _a_rng,
        const utility::seed_t& _a_seed
    )
        : _m_rng(_a_rng)
        , _m_seed(_a_seed)
        , _m_calls(0)
    {}
};

_END_ABC_UTILITY_NS
_BEGIN_ABC_UTILITY_PARSER_NS

template <>
struct default_parser_t<global_seed_t> : public parser_base_t<global_seed_t>
{
    __constexpr result_t<global_seed_t>
                run_parser(
                    parser_input_t& _a_parse_input
                ) const
    {
        using namespace std;
        const result_t<unsigned int> _l_variant_result{
            utility::parser::default_parser_t<unsigned int>().run_parser(
                _a_parse_input
            )
        };
        if (_l_variant_result.has_value())
        {
            return result_t<global_seed_t>(
                complete_global_seed_t(_l_variant_result.value())
            );
        }
        else
        {
            return unexpected(_l_variant_result.error());
        }
    }
};

template <>
struct default_parser_t<complete_global_seed_t>
    : public parser_base_t<complete_global_seed_t>
{
    __constexpr result_t<complete_global_seed_t>
                run_parser(
                    parser_input_t& _a_parse_input
                ) const
    {
        using namespace std;
        const result_t<variant<unsigned int, std::vector<uint32_t>>>
            _l_variant_result{utility::parser::default_parser_t<
                                  variant<unsigned int, std::vector<uint32_t>>>(
            )
                                  .run_parser(_a_parse_input)};
        if (_l_variant_result.has_value())
        {
            return result_t<complete_global_seed_t>(
                complete_global_seed_t(_l_variant_result.value())
            );
        }
        else
        {
            return unexpected(_l_variant_result.error());
        }
    }
};

_END_ABC_UTILITY_PARSER_NS
_BEGIN_ABC_UTILITY_PRINTER_NS

template <>
struct default_printer_t<global_seed_t> : public printer_base_t<global_seed_t>
{
    __constexpr std::string
                run_printer(
                    const global_seed_t& _a_parse_input
                ) const
    {
        using namespace std;
        return "global_seed_t";
    }
};

template <>
struct default_printer_t<complete_global_seed_t>
    : public printer_base_t<complete_global_seed_t>
{
    __constexpr std::string
                run_printer(
                    const complete_global_seed_t& _a_parse_input
                ) const
    {
        using namespace std;
        return "complete_global_seed_t";
    }
};

_END_ABC_UTILITY_PRINTER_NS

/*!
 * formatter for registered_test_data_t
 */
template <>
struct fmt::formatter<abc::utility::global_seed_t> : formatter<string_view>
{
    // parse is inherited from formatter<string_view>.
    // Can't be constexpr due to use of fmt::format.
    __no_constexpr auto
        format(abc::utility::global_seed_t _a_rtd, format_context& _a_ctx) const
        -> format_context::iterator;
};

__no_constexpr_imp auto
    fmt::formatter<abc::utility::global_seed_t>::format(
        abc::utility::global_seed_t _a_rtd,
        format_context&             _a_ctx
    ) const -> format_context::iterator
{
    using namespace std;
    string _l_rv{fmt::format(
        "{0}"
        "{{{1} = {2}"
        "}}",
        typeid(_a_rtd).name(),
        "_l_inner_seed",
        "placeholder"
        // _a_rtd._l_inner_seed
    )};
    return formatter<string_view>::format(_l_rv, _a_ctx);
}