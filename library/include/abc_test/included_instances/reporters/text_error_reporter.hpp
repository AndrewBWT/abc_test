#pragma once
#include "abc_test/core/reporters/error_reporter.hpp"
#include "abc_test/utility/io/threaded_text_output_reporter.hpp"
_BEGIN_ABC_REPORTERS_NS
/*!
 * Object used to print data about tests to some text output - either the
 * console or a file
 */
struct text_error_reporter_t
    : public error_reporter_t
    , public utility::io::threated_text_output_reporter_t
{
public:
    /*!
    Constructor for std::cerr to be where the output is posted.
    */
    __no_constexpr
        text_error_reporter_t(const bool _a_coloured) noexcept;
    /*!
    Constructor whcih allows a file to be where the output is posted.
    */
    __no_constexpr
        text_error_reporter_t(const std::filesystem::path& _a_file_output
        ) noexcept;
    /*!
     * Describes how to deal with an error_t being reported.
     */
    __no_constexpr virtual void
        process_error(const _ABC_NS_ERRORS::abc_test_error_t& _a_error
        ) const noexcept override;
    /*!
     * Function for dealing with simple information; in this case, it is
     * streamed to the output.
     */
    __no_constexpr virtual void
        report_information(const std::string_view _a_str) const noexcept;
    /*!
     * What to do if the test suite is terminated - e.g. any cleaning up
     */
    __no_constexpr virtual void
        exit() const noexcept;
};

_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS
__no_constexpr_imp
    text_error_reporter_t::text_error_reporter_t(
        const bool _a_coloured
    ) noexcept
    : threated_text_output_reporter_t(std::cerr)
{}

__no_constexpr_imp
    text_error_reporter_t::text_error_reporter_t(
        const std::filesystem::path& _a_file_output
    ) noexcept
    : threated_text_output_reporter_t(_a_file_output)
{}

__no_constexpr_imp void
    text_error_reporter_t::process_error(
        const _ABC_NS_ERRORS::abc_test_error_t& _a_error
    ) const noexcept
{
    using namespace std;
    using namespace _ABC_NS_UTILITY_STR;
    write_line(u8"ERROR ENCOUNTERED");
    /*
        using namespace std;
    using namespace _ABC_NS_UTILITY_STR;
    write_line(fmt::format(
        u8"ERROR:\n"
        u8"   Error Message: \"{0}\"\n"
        u8"   Source of error:\n"
        u8"      File: {1}\n"
        u8"      Function: {2}\n"
        u8"      Line: {3}\n"
        u8"   Stacktrace: {4}",
        _a_setup_test_error.error_msg(),
        cast_string_to_u8string(_a_setup_test_error.source_location().file_name(
        )),
        cast_string_to_u8string(
            _a_setup_test_error.source_location().function_name()
        ),
        _a_setup_test_error.source_location().line(),
        _a_setup_test_error.opt_stacktrace().has_value()
            ? cast_string_to_u8string(
                  to_string(_a_setup_test_error.opt_stacktrace().value())
              )
            : u8"No stacktrace"
    ));
    */
}

__no_constexpr_imp void
    text_error_reporter_t::report_information(
        const std::string_view _a_str
    ) const noexcept
{
    using namespace std;
    using namespace _ABC_NS_UTILITY_STR;
    write_line(u8"WARNING: " + cast_string_to_u8string(_a_str));
}

__no_constexpr_imp void
    text_error_reporter_t::exit() const noexcept
{
    write_line(u8"Preparing to exit...");
}

_END_ABC_REPORTERS_NS