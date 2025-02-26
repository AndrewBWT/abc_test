#pragma once


#include "abc_test/internal/ds/type_synonyms.hpp"
#include "abc_test/internal/utility/internal/macros.hpp"
#include "abc_test/internal/utility/str/string_table.hpp"

_BEGIN_ABC_NS

class cli_results_t
{
public:
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
        show_log() const noexcept
    {
        return false;
    }

    __constexpr void
        log_msg() const noexcept
    {
        // std::cout << "log" << std::endl;
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
            abc::utility::str::string_table_t&& _a_table
        ) noexcept
    {
        _m_output.push_back(_a_table());
    }

    __constexpr const std::vector<std::string>&
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
            const std::string_view _a_str
        ) noexcept
    {
        using namespace std;
        _m_errors.push_back(string(_a_str));
        _m_terminate_early = true;
    }

    __constexpr std::string
                errors() const noexcept
    {
        using namespace std;
        string _l_rv{};
        for (const string_view _a_str : _m_errors)
        {
            _l_rv.append(_a_str);
            _l_rv.append("\n");
        }
        return _l_rv;
    }

    __no_constexpr_imp void
        add_memoized_data(
            const bool _a_retain_previous_results,
            const std::string_view _a_flag,
            const std::string_view _a_printed_value,
            const std::string_view _a_source
        ) noexcept
    {
        using namespace std;
        if (_m_memoized_processed_flags.contains(string(_a_flag)))
        {
            if (_a_retain_previous_results)
            {
                vector<string> _l_prev = get<1>(_m_memoized_processed_flags.at(string(_a_flag)).back());
                _l_prev.push_back(string(_a_source));
                _m_memoized_processed_flags.at(string(_a_flag))
                    .push_back(make_tuple(
                        string(_a_printed_value),
                        _l_prev
                    ));
            }
            else
            {
                _m_memoized_processed_flags.at(string(_a_flag))
                    .push_back(make_tuple(
                        string(_a_printed_value),
                        vector<string>(1, string(_a_source))
                    ));
            }
        }
        else
        {
            _m_memoized_processed_flags.insert(
                { string(_a_flag),
                 {{string(_a_printed_value),
                   vector<string>(1, string(_a_source))}} }
            );
        }
    }

    ds::memoized_cli_history_t _m_memoized_processed_flags;
    std::vector<std::string>   _m_output;
    bool                       _m_terminate_early = false;
    std::vector<std::string>   _m_errors;

    __constexpr const          ds::memoized_cli_history_t&
                               memoized_data() const noexcept
    {
        return _m_memoized_processed_flags;
    }
};

_END_ABC_NS