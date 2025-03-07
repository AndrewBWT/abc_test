#pragma once
#include "abc_test/core/ds/source/single_source.hpp"
#include "abc_test/core/test_runner.hpp"
#include "abc_test/core/global.hpp"
#include "abc_test/utility/internal/macros.hpp"

#include <exception>
_BEGIN_ABC_LOGGING_NS

/*!
 * @brief Object used to create and log messages with control structures in
 * abc_test.
 *
 * Specifically, when an object of this instance is created, it is logged with
 * the test_runner_t. Whenever an assertion is then encountered, any log messags
 * which ahve been logged are attached to that assertion.
 *
 * If they are persistant messages, they are attached to every assertion until
 * they go out of scorpe. Otherwise, they are only attached to the first one
 * they encounter.
 */
struct log_msg_t
{
public:
    /*!
     * @brief Constructor.
     * @param _a_single_source The source of the log message.
     * @param _a_str The string associated with the log message.
     * @param _a_delete_after_use Boolean denoting whether this is a persistent
     * (false) or not (true) log message.
     * @return
     */
    __no_constexpr
        log_msg_t(
            const ds::single_source_t& _a_single_source,
            const std::string_view     _a_str,
            const bool                 _a_delete_after_use
        ) noexcept;
    /*!
     * @brief Destructor for log_msg_t.
     *
     * This control the logic for when it is a mesage which only stops being
     * used after it is deleted.
     *
     * It is also used when an unexpected exception is thrown by the testing
     * library.
     *
     * @return
     */
    __no_constexpr ~log_msg_t() noexcept;
    /*!
     * @brief Returns a cref to the string component of the log_msg_t object.
     * @return A cref to the string component of the object.
     */
    __constexpr const std::string_view
                      str() const noexcept;
    /*!
     * @brief Returns true if this message should be deleted after its first
     * use; false otherwise.
     * @return Bool denoting whether this object should continue to be used
     * after its first use.
     */
    __constexpr bool
        delete_after_use() const noexcept;
protected:
    std::string   _m_str;
    log_msg_itt_t _m_iterator;
    bool          _m_delete_after_use;
};

/*!
 * @brief Function used to print an arbitrary variable, when also given a
 * string.
 *
 * @tparam The type to print. If it is fmt::formattable, it prints the the
 * element. Otherwise it prints "[?]".
 *
 * @param _a_str_representation The string representation of the variable.
 * @param The variable itself.
 * @return A string representation in the form "_a_str_represntation =
 * fmt::format({0},_a_value)".
 */
template <typename T>
__constexpr std::string
    fmt_format(const std::string_view _a_str_representation, const T& _a_value)
        noexcept;

_END_ABC_LOGGING_NS

_BEGIN_ABC_LOGGING_NS

__no_constexpr_imp
    log_msg_t::log_msg_t(
        const ds::single_source_t& _a_single_source,
        const std::string_view     _a_str,
        const bool                 _a_delete_after_use
    ) noexcept
    : _m_str(_a_str)
    , _m_iterator(global::get_this_threads_test_runner_ref()
                      .add_log_msg(std::ref(*this), _a_single_source))
    , _m_delete_after_use(_a_delete_after_use)
{}

__no_constexpr_imp log_msg_t::~log_msg_t() noexcept
{
    using namespace std;
    using namespace global;
    if (uncaught_exceptions() > 0)
    {
        get_this_threads_test_runner_ref().add_cached_log_msg(_m_str);
    }
    if (not _m_delete_after_use)
    {
        get_this_threads_test_runner_ref().remove_log_msg(_m_iterator);
    }
}

__constexpr_imp const std::string_view
                      log_msg_t::str() const noexcept
{
    return _m_str;
}

__constexpr_imp bool
    log_msg_t::delete_after_use() const noexcept
{
    return _m_delete_after_use;
}

template <typename T>
__constexpr std::string
            fmt_format(
                const std::string_view _a_str_representation,
                const T&               _a_value
            ) noexcept
{
    if constexpr (fmt::formattable<T>)
    {
        return fmt::format("{0} = {1}", _a_str_representation, _a_value);
    }
    else
    {
        return fmt::format("{0} = [?]", _a_str_representation);
    }
}

_END_ABC_LOGGING_NS