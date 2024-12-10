#pragma once

#include "abc_test/internal/ds/source/single_source.hpp"
#include "abc_test/internal/utility/internal/macros.hpp"
#include "abc_test/internal/utility/str/string_utility.hpp"

#include <fmt/ranges.h>
#include <fmt/std.h>
#include <map>
#include <source_location>

_BEGIN_ABC_MATCHER_NS

/*!
 * @brief Object used to compare std::source_locations.
 *
 */
struct source_location_lt_t
{
    /*!
     * @brief Operator which compares two std::source_locations.
     * @param _a_lhs The left-most argument.
     * @param _a_rhs The right-most argument.
     * @return True if _a_lhs < _a_rhs. False otherwise.
     */
    __constexpr bool
        operator()(
            const std::source_location& _a_lhs,
            const std::source_location& _a_rhs
        ) const noexcept;
};

/*!
 * @brief Objct which processes single_source elements and links single source
 * locations with lists of source string represetnations.
 */
class matcher_source_map_t
{
public:
    /*!
     * @brief Inserts a single_source_t into this object.
     * @param _a_source The single_source_t to insert.
     */
    __no_constexpr void
        insert(const ds::single_source_t& _a_source);
    /*!
     * @brief Returns a cref to the object's internal map.
     * @return A cref to the object's internal map.
     */
    __constexpr const std::map<
        std::source_location,
        std::vector<std::string>,
        source_location_lt_t>&
        map() const noexcept;
    /*!
     * @brief Tells the caller whether the internal map is emtpy or not.
     * @return False if the map is empty; true otherwise.
     */
    __constexpr bool
        has_elements() const noexcept;
    /*!
     * @brief Gathers a string which represents this object.
     *
     * This string is used for output, hence we do not use a fmt representation.
     *
     * @return String representation of this source_map_t object.
     */
    __no_constexpr_or_inline std::string
        gather_list_of_sources_and_representations() const noexcept;

    __no_constexpr_imp void
        clear()
    {
        _m_internal_map.clear();
        _m_has_elements = false;
    }

    __no_constexpr bool
        operator==(const matcher_source_map_t& _a_msm) const noexcept;
private:
    bool _m_has_elements = false;
    std::map<
        std::source_location,
        std::vector<std::string>,
        source_location_lt_t>
        _m_internal_map;
    // Can't be constexpr as changes map.
    __no_constexpr void
        insert_source(const std::source_location& _a_sl) noexcept;
};

_END_ABC_MATCHER_NS

/*!
 * formatter for post_setup_test_ata object.
 */
template <>
struct fmt::formatter<_ABC_NS_MATCHER::matcher_source_map_t>
    : formatter<string_view>
{
    /*!
     * Provides a formatter for a poset_setup_test_data_t object
     */
    // Can't be constexpr due to fmt.
    __no_constexpr auto
        format(
            _ABC_NS_MATCHER::matcher_source_map_t _a_iti,
            format_context&                       _a_cxt
        ) const -> format_context::iterator;
};

_BEGIN_ABC_MATCHER_NS

__constexpr_imp bool
    source_location_lt_t::operator()(
        const std::source_location& _a_lhs,
        const std::source_location& _a_rhs
    ) const noexcept
{
    using namespace std;
    const string_view _l_lhs_fin{_a_lhs.file_name()};
    const string_view _l_rhs_fin{_a_rhs.file_name()};
    if (_l_lhs_fin != _l_rhs_fin)
    {
        return _l_lhs_fin < _l_rhs_fin;
    }
    else
    {
        const string_view _l_lhs_fn{_a_lhs.file_name()};
        const string_view _l_rhs_fn{_a_rhs.file_name()};
        if (_l_lhs_fn != _l_rhs_fn)
        {
            return _l_lhs_fn < _l_rhs_fn;
        }
        else
        {
            if (_a_lhs.line() != _a_rhs.line())
            {
                return _a_lhs.line() < _a_rhs.line();
            }
            else
            {
                return _a_lhs.column() < _a_rhs.column();
            }
        }
    }
}

__no_constexpr_imp void
    matcher_source_map_t::insert(
        const ds::single_source_t& _a_source
    )
{
    using namespace std;
    using namespace ds;
    _m_has_elements = true;
    insert_source(_a_source.source_location());
    _m_internal_map.at(_a_source.source_location())
        .push_back(string(_a_source.source_code_representation()));
}

__constexpr_imp const std::
    map<std::source_location, std::vector<std::string>, source_location_lt_t>&
    matcher_source_map_t::map() const noexcept
{
    return _m_internal_map;
}

__constexpr_imp bool
    matcher_source_map_t::has_elements() const noexcept
{
    return _m_has_elements;
}

__no_constexpr_imp bool
    matcher_source_map_t::operator==(
        const matcher_source_map_t& _a_msm
    ) const noexcept
{
    using namespace std;

    std::map<source_location, vector<string>, source_location_lt_t> _l_msm_map{
        _a_msm.map()
    };
    if (_m_internal_map.size() != _l_msm_map.size())
    {
        return false;
    }
    else
    {
        for (const pair<source_location, vector<string>>& _l_element :
             _m_internal_map)
        {
            if (not _l_msm_map.contains(_l_element.first))
            {
                return false;
            }
            else
            {
                const vector<string>& _l_msm_element{ _l_msm_map.find(_l_element.first)->second };
                if (_l_msm_element != _l_element.second)
                {
                    return false;
                }
            }
        }
        return true;
    }
    return true;
}

__no_constexpr_imp void
    matcher_source_map_t::insert_source(
        const std::source_location& _a_sl
    ) noexcept
{
    if (not _m_internal_map.contains(_a_sl))
    {
        _m_internal_map.insert({_a_sl, {}});
    }
}

_END_ABC_MATCHER_NS

__no_constexpr_imp auto
    fmt::formatter<_ABC_NS_MATCHER::matcher_source_map_t>::format(
        _ABC_NS_MATCHER::matcher_source_map_t _a_rtd,
        format_context&                       _a_ctx
    ) const -> format_context::iterator
{
    size_t _m_thread_resourses_required;
    using namespace std;
    string _l_rv{fmt::format(
        "{0} {{"
        "{1} = {2}"
        "}}",
        typeid(_a_rtd).name(),
        "_m_internal_map",
        _a_rtd.map()
    )};
    return formatter<string_view>::format(_l_rv, _a_ctx);
}