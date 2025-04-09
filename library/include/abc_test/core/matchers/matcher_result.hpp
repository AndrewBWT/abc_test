#pragma once

#include "abc_test/core/matchers/source_map.hpp"
#include "abc_test/utility/internal/macros.hpp"
#include "abc_test/core/ds/type_synonyms.hpp"

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
        const std::u8string_view _a_str
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
    __constexpr const std::u8string_view
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
    std::u8string _m_str    = u8"false";
};
enum class enum_bba_inner_assertion_type_t
{
    STATIC_ASSERTION,
    MATCHER_BASED_ASSERTION
};

__constexpr std::u8string
            get_str(
                const enum_bba_inner_assertion_type_t _a_enum
            )
{
    using enum enum_bba_inner_assertion_type_t;
    switch (_a_enum)
    {
    case STATIC_ASSERTION:
        return u8"static";
    case MATCHER_BASED_ASSERTION:
        return u8"matcher-based";
    default:
        throw errors::unaccounted_for_enum_exception(_a_enum);
    }
}

class bba_inner_assertion_type_t
{
public:
    __no_constexpr_imp
        bba_inner_assertion_type_t() noexcept
        : bba_inner_assertion_type_t(
              false,
              matcher_result_t(),
              _ABC_NS_DS::log_infos_t{},
              std::optional<ds::single_source_t>(),
              std::optional<std::u8string>(),
              matcher_source_map_t()
          )
    {}

    __no_constexpr_imp
        bba_inner_assertion_type_t(
            const bool                                _a_terminate,
            const bool                                _a_pass_or_failure,
            const std::optional<std::u8string>&         _a_opt_msg,
            const _ABC_NS_DS::log_infos_t&            _a_log_infos,
            const std::optional<ds::single_source_t>& _a_source
            = std::optional<ds::single_source_t>{}
        ) noexcept
        : _m_matcher_result(matcher_result_t(_a_pass_or_failure, u8""))
        , _m_annotation(_a_opt_msg)
        , _m_source(_a_source)
        , _m_source_map(matcher_source_map_t{})
        , _m_enum_bba_inner_assertion_type(
              enum_bba_inner_assertion_type_t::STATIC_ASSERTION
          )
        , _m_terminate(_a_terminate)
        , _m_log_infos(_a_log_infos)
    {}

    __no_constexpr_imp
        bba_inner_assertion_type_t(
            const bool                                _a_terminate,
            const matcher_result_t&                   _a_matcher_result,
            const _ABC_NS_DS::log_infos_t&            _a_log_infos,
            const std::optional<ds::single_source_t>& _a_source
            = std::optional<ds::single_source_t>{},
            const std::optional<std::u8string>& _a_annotation
            = std::optional<std::u8string>{},
            const matcher_source_map_t& _a_matcher_source_map
            = matcher_source_map_t()
        ) noexcept
        : _m_matcher_result(_a_matcher_result)
        , _m_annotation(_a_annotation)
        , _m_source(_a_source)
        , _m_source_map(_a_matcher_source_map)
        , _m_enum_bba_inner_assertion_type(
              enum_bba_inner_assertion_type_t::MATCHER_BASED_ASSERTION
          )
        , _m_terminate(_a_terminate)
        , _m_log_infos(_a_log_infos)
    {}

    __no_constexpr_imp
        bba_inner_assertion_type_t(
            const _ABC_NS_DS::log_infos_t&            _a_log_infos,
            const bool                                _a_terminate,
            const matcher_result_t&                   _a_matcher_result,
            const std::optional<ds::single_source_t>& _a_source,
            const std::optional<std::u8string>&         _a_annotation,
            const matcher_source_map_t&               _a_matcher_source_map,
            const enum_bba_inner_assertion_type_t
                _a_enum_bba_inner_assertion_type
        ) noexcept
        : _m_matcher_result(_a_matcher_result)
        , _m_annotation(_a_annotation)
        , _m_source(_a_source)
        , _m_source_map(_a_matcher_source_map)
        , _m_enum_bba_inner_assertion_type(_a_enum_bba_inner_assertion_type)
        , _m_terminate(_a_terminate)
        , _m_log_infos(_a_log_infos)
    {}

    __constexpr_imp const matcher_result_t&
        matcher_result() const noexcept
    {
        return _m_matcher_result;
    }

    __constexpr_imp const std::optional<std::u8string>&
                          annotation() const noexcept
    {
        return _m_annotation;
    }

    __constexpr_imp const matcher_source_map_t&
        source_map() const noexcept
    {
        return _m_source_map;
    }

    __constexpr_imp const std::optional<ds::single_source_t>&
                          source() const noexcept
    {
        return _m_source;
    }

    __constexpr_imp enum_bba_inner_assertion_type_t
        assertion_type() const noexcept
    {
        return _m_enum_bba_inner_assertion_type;
    }

    __constexpr_imp bool
        terminate() const noexcept
    {
        return _m_terminate;
    }

    __constexpr_imp const _ABC_NS_DS::log_infos_t&
                          log_infos() const noexcept
    {
        return _m_log_infos;
    }
private:
    _ABC_NS_DS::log_infos_t            _m_log_infos;
    matcher_result_t                   _m_matcher_result;
    std::optional<std::u8string>         _m_annotation;
    matcher_source_map_t               _m_source_map;
    std::optional<ds::single_source_t> _m_source;
    enum_bba_inner_assertion_type_t    _m_enum_bba_inner_assertion_type;
    bool                               _m_terminate;
};

// using matcher_result_with_annotation_and_source_info_t = std::tuple<
//    matcher_result_t,
//   std::optional<std::string>,
//  matcher_source_map_t>;
// using matcher_res_info_with_caller_t =
// std::pair<std::optional<ds::single_source_t>,
// matcher_result_with_annotation_and_source_info_t>;
using matcher_res_infos_t = std::vector<bba_inner_assertion_type_t>;
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
        const std::u8string_view _a_str
    ) noexcept
    //: _m_ran(_a_ran)
    : _m_passed(_a_passed), _m_str(_a_str)
{}

__constexpr_imp bool
    matcher_result_t::passed() const noexcept
{
    return _m_passed;
}

__constexpr_imp const std::u8string_view
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