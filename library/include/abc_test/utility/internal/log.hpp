#pragma once
#include "abc_test/utility/internal/log/params.hpp"
#include "abc_test/utility/io/threaded_ostream_output_reporter.hpp"
#include "abc_test/utility/str/unicode.hpp"

#include <fmt/std.h>
#include <fmt/xchar.h>
#include <iostream>
#include <map>


#ifndef __LOGGING_ON
    #define __LOGGING_ON 0
#endif

#if __LOGGING_ON == 0
    #define _LIBRARY_LOG(Enum, Str)
#else
    #define _LIBRARY_LOG(Enum, Str) abc::utility::internal::log(Enum, Str)
#endif

_BEGIN_ABC_UTILITY_INTERNAL_NS

/*!
 * Internal logging object, specifically designed for showing debug information.
 * Extends the threaded_ostream_output_reporter_t object, allowing multiple
 * threads to write to this object.
 */
struct internal_log_t : public io::threaded_ostream_output_reporter_t
{
public:
    /*!
     * Default constructor, initialised using a default internal_log_params_t
     * object.
     */
    // Cannot be constexpr as it calls internal_log_t(internal_log_params_t)
    // constructor.
    __no_constexpr
        internal_log_t() noexcept;
    /*!
     * Constructor which takes a bespoke internal_log_params_t object.
     */
    // Cannot be constexpr due to use of io::threaded_ostream_output_reporter_t.
    __no_constexpr
        internal_log_t(const internal_log_params_t& _a_internal_logger_params
        ) noexcept;
    /*!
     * Sets the object's internal_log_paramts_t object to that given as an
     * argument.
     */
    __constexpr void
        set_internal_logger_params(
            const internal_log_params_t& _a_internal_logger_params
        ) noexcept;
    /*!
     * Given a specific enum and a string to write to the log file,
     * writes the log file if the corresponding variable in the object's
     * internal_log_params_t variable is set to true.
     *
     * Otherwise, nothing is logged.
     */
    __constexpr void
        write(
            const internal_log_enum_t _a_internal_logger_enum,
            const std::string_view    _a_str
        ) noexcept;
private:
    internal_log_params_t                  _m_internal_logger_params;
    std::map<std::thread::id, std::size_t> _m_thread_map;
    __no_constexpr                         std::size_t
                                           get_thread_id() noexcept;
};

/*!
 * The global function used to log data. This calls the global internal_log_t
 * object.
 */
// Cannot be constexpr due to call to get_internal_library_logger.
__no_constexpr void
    log(const internal_log_enum_t _a_internal_logger_enum,
        const std::string_view    _a_str) noexcept;
/*!
 * Sets the internal_log_params_t object of the global internal_log_t object to
 * that given as an argument.
 */
// Cannot be constexpr due to call to get_internal_library_logger.
__no_constexpr void
    set_internal_library_logger(
        const internal_log_params_t& _a_internal_looger_params
    ) noexcept;

namespace
{
// Cannot be constexpr or inline due to use of static.
__no_constexpr_or_inline internal_log_t&
    get_internal_library_logger() noexcept;
} // namespace

_END_ABC_UTILITY_INTERNAL_NS

_BEGIN_ABC_UTILITY_INTERNAL_NS
__no_constexpr_imp
    internal_log_t::internal_log_t() noexcept
    : internal_log_t(internal_log_params_t())
{}

__no_constexpr_imp
    internal_log_t::internal_log_t(
        const internal_log_params_t& _a_internal_logger_params
    ) noexcept
    : threaded_ostream_output_reporter_t(std::cout)
    , _m_internal_logger_params(_a_internal_logger_params)
{}

__constexpr_imp void
    internal_log_t::set_internal_logger_params(
        const internal_log_params_t& _a_internal_logger_params
    ) noexcept
{
    _m_internal_logger_params = _a_internal_logger_params;
}

__constexpr_imp void
    internal_log_t::write(
        const internal_log_enum_t _a_internal_logger_enum,
        const std::string_view    _a_str
    ) noexcept
{
    using namespace std;
    using namespace _ABC_NS_UTILITY_STR;
    if (_m_internal_logger_params.is_set(_a_internal_logger_enum))
    {
        threaded_ostream_output_reporter_t::write_line(fmt::format(
            u8"LOG: {0}: {1} - {2}",
            get_thread_id(),
            to_str(_a_internal_logger_enum),
            unpack_string_to_u8string(_a_str)
        ));
    }
}

__no_constexpr_imp std::size_t
                   internal_log_t::get_thread_id() noexcept
{
    using namespace std;
    using enum internal_log_enum_t;
    const thread::id _l_id{this_thread::get_id()};
    if (not _m_thread_map.contains(_l_id))
    {
        const size_t _l_new_thread_id{_m_thread_map.size()};
        threaded_ostream_output_reporter_t::write_line(fmt::format(
            u8"LOG: {0} : {1} - "
            u8"Mapping new thread ID (thread::id = {2}) to {3}",
            _l_new_thread_id,
            to_str(THREAD_MAPPING),
            _l_id,
            _l_new_thread_id
        ));
        _m_thread_map.insert({_l_id, _l_new_thread_id});
    }
    return _m_thread_map.at(_l_id);
}

__no_constexpr_imp void
    log(
        const internal_log_enum_t _a_internal_logger_enum,
        const std::string_view    _a_str
    ) noexcept
{
    internal_log_t& _l_il{get_internal_library_logger()};
    _l_il.write(_a_internal_logger_enum, _a_str);
}

__no_constexpr_imp void
    set_internal_library_logger(
        const internal_log_params_t& _a_internal_looger_params
    ) noexcept
{
    internal_log_t& _l_il{get_internal_library_logger()};
    _l_il.set_internal_logger_params(_a_internal_looger_params);
}

namespace
{
__no_constexpr_or_inline_imp internal_log_t&
    get_internal_library_logger() noexcept
{
    static internal_log_t _s_il;
    return _s_il;
}
} // namespace

_END_ABC_UTILITY_INTERNAL_NS