#pragma once


#include "abc_test/core/ds/type_synonyms.hpp"
#include "abc_test/utility/internal/macros.hpp"

_BEGIN_ABC_UTILITY_CLI_NS

enum class cli_results_status_t
{
    can_continue,
    //! Has errors which do not allow the program to continue.
    has_errors,
    //! Text output which must be shown. Continuing the program is not an option
    has_text_output,
};

/*!
 * @brief Structure which holds result information about a cli_t object.
 */
class cli_results_t
{
public:
    __constexpr cli_results_status_t
        status() const noexcept
    {
        return _m_status;
    }

    __constexpr bool
        terminate_early() const noexcept
    {
        return _m_terminate_early;
    }

    __constexpr bool
        has_errors() const noexcept
    {
        return _m_errors.size() > 0;
    }

    __constexpr bool
        has_warnings() const noexcept
    {
        return false;
    }

    __constexpr bool
        can_continue() const noexcept
    {
        return not _m_terminate_early;
    }

    __constexpr bool
        has_text_output() const noexcept
    {
        return _m_output.size() > 0;
    }

    __constexpr void
        add_table_output(
            std::u8string&& _a_table
        ) noexcept
    {
        _m_status = cli_results_status_t::has_text_output;
        _m_output.push_back(_a_table);
    }

    __constexpr const std::vector<std::u8string>&
                      text_output() const noexcept
    {
        return _m_output;
    }

    __constexpr void
        set_terminate_early(
            const bool _a_terminate_early
        )
    {
        _m_terminate_early = _a_terminate_early;
    }

    __constexpr void
        add_error(
            const std::u8string_view _a_str
        ) noexcept
    {
        using namespace std;
        _m_status = cli_results_status_t::has_errors;
        _m_errors.push_back(u8string(_a_str));
        _m_terminate_early = true;
    }

    __constexpr std::u8string
                warnings() const noexcept
    {
        return u8"";
    }

    __constexpr std::u8string
                log() const noexcept
    {
        return u8"";
    }

    __constexpr std::u8string
                errors() const noexcept
    {
        using namespace std;
        u8string _l_rv{};
        for (const u8string_view _a_str : _m_errors)
        {
            _l_rv.append(_a_str);
            _l_rv.append(u8"\n");
        }
        return _l_rv;
    }

    __no_constexpr_imp void
        add_memoized_data(
            const bool               _a_retain_previous_results,
            const std::u8string_view _a_flag,
            const std::u8string_view _a_printed_value,
            const std::u8string_view _a_source
        ) noexcept
    {
        using namespace std;
        if (_m_memoized_processed_flags.contains(u8string(_a_flag)))
        {
            if (_a_retain_previous_results)
            {
                vector<u8string> _l_prev = get<1>(
                    _m_memoized_processed_flags.at(u8string(_a_flag)).back()
                );
                _l_prev.push_back(u8string(_a_source));
                _m_memoized_processed_flags.at(u8string(_a_flag))
                    .push_back(make_tuple(u8string(_a_printed_value), _l_prev));
            }
            else
            {
                _m_memoized_processed_flags.at(u8string(_a_flag))
                    .push_back(make_tuple(
                        u8string(_a_printed_value),
                        vector<u8string>(1, u8string(_a_source))
                    ));
            }
        }
        else
        {
            _m_memoized_processed_flags.insert(
                {u8string(_a_flag),
                 {{u8string(_a_printed_value),
                   vector<u8string>(1, u8string(_a_source))}}}
            );
        }
    }

    ds::memoized_cli_history_t _m_memoized_processed_flags;
    std::vector<std::u8string> _m_output;
    bool                       _m_terminate_early = false;
    std::vector<std::u8string> _m_errors;
    cli_results_status_t _m_status = cli_results_status_t::can_continue;

    __constexpr const          ds::memoized_cli_history_t&
                               memoized_data() const noexcept
    {
        return _m_memoized_processed_flags;
    }
};

_END_ABC_UTILITY_CLI_NS