#pragma once
#include "abc_test/core/ds/source/single_source.hpp"
#include "abc_test/core/ds/source/source_pair.hpp"
#include "abc_test/core/errors/test_assertion_exception.hpp"
#include "abc_test/core/matchers/matcher_wrapper.hpp"
#include "abc_test/core/matchers/static_matcher.hpp"
#include "abc_test/core/test_assertions/macro_objects.hpp"
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
        true
    );
    if constexpr (Annotated)
    {
        bba_inner_assertion_type_t _l_tuple(
            ( std::same_as<T, _ABC_NS_REPORTS::terminate_t>
              || std::same_as<T, _ABC_NS_REPORTS::pass_or_terminate_t> ),
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
            ( std::same_as<T, _ABC_NS_REPORTS::terminate_t>
              || std::same_as<T, _ABC_NS_REPORTS::pass_or_terminate_t> ),
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
                const std::optional<std::u8string>& _a_opt_str_msg,
                const std::string_view              _a_str_representation,
                const std::source_location&         _a_source_location
            )
{
    using namespace _ABC_NS_MATCHER;
    matcher_source_map_t _l_msm;
    const bool           _l_pass{std::same_as<T, _ABC_NS_REPORTS::pass_t>};
    global::get_this_threads_current_test().add_current_for_loop_stack_to_trie(
        not _l_pass
    );
    auto ki = global::get_this_threads_test_runner_ref().get_log_infos(false);
    bba_inner_assertion_type_t _l_tuple(
        ( std::same_as<T, _ABC_NS_REPORTS::terminate_t>
          || std::same_as<T, _ABC_NS_REPORTS::pass_or_terminate_t> ),
        _l_pass,
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
        not _a_matcher.matcher_result().passed()
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

class multi_element_test_block_t
{
public:
    _ABC_NS_DS::source_pair_t _m_source;
    std::string               _m_test_annotation;
    bool                      _m_processed;

    inline multi_element_test_block_t(
        const std::string_view             _a_test_annotation,
        const _ABC_NS_DS::single_source_t& _a_source
    ) noexcept
        : _m_source(_ABC_NS_DS::source_pair_t(_a_source))
        , _m_test_annotation(_a_test_annotation)
        , _m_processed(false)
    {}

    /*!
     * @brief Returns a cref to the object's optional test annotation.
     * @return A cref to the object's optional test annotation.
     */
    __constexpr const std::string&
                      test_annotation() const noexcept
    {
        return _m_test_annotation;
    }

    /*!
     * @brief Registers an end source with the object.
     *
     * This should be called by the macro _END_BLOCK.
     *
     * @param _a_end_source The source location being registered to this object.
     */

    __constexpr void
        register_end(
            const _ABC_NS_DS::single_source_t& _a_end_source
        ) noexcept
    {
        _m_source.set_end_source(_a_end_source);
    }

    /*!
     * @brief Gets a cref to the object's ds::source_pair_t.
     * @return A cref to the object's ds::source_pair_t.
     */
    __constexpr const _ABC_NS_DS::source_pair_t&
                      source() const noexcept
    {
        return _m_source;
    }

    /*!
     * @brief Tells the caller whether the object has been processed - that is,
     * the function register_end called on the object.
     * @return True if the object has been processed; false otherwise.
     */
    __constexpr void
        set_processed() noexcept
    {
        _m_processed = true;
    }



    template <typename T2, bool Has_Annotation>
    requires std::derived_from<T2, _ABC_NS_REPORTS::assertion_status_base_t>
    __no_constexpr_or_inline multi_element_test_block_t&
        operator<<(
            MacroAProxy<T2, Has_Annotation>&& _a_proxy
        ) noexcept
    {
        _ABC_NS_MATCHER::bba_inner_assertion_type_t _l_matcher_info{
            _a_proxy.make_bba_inner_assertion_type()
        };
        _m_assertions.push_back(_l_matcher_info);
        if constexpr (std::same_as<T2, _ABC_NS_REPORTS::pass_or_terminate_t>)
        {
            if (_l_matcher_info.matcher_result().passed() == false)
            {
                using namespace _ABC_NS_REPORTS;
                using namespace _ABC_NS_MATCHER;
                assertion_ptr_t<false, T2> _l_gur;
                matcher_res_infos_t        _l_mtr{get_matcher()};
                _l_gur = std::make_unique<multi_element_assertion_block_t<T2>>(
                    _l_matcher_info.matcher_result().passed(),
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

    /*template <typename T2>
    requires std::derived_from<T2, _ABC_NS_REPORTS::assertion_status_base_t>
    __constexpr_imp multi_element_test_block_t&
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
    }*/

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
template <typename T>
    requires std::derived_from<T, _ABC_NS_REPORTS::dynamic_status_t>
struct MacroAProxy3
{
    abc::multi_element_test_block_t _m_matcher;
    std::source_location                            _m_source;
    std::string                                     _m_matcher_str;
    std::string                                     _m_macro_str;

    MacroAProxy3(
        const abc::multi_element_test_block_t& _a_matcher,
        const std::source_location& _a_source,
        const std::string_view                                 _a_matcher_str,
        const std::string_view                                 _a_macro_str
    )
        : _m_matcher(_a_matcher)
        , _m_source(_a_source)
        , _m_macro_str(_a_macro_str)
        , _m_matcher_str(_a_matcher_str)
    {
    }

    inline ~MacroAProxy3()
    {
        using namespace _ABC_NS_REPORTS;
        using namespace _ABC_NS_MATCHER;
        auto _l_test_runner{ _ABC_NS_GLOBAL::get_this_threads_test_runner_ref() };
        _m_matcher.register_end(_ABC_NS_DS::single_source_t(
            _m_macro_str, _m_source
        ));
        _m_matcher.set_processed();
        assertion_ptr_t<false, T> _l_gur;
        bool                      _l_passed{ true };
        for (const bba_inner_assertion_type_t& _l_ki : _m_matcher.get_matcher())
        {
            if (not _l_ki.matcher_result().passed())
            {
                _l_passed = false;
            }
        }
        if (_m_matcher.get_matcher().size() == 0)
        {
            _l_passed = false;
        }
        matcher_res_infos_t _l_mtr{ _m_matcher.get_matcher() };
        _l_gur = make_unique<multi_element_assertion_block_t<T>>(
            _l_passed,
            _m_matcher.source(),
            _l_test_runner.get_log_infos(false),
            _l_mtr,
            _m_matcher.test_annotation(),
            _l_test_runner.current_assertion_index()
        );
        _l_test_runner.add_assertion(_l_gur);
        return_result<T>(_l_passed);
    }
};

template <typename T>
static auto
make_macroAProxy(
    const abc::multi_element_test_block_t& _a_matcher,
    const std::source_location& _a_source,
    const std::string_view                                 _a_matcher_str,
    const std::string_view                                 _a_macro_str
) -> MacroAProxy3<T>
{
    return MacroAProxy3<T>(
        _a_matcher, _a_source, _a_matcher_str, _a_macro_str
    );
}

_END_ABC_NS
