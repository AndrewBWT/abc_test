#pragma once

#include "abc_test/internal/utility/internal/macros.hpp"
#include "abc_test/internal/matchers/source_map.hpp"
#include <vector>

_BEGIN_ABC_MATCHER_NS

/*!
 * @brief Structure holding a matcher's result.
 */
class matcher_result_t
{
public:
    /*!
     * @brief Default constructor
     */
    __constexpr
    matcher_result_t() noexcept
        = default;
    /*!
     * @brief Constructor.
     * @param _a_ran Whether the matcher has been ran.
     * @param _a_passed Whether the matcher passed.
     * @param _a_str The string describing the matcher.
     */
    __constexpr
    matcher_result_t(
        // const bool             _a_ran,
        const bool             _a_passed,
        const std::string_view _a_str
    ) noexcept;
    /*!
     * @brief Tells the caller whether the associated mathcer_t passed.
     * @return True if the matcher_t passed; false otherwise.
     */
    __constexpr bool
        passed() const noexcept;
    /*!
     * @brief Gets a cref to the string associated with this matcher_result_t.
     * @return A cref to the internal object's string.
     */
    __constexpr const std::string_view
                      str() const noexcept;
    /*!
     * @brief Tells the caller whether the matcher_t this matcher_result_t is
     * associated with has been ran.
     * @return True if the matcher_t has been ran; false otherwise.
     */
    //__constexpr bool
    // ran() const noexcept;
private:
    // bool        _m_ran    = false;
    bool        _m_passed = false;
    std::string _m_str;
};
using matcher_res_info_t = std::tuple<matcher_result_t, std::optional<std::string>,matcher_source_map_t>;
using matcher_res_infos_t = std::vector<matcher_res_info_t>;
/*class assertion_wrapper_pc_t
{

};

template<typename T>
    requires std::derived_from < T, _ABC_NS_REPORTS::dynamic_status_t>
class assertion_wrapper_t : public assertion_wrapper_pc_t
{

};

using assertion_wp_t = std::shared_ptr<assertion_wrapper_pc_t>;

template<bool Annotated>
__constexpr assertion_wp_t make_block_check_matcher(
    const _ABC_NS_MATCHER::matcher_wrapper_t <Annotated>& _a_matcher,
    const std::string_view _a_str_representation
)
{
    return assertion_wp_t();
}*/
_END_ABC_MATCHER_NS

_BEGIN_ABC_MATCHER_NS
__constexpr_imp
    matcher_result_t::matcher_result_t(
        // const bool             _a_ran,
        const bool             _a_passed,
        const std::string_view _a_str
    ) noexcept
    //: _m_ran(_a_ran)
    : _m_passed(_a_passed), _m_str(_a_str)
{}

__constexpr_imp bool
    matcher_result_t::passed() const noexcept
{
    return _m_passed;
}

__constexpr_imp const std::string_view
                      matcher_result_t::str() const noexcept
{
    return _m_str;
}

//__constexpr_imp bool
 //   matcher_result_t::ran() const noexcept
//{
 //   return _m_ran;
//}

_END_ABC_MATCHER_NS