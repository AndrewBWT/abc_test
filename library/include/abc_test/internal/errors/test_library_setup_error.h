#pragma once
#include "abc_test/internal/utility/internal/macros.h"

#include <source_location>
#include <stacktrace>

_BEGIN_ABC_ERRORS_NS

/*!
 * @brief Structure which holds errors found when setting up the test suite.
 *
 * To be clear, these are not exceptions; they are errors which do not break the
 * logic of the program. For example, a test which may not adhere to some
 * pre-condition.
 */
struct setup_error_t
{
public:
    __constexpr
    setup_error_t() noexcept
        = delete;
    /*!
     * @brief Constructor. This does not include a stacktrace.
     * @param _a_error_msg The message associated with the error.
     * @param _a_unrecoverable_error Whether it is an unrecoverable error - and
     * would require termination.
     * @param _a_source_location The source location of the error.
     */
    __constexpr
    setup_error_t(
        const std::string_view      _a_error_msg,
        const bool                  _a_unrecoverable_error,
        const std::source_location& _a_source_location
        = std::source_location::current()
    ) noexcept;
    /*!
     * @brief Constructor.
     * @param _a_error_msg The error message assocaited with the error.
     * @param _a_unrecoverable_error Whether the error can be recovered from.
     * @param _a_stacktrace The associated stacktrace with the error.
     * @param _a_source_location The source locatin of the error.
     * @return
     */
    __constexpr
    setup_error_t(
        const std::string_view      _a_error_msg,
        const bool                  _a_unrecoverable_error,
        const std::stacktrace&      _a_stacktrace,
        const std::source_location& _a_source_location
        = std::source_location::current()
    ) noexcept;
    /*!
     * @brief Tells the caller whether this error can be recovered from.
     * @return Bool; true if the error can be recovered from, false otherwise.
     */
    __constexpr bool
        unrecoverable_error() const noexcept;
    /*!
     * @brief Returns the caller a cref to the setup_test_error_t's error
     * message.
     * @return A cref to the object's error message.
     */
    __constexpr const std::string_view
                      error_msg() const noexcept;
    /*!
     * @brief Returns to the caller a cref to the object's optional stacktrace.
     * @return A cref to the objects optinoal stacktrace.
     */
    __constexpr const std::optional<std::stacktrace>&
                      opt_stacktrace() const noexcept;
    /*!
     * @brief Returns to the caller a cref to the object's source_location.
     * @return A cref to the objects source_location.
     */
    __constexpr const std::source_location&
                      source_location() const noexcept;
private:
    std::string                    _m_error_msg;
    bool                           _m_unrecoverable_error;
    std::optional<std::stacktrace> _m_opt_stacktrace;
    std::source_location           _m_source_location;
    __constexpr
    setup_error_t(
        const std::string_view                _a_error_msg,
        const bool                            _a_unrecoverable_error,
        const std::optional<std::stacktrace>& _a_opt_stacktrace,
        const std::source_location&           _a_source_location
    ) noexcept;
};

using opt_setup_error_t = std::optional<setup_error_t>;
_END_ABC_ERRORS_NS

_BEGIN_ABC_ERRORS_NS
__constexpr_imp
    setup_error_t::setup_error_t(
        const std::string_view      _a_error_msg,
        const bool                  _a_unrecoverable_error,
        const std::source_location& _a_source_location
    ) noexcept
    : setup_error_t(
          _a_error_msg,
          _a_unrecoverable_error,
          std::optional<std::stacktrace>{},
          _a_source_location
      )
{}

__constexpr_imp
    setup_error_t::setup_error_t(
        const std::string_view      _a_error_msg,
        const bool                  _a_unrecoverable_error,
        const std::stacktrace&      _a_stacktrace,
        const std::source_location& _a_source_location
    ) noexcept
    : setup_error_t(
          _a_error_msg,
          _a_unrecoverable_error,
          std::optional<std::stacktrace>(_a_stacktrace),
          _a_source_location
      )
{}

__constexpr_imp bool
    setup_error_t::unrecoverable_error() const noexcept
{
    return _m_unrecoverable_error;
}

__constexpr_imp const std::string_view
                      setup_error_t::error_msg() const noexcept
{
    return _m_error_msg;
}

__constexpr_imp const std::optional<std::stacktrace>&
                      setup_error_t::opt_stacktrace() const noexcept
{
    return _m_opt_stacktrace;
}

__constexpr_imp const std::source_location&
                      setup_error_t::source_location() const noexcept
{
    return _m_source_location;
}

__constexpr_imp
    setup_error_t::setup_error_t(
        const std::string_view                _a_error_msg,
        const bool                            _a_unrecoverable_error,
        const std::optional<std::stacktrace>& _a_opt_stacktrace,
        const std::source_location&           _a_source_location
    ) noexcept
    : _m_error_msg(_a_error_msg)
    , _m_unrecoverable_error(_a_unrecoverable_error)
    , _m_opt_stacktrace(_a_opt_stacktrace)
    , _m_source_location(_a_source_location)
{}

_END_ABC_ERRORS_NS