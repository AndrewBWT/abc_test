#pragma once

#include "abc_test/gen_data/gen_data_base.h"
#include "abc_test/utility/io/file/opt_file_rw.h"

_BEGIN_ABC_NS
//no opt means generate next failed. a value means it was a success.
//Value represents new mode value.
using generate_next_return_type_t = std::optional<std::size_t>;

template<
	typename T,
	typename Rep_Data
>
class gen_data_with_repetition_type_t : public gen_data_base_t<T>
{
public:
	template<
		typename R = std::vector<T>
	>
	__constexpr
		gen_data_with_repetition_type_t(
			const size_t _a_elements_generated,
			const utility::io::opt_file_rw_info_t<T>& _a_core_data_rw_info,
			R&& _a_elements,
			const std::string_view _a_rep_data_file_extension,
			const utility::io::opt_file_rw_info_t<Rep_Data>& _a_rep_data_rw_info,
			const test_options_t* _a_test_options = global::get_global_test_options_ptr()
		) noexcept;
	__constexpr
		gen_data_with_repetition_type_t(
			const size_t _a_elements_generated,
			const utility::io::opt_file_rw_info_t<Rep_Data>& _a_rep_data_rw_info,
			const test_options_t* _a_test_options = global::get_global_test_options_ptr()
		) noexcept;
	template<
		typename R = std::vector<T>
	>
	__constexpr
		gen_data_with_repetition_type_t(
			const size_t _a_elements_generated,
			const utility::io::opt_file_rw_info_t<T>& _a_core_data_rw_info,
			R&& _a_elements,
			const test_options_t* _a_test_options = global::get_global_test_options_ptr()
		) noexcept;
	__constexpr
		gen_data_with_repetition_type_t(
			const size_t _a_elements_generated,
			const test_options_t* _a_test_options = global::get_global_test_options_ptr()
		) noexcept;
	T _m_element;
	std::size_t _m_elements_generated;
	test_runner_t* _m_test_runner;
	__constexpr
		virtual
		bool
		has_current_element(
		) const noexcept final;
	__constexpr
		virtual
		bool
		subclass_has_current_element(
		) const noexcept = 0;
	__constexpr
		virtual
		const T&
		current_element(
		) const noexcept final;
	__constexpr
		virtual
		const T&
		subclass_current_element(
		) const noexcept = 0;
	__constexpr
		virtual
		bool
		generate_next(
		) final;
	__constexpr
		virtual
		bool
		subclass_generate_next(
		) = 0;
	//__constexpr
	//	virtual
	//	std::string
	//	get_repetition_data_as_string(
	//	) const noexcept final;
	__constexpr
		virtual
		Rep_Data
		subclass_get_repetition_data(
		) const noexcept = 0;
	__constexpr
		virtual
		void
		set_data_using_mode_and_string_representing_repetition_data(
			const std::size_t _a_mode,
			const std::string_view _a_additional_data
		);
	__constexpr
		virtual
		void
		subclass_set_data_using_mode_and_repetition_data(
			const std::size_t _a_mode,
			const Rep_Data _a_rep_data
		) = 0;
	__constexpr
		virtual
		ds::test_failure_func_t
		create_test_failure_function(
		) noexcept final;
	__constexpr
		virtual
		void
		subclass_reset_data(
		) noexcept = 0;
private:
	//! Contains the core data reader/writer.
	utility::io::opt_file_rw_t<T,0> _m_core_data_rw_file;
	//Contains the reader/writer for Rep_Data, unique to a specific instance.
	utility::io::opt_file_rw_t<Rep_Data,1> _m_rep_data_rw_file;
	//! A reader/writer for the rep_data. Used when reading repetition_data from a string.
	utility::str::rw_info_t<Rep_Data> _m_rep_data_rw_info;
	//! A reader/writer for a file index. Used when reading a line index from a string.
	utility::str::rw_info_t<std::size_t> _m_line_reader_writer;
	//! Tells us whether the data in this current element has been written to a file.
	bool _m_has_current_element_been_written_to_file;
	__constexpr
		generate_next_return_type_t
		initialise_rep_data_level(
		) noexcept;
	__constexpr
		generate_next_return_type_t
		initialise_subclass_level(
		) noexcept;
	__constexpr
		generate_next_return_type_t
		generate_next_inner(
		);
	/*static const std::size_t _S_THIS_CLASS_MODE = 1;
	utility::io::opt_file_rw_t<Rep_Data> _m_associated_file;
	Rep_Data _m_rep_data;
	utility::str::rw_info_t<Rep_Data> _m_printer_parser;
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
			utility::io::opt_file_rw_t<Rep_Data>(
				utility::io::file_rw_t<Rep_Data>(
					utility::io::file_rw_info_t<Rep_Data>(_a_file_name.value()),
					_a_extension, _a_test_options))
			: utility::io::opt_file_rw_t<Rep_Data>{})
		, _m_printer_parser(utility::str::rw_info_t<Rep_Data>())
	{

	}
	__constexpr_imp
		//virtual
		std::string
		subclass_get_repetition_data_as_string(
		) const noexcept
	{
		using namespace std;
		using namespace std;
		return _m_printer_parser.printer().run_printer(_m_rep_data);
	}
	__constexpr
	//	virtual
		void
		set_data_using_mode_and_parsed_repetition_data(
			const std::size_t _a_mode,
			const REP_TYPE _a_rep_type
		) = 0;
	__constexpr_imp
	//	virtual 
		void
		set_data_using_mode_and_string_representing_repetition_data(
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
	__constexpr_imp
		virtual
		bool
		subclass_generated_next(
		)
	{

	}*/
};

_END_ABC_NS

_BEGIN_ABC_NS

template<
	typename T,
	typename Rep_Data
>
template<
	typename R
>
__constexpr_imp
	gen_data_with_repetition_type_t<T, Rep_Data>::gen_data_with_repetition_type_t(
		const size_t _a_elements_generated,
		const utility::io::opt_file_rw_info_t<T>& _a_core_data_rw_info,
		R&& _a_elements,
		const std::string_view _a_rep_data_file_extension,
		const utility::io::opt_file_rw_info_t<Rep_Data>& _a_rep_data_rw_info,
		const test_options_t* _a_test_options
	) noexcept
	: gen_data_base_t<T>()
	, _m_core_data_rw_file(utility::io::opt_file_rw_t<T, 0>(_a_core_data_rw_info, 
		_a_test_options->_m_general_data_extension, std::forward<R>(_a_elements)))
	, _m_rep_data_rw_file(utility::io::opt_file_rw_t<Rep_Data, 1>(_a_rep_data_rw_info,
		_a_rep_data_file_extension))
	, _m_rep_data_rw_info(utility::str::rw_info_t<Rep_Data>())
	, _m_has_current_element_been_written_to_file(false)
{

}
template<
	typename T,
	typename Rep_Data
>
__constexpr_imp
	gen_data_with_repetition_type_t<T, Rep_Data>::gen_data_with_repetition_type_t(
		const size_t _a_elements_generated,
		const utility::io::opt_file_rw_info_t<Rep_Data>& _a_rep_data_rw_info,
		const test_options_t* _a_test_options
	) noexcept
	: gen_data_with_repetition_type_t<T, Rep_Data>(
		_a_elements_generated, {}, {}, _a_rep_data_rw_info, _a_test_options)
{

}
template<
	typename T,
	typename Rep_Data
>
template<
	typename R
>
__constexpr_imp
	gen_data_with_repetition_type_t<T, Rep_Data>::gen_data_with_repetition_type_t(
		const size_t _a_elements_generated,
		const utility::io::opt_file_rw_info_t<T>& _a_core_data_rw_info,
		R&& _a_elements,
		const test_options_t* _a_test_options
	) noexcept
	: gen_data_with_repetition_type_t<T, Rep_Data>(
		_a_elements_generated, _a_core_data_rw_info, _a_elements, "", {}, _a_test_options)
{

}
template<
	typename T,
	typename Rep_Data
>
__constexpr_imp
	gen_data_with_repetition_type_t<T, Rep_Data>::gen_data_with_repetition_type_t(
		const size_t _a_elements_generated,
		const test_options_t* _a_test_options
	) noexcept
	: gen_data_with_repetition_type_t<T, Rep_Data>(
		_a_elements_generated, {}, {}, "",{}, _a_test_options)
{

}
template<
	typename T,
	typename Rep_Data
>
__constexpr_imp
	bool
	gen_data_with_repetition_type_t<T, Rep_Data>::has_current_element(
	) const noexcept
{
	if (_m_core_data_rw_file.has_current_element(this->mode()))
	{
		return true;
	}
	else if (_m_rep_data_rw_file.has_current_element(this->mode()))
	{
		return true;
	}
	else
	{
		return subclass_has_current_element();
	}
}
template<
	typename T,
	typename Rep_Data
>
__constexpr_imp
	const T&
	gen_data_with_repetition_type_t<T, Rep_Data>::current_element(
	) const noexcept
{
	if (_m_core_data_rw_file.has_current_element(this->mode()))
	{
		return _m_core_data_rw_file.current_element();
	}
	else if (_m_rep_data_rw_file.has_current_element(this->mode()))
	{
		return subclass_current_element();
//		return _m_rep_data_rw_file.current_element();
	}
	else
	{
		return subclass_current_element();
	}
}
template<
	typename T,
	typename Rep_Data
>
__constexpr_imp
	bool
	gen_data_with_repetition_type_t<T, Rep_Data>::generate_next(
	) 
{
	const generate_next_return_type_t _l_result{ generate_next_inner() };
	if (_l_result.has_value())
	{
		const size_t _l_mode{ _l_result.value() };
		_m_has_current_element_been_written_to_file = false;
		this->_m_repetition_data.set_mode(_l_mode);
		switch (_l_mode)
		{
		case 0:
			this->_m_repetition_data.set_string(
				_m_line_reader_writer.printer().run_printer(_m_core_data_rw_file.elements_read()));
			break;
		case 1:
			subclass_set_data_using_mode_and_repetition_data(this->mode(),
				_m_rep_data_rw_file.current_element());
			this->_m_repetition_data.set_string(
				_m_line_reader_writer.printer().run_printer(_m_rep_data_rw_file.elements_read()));
			break;
		case 2:
			this->_m_repetition_data.set_string(
				_m_rep_data_rw_info.printer().run_printer(subclass_get_repetition_data()));
			break;
		default:
			throw errors::test_library_exception_t("Could not find data mode");
		}
		
	}
	return _l_result.has_value();
}
/*template<
	typename T,
	typename Rep_Data
>
__constexpr_imp
	std::string
	gen_data_with_repetition_type_t<T, Rep_Data>::get_repetition_data_as_string(
	) const noexcept
{
	return _m_rep_data_rw_info.printer().run_printer(subclass_get_repetition_data());
}*/
template<
	typename T,
	typename Rep_Data
>
__constexpr_imp
	void
	gen_data_with_repetition_type_t<T, Rep_Data>::set_data_using_mode_and_string_representing_repetition_data(
		const std::size_t _a_mode,
		const std::string_view _a_additional_data
) 
{
	subclass_set_data_using_mode_and_repetition_data(
		_a_mode,
		_m_rep_data_rw_info.parser().run_parser_with_exception(_a_additional_data)
	);
}
template<
	typename T,
	typename Rep_Data
>
__constexpr_imp
	ds::test_failure_func_t
	gen_data_with_repetition_type_t<T, Rep_Data>::create_test_failure_function(
	) noexcept
{

	return [this]()
		{
			using namespace std;
			using namespace ds;
			if (optional<repetition_data_t> _l_res{ _m_core_data_rw_file.log_failure_and_return_rep_data(
				_m_has_current_element_been_written_to_file, this->_m_repetition_data) }; _l_res.has_value())
			{
				return _l_res.value();
			}
			else if (optional<repetition_data_t> _l_res{ _m_rep_data_rw_file.log_failure_and_return_rep_data(
				_m_has_current_element_been_written_to_file, this->_m_repetition_data) }; _l_res.has_value())
			{
				return _l_res.value();
			}
			else
			{
				return this->_m_repetition_data;
			}
		};
}
template<
	typename T,
	typename Rep_Data
>
__constexpr_imp
	generate_next_return_type_t
	gen_data_with_repetition_type_t<T, Rep_Data>::initialise_rep_data_level(
	) noexcept
{
	return _m_rep_data_rw_file.has_current_element(this->mode()) ? std::size_t{ 1 } : 
		initialise_subclass_level();
}
template<
	typename T,
	typename Rep_Data
>
__constexpr_imp
	generate_next_return_type_t
	gen_data_with_repetition_type_t<T, Rep_Data>::initialise_subclass_level(
	) noexcept
{
	return subclass_has_current_element() ? std::size_t{2} : generate_next_return_type_t{};
}
template<
	typename T,
	typename Rep_Data
>
__constexpr_imp
	generate_next_return_type_t
	gen_data_with_repetition_type_t<T, Rep_Data>::generate_next_inner(
	)
{
	//If core data has a current element.
	if (_m_core_data_rw_file.has_current_element(this->mode()))
	{
		//Logically, try to generate a next element.
		return _m_core_data_rw_file.generate_next(this->mode()) ? std::size_t{ 0 } : initialise_rep_data_level();
	}
	//Basicallys ame logic, but one level down. Does the rep data currently have an element.
	else if (_m_rep_data_rw_file.has_current_element(this->mode()))
	{
		return _m_rep_data_rw_file.generate_next(this->mode()) ? std::size_t{ 1 } : initialise_subclass_level();
	}
	else
	{
		return subclass_generate_next() ? std::size_t{ 2 } : generate_next_return_type_t{};
	}
}
_END_ABC_NS