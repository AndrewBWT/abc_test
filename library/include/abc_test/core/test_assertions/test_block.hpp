#pragma once
#include "abc_test/core/ds/source/single_source.hpp"
#include "abc_test/core/ds/source/source_pair.hpp"
#include "abc_test/core/errors/test_assertion_exception.hpp"
#include "abc_test/core/matchers/matcher_wrapper.hpp"
#include "abc_test/core/matchers/static_matcher.hpp"
#include "abc_test/core/test_reports/assertion_status/pass_or_fail.hpp"
#include "abc_test/core/test_reports/assertion_status/pass_or_terminate.hpp"
#include "abc_test/core/test_reports/assertion_status/terminate.hpp"
#include "abc_test/core/test_reports/multi_element_assertion_block.hpp"
#include "abc_test/utility/do_not_optimise.hpp"
#include "abc_test/utility/internal/macros.hpp"

#include <optional>

_BEGIN_ABC_NS
using test_block_matcher_element_t
    = std::pair<std::optional<std::string>, _ABC_NS_MATCHER::matcher_result_t>;
using test_block_matcher_elements_t = std::vector<test_block_matcher_element_t>;

/*!
 * @brief Structure used when creating test blocks; testing apparatus in which
 * an assertion is declared in a block, and its only checked when destroyed.
 * @tparam Assertion_Type The assertion type. Must be derived from
 * dynamic_status_t.
 */
// template <typename Assertion_Type>
// requires std::derived_from<Assertion_Type, _ABC_NS_REPORTS::dynamic_status_t>

template <typename T>
requires std::derived_from<T, _ABC_NS_REPORTS::assertion_status_base_t>
struct assertion_wp_t
{
    _ABC_NS_MATCHER::bba_inner_assertion_type_t _m_matcher_info;
};

// template <typename T>
// requires std::derived_from<T, _ABC_NS_REPORTS::dynamic_status_t>
class test_block_t
{
public:
    __constexpr
    test_block_t() noexcept
        = delete;
    /*!
     * @brief Constructor
     * @param _a_test_annotation The block's annotation.
     * @param _a_source The blocks' beginning source.
     */
    __no_constexpr
        test_block_t(
            const std::string_view             _a_test_annotation,
            const _ABC_NS_DS::single_source_t& _a_source
        ) noexcept;
    /*!
     * @brief Destructor.
     *
     * The destructor will issue a warning to the test_runner_t if its matcher
     * has not been checked. If the user is using the macros, this will be done
     * when ending a test block.
     */
    __constexpr ~test_block_t() noexcept;
    /*!
     * @brief Equals operator taking a matcher_wrapper_t. This is used when
     * wanting to attach a matcher_wrapper_t to this object.
     * @tparam Boolean denoting whether the argument matcher_wrapper_t has an
     * annotation.
     * @param _a_element The matcher_wrapper_t to attach to this object.
     * @return Reference to test_block_t with matcher_wrapper_t attached.
     */
    // __constexpr_imp test_block_t&
    // operator=(
    // const assertion_wp_t& _a_element
    // ) noexcept;
    /*!
     * @brief Returns a cref to the object's matcher_base_ptr_t to the caller.
     * @return A cref to the object's matcher_base_ptr_t.
     */
    // __constexpr const test_block_matcher_elements_t&
    //                   get_results() const noexcept;
    // __constexpr const _ABC_NS_MATCHER::matcher_source_map_t&
    // map_source() const noexcept;
    /*!
     * @brief Returns a cref to the object's optional test annotation.
     * @return A cref to the object's optional test annotation.
     */
    __constexpr const std::string&
                      test_annotation() const noexcept;
    /*!
     * @brief Registers an end source with the object.
     *
     * This should be called by the macro _END_BLOCK.
     *
     * @param _a_end_source The source location being registered to this object.
     */

    __constexpr void
        register_end(const _ABC_NS_DS::single_source_t& _a_end_source) noexcept;
    /*!
     * @brief Gets a cref to the object's ds::source_pair_t.
     * @return A cref to the object's ds::source_pair_t.
     */
    __constexpr const _ABC_NS_DS::source_pair_t&
                      source() const noexcept;
    /*!
     * @brief Tells the caller whether the object has been processed - that is,
     * the function register_end called on the object.
     * @return True if the object has been processed; false otherwise.
     */
    __constexpr void
        set_processed() noexcept;
private:
    _ABC_NS_DS::source_pair_t _m_source;
    std::string               _m_test_annotation;
    // assertion_wp_t         _m_matchers;
    // _ABC_NS_MATCHER::matcher_source_map_t _m_map_source;
    bool _m_processed;
};

// template <typename T>
// requires std::derived_from<T, _ABC_NS_REPORTS::dynamic_status_t>
class single_element_test_block_t : public test_block_t
{
public:
    using test_block_t::test_block_t;

    template <typename T2>
    requires std::derived_from<T2, _ABC_NS_REPORTS::dynamic_status_t>
    __no_constexpr_imp test_block_t&
        operator=(
            const assertion_wp_t<T2>& _a_element
        ) noexcept
    {
        _m_assertion = _a_element._m_matcher_info;
        return *this;
    }

    __constexpr_imp const _ABC_NS_MATCHER::bba_inner_assertion_type_t&
                          get_matcher() const noexcept
    {
        return _m_assertion;
    }
private:
    _ABC_NS_MATCHER::bba_inner_assertion_type_t _m_assertion;
};

template <typename T>
requires std::derived_from<T, _ABC_NS_REPORTS::dynamic_status_t>
__constexpr assertion_wp_t<_ABC_NS_REPORTS::pass_or_fail_t>
            make_entity_bba_cmpatable(
                const test_block_t& _a_matcher
            )
{
    using namespace _ABC_NS_MATCHER;

    bba_inner_assertion_type_t _l_tuple;
    // get<0>(_l_tuple.second) =
    // _a_matcher.internal_matcher().get()->matcher_result();
    // if constexpr (Annotated)
    // {
    //    get<1>(_l_tuple.second) = _a_matcher.annotation();
    // }
    // _a_matcher.internal_matcher()->gather_map_source(get<2>(_l_tuple.second));
    return assertion_wp_t<_ABC_NS_REPORTS::pass_or_fail_t>{_l_tuple};
}

template <typename T, bool Annotated>
requires std::derived_from<T, _ABC_NS_REPORTS::dynamic_status_t>
__constexpr assertion_wp_t<_ABC_NS_REPORTS::pass_or_fail_t>
            make_entity_bba_compatable(
                const _ABC_NS_MATCHER::matcher_wrapper_t<Annotated>& _a_matcher
            )
{
    using namespace _ABC_NS_MATCHER;
    matcher_source_map_t _l_msm;
    _a_matcher.gather_map_source(_l_msm);
    global::get_this_threads_current_test().add_current_for_loop_stack_to_trie(
    );
    if constexpr (Annotated)
    {
        bba_inner_assertion_type_t _l_tuple(
            (std::same_as<T, _ABC_NS_REPORTS::terminate_t>
                || std::same_as<T, _ABC_NS_REPORTS::pass_or_terminate_t>),
            _a_matcher.matcher_result(),
            std::optional<ds::single_source_t>{},
            std::optional<std::string>(_a_matcher.annotation()),
            _l_msm
        );
        return assertion_wp_t<_ABC_NS_REPORTS::pass_or_fail_t>{_l_tuple};
    }
    else
    {
        bba_inner_assertion_type_t _l_tuple(
            (std::same_as<T, _ABC_NS_REPORTS::terminate_t>
                || std::same_as<T, _ABC_NS_REPORTS::pass_or_terminate_t>),
            _a_matcher.matcher_result(),
            std::optional<ds::single_source_t>{},
            std::optional<std::string>{},
            _l_msm
        );
        return assertion_wp_t<_ABC_NS_REPORTS::pass_or_fail_t>{_l_tuple};
    }
}

template <typename T>
requires std::derived_from<T, _ABC_NS_REPORTS::static_status_t>
__constexpr assertion_wp_t<T>
            make_entity_bba_compatable(
                const bool                        _a_pass,
                const std::optional<std::u8string>& _a_opt_str_msg,
                const std::string_view            _a_str_representation,
                const std::source_location&       _a_source_location
            )
{
    using namespace _ABC_NS_MATCHER;
    matcher_source_map_t _l_msm;
    global::get_this_threads_current_test().add_current_for_loop_stack_to_trie(
    );
    auto ki = global::get_this_threads_test_runner_ref().get_log_infos(false);
    bba_inner_assertion_type_t _l_tuple(
        ( std::same_as<T, _ABC_NS_REPORTS::terminate_t>
          || std::same_as<T, _ABC_NS_REPORTS::pass_or_terminate_t> ),
        _a_pass,
        _a_opt_str_msg,
        ki,
        ds::single_source_t(_a_str_representation, _a_source_location)
    );
    return assertion_wp_t<T>{_l_tuple};
}

template <typename T, bool Annotated>
requires std::derived_from<T, _ABC_NS_REPORTS::dynamic_status_t>
__constexpr assertion_wp_t<T>
            make_entity_bba_compatable(
                const _ABC_NS_MATCHER::matcher_wrapper_t<Annotated>& _a_matcher,
                const std::string_view      _a_str_representation,
                const std::source_location& _a_source_location
            )
{
    using namespace _ABC_NS_MATCHER;
    matcher_source_map_t _l_msm;
    global::get_this_threads_current_test().add_current_for_loop_stack_to_trie(
    );
    auto ki = global::get_this_threads_test_runner_ref().get_log_infos(false);
    _a_matcher.gather_map_source(_l_msm);
    if constexpr (Annotated)
    {
        bba_inner_assertion_type_t _l_tuple(
            ( std::same_as<T, _ABC_NS_REPORTS::terminate_t>
              || std::same_as<T, _ABC_NS_REPORTS::pass_or_terminate_t> ),
            _a_matcher.matcher_result(),
            ki,
            ds::single_source_t(_a_str_representation, _a_source_location),
            std::optional<std::u8string>(_a_matcher.annotation()),
            _l_msm
        );
        return assertion_wp_t<T>{_l_tuple};
    }
    else
    {
        bba_inner_assertion_type_t _l_tuple(
            ( std::same_as<T, _ABC_NS_REPORTS::terminate_t>
              || std::same_as<T, _ABC_NS_REPORTS::pass_or_terminate_t> ),
            _a_matcher.matcher_result(),
            ki,
            ds::single_source_t(_a_str_representation, _a_source_location),
            std::optional<std::u8string>{},
            _l_msm
        );
        return assertion_wp_t<T>{_l_tuple};
    }
}


class multi_element_test_block_t : public test_block_t //<T>
{
public:
    using test_block_t::test_block_t;

    template <typename T2>
    requires std::derived_from<T2, _ABC_NS_REPORTS::assertion_status_base_t>
    __constexpr_imp test_block_t&
        operator+=(
            const assertion_wp_t<T2>& _a_element
        )
    {
        _m_assertions.push_back(_a_element._m_matcher_info);
        if constexpr (std::same_as<T2, _ABC_NS_REPORTS::pass_or_terminate_t>)
        {
            if (_a_element._m_matcher_info.matcher_result().passed() == false)
            {
                using namespace _ABC_NS_REPORTS;
                using namespace _ABC_NS_MATCHER;
                assertion_ptr_t<false, T2> _l_gur;
                bool                       _l_passed{true};
                matcher_res_infos_t        _l_mtr{get_matcher()};
                _l_gur = std::make_unique<multi_element_assertion_block_t<T2>>(
                    _a_element._m_matcher_info.matcher_result().passed(),
                    source(),
                    global::get_this_threads_test_runner_ref().get_log_infos(
                        false
                    ),
                    _l_mtr,
                    test_annotation()
                );
                global::get_this_threads_test_runner_ref().add_assertion(_l_gur
                );
                throw _ABC_NS_ERRORS::test_assertion_exception_t();
            }
        }
        else if constexpr (std::same_as<T2, _ABC_NS_REPORTS::terminate_t>)
        {
            using namespace _ABC_NS_REPORTS;
            using namespace _ABC_NS_MATCHER;
            assertion_ptr_t<false, T2> _l_gur;
            bool                       _l_passed{true};
            matcher_res_infos_t        _l_mtr{get_matcher()};
            _l_gur = std::make_unique<multi_element_assertion_block_t<T2>>(
                std::monostate(),
                source(),
                global::get_this_threads_test_runner_ref().get_log_infos(false),
                _l_mtr,
                test_annotation()
            );
            global::get_this_threads_test_runner_ref().add_assertion(_l_gur);
            throw _ABC_NS_ERRORS::test_assertion_exception_t();
        }
        return *this;
    }

    __constexpr_imp const _ABC_NS_MATCHER::matcher_res_infos_t&
                          get_matcher() const noexcept
    {
        return _m_assertions;
    }
private:
    _ABC_NS_MATCHER::matcher_res_infos_t _m_assertions;
};

_END_ABC_NS

_BEGIN_ABC_NS

__no_constexpr_imp
    test_block_t::test_block_t(
        const std::string_view             _a_test_annotation,
        const _ABC_NS_DS::single_source_t& _a_source
    ) noexcept
    : _m_source(_ABC_NS_DS::source_pair_t(_a_source))
    , _m_test_annotation(_a_test_annotation)
    , _m_processed(false)
{}

__constexpr_imp test_block_t::~test_block_t() noexcept
{
    using namespace _ABC_NS_GLOBAL;
    using namespace std;
    using namespace _ABC_NS_REPORTS;
    if (not _m_processed)
    {
        // get_this_threads_test_runner_ref().add_text_warning(fmt::format(
        //     "test_block_t is being deleted, however it has not been "
        // "processed for its true or false status. "
        // ));
    }
}

/*__constexpr_imp test_block_t&
                test_block_t::operator=(
        const assertion_wp_t& _a_element
    ) noexcept
{
    _m_map_source.clear();
    _a_element.internal_matcher()->gather_map_source(_m_map_source);
    std::pair<std::optional<std::string>, _ABC_NS_MATCHER::matcher_result_t>
        _l_this_result{};
    _l_this_result.second = _a_element.internal_matcher()->matcher_result();
    if constexpr (Has_Annotation)
    {
        _l_this_result.first
            = std::optional<std::string>(_a_element.annotation());
    }
    _m_matchers.push_back(_l_this_result);
    return *this;
}*/

/*template <typename Assertion_Type>
requires std::derived_from<Assertion_Type, _ABC_NS_REPORTS::dynamic_status_t>
__constexpr_imp const test_block_matcher_elements_t&
    test_block_t<Assertion_Type>::get_results() const noexcept
{
    return _m_matchers;
}

__constexpr_imp const _ABC_NS_MATCHER::matcher_source_map_t&
                      test_block_t::map_source() const noexcept
{
    return _m_map_source;
}*/

__constexpr_imp const std::string&
                      test_block_t::test_annotation() const noexcept
{
    return _m_test_annotation;
}

__constexpr_imp void
    test_block_t::register_end(
        const _ABC_NS_DS::single_source_t& _a_end_source
    ) noexcept
{
    _m_source.set_end_source(_a_end_source);
}

__constexpr_imp const _ABC_NS_DS::source_pair_t&
                      test_block_t::source() const noexcept
{
    return _m_source;
}

__constexpr_imp void
    test_block_t::set_processed() noexcept
{
    _m_processed = true;
}

_END_ABC_NS
