#pragma once

#include "abc_test/gen_data/gen_data_base.h"

_BEGIN_ABC_NS
template<
	typename T,
	typename REP_TYPE
>
class gen_data_with_repetition_type_t : public gen_data_base_t<T>
{
public:
	static const std::size_t _S_THIS_CLASS_MODE = 1;
	utility::io::opt_file_rw_t<REP_TYPE> _m_associated_file;
	REP_TYPE _m_rep_data;
	utility::str::rw_info_t<REP_TYPE> _m_printer_parser;
	template<
		typename R = std::vector<T>
	>
	__constexpr_imp
		gen_data_with_repetition_type_t(
			const size_t _a_elements_generated,
			const utility::io::opt_file_rw_info_t<T>& _a_templated_file_rw,
			R&& _a_elements,
			const utility::io::opt_file_name_t& _a_file_name,
			const std::string_view _a_extension,
			const test_options_t* _a_test_options = global::get_global_test_options_ptr()
		) noexcept
		: gen_data_base_t<T>(
			_a_elements_generated,_a_templated_file_rw,_a_elements)
		, _m_associated_file(_a_file_name.has_value() ?
			utility::io::opt_file_rw_t<REP_TYPE>(
				utility::io::file_rw_t<REP_TYPE>(
					utility::io::file_rw_info_t<REP_TYPE>(_a_file_name.value()),
					_a_extension, _a_test_options))
			: utility::io::opt_file_rw_t<REP_TYPE>{})
		, _m_printer_parser(utility::str::rw_info_t<REP_TYPE>())
	{

	}
	__constexpr_imp
		virtual
		std::string
		subclass_get_repetition_data_as_string(
		) const noexcept
	{
		using namespace std;
		using namespace std;
		return _m_printer_parser.printer().run_printer(_m_rep_data);
	}
	__constexpr
		virtual
		void
		set_data_using_mode_and_parsed_repetition_data(
			const std::size_t _a_mode,
			const REP_TYPE _a_rep_type
		) = 0;
	__constexpr_imp
		virtual 
		void
		subclass_set_data_using_mode_and_string_representing_repetition_data(
			const std::size_t _a_mode,
			const std::string_view _a_additional_data
		) override
	{
		using namespace utility::io;
		using namespace errors;
		if (_a_mode == _S_THIS_CLASS_MODE)
		{
			//Data comes from file.
			//Element is an integer pertaining to a file index.
		//runs all the logic for reading from the file.
			if (_m_associated_file.has_value())
			{
				file_rw_t<REP_TYPE>& _l_associated_file{ _m_associated_file.value() };
				if (_l_associated_file.read_all_elements())
				{
					throw test_library_exception_t(
						fmt::format("Attempting to increment gen_data_base_t using additional "
							"data when it is in read-file mode, and when all elements in the file "
							"have been read. "));
				}
				else
				{
					const size_t _l_elements_read{ _l_associated_file.elements_read() };
					const size_t _l_file_element{ this->_m_rw_info.parser().run_parser_with_exception(_a_additional_data) };
					if (_l_elements_read > _l_file_element)
					{
						throw test_library_exception_t(
							fmt::format(
								"Attempting to increment gen_data using additional "
								"data when it is in read-file mode. The number of elemnets "
								"read is less than the index given to re-run this test. "
								"Index given = {0}, file elemnets read = {1}.",
								_l_file_element,
								_l_elements_read));
					}
					else
					{
						while (_l_elements_read < _l_file_element)
						{
							const bool _l_element_read_correctly{ _l_associated_file.read_next_element() };
							if (not _l_element_read_correctly)
							{
								throw test_library_exception_t(fmt::format(
									"Expected to be able to read {0} elements from the file, however only "
									"{1} elements could be read before a line failed to be read.",
									_l_file_element, _l_associated_file.elements_read()
								));
							}
						}
					}
				}
			}
			else
			{
				throw test_library_exception_t(
					fmt::format(
						"_a_mode = {0}. "
						"This should mean that this gen_data object's repetition data comes from a file. "
						"However, this gen_data object has no associated file. ",
						_a_mode));
			}
		}
		else
		{
			//Data isn't from file, just a generic element.
			const REP_TYPE _l_rt{ _m_printer_parser.parser().run_parser_with_exception(_a_additional_data) };
			set_data_using_mode_and_parsed_repetition_data(_a_mode, _l_rt);
		}
	}
	__constexpr
		virtual
		REP_TYPE
		subclass_get_ret_data(
		) = 0;
	__constexpr_imp
		ds::repetition_data_t
		subclass_write_failed_value_to_file_and_return_repetition_data(
		) noexcept
	{
		using namespace ds;
		using namespace std;
		if (_m_associated_file.has_value() && this->_m_repetition_data.mode() != _S_THIS_CLASS_MODE
			&& 
			true
			//are_failed_values_written_to_files()
			)
		{
			//if (not this->_m_has_been_called_previously)
			{
				//Only write the data if this hasn't been called before.
				_m_associated_file.value().write_element(subclass_get_ret_data());
			//	_m_has_been_called_previously = true;
			}
			//Call subclass variant incase data also needs to be written to a folder in the subclass,
			// however we always return the element from the top file if it writes to more than one file.
			subclass_write_failed_value_to_file_and_return_repetition_data();
			//Create new repetition_data which points to the entry in the file.
			return repetition_data_t(this->_m_repetition_data.for_loop_index(),
				this->_m_repetition_data.generation_collection_index(),
				0, subclass_get_repetition_data_as_string());
		}
		else
		{
			return this->_m_repetition_data;
		}
	}
	__constexpr_imp
		void
		subclass_set_next_mode(
		) noexcept
	{
		this->_m_repetition_data.set_mode(1);
	}
};

_END_ABC_NS