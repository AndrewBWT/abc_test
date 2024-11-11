#pragma once
#include <optional>
#include <string>

// #include "abc_test/internal/test_runner.hpp"
#include "abc_test/internal/matchers/matcher_result.hpp"
#include "abc_test/internal/matchers/source_map.hpp"

#include <map>

_BEGIN_ABC_NS
// Forward declaration
struct test_runner_t;
_END_ABC_NS
_BEGIN_ABC_MATCHER_NS

/*!
 * @brief The abstract class for all matchers.
 */
struct matcher_base_t
{
public:
    /*!
     * @brief Default constructor.
     */
    __constexpr
    matcher_base_t() noexcept
        = delete;
    __constexpr
        matcher_base_t(const matcher_result_t& _a_matcher_result,
            const std::vector<ds::single_source_t>& _a_sources = std::vector<ds::single_source_t>()) noexcept
        : _m_test_result(_a_matcher_result)
        , _m_sources(_a_sources)
    {
        
    }
    /*!
     * @brief Function runs the test and returns a matcher_result_t to the
     * caller.
     * @param _a_test_runner The thread's test_runner_t object.
     * @return A cref to the matcher_result_t.
     */
  //  __constexpr const matcher_result_t&
  //      run_test(test_runner_t& _a_test_runner);
    /*!
     * @brief Returns the object's matcher_result_t to the caller.
     * @return A cref to the object's matcher_result_t.
     */
    __constexpr const matcher_result_t&
        matcher_result() const noexcept;
   // __constexpr virtual matcher_result_t generate_matcher_result() const noexcept = 0;
    /*!
     * @brief Fills the matcher_source_map_t argument with the object's sources.
     * @param _a_matcher_source_map The matcher_source_map_t which is filled
     * with the object's sources.
     */
    __constexpr virtual void
        gather_map_source(matcher_source_map_t& _a_matcher_source_map
        ) const noexcept;
    /*!
     * @brief Adds a source to this matcher.
     * @param _a_source The source to add to the matcher.
     */
    __constexpr void
        add_source_info(const ds::single_source_t& _a_source) noexcept;
    __constexpr const std::vector<ds::single_source_t>&
        get_sources() const noexcept;
protected:
    matcher_result_t                 _m_test_result;
    std::vector<ds::single_source_t> _m_sources;
private:
  //  __constexpr virtual matcher_result_t
  //      run(test_runner_t& _a_test_runner)
  //      = 0;
};

_END_ABC_MATCHER_NS

_BEGIN_ABC_MATCHER_NS

/*__constexpr_imp const matcher_result_t&
    matcher_base_t::run_test(
        test_runner_t& _a_test_runner
    )
{
    if (not _m_test_result.ran())
    {
        _m_test_result = run(_a_test_runner);
    }
    return _m_test_result;
}*/

__constexpr_imp const matcher_result_t&
    matcher_base_t::matcher_result() const noexcept
{
    return _m_test_result;
}

__constexpr_imp void
    matcher_base_t::gather_map_source(
        matcher_source_map_t& _a_matcher_source_map
    ) const noexcept
{
    using namespace ds;
    for (const single_source_t& _l_source : _m_sources)
    {
        _a_matcher_source_map.insert(_l_source);
    }
}

__constexpr_imp void
    matcher_base_t::add_source_info(
        const ds::single_source_t& _a_source
    ) noexcept
{
    _m_sources.push_back(_a_source);
}
__constexpr_imp const std::vector<ds::single_source_t>&
matcher_base_t::get_sources() const noexcept
{
    return _m_sources;
}

_END_ABC_MATCHER_NS