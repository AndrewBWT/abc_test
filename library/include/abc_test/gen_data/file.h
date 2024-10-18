#pragma once

#include <initializer_list>
#include <vector>
#include "abc_test/core/test_runner.h"
#include "abc_test/gen_data/gen_data_base.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <scn/scan.h>
#include <scn/ranges.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include "abc_test/gen_data/gen_data_with_repetition_type_and_element.h"

#include "abc_test/utility/io/file/file_reader.h"
#include "abc_test/utility/io/file/file_writer.h"

#include "abc_test/core/global.h"

_BEGIN_ABC_NS
using file_type_rep_data_t = char;
/*!
* Object describing data taken from a file
*/
	template<
		typename T
	>
	struct file_data_t : public data_generator_with_file_support_and_element_storage_t<T, file_type_rep_data_t>
	{
	public:
		/*!
		* Default file data object. No default object
		*/
		__constexpr
			file_data_t(
			) noexcept = delete;
		/*!
		* Construct with initial values. Takes file name without extension and a set of 
		* initial values to write to the file if it doesn't previously exist.
		*/
		template<
			typename R = std::vector<T>
		>
		__constexpr
			file_data_t(
				const utility::io::file_rw_info_t<T>& _a_templated_file_rw,
				R&& _a_initial_values = {}
			);
		__constexpr
			virtual
			void
			subclass_reset_data(
			) noexcept final
		{

		}
		/*!
		* Determines whether the object has a current element. As this is a file streaming object,
		* there are no additional modes assocated with it. therefore, it always returns false.
		*/
		__constexpr
			virtual
			bool
			subclass_has_current_element(
			) const noexcept;
		/*!
		* Generates the next element. Will alwyas return false.
		*/
		__constexpr
			virtual
			bool
			subclass_generate_next(
			);
		__constexpr
			virtual
			void
			subclass_set_data_using_mode_and_repetition_data(
				const std::size_t _a_mode,
				const file_type_rep_data_t _a_rep_type
			);
		__constexpr
			virtual
			bool
			are_failed_values_written_to_files(
			) const noexcept;
		__constexpr
			virtual
			file_type_rep_data_t
			subclass_get_repetition_data(
			) const noexcept
		{
			return file_type_rep_data_t();
		}
	};
	template<
		typename T,
		typename R
	>
	__constexpr
		data_generator_collection_t<T>
		file_data(
			const utility::io::file_name_t& _a_filename,
			R&& _a_init_elements
		);
	_END_ABC_NS

		_BEGIN_ABC_NS
	template<
		typename T
	>
	template<
		typename R
	>
	__constexpr_imp
		file_data_t<T>::file_data_t(
			const utility::io::file_rw_info_t<T>& _a_templated_file_rw,
			R&& _a_initial_values
		)
		: data_generator_with_file_support_and_element_storage_t<T, file_type_rep_data_t>(0, _a_templated_file_rw,
			std::forward<R>(_a_initial_values), "", 
			utility::io::opt_file_rw_info_t<file_type_rep_data_t>{}, T{},
			global::get_global_test_options_ptr())
	{
	}
	template<
		typename T
	>
	__constexpr_imp
		bool
		file_data_t<T>::subclass_has_current_element(
		) const noexcept 
	{
		return false;
	}
	template<
		typename T
	>
	__constexpr_imp
		bool
		file_data_t<T>::subclass_generate_next(
		)
	{
		return false;
	}
	template<
		typename T
	>
	__constexpr_imp
		void
		file_data_t<T>::subclass_set_data_using_mode_and_repetition_data(
			const std::size_t _a_mode,
			const file_type_rep_data_t _a_rep_type
		) {

	}
	template<
		typename T
	>
	__constexpr_imp
		bool
		file_data_t<T>::are_failed_values_written_to_files(
		) const noexcept
	{
		return false;
	}

	template<
		typename T,
		typename R
	>
	__constexpr_imp
		data_generator_collection_t<T>
		file_data(
			const utility::io::file_name_t& _a_filename,
			R&& _a_init_elements = R{}
		)
	{
		using namespace std;
		return unary_collection(new file_data_t<T>(
			utility::io::file_rw_info_t<T>(_a_filename), forward<R>(_a_init_elements)));
	}
	_END_ABC_NS