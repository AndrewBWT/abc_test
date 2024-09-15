#pragma once

#include <filesystem>
#include "abc_test/core/ds/repetitions/repetition_tree.h"
#include <map>
#include "abc_test/utility/rng.h"
#include "abc_test/core/reporters/error_reporter_controller.h"

#include "fmt/color.h"

#include "abc_test/utility/internal/log.h"

_BEGIN_ABC_NS
using test_path_delimiter = std::string;
using test_path_delimiter_ref = std::string_view;
using path_t = std::filesystem::path;
/*!
* A struct containing all the test options.
*/
struct test_options_t
{
public:
	/*!
	* Default constructor
	*/
	__constexpr
		test_options_t(
		) noexcept;
	/*!
	* Validates the input to ensure it is correct
	*/
	__constexpr
		void
		validate_input(
			reporters::error_reporter_controller_t& _a_erc
		) noexcept;
	/*!
	* Checks if this configuration of test_options allows tests to be repeated.
	*/
	__constexpr
		bool
		repeating_tests(
		) const noexcept;
	/*!
	* Checks if a specific test is to be re-ran.
	*/
	__constexpr
		bool
		check_if_test_is_to_be_ran(
			const size_t _m_discovery_id
		) const noexcept;
	/*!
	* Gets the set of test_repetition_configurations.
	*/
	__constexpr
		const std::map<size_t, ds::repetition_tree_t>&
		get_test_repetition_configurations(
		) const noexcept;
	//! The path delimiter used to organise tests. E.g. written as "a::b::c" with a delimieter of "::",
	//! this separates the test into the folder a -> b -> c. 
	test_path_delimiter _m_path_delimiter;
	//! The root path where all test data files are located.
	path_t _m_test_data_root_path;
	//! Whether to create test folders if they do not exist.
	bool _m_create_test_folders_if_dont_exist;
	//! The seed values used
	utility::seed_t _m_seed_values;
	//! The default number of random elements to generate in random tests.
	size_t _m_default_numb_random_elements_to_generate;
	//! How many elements to take from the global seed to initiailse per-test random generators.
	size_t _m_numb_elements_used_to_seed_random_generators;
	//! The general file data extension used for reading and writing data in the system.
	std::string _m_general_data_extension;
	//! The string used to signify comments in the random data files.
	std::string _m_comment_str;
	//! Whether to write random data to associated files. 
	bool _m_write_random_data_to_file;
	//! The number of threads allocated to the test.
	size_t _m_threads;
	//! The console length used in output.,
	std::size_t _m_console_line_length;
	//! The string used which represents repeated text and random seeds.
	std::string _m_repeat_tests_string;
	//! An array of characters used in text output. 
	char _m_separator_chars[10];
	//! The failure style used when highlighting information in text output.
	fmt::text_style _m_failure_text_style;
	//! The style used when highlighting information in text output.
	fmt::text_style _m_highlighted_info_style;
	//! The (slight) style used when highlighting information in text output.
	fmt::text_style _m_slight_highlight_style;
	//! How big of an indentation to use in text output
	std::size_t _m_indent_size;
	//! Whether to exist on recoverable errors when setting up tests. 
	//! Generally errors suggest something fundamentally wrong
	bool _m_exit_on_recoverable_teset_setup_error;

	bool _m_allow_empty_test_reporters_to_be_used;

	bool _m_allow_empty_error_reporters_to_be_used;

	bool _m_internal_logging;

	std::string _m_enumerate_data_file_type;
	std::string _m_random_data_file_type;
private:
	std::map<size_t, ds::repetition_tree_t> _m_test_repetition_configurations;
	__constexpr
		void
		validate_repeat_tests_string(
			reporters::error_reporter_controller_t& _a_erc
		) noexcept;
	__constexpr
		void
		verify_no_confliting_options(
			reporters::error_reporter_controller_t& _a_erc
		) noexcept;
};
namespace
{
	const char _c_repeat_tests_delimter{ ':' };
}
_END_ABC_NS

_BEGIN_ABC_NS
__constexpr_imp
	test_options_t::test_options_t(
	) noexcept
	: _m_path_delimiter("::")
	, _m_test_data_root_path(std::filesystem::current_path())
	, _m_create_test_folders_if_dont_exist(true)
	, _m_seed_values({})
	, _m_default_numb_random_elements_to_generate(5)
	, _m_numb_elements_used_to_seed_random_generators(1)
	, _m_comment_str("#")
	, _m_write_random_data_to_file(false)
	, _m_console_line_length(120)
	, _m_threads(0)
	, _m_separator_chars{ '=' }
	, _m_failure_text_style(fmt::fg(fmt::color::red))
	, _m_highlighted_info_style(fmt::fg(fmt::color::teal))
	, _m_slight_highlight_style(fmt::fg(fmt::color::gray))
	, _m_indent_size(3)
	, _m_exit_on_recoverable_teset_setup_error(true)
	, _m_allow_empty_error_reporters_to_be_used(false)
	, _m_allow_empty_test_reporters_to_be_used(false)
	, _m_internal_logging(false)
	, _m_enumerate_data_file_type("ed")
	, _m_random_data_file_type("rd")
	, _m_general_data_extension("gd")
{
}
__constexpr_imp
	void
	test_options_t::validate_input(
		reporters::error_reporter_controller_t& _a_erc
	) noexcept
{
	validate_repeat_tests_string(_a_erc);
	verify_no_confliting_options(_a_erc);
}
__constexpr_imp
	bool
	test_options_t::repeating_tests(
	) const noexcept
{
	return _m_test_repetition_configurations.size() > 0;
}
__constexpr_imp
	bool
	test_options_t::check_if_test_is_to_be_ran(
		const size_t _m_discovery_id
	) const noexcept
{
	if (_m_test_repetition_configurations.size() == 0)
	{
		return true;
	}
	else
	{
		return _m_test_repetition_configurations.contains(_m_discovery_id);
	}
}
__constexpr_imp
	const std::map<size_t, ds::repetition_tree_t>&
	test_options_t::get_test_repetition_configurations(
	) const noexcept
{
	return _m_test_repetition_configurations;
}
__constexpr_imp
	void
	test_options_t::validate_repeat_tests_string(
		reporters::error_reporter_controller_t& _a_erc
	) noexcept
{
	//Format in the form "1:string!2:string "
	using namespace std;
	using namespace ds;
	using namespace errors;
	using enum utility::internal::internal_log_enum_t;
	vector<string> _l_strs;
	size_t _l_pos{ 0 };
	//First element is for the random generator.
	if (_m_repeat_tests_string.empty())
	{
		_LIBRARY_LOG(PARSING_SEED, fmt::format("_m_repeat_tests_string is empty, no parsing required."));
	}
	else 
	{
		_LIBRARY_LOG(PARSING_SEED, fmt::format("Attempting to parse \"{0}\"", _m_repeat_tests_string));
		_l_pos = _m_repeat_tests_string.find(_c_repeat_tests_delimter, _l_pos);
		const string _l_rnd_gen_str{ _m_repeat_tests_string.substr(0,
			(_l_pos == string::npos) ? _m_repeat_tests_string.size() : _l_pos) };
		_LIBRARY_LOG(PARSING_SEED, fmt::format("Random generator string identified as \"{0}\"", _l_rnd_gen_str));
		size_t _l_pair_idx{ 0 };
		++_l_pos;
		//_INTERNAL_LOG(TEST_OPTIONS, fmt::format("Attempting to parse \"{0}\"", _m_repeat_tests_string));
		while (_l_pos < _m_repeat_tests_string.size())
		{
			const size_t _l_mid_delimiter{ _m_repeat_tests_string.find(_c_repeat_tests_delimter, _l_pos) };
			if (_l_mid_delimiter == string::npos)
			{
				_a_erc.report_error(
					setup_error_t(fmt::format("Repeations, after the first random generator string, are in pairs. "
						"A delimiter ({0}) separating the pair at index {1} could not be found (to be clear, "
						"this is element {2} in the sequence of delimited strings. ",
						_c_repeat_tests_delimter, _l_pair_idx, _l_pair_idx + 2
					), false));
				return;
			}
			else
			{
				const string _l_integer_str{ _m_repeat_tests_string.substr(_l_pos,_l_mid_delimiter - _l_pos) };
				_LIBRARY_LOG(PARSING_SEED, fmt::format("First elemnet of pair index {0} identified as \"{1}\"",
					_l_pair_idx,
					_l_integer_str));
				size_t _l_value;
				auto [_l_ptr, _l_ec] = from_chars(_l_integer_str.data(), _l_integer_str.data() + _l_integer_str.size(), _l_value);
				if (_l_ec != errc())
				{
					_a_erc.report_error(
						setup_error_t(fmt::format(
							"Could not parse test index {0} (\"{1}\")",
							_l_pair_idx, _l_integer_str
						), false));
					return;
				}
				else
				{
					_l_pos = _l_mid_delimiter + 1;
					const size_t _l_repeat_delimter{ _m_repeat_tests_string.find(_c_repeat_tests_delimter, _l_pos) };
					const string _l_second_str{ _m_repeat_tests_string.substr(_l_pos,
						_l_repeat_delimter == string::npos ? _m_repeat_tests_string.size() - _l_pos :
						_l_repeat_delimter - _l_pos) };
					_LIBRARY_LOG(PARSING_SEED, fmt::format("Second elemnet of pair index {0} identified as \"{1}\"",
						_l_pair_idx,
						_l_second_str));
					_l_pos = _l_repeat_delimter == string::npos ? _m_repeat_tests_string.size() : _l_pos + 1;
					auto _l_rt{ parse_repetition_tree(_l_second_str, *this, 1) };
					if (_l_rt.has_value())
					{
						_LIBRARY_LOG(PARSING_SEED, fmt::format("Inserting pair {0} and {1} into "
							"_m_test_repetition_configurations",
							_l_value,
							_l_rt.value()));
						_m_test_repetition_configurations.insert({ _l_value, _l_rt.value() });
					}
					else
					{
						_a_erc.report_error(
							setup_error_t(fmt::format(
								"Could not parse test index {0} (\"{1}\")",
								_l_pair_idx, _l_second_str
							), false));
						return;
					}
				}
			}
			_l_pair_idx++;
		}
	}
}

__constexpr_imp
	void
	test_options_t::verify_no_confliting_options(
		reporters::error_reporter_controller_t& _a_erc
	) noexcept
{
	using namespace std::filesystem;
	using namespace errors;
	//This will just be a really long list of things that cannot be done really...
	//Can't write data to the file and repeat tests.
	if (_m_write_random_data_to_file && repeating_tests())
	{
		_a_erc.report_error(setup_error_t(fmt::format(
			"We cannot repeat tests while writing failed tests to a data file. "
			"As the test is re-run, the same failed test cases will be written to the file. "
			"Consider changing _m_write_random_data_to_file to false (currently true) or "
			"removing the test repetition configuration(s)."
		), true));
	}
	if (not exists(_m_test_data_root_path))
	{
		_a_erc.report_error(setup_error_t(fmt::format(
			"_m_test_data_root_path (\"{0}\") must exist for it to be as the root of a testing hierarchy. "
			"Currently it does not. ",
			_m_test_data_root_path.string()
		), true));
	}
}
_END_ABC_NS