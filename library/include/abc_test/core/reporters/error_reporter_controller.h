#pragma once
#include "abc_test/core/reporters/error_reporter.h"
#include "abc_test/utility/ptr_utility.h"

_BEGIN_ABC_REPORTERS_NS
/*!
 * @brief This structure reports errors to all of the error reports. It also
 * contains logic pertaining to the termination of the testing suite, if erros
 * are encountered that it is unable to recover from.
 */
struct error_reporter_controller_t
{
public:
    /*!
     * @brief Default constructor.
     */
    __no_constexpr
        error_reporter_controller_t() noexcept;
    /*!
     * @brief Adds a collection of reporters to the structure.
     * @param _a_reporters Cref to the error_reporters_t object to be added.
     */
    __constexpr void
        add_reporters(const error_reporters_t& _a_reporters) noexcept;
    /*!
     * @brief Adds a collection of reporters to the structure.
     * @param _a_reporters Rvalue error_reporters_t object to be added.
     */
    __constexpr void
        add_reporters(error_reporters_t&& _a_reporters) noexcept;
    /*!
     * @brief Processes a single error.
     *
     * All the internal error_reporter_t objects are passed the setup_error_t
     * argument, and allowed to process it.
     *
     * @param _a_error The setup_error_t object to be processed.
     */
    __no_constexpr void
        report_error(const errors::setup_error_t& _a_error) noexcept;
    /*!
     * @brief Processes a string warning.
     *
     * Unlike report_error, this function can be thought of a way of reporting
     * "information". We use this almost exclusively to report warnings to the
     * user, however it could be used in other manners.
     *
     * @param _a_str The string being reported.
     */
    __constexpr void
        report_information(const std::string_view _a_str) noexcept;
    /*!
     * @brief The exit function. This function terminates the program.
     *
     * Specifically, this function calls exit_no_lock() then calls std::exit
     * with an error code of -1.
     *
     * Note this function does not check whether it should exit, it assumes this
     * check has been performed.
     *
     */
    __no_constexpr void
        hard_exit() noexcept;
    /*!
     * @brief Tells the caller whether this object has recieved an error for
     * which it believes it should terminate.
     * @return True if the object believes it should termiante; false otherwise.
     */
    __constexpr bool
        should_exit() noexcept;
    /*!
     * @brief Idnetical to hard_exit except it does not call std::exit. Thsi is
     * left to the user to do.
     * 
     * It also only performs the soft exit if _m_should_exit is true.
     * 
     * @return True if _m_should_exit is true; false otherwise.
     */
    __constexpr bool
        soft_exit() noexcept;
    /*!
     * @brief Returns the number of times a catastrophic error has been recorded
     * by the system.
     *
     * Due to the multi-threaded nature of abc_test, it is possible for multiple
     * tests to produce erros which require the test suite to be termianted.
     * This function tells the caller how many of these "catastrophic errors"
     * have been encountered.
     *
     * @return The number of catastrophic errors encountered by this object.
     */
    __constexpr std::size_t
                catastrophic_errors() noexcept;
private:
    error_reporters_t _m_error_reporters;
    bool              _m_should_exit;
    std::size_t       _m_catastrophic_errors;
    std::mutex        _m_errors_mutex;
    /*!
     * @brief Generic function to add error_reporter_t to this object.
     */
    template <typename T>
    __constexpr void
        add_reporters_internal(T&& _a_reporters) noexcept;
    /*!
     * @brief Calls the exit function on all the internal error_report_t.
     *
     * Note that this function assumes a lock has been obtained on this object.
     *
     * objects. But does not any termination functions itself
     */
    __constexpr void
        exit_no_lock() noexcept;
};

_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS
__no_constexpr_imp
    error_reporter_controller_t::error_reporter_controller_t() noexcept
    : _m_error_reporters(error_reporters_t())
    // , _m_test_options(_a_options)
    , _m_should_exit(false)
    , _m_catastrophic_errors(0)
    , _m_errors_mutex(std::mutex())
{}

__constexpr_imp void
    error_reporter_controller_t::add_reporters(
        const error_reporters_t& _a_reporters
    ) noexcept
{
    add_reporters_internal(std::forward<const error_reporters_t&>(_a_reporters)
    );
}

__constexpr_imp void
    error_reporter_controller_t::add_reporters(
        error_reporters_t&& _a_reporters
    ) noexcept
{
    add_reporters_internal(std::forward<error_reporters_t&&>(_a_reporters));
}

__no_constexpr_imp void
    error_reporter_controller_t::report_error(
        const errors::setup_error_t& _a_error
    ) noexcept
{
    using namespace std;
    using namespace utility;
    unique_lock _l_error_reporter_unique_lock(_m_errors_mutex);
    for (const shared_and_raw_ptr<error_reporter_t>& _l_reporter :
         _m_error_reporters)
    {
        error_reporter_t* _l_reporter_ptr{get_ptr(_l_reporter)};
        _l_reporter_ptr->report_error(_a_error);
    }
    if (_a_error.unrecoverable_error())
    {
        _m_catastrophic_errors++;
        _m_should_exit = true;
    }
}

__constexpr_imp void
    error_reporter_controller_t::report_information(
        const std::string_view _a_str
    ) noexcept
{
    using namespace std;
    using namespace utility;
    unique_lock _l_error_reporter_unique_lock(_m_errors_mutex);
    for (const shared_and_raw_ptr<error_reporter_t>& _l_reporter :
         _m_error_reporters)
    {
        error_reporter_t* _l_reporter_ptr{get_ptr(_l_reporter)};
        _l_reporter_ptr->report_information(_a_str);
    }
}

__no_constexpr_imp void
    error_reporter_controller_t::hard_exit() noexcept
{
    using namespace std;
    unique_lock _l_error_reporter_unique_lock(_m_errors_mutex);
    exit_no_lock();
    _m_errors_mutex.unlock();
    std::exit(-1);
}

__constexpr_imp bool
    error_reporter_controller_t::should_exit() noexcept
{
    using namespace std;
    unique_lock _l_error_reporter_unique_lock(_m_errors_mutex);
    return _m_should_exit;
}

__constexpr_imp bool
    error_reporter_controller_t::soft_exit() noexcept
{
    using namespace std;
    unique_lock _l_error_reporter_unique_lock(_m_errors_mutex);
    if (_m_should_exit)
    {
        exit_no_lock();
    }
    return _m_should_exit;
}

__constexpr_imp std::size_t
                error_reporter_controller_t::catastrophic_errors() noexcept
{
    using namespace std;
    unique_lock _l_error_reporter_unique_lock(_m_errors_mutex);
    return _m_catastrophic_errors;
}

template <typename T>
__constexpr_imp void
    error_reporter_controller_t::add_reporters_internal(
        T&& _a_reporters
    ) noexcept
{
    _m_error_reporters.insert(
        _m_error_reporters.end(), _a_reporters.begin(), _a_reporters.end()
    );
}

__constexpr_imp void
    error_reporter_controller_t::exit_no_lock() noexcept
{
    using namespace std;
    using namespace utility;
    for (const shared_and_raw_ptr<error_reporter_t>& _l_reporter :
         _m_error_reporters)
    {
        error_reporter_t* _l_reporter_ptr{utility::get_ptr(_l_reporter)};
        _l_reporter_ptr->exit();
    }
}

_END_ABC_REPORTERS_NS