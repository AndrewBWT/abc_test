#pragma once

#include "abc_test/core/errors/test_library_exception.hpp"
#include "abc_test/utility/parsers/default_parser.hpp"
#include "abc_test/utility/printers/default_printer.hpp"
#include "abc_test/utility/rng/seed.hpp"

#include <optional>
#include <scn/scan.h>
#include <string>
#include <variant>
#include <vector>

_BEGIN_ABC_UTILITY_NS

/*!
 * @brief This class describes a global seed in abc_test.
 *
 * A global seed can be set with either a single unsigned integer or a vector of
 * unsigned integers.
 *
 * The single integer is used to initialize srand, and then values taken using
 * std::rand.
 *
 * A vector of integers is used to initialize a RNG directly, using its
 * initializer function which takes a vector of unsigned integers (if it
 * exists).
 */
class complete_global_seed_t
{
public:
    /*!
     * @brief The internal seed type, made available publicly.
     */
    std::variant<unsigned int, seed_t> inner_seed = seed_t();
    /*!
     * @brief This function will create a valid seed_t value using the internal
     * inner_seed value.
     * @param _a_seed_to_create_size The size the created vector should be.
     * @return The created seed_t value.
     */
    __constexpr seed_t
        create_seed_from_global_seed(const std::size_t _a_seed_to_create_size
        ) const;
private:
};

_END_ABC_UTILITY_NS

_BEGIN_ABC_UTILITY_PRINTER_NS

template <>
struct default_printer_t<complete_global_seed_t>
    : public printer_base_t<complete_global_seed_t>
{
    __constexpr std::u8string
                run_printer(const complete_global_seed_t& _a_parse_input) const;
};

_END_ABC_UTILITY_PRINTER_NS
_BEGIN_ABC_UTILITY_PARSER_NS

template <>
struct default_parser_t<complete_global_seed_t>
    : public parser_base_t<complete_global_seed_t>

{
    __constexpr result_t<complete_global_seed_t>
                run_parser(parser_input_t& _a_parse_input) const;
};

_END_ABC_UTILITY_PARSER_NS

_BEGIN_ABC_UTILITY_NS

__constexpr_imp seed_t
    complete_global_seed_t::create_seed_from_global_seed(
        const std::size_t _a_seed_to_create_size
    ) const
{
    if (const unsigned int* _l_ptr{get_if<unsigned int>(&inner_seed)};
        _l_ptr != nullptr)
    {
        seed_t           _l_rv(_a_seed_to_create_size);
        seed_t::iterator _l_end{end(_l_rv)};
        srand(*_l_ptr);
        for (seed_t::iterator _l_itt{begin(_l_rv)}; _l_itt != _l_end; ++_l_itt)
        {
            *_l_itt = rand();
        }
        return _l_rv;
    }
    else if (const seed_t * _l_ptr{get_if<seed_t>(&inner_seed)};
             _l_ptr != nullptr)
    {
        return *_l_ptr;
    }
    else
    {
        throw errors::unaccounted_for_variant_exception(inner_seed);
    }
}

_END_ABC_UTILITY_NS

_BEGIN_ABC_UTILITY_PRINTER_NS

__constexpr_imp std::u8string
                default_printer_t<complete_global_seed_t>::run_printer(
        const complete_global_seed_t& _a_object
    ) const
{
    using namespace std;
    return default_printer_t<variant<unsigned int, seed_t>>().run_printer(
        _a_object.inner_seed
    );
}

_END_ABC_UTILITY_PRINTER_NS

_BEGIN_ABC_UTILITY_PARSER_NS

__constexpr_imp result_t<complete_global_seed_t>
            default_parser_t<complete_global_seed_t>::run_parser(
        parser_input_t& _a_parse_input
    ) const
{
    using namespace std;
    return default_parser_t<variant<unsigned int, seed_t>>()
        .run_parser(_a_parse_input)
        .transform(
            [](const auto& _l_result)
            {
                return complete_global_seed_t{_l_result};
            }
        );
}

_END_ABC_UTILITY_PARSER_NS