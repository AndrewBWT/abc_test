#pragma once
#include "abc_test/core/errors/test_library_exception.hpp"
#include "abc_test/utility/internal/macros.hpp"
#include "abc_test/utility/internal/unreachable.hpp"
#include "abc_test/utility/parsers/parser_input.hpp"
#include "abc_test/utility/str/unicode.hpp"
#include "abc_test/utility/types.hpp"

#include <algorithm>
#include <concepts>
#include <list>
#include <map>
#include <optional>
#include <set>
#include <vector>

// Forward declarations
_BEGIN_ABC_UTILITY_PRINTER_NS
template <typename T>
struct default_printer_t;
_END_ABC_UTILITY_PRINTER_NS

_BEGIN_ABC_UTILITY_NS
/*!
 * @brief A type synonym for the underlying character type used in the
 * enum_list_t's string.
 */
using enum_list_str_char_t = char8_t;
/*!
 * @brief A type synonym for hte previously mentioned string.
 */
using enum_list_str_t      = std::basic_string_view<enum_list_str_char_t>;
/*!
 * @brief A type synonym for the list of pairs of enums and enum_list_str_t
 * entries which denotes the string entity representing each enum.
 *
 * @tparam T
 */
template <typename T>
using enum_list_t = std::vector<std::pair<T, enum_list_str_t>>;
/*!
 * @brief A concept for enums that have a get_enum_list function associated with
 * them.
 */
template <typename T>
concept enum_has_list_c = std::is_enum_v<T> && requires () {
    { get_enum_list<T>() } -> std::same_as<enum_list_t<T>>;
};
/*!
 * @brief This function, defined for a specific enum, when paired with the
 * enum_helper_string_t class, allows enusm to be printed, parsed and
 * enumerated.
 * @tparam T The enum paramter.
 * @return
 */
template <typename T>
requires std::is_enum_v<T>
__constexpr enum_list_t<T>
            get_enum_list() noexcept = delete;
/*!
 * @brief This enum is used to denote how to print and parse strings associated
 * with an enum.
 *
 */
enum class enum_helper_string_type_e
{
    /*!
     * @brief The printing and parsing will assume that the enum's associated
     * string is in all lower case.
     */
    lower,
    /*!
     * @brief The printing and parsing will assume that the enum's associated
     * string is in all upper case.
     */
    upper,
    /*!
     * @brief The printing and parsing will assume that the enum's associated
     * string is exactly as was found in the source code.
     */
    unchanged
};

/*!
 * @brief This class allows the user to print, parse and enumerate enums.
 * Essentially it acts as a utility class, allowing the user to run functions on
 * enums. Instances of the class are created using the only constructor. The
 * user should also note that the function
 * get_thread_local_enumerate_enum_helper is used so as to ensure that the class
 * is only created once per thread.
 * @tparam T The underlying enum parameter.
 */
template <typename T>
requires enum_has_list_c<T>
struct enumerate_enum_helper_t
{
    __constexpr
    enumerate_enum_helper_t()
        = delete;
    /*!
     * @brief The core constructor for the class.
     *
     * The created strings for each enum_helper_string_type_e must be unique for
     * each enum. That is to say, a string can't point to two different enums.
     * This would mean that parsing would become ambigious. However, you can
     * have the same enum with different strings. This will mean that both
     * strings will be parsed by the strings, however when printing only the
     * first added to the object will be used to print.
     *
     * In addition to this, the _a_enum_list argument cannot be empty.
     *
     * @param _a_enum_list The enum_list_t object used to construct this object.
     *
     */
    __constexpr
    enumerate_enum_helper_t(const enum_list_t<T>& _a_enum_list);
    /*!
     * @brief Compares two enums and works out whether the first is less than
     * the second.
     *
     * This ordering is based on the order the enums were encountered in the
     * constructor.
     * @param _a_l The first argument.
     * @param _a_r The second argument.
     * @return True if _a_l < _a_r. False otherwise.
     */
    __constexpr bool
        less_than(const T& _a_l, const T& _a_r) const
        noexcept(unreachable_does_not_throw_exception);
    /*!
     * @brief Compares two enums and works out whether the first is equal to
     * the second.
     *
     * This ordering is based on the order the enums were encountered in the
     * constructor.
     * @param _a_l The first argument.
     * @param _a_r The second argument.
     * @return True if _a_l == _a_r. False otherwise.
     */
    __constexpr bool
        equal(const T& _a_l, const T& _a_r) const
        noexcept(unreachable_does_not_throw_exception);
    /*!
     * @brief Returns the minimum enum in the enum list given to the
     * constructor.
     *
     * This is why the enum_list cannot be empty; there must be at least one
     * value.
     *
     * @return The smallest enum in the enum set such that all other enums are
     * greater than this enum.
     */
    __constexpr T
        min() const noexcept;
    /*!
     * @brief Returns the maximum enum in the enum list given to the
     * constructor.
     *
     * This is why the enum_list cannot be empty; there must be at least one
     * value.
     *
     * @return The smallest enum in the enum set such that all other enums are
     * less than this enum.
     */
    __constexpr T
        max() const noexcept;
    /*!
     * @brief Increments the given enum.
     *
     * This function increments _a_element by _a_times_called, but not going
     * over _a_max_value. If _a_max_value == std::nullopt, _a_max_value = max().
     *
     * If _a_times_called is greater than the number of times incremented, then
     * this remainder is maintained in _a_times_called, and false returned. If
     * _a_times_called == 0 after the incrmenet has finished, then the function
     * returns true.
     *
     * If _a_element > _a_max_value, then this function has undefined behaviour.
     *
     * @param _a_element The enum to increment.
     * @param _a_times_called The number of times to incrmenet _a_element.
     * @param _a_max_value The maximum value the enum can attain. If this is set
     * to std::nullopt, then the enum will go to max().
     * @return True if, after incrementing, _a_times_called == 0. False
     * otherwise.
     */
    __constexpr bool
        increment(
            T&                      _a_element,
            std::size_t&            _a_times_called,
            const std::optional<T>& _a_max_value
        ) const noexcept(unreachable_does_not_throw_exception);
    /*!
     * @brief Decrements the given enum.
     *
     * This function decrements _a_element by _a_times_called, but not going
     * below _a_min_value. If _a_max_value == std::nullopt, _a_max_value =
     * min().
     *
     * If _a_times_called is greater than the number of times decremented, then
     * this remainder is maintained in _a_times_called, and false returned. If
     * _a_times_called == 0 after the decrmenet has finished, then the function
     * returns true.
     *
     * If _a_element < _a_min_value, then this function has undefined behaviour.
     *
     * @param _a_element The enum to decrement.
     * @param _a_times_called The number of times to decrmenet _a_element.
     * @param _a_max_value The maximum value the enum can attain. If this is set
     * to std::nullopt, then the enum will go to min().
     * @return True if, after decrementing, _a_times_called == 0. False
     * otherwise.
     */
    __constexpr bool
        decrement(
            T&                      _a_element,
            std::size_t&            _a_times_called,
            const std::optional<T>& _a_max_value
        ) const noexcept(unreachable_does_not_throw_exception);
    /*!
     * @brief Calculates the difference between two enums. The ordering of the
     * enums do not matter. difference(a,b) == difference(b,a).
     * @param _a_arg1 The first enum to calculate the difference from.
     * @param _a_arg2 The second enum to calculate the difference from.
     * @return The difference between a and b. Specifically, this can be thought
     * of as the difference in the indexes in the vector given as the arguemnt
     * to the constructor.
     */
    __constexpr std::size_t
                difference(const T& _a_arg1, const T& _a_arg2) const
        noexcept(unreachable_does_not_throw_exception);
    /*!
     * @brief Prints a textual representation of the given enum.
     *
     * Depending on _a_enum_string_case and the strings given to the
     * constructor, this will return different results.
     *
     * If _a_enum_string_case == unchanged, then this function will print the
     * enum's string as given to the constructor.
     *
     * If _a_enum_string_case == lower, then this function will print the enum's
     * string after having the function std::lowercase applied to each
     * character.
     *
     * If _a_enum_string_case == upper, then this function will print the enum's
     * string after having the function std::uppercase applied to each
     * character.
     *
     * @param _a_element The enum to print a text representation of.
     * @param _a_enum_string_case The enum denoting what type of string to
     * print.
     * @return A u8string representation of the enum.
     *
     * Note that if _a_enum_string_case or _a_element are invalid enums, then
     * this function has undefined behaviour.
     */
    __constexpr std::u8string
                print(
                    const T&                        _a_element,
                    const enum_helper_string_type_e _a_enum_string_case
                    = enum_helper_string_type_e::unchanged
                ) const noexcept(unreachable_does_not_throw_exception);

    __constexpr result_t<T>
                parse_enum(
                    parser::parser_input_t&         _a_parse_input,
                    const enum_helper_string_type_e _a_enum_string_case
                    = enum_helper_string_type_e::unchanged
                ) const noexcept(unreachable_does_not_throw_exception);
    /*!
     * @brief Returns the cardinality of enums of T.
     * @return The number of entities of T.
     */
    __constexpr std::size_t
                size() const noexcept;
    /*!
     * @brief Gets the internal std::size_t representation of the enum given as
     * the argument.
     * @param _a_enum The enum to use to get the internal representation.
     * @return The internal representaiton.
     */
    __constexpr std::size_t
                enums_idx(const T& _a_enum) const
        noexcept(unreachable_does_not_throw_exception);
    /*!
     * @brief Given the internal representation, gets the enum associated with
     * it.
     * @param _a_idx The internal representation.
     * @return The enum associated with the internal representation.
     */
    __constexpr T
        idxs_enum(const std::size_t _a_idx) const
        noexcept(unreachable_does_not_throw_exception);
private:
    using str_t     = std::basic_string<enum_list_str_char_t>;
    using str_set_t = std::set<str_t>;
    using str_ref_t = typename str_set_t::iterator;
    using idx_t     = std::size_t;

    // Comparison operator for str_ref_t entities.
    struct str_ref_comparison_t
    {
        __constexpr bool
            operator()(const str_ref_t& _a_arg1, const str_ref_t& _a_arg2)
                const noexcept;
    };

    using str_ref_to_idx_t   = std::map<str_ref_t, idx_t, str_ref_comparison_t>;
    using idx_t_to_str_ref_t = std::map<idx_t, std::vector<str_ref_t>>;
    str_set_t          _m_lower_case_strings;
    str_set_t          _m_upper_case_strings;
    str_set_t          _m_unchanged_strings;
    std::map<idx_t, T> _m_idx_to_elements;
    std::map<T, idx_t> _m_elements_to_idx;
    str_ref_to_idx_t   _m_string_to_elements;
    str_ref_to_idx_t   _m_lower_string_to_elements;
    str_ref_to_idx_t   _m_upper_string_to_elements;
    idx_t_to_str_ref_t _m_elements_to_string;
    idx_t_to_str_ref_t _m_elements_to_lower_string;
    idx_t_to_str_ref_t _m_elements_to_upper_string;
    /*
     * Gets the str_ref_to_idx_t related to the enum_helper_string_type_e.
     */
    __constexpr const str_ref_to_idx_t&
        get_string_to_elements_map(const enum_helper_string_type_e _a_enum
        ) const noexcept(unreachable_does_not_throw_exception);
    /*
     * Gets the idx_t_to_str_ref_t related to the enum_helper_string_type_e.
     */
    __constexpr const idx_t_to_str_ref_t&
        get_elements_to_string_map(const enum_helper_string_type_e _a_enum
        ) const noexcept(unreachable_does_not_throw_exception);
    /*
     * Inserts a string into the internal data structures.
     */
    __constexpr void
        insert_string(
            const enum_list_t<T>&           _a_enum_list,
            const enum_helper_string_type_e _a_enum,
            const enum_list_str_t           _a_string_to_insert,
            const idx_t                     _a_index_to_insert,
            str_set_t&                      _a_str_set_to_insert_into,
            str_ref_to_idx_t&               _a_str_ref_to_idx_to_insert_into,
            idx_t_to_str_ref_t&             _a_idx_to_str_ref_to_insert_into
        );
};

namespace detail
{
// Creates the constructor exception.
template <typename T>
__constexpr void
    create_constructor_exception(
        const enum_list_t<T>&    _a_enum_list,
        const std::u8string_view _a_reason
    );

// Simple function for printing enum_helper_string_type_e. This is used when
// printing constructor error messages - we cannot bootstrap that as it might
// get stuck in an infinite loop.
__constexpr std::u8string_view
            simple_enum_helper_string_type_to_string(
                const enum_helper_string_type_e _a_enum
            ) noexcept(unreachable_does_not_throw_exception);
} // namespace detail

/*!
 * @brief Gets the thread local enumerate_enum_helper_t for T.
 * @tparam T The enum paramter.
 * @return The thread's enumerate_enum_helper_t<T> entity.
 */
template <typename T>
requires enum_has_list_c<T>
__constexpr const enumerate_enum_helper_t<T>&
                  get_thread_local_enumerate_enum_helper() noexcept;
_END_ABC_UTILITY_NS

/*!
 * @brief Enum for creating list of entries for an enum_list_t object.
 *
 * The created string will exactly minimc the name of the enum.
 */
#define _ENUM_LIST_ENTRY(x) {x, u8#x}

_BEGIN_ABC_UTILITY_NS
template <typename T>
requires enum_has_list_c<T>
__constexpr_imp
    enumerate_enum_helper_t<T>::enumerate_enum_helper_t(
        const enum_list_t<T>& _a_enum_list
    )
{
    using namespace _ABC_NS_ERRORS;
    using namespace std;
    for (auto& [_l_element, _l_str] : _a_enum_list)
    {
        idx_t _l_idx;

        if (_m_elements_to_idx.contains(_l_element))
        {
            _l_idx = _m_elements_to_idx.at(_l_element);
        }
        else
        {
            _l_idx = _m_elements_to_idx.size();
            _m_elements_to_idx.insert({_l_element, _l_idx});
            _m_idx_to_elements.insert({_l_idx, _l_element});
        };
        insert_string(
            _a_enum_list,
            enum_helper_string_type_e::unchanged,
            _l_str,
            _l_idx,
            _m_unchanged_strings,
            _m_string_to_elements,
            _m_elements_to_string
        );
        basic_string<enum_list_str_char_t> _l_str_cpy{_l_str};
        // Convert to lower case.
        transform(
            _l_str_cpy.begin(),
            _l_str_cpy.end(),
            _l_str_cpy.begin(),
            [](enum_list_str_char_t _l_char)
            {
                return is_valid_ascii_char(_l_char)
                           ? static_cast<enum_list_str_char_t>(
                                 std::tolower(static_cast<char>(_l_char))
                             )
                           : _l_char;
            }
        );
        insert_string(
            _a_enum_list,
            enum_helper_string_type_e::lower,
            _l_str_cpy,
            _l_idx,
            _m_lower_case_strings,
            _m_lower_string_to_elements,
            _m_elements_to_lower_string
        );
        // Conver to upper case.
        transform(
            _l_str_cpy.begin(),
            _l_str_cpy.end(),
            _l_str_cpy.begin(),
            [](enum_list_str_char_t _l_char)
            {
                return is_valid_ascii_char(_l_char)
                           ? static_cast<enum_list_str_char_t>(
                                 std::toupper(static_cast<char>(_l_char))
                             )
                           : _l_char;
            }
        );
        insert_string(
            _a_enum_list,
            enum_helper_string_type_e::upper,
            _l_str_cpy,
            _l_idx,
            _m_upper_case_strings,
            _m_upper_string_to_elements,
            _m_elements_to_upper_string
        );
    }
    if (_a_enum_list.size() == 0)
    {
        detail::create_constructor_exception(
            _a_enum_list,
            u8"enumerate_enum_helper_t objects created using empty "
            u8"enum_list_t objects are not allowed"
        );
    }
}

template <typename T>
requires enum_has_list_c<T>
__constexpr_imp bool
    enumerate_enum_helper_t<T>::less_than(
        const T& _a_l,
        const T& _a_r
    ) const
    noexcept(
        unreachable_does_not_throw_exception
    )
{
    return enums_idx(_a_l) < enums_idx(_a_r);
}

template <typename T>
requires enum_has_list_c<T>
__constexpr_imp bool
    enumerate_enum_helper_t<T>::equal(
        const T& _a_l,
        const T& _a_r
    ) const
    noexcept(
        unreachable_does_not_throw_exception
    )
{
    return enums_idx(_a_l) == enums_idx(_a_r);
}

template <typename T>
requires enum_has_list_c<T>
__constexpr_imp T
    enumerate_enum_helper_t<T>::min() const noexcept
{
    return idxs_enum(0);
}

template <typename T>
requires enum_has_list_c<T>
__constexpr_imp T
    enumerate_enum_helper_t<T>::max() const noexcept
{
    return idxs_enum(_m_elements_to_idx.size() - 1);
}

template <typename T>
requires enum_has_list_c<T>
__constexpr_imp bool
    enumerate_enum_helper_t<T>::increment(
        T&                      _a_element,
        std::size_t&            _a_times_called,
        const std::optional<T>& _a_max_value
    ) const
    noexcept(
        unreachable_does_not_throw_exception
    )
{
    using namespace std;
    const size_t _l_current_idx{enums_idx(_a_element)};
    const size_t _l_max_value{
        _a_max_value.has_value() ? enums_idx(_a_max_value.value())
                                 : (_m_elements_to_idx.size() - 1)
    };
    if (_l_max_value < _l_current_idx)
    {
        _ABC_UNREACHABLE(fmt::format(
            u8"enumerate_enum_helper_t::increment has a maximum enum value "
            u8"greater than the enum argument. The enum \"{0}\" argument "
            u8"has a "
            u8"text "
            u8"representation of \"{1}\", while the maximum enum value has "
            u8"a "
            "text representation of \"{2}\".",
            type_id<T>(),
            print(_a_element),
            print(_a_max_value.value())
        ));
    }
    else
    {
        const size_t _l_max_times_called{_l_max_value - _l_current_idx};
        if (_a_times_called > _l_max_times_called)
        {
            _a_times_called -= _l_max_times_called;
            _a_element       = _m_idx_to_elements.at(_l_max_value);
            return false;
        }
        else
        {
            _a_element
                = _m_idx_to_elements.at(_l_current_idx + _a_times_called);
            _a_times_called = 0;
            return true;
        }
    }
}

template <typename T>
requires enum_has_list_c<T>
__constexpr_imp bool
    enumerate_enum_helper_t<T>::decrement(
        T&                      _a_element,
        std::size_t&            _a_times_called,
        const std::optional<T>& _a_min_value
    ) const
    noexcept(
        unreachable_does_not_throw_exception
    )
{
    using namespace std;
    const size_t _l_current_idx{enums_idx(_a_element)};
    const size_t _l_min_value{
        _a_min_value.has_value() ? enums_idx(_a_min_value.value()) : 0
    };
    if (_l_min_value > _l_current_idx)
    {
        _ABC_UNREACHABLE(fmt::format(
            u8"enumerate_enum_helper_t::decrement has a minimum enum value "
            u8"greater than the enum argument. The enum \"{0}\" argument "
            u8"has a "
            u8"text "
            u8"representation of \"{1}\", while the minimum enum value has "
            u8"a "
            "text representation of \"{2}\".",
            type_id<T>(),
            print(_a_element),
            print(_a_min_value.value())
        ));
    }
    else
    {
        const size_t _l_max_times_called{_l_current_idx - _l_min_value};
        if (_a_times_called > _l_max_times_called)
        {
            _a_times_called -= _l_max_times_called;
            _a_element       = _m_idx_to_elements.at(_l_min_value);
            return false;
        }
        else
        {
            _a_element
                = _m_idx_to_elements.at(_l_current_idx - _a_times_called);
            _a_times_called = 0;
            return true;
        }
    }
}

template <typename T>
requires enum_has_list_c<T>
__constexpr_imp std::size_t
                enumerate_enum_helper_t<T>::difference(
        const T& _a_arg1,
        const T& _a_arg2
    ) const
    noexcept(
        unreachable_does_not_throw_exception
    )
{
    const size_t _l_arg1_idx{enums_idx(_a_arg1)};
    const size_t _l_arg2_idx{enums_idx(_a_arg2)};
    return (_l_arg1_idx < _l_arg2_idx) ? (_l_arg2_idx - _l_arg1_idx)
                                       : (_l_arg1_idx - _l_arg2_idx);
}

template <typename T>
requires enum_has_list_c<T>
__constexpr std::u8string
            enumerate_enum_helper_t<T>::print(
        const T&                        _a_element,
        const enum_helper_string_type_e _a_enum_string_case
    ) const
    noexcept(
        unreachable_does_not_throw_exception
    )
{
    using namespace std;
    using enum enum_helper_string_type_e;
    enum_list_str_t           _l_rv;
    const size_t              _l_idx{enums_idx(_a_element)};
    const idx_t_to_str_ref_t& _l_idx_to_str_ref{
        get_elements_to_string_map(_a_enum_string_case)
    };
    return *(_l_idx_to_str_ref.at(_l_idx)[0]);
}

template <typename T>
requires enum_has_list_c<T>
__constexpr result_t<T>
            enumerate_enum_helper_t<T>::parse_enum(
        parser::parser_input_t&         _a_parse_input,
        const enum_helper_string_type_e _a_enum_string_case
    ) const
    noexcept(
        unreachable_does_not_throw_exception
    )
{
    using namespace std;
    const str_ref_to_idx_t& _l_string_to_elements_ref{
        get_string_to_elements_map(_a_enum_string_case)
    };
    for (const auto& [_l_sv, _l_enum] : _l_string_to_elements_ref)
    {
        if (_a_parse_input.check_and_advance(*_l_sv))
        {
            return idxs_enum(_l_enum);
        }
    }
    return unexpected(fmt::format(
        u8"Could not parse string \"{0}\" to enum of type \"{1}\".",
        _a_parse_input,
        type_id<T>()
    ));
}

template <typename T>
requires enum_has_list_c<T>
__constexpr std::size_t
            enumerate_enum_helper_t<T>::size() const noexcept
{
    return _m_idx_to_elements.size();
}

template <typename T>
requires enum_has_list_c<T>
__constexpr std::size_t
            enumerate_enum_helper_t<T>::enums_idx(
        const T& _a_enum
    ) const
    noexcept(
        unreachable_does_not_throw_exception
    )
{
    if (_m_elements_to_idx.contains(_a_enum))
    {
        return _m_elements_to_idx.at(_a_enum);
    }
    else
    {
        using namespace std;
        _ABC_UNREACHABLE(fmt::format(
            u8"enumerate_enum_helper_t<{0}> does not contain the given "
            u8"enum. "
            u8"The enum's value has no known text representation. The "
            u8"enum, "
            u8"represented in its underlying type \"{1}\", has a value of "
            u8"{2}.",
            type_id<T>(),
            type_id<underlying_type_t<T>>(),
            to_underlying(_a_enum)
        ));
    }
}

template <typename T>
requires enum_has_list_c<T>
__constexpr T
    enumerate_enum_helper_t<T>::idxs_enum(
        const std::size_t _a_idx
    ) const
    noexcept(
        unreachable_does_not_throw_exception
    )
{
    if (_m_idx_to_elements.contains(_a_idx))
    {
        return _m_idx_to_elements.at(_a_idx);
    }
    else
    {
        using namespace std;
        _ABC_UNREACHABLE(fmt::format(
            u8"enumerate_enum_helper_t<{0}> does not contain an enum which "
            u8"corresponds with the index {1}.",
            type_id<T>(),
            _a_idx
        ));
    }
}

template <typename T>
requires enum_has_list_c<T>
__constexpr_imp bool
    enumerate_enum_helper_t<T>::str_ref_comparison_t::operator()(
        const str_ref_t& _a_arg1,
        const str_ref_t& _a_arg2
    ) const noexcept
{
    return *_a_arg1 < *_a_arg2;
}

template <typename T>
requires enum_has_list_c<T>
__constexpr const enumerate_enum_helper_t<T>::str_ref_to_idx_t&
                  enumerate_enum_helper_t<T>::get_string_to_elements_map(
        const enum_helper_string_type_e _a_enum
    ) const
    noexcept(
        unreachable_does_not_throw_exception
    )
{
    using enum enum_helper_string_type_e;
    switch (_a_enum)
    {
    case lower:
        return _m_lower_string_to_elements;
    case upper:
        return _m_upper_string_to_elements;
    case unchanged:
        return _m_string_to_elements;
    default:
        abc::unreachable_enum(_a_enum, u8"enumerate_enum_helper_t::parse");
        return _m_string_to_elements;
    }
}

template <typename T>
requires enum_has_list_c<T>
__constexpr const enumerate_enum_helper_t<T>::idx_t_to_str_ref_t&
                  enumerate_enum_helper_t<T>::get_elements_to_string_map(
        const enum_helper_string_type_e _a_enum
    ) const
    noexcept(
        unreachable_does_not_throw_exception
    )
{
    using enum enum_helper_string_type_e;
    switch (_a_enum)
    {
    case lower:
        return _m_elements_to_lower_string;
    case upper:
        return _m_elements_to_upper_string;
    case unchanged:
        return _m_elements_to_string;
    default:
        abc::unreachable_enum(_a_enum, u8"enumerate_enum_helper_t::print");
        return _m_elements_to_string;
    }
}

template <typename T>
requires enum_has_list_c<T>
__constexpr void
    enumerate_enum_helper_t<T>::insert_string(
        const enum_list_t<T>&           _a_enum_list,
        const enum_helper_string_type_e _a_enum,
        const enum_list_str_t           _a_string_to_insert,
        const idx_t                     _a_index_to_insert,
        str_set_t&                      _a_str_set_to_insert_into,
        str_ref_to_idx_t&               _a_str_ref_to_idx_to_insert_into,
        idx_t_to_str_ref_t&             _a_idx_to_str_ref_to_insert_into
    )
{
    using namespace std;
    auto _l_str_ref{
        _a_str_set_to_insert_into.insert(str_t(_a_string_to_insert)).first
    };
    auto _l_found_idx{_a_str_ref_to_idx_to_insert_into.find(_l_str_ref)};
    if (_l_found_idx != end(_a_str_ref_to_idx_to_insert_into)
        && _l_found_idx->second != _a_index_to_insert)
    {
        detail::create_constructor_exception(
            _a_enum_list,
            fmt::format(
                u8"in enumerate_enum_helper_t, enums with overlapping "
                u8"text representation's are not allowed. The first issue "
                u8"encountered is that "
                u8"the text representation \"{0}\" (accessed when printing "
                u8"or parsing using the {1} value "
                u8"\"{2}\") is already mapped to the \"{3}\" enum "
                u8"represented by the underlying \"{4}\" value {5}. An "
                u8"attempt is being made by the constructor to map this "
                u8"text representation to the \"{3}\" enum represented by "
                u8"the underlying \"{4}\" value {6}. Doing so would "
                u8"introduce ambiguity when parsing enums, and is "
                u8"therefore not allowed.",
                _a_string_to_insert,
                type_id<enum_helper_string_type_e>(),
                utility::detail::simple_enum_helper_string_type_to_string(
                    _a_enum
                ),
                type_id<T>(),
                type_id<underlying_type_t<T>>(),
                to_underlying(
                    idxs_enum(_a_str_ref_to_idx_to_insert_into.at(_l_str_ref))
                ),
                to_underlying(idxs_enum(_a_index_to_insert))
            )
        );
    }
    _a_str_ref_to_idx_to_insert_into.insert({_l_str_ref, _a_index_to_insert});
    if (not _a_idx_to_str_ref_to_insert_into.contains(_a_index_to_insert))
    {
        _a_idx_to_str_ref_to_insert_into.insert(
            {_a_index_to_insert, vector<str_ref_t>()}
        );
    }
    _a_idx_to_str_ref_to_insert_into.at(_a_index_to_insert)
        .push_back(_l_str_ref);
}

template <typename T>
requires enum_has_list_c<T>
__constexpr_imp const enumerate_enum_helper_t<T>&
                      get_thread_local_enumerate_enum_helper() noexcept
{
    thread_local enumerate_enum_helper_t<T> _static_enumerate_enum_helper(
        get_enum_list<T>()
    );
    return _static_enumerate_enum_helper;
}

namespace detail
{
template <typename T>
__constexpr void
    create_constructor_exception(
        const enum_list_t<T>&    _a_enum_list,
        const std::u8string_view _a_reason
    )
{
    using namespace errors;
    using namespace std;
    vector<pair<underlying_type_t<T>, u8string_view>> _l_underlying_enum_list;
    ranges::transform(
        _a_enum_list,
        back_inserter(_l_underlying_enum_list),
        [](const auto& _l_enum_str_pair) -> decltype(_l_underlying_enum_list
                                         )::value_type
        {
            return make_pair(
                to_underlying(_l_enum_str_pair.first), _l_enum_str_pair.second
            );
        }
    );
    using namespace abc::utility::printer;
    throw test_library_exception_t(fmt::format(
        u8"An enumerate_enum_helper_t"
        "<{0}> object cannot be "
        "constructed using the enum_list_t argument \"{1}\". Note that in the "
        "proceeding text representation of the enum_list_t argument, the "
        "\"{0}\" "
        "elements have been converted to the "
        "enum's underlying integer type \"{2}\". "
        "The reason the constructor failed is "
        "that {3}.",
        type_id<T>(),
        default_printer_t<decltype(_l_underlying_enum_list)>{}.run_printer(
            _l_underlying_enum_list
        ),
        type_id<underlying_type_t<T>>(),
        _a_reason
    ));
}

__constexpr_imp std::u8string_view
                simple_enum_helper_string_type_to_string(
                    const enum_helper_string_type_e _a_enum
                )
        noexcept(
            unreachable_does_not_throw_exception
        )
{
    using enum enum_helper_string_type_e;
    switch (_a_enum)
    {
    case lower:
        return u8"lower";
    case upper:
        return u8"upper";
    case unchanged:
        return u8"unchanged";
    default:
        abc::unreachable_enum(
            _a_enum, u8"detail::simple_enum_helper_string_type_to_string"
        );
        return u8"unknown";
    }
}
} // namespace detail

_END_ABC_UTILITY_NS

namespace abc::utility
{
template <>
__constexpr auto
    get_enum_list() noexcept -> enum_list_t<enum_helper_string_type_e>
{
    using enum enum_helper_string_type_e;
    return {
        _ENUM_LIST_ENTRY(lower),
        _ENUM_LIST_ENTRY(upper),
        _ENUM_LIST_ENTRY(unchanged)
    };
}
} // namespace abc::utility
