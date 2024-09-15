#pragma once
#include <vector>
#include <optional>
#include <memory>
#include <string>
#include <functional>
#include <scn/scan.h>
#include "abc_test/global.h"
#include <expected>

#include "abc_test/utility/io/file/file_name.h"
#include "abc_test/utility/io/file/file_reader.h"
#include "abc_test/utility/io/file/file_writer.h"
#include "abc_test/core/ds/repetitions/repetition_data.h"
#include "abc_test/utility/io/file/file_rw.h"
#include "abc_test/core/ds/types.h"

_BEGIN_ABC_NS
/*!
* This type synonym refers to the function used when a test is failed;
* Under some circumstances, the failed repetition_data_t element used to
* log this error is different to that used to create the test point;
* for example when failures are written to a file, to repeat that test
* the file must be navigated.
*/
template<
	typename T
>
struct gen_data_base_t
{
public:
	__constexpr
		ds::repetition_data_t*
		repetition_data_ptr(
		) noexcept;
	__constexpr
		std::size_t
		mode(
		) const noexcept;
	__constexpr
		virtual
		bool
		has_current_element(
		) const noexcept = 0;
	__constexpr
		virtual
		const T&
		current_element(
		) const noexcept = 0;
	__constexpr
		virtual
		bool
		generate_next(
		) = 0;
	//__constexpr
	//	virtual
	//	std::string
	//	get_repetition_data_as_string(
	//	) const noexcept = 0;
	__constexpr
		virtual
		void
		set_data_using_mode_and_string_representing_repetition_data(
			const std::size_t _a_mode,
			const std::string_view _a_additional_data
		) = 0;
	__constexpr
		virtual
		ds::test_failure_func_t
		create_test_failure_function(
		) noexcept = 0;
protected:
	mutable ds::repetition_data_t _m_repetition_data;
};
/*public:
	template<
		typename R = std::vector<T>
	>
	__constexpr
		gen_data_base_t(
			const size_t _a_elements_generated,
			const utility::io::opt_file_rw_info_t<T>& _a_templated_file_rw,
			R&& _a_elements
		) noexcept;
	__constexpr
		bool
		has_current_element_l1(
		) const noexcept;
	__constexpr
		virtual
		bool
		has_current_element_l2(
		) const noexcept = 0;
	__constexpr
		const T&
		current_element_l1(
		) const noexcept;
	__constexpr
		virtual
		const T&
		current_element_l2(
		) const noexcept = 0;
	__constexpr
		bool
		generate_next_l1(
		);
	__constexpr
		virtual
		bool
		generate_next_l2(
		) = 0;
	__constexpr
		std::string
		get_repetition_data_as_string_l1(
		) const noexcept;
	__constexpr
		virtual
		std::string
		get_repetition_data_as_string_l2(
		) const noexcept = 0;
	//__constexpr
	//	virtual
	//	std::size_t
	//	infer_correct_mode(
	//	) const noexcept;
	__constexpr
		void
		set_data_using_mode_and_string_representing_repetition_data_l1(
			const std::size_t _a_mode,
			const std::string_view _a_additional_data
		);
	__constexpr
		virtual
		void
		set_data_using_mode_and_string_representing_repetition_data_l2(
			const std::size_t _a_mode,
			const std::string_view _a_additional_data
		) = 0;
	__constexpr
		ds::repetition_data_t*
		repetition_data_ptr(
		) noexcept;
	__constexpr
		ds::repetition_data_t
		write_failed_value_to_file_and_return_repetition_data_l1(
		) noexcept;
	__constexpr
		virtual
		ds::repetition_data_t
		write_failed_value_to_file_and_return_repetition_data_l2(
		) noexcept = 0;
	__constexpr
		virtual
		test_failure_func_t
		create_test_failure_function(
		) noexcept;
	__constexpr
		virtual
		bool
		are_failed_values_written_to_files(
		) const noexcept = 0;
protected:
	std::size_t _m_elements_generated;
	const test_options_t* _m_test_options;
	test_runner_t* _m_test_runner;
	mutable ds::repetition_data_t _m_repetition_data;
	utility::io::opt_file_rw_t<T> _m_associated_file;
	bool _m_has_been_called_previously;
	using file_data_index_t = std::size_t;
	utility::str::rw_info_t<file_data_index_t> _m_rw_info;
	__constexpr
		virtual
		void
		set_next_mode_l1(
		) noexcept = 0;
	__constexpr
		bool
		associated_file_has_current_element(
		) const noexcept;
	__constexpr
		bool
		not_in_read_file_mode_and_can_write_to_file(
		) const noexcept;
private:
	static const std::size_t _S_THIS_CLASS_MODE = 0;
	template<
		typename R = std::vector<T>
	>
	__constexpr
		gen_data_base_t(
			const size_t _a_elements_generated,
			const utility::io::opt_file_rw_info_t<T>& _a_templated_file_rw,
			R&& _a_elements,
			const test_options_t* _a_test_options_ptr,
			test_runner_t* _a_test_runner_ptr
		) noexcept;
	__constexpr
		std::string
		repetition_data_as_string(
		) const noexcept;
};*/
template<
	typename T
>
using gen_data_ptr_t = std::shared_ptr<gen_data_base_t<T>>;
_END_ABC_NS

_BEGIN_ABC_NS
template<
	typename T
>
__constexpr_imp
	ds::repetition_data_t*
	gen_data_base_t<T>::repetition_data_ptr(
	) noexcept
{
	return &_m_repetition_data;
}
template<
	typename T
>
__constexpr_imp
	std::size_t
	gen_data_base_t<T>::mode(
	) const noexcept
{
	return _m_repetition_data.mode();
}
/*template<
	typename T
>
template<
	typename R
>
__constexpr_imp
	gen_data_base_t<T>::gen_data_base_t(
		const size_t _a_elements_generated,
		const utility::io::opt_file_rw_info_t<T>& _a_templated_file_rw,
		R&& _a_elements
	) noexcept
	: gen_data_base_t<T>(_a_elements_generated, _a_templated_file_rw, _a_elements,
		global::get_global_test_options_ptr(),
		global::get_this_threads_test_runner_ptr())
{	
}
template<
	typename T
>
__constexpr_imp
	bool
	gen_data_base_t<T>::has_current_element_l1(
	) const noexcept
{
	//If the mode is zero, there is an associated file element and that associated
	//file element has a value
	return (associated_file_has_current_element()) ?
		_m_associated_file.value().has_current_element() :
		has_current_element_l2();
}
template<
	typename T
>
__constexpr_imp
	const T&
	gen_data_base_t<T>::current_element_l1(
	) const noexcept
{
	//Set the current element repetition data. Probably not necessary.
	_m_repetition_data.set_string(get_repetition_data_as_string());
	return (associated_file_has_current_element()) ?
		_m_associated_file.value().current_element() :
		current_element_l2();
}
template<
	typename T
>
__constexpr_imp
	bool
	gen_data_base_t<T>::generate_next_l1(
	)
{
	bool _l_result{ false };
	//If we're in mode zero
	if (_m_repetition_data.mode() == _S_THIS_CLASS_MODE)
	{
		//If there is an associated file, all its elements have not been read,
		//and another element was read successfully...
		if (_m_associated_file.has_value() && not _m_associated_file.value().read_all_elements() &&
			_m_associated_file.value().read_next_element()
			)
		{
			_l_result = true;
		}
		else
		{
			//Else, set up next mode using the derived class.
			//This will always be already set up, so we've just gotta get
			//the bool saying whether its got a current element.
			subclass_set_next_mode();
			_l_result = has_current_element_l2();
		}
	}
	else
	{
		_l_result = generate_next_l2();
	}
	if (_l_result)
	{
		_m_has_been_called_previously = false;
	}
	return _l_result;
}
template<
	typename T
>
__constexpr_imp
	std::string
	gen_data_base_t<T>::get_repetition_data_as_string_l1(
	) const noexcept 
{
	using namespace std;
	return associated_file_has_current_element() ?
		repetition_data_as_string() :
		get_repetition_data_as_string_l2();
}*/
	/*template<
		typename T
	>
	__constexpr_imp
		std::size_t
		gen_data_base_t<T>::infer_correct_mode(
		) const noexcept
	{
		if (_m_associated_file.has_value() && _m_repetition_data.mode() != 0 &&
			are_failed_values_written_to_files())
		{
			return 0;
		}
		else
		{
			return _m_repetition_data.mode();
		}
	}*/
/*template<
	typename T
>
__constexpr_imp
	ds::repetition_data_t*
	gen_data_base_t<T>::repetition_data_ptr(
	) noexcept
{
	return &_m_repetition_data;
}
template<
	typename T
>
__constexpr_imp
	void
	gen_data_base_t<T>::set_data_using_mode_and_string_representing_repetition_data_l1(
		const std::size_t _a_mode,
		const std::string_view _a_additional_data
	)
{
	using namespace utility::io;
	using namespace errors;
	using namespace utility::str;
	if (_a_mode == _S_THIS_CLASS_MODE)
	{
		//runs all the logic for reading from the file.
		if (_m_associated_file.has_value())
		{
			file_rw_t<T>& _l_associated_file{ _m_associated_file.value() };
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
				const size_t _l_file_element{ _m_rw_info.parser().run_parser_with_exception(_a_additional_data)};
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
		//Call the subclass 
		set_data_using_mode_and_string_representing_repetition_data_l2(_a_mode, _a_additional_data);
	}
	_m_repetition_data.set_mode(_a_mode);
	_m_repetition_data.set_string(get_repetition_data_as_string());
	_m_has_been_called_previously = false;
}
template<
	typename T
>
__constexpr_imp
	ds::repetition_data_t
	gen_data_base_t<T>::write_failed_value_to_file_and_return_repetition_data_l1(
	) noexcept
{
	using namespace ds;
	using namespace std;
	if (not_in_read_file_mode_and_can_write_to_file())
	{
		if (not _m_has_been_called_previously)
		{
			//Only write the data if this hasn't been called before.
			_m_associated_file.value().write_element(subclass_current_element());
			_m_has_been_called_previously = true;
		}
		//Call subclass variant incase data also needs to be written to a folder in the subclass,
		// however we always return the element from the top file if it writes to more than one file.
		write_failed_value_to_file_and_return_repetition_data_l2();
		//Create new repetition_data which points to the entry in the file.
		return repetition_data_t(_m_repetition_data.for_loop_index(), 
			_m_repetition_data.generation_collection_index(),
			0, repetition_data_as_string());
	}
	else
	{
		return write_failed_value_to_file_and_return_repetition_data_l2();
	}
}
template<
	typename T
>
__constexpr_imp
	test_failure_func_t
	gen_data_base_t<T>::create_test_failure_function(
	) noexcept
{
	return [&]() 
	{
		return (*this).write_failed_value_to_file_and_return_repetition_data();
	};
}*/
	/*template<
		typename T
	>
	__constexpr_imp
		bool
		gen_data_base_t<T>::set_next_mode(
		) noexcept
	{
		_m_repetition_data.set_mode(1);
		return has_current_element_();
	}*/
/*template<
	typename T
>
__constexpr_imp
	bool
	gen_data_base_t<T>::associated_file_has_current_element(
	) const noexcept
{
	return _m_repetition_data.mode() == _S_THIS_CLASS_MODE &&
		_m_associated_file.has_value() &&
		_m_associated_file.value().has_current_element();
}
	template<
		typename T
	>
	__constexpr_imp
		bool
		gen_data_base_t<T>::not_in_read_file_mode_and_can_write_to_file(
		) const noexcept
	{
		return (_m_associated_file.has_value() && _m_repetition_data.mode() != _S_THIS_CLASS_MODE 
			&& are_failed_values_written_to_files());
	}
template<
	typename T
>
template<
	typename R
>
__constexpr_imp
	gen_data_base_t<T>::gen_data_base_t(
		const size_t _a_elements_generated,
		const utility::io::opt_file_rw_info_t<T>& _a_templated_file_rw,
		R&& _a_elements,
		const test_options_t* _a_test_options_ptr,
		test_runner_t* _a_test_runner_ptr
	) noexcept
	: _m_elements_generated(_a_elements_generated)
	, _m_test_options(_a_test_options_ptr)
	, _m_test_runner(global::get_this_threads_test_runner_ptr())
	, _m_associated_file(_a_templated_file_rw.has_value() ?
		utility::io::opt_file_rw_t<T>(
			utility::io::file_rw_t<T>(_a_templated_file_rw.value(),_a_elements)) :
		utility::io::opt_file_rw_t<T>{})
	, _m_repetition_data(ds::repetition_data_t(_a_templated_file_rw.has_value() ? 0 : 1))
	, _m_has_been_called_previously(false)
	, _m_rw_info(utility::str::rw_info_t<file_data_index_t>())
{

}
template<
	typename T
>
__constexpr_imp
	std::string
	gen_data_base_t<T>::repetition_data_as_string(
	) const noexcept
{
	return _m_rw_info.printer().run_printer(_m_associated_file.value().elements_read());
}*/
_END_ABC_NS