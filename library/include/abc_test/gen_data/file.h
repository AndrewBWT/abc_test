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
#include "abc_test/gen_data/static_data.h"

#include "abc_test/utility/io/file/file_reader.h"
#include "abc_test/utility/io/file/file_writer.h"

#include "abc_test/global.h"

_BEGIN_ABC_GD_NS
	/*!
	* Object describing data taken from a file
	*/
	template<
		typename T
	>
	struct file_data_t : public gen_data_base_t<T>
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
		/*!
		* Determines whether the object has a current element. As this is a file streaming object,
		* there are no additional modes assocated with it. therefore, it always returns false.
		*/
		__constexpr
			virtual
			bool
			has_current_element_(
			) const noexcept;
		/*!
		* Returns the current object. In this case, it will always return a default constructed T object.
		*/
		__constexpr
			virtual
			const T&
			current_element_(
			) const noexcept;
		/*!
		* Generates the next element. Will alwyas return false.
		*/
		__constexpr
			virtual
			bool
			generate_next_(
			);
		__constexpr
			virtual
			void
			increment_using_additional_data_(
				const std::size_t _a_mode,
				const std::string_view _a_additional_data
			);
		__constexpr
			virtual
			bool
			are_failed_values_written_to_files(
			) const noexcept;
		__constexpr
			virtual
			std::string
			get_additional_string_data_(
			) const noexcept
		{
			return "";
		}
	};
	template<
		typename T
	>
	__constexpr
		gen_data_ptr_t<T>
		file_data(
			const utility::io::file_name_t _a_filename
		);
	template<
		typename T,
		typename R
	>
	__constexpr
		gen_data_ptr_t<T>
		file_data(
			const utility::io::file_name_t _a_filename,
			R&& _a_init_elements
		);
	__constexpr
		const std::string_view
		file_data_extension(
		) noexcept;
	_END_ABC_NS
		_END_NS

		_BEGIN_ABC_GD_NS
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
		: gen_data_base_t<T>(0, std::optional<utility::io::file_rw_info_t<T>>(_a_templated_file_rw), 
			file_data_extension(),
			_a_initial_values)
	{
	}
	template<
		typename T
	>
	__constexpr_imp
		bool
		file_data_t<T>::has_current_element_(
		) const noexcept 
	{
		return false;
	}
	template<
		typename T
	>
	__constexpr_imp
		const T&
		file_data_t<T>::current_element_(
		) const noexcept
	{
		return this->_m_associated_file.value().current_element();
	}
	template<
		typename T
	>
	__constexpr_imp
		bool
		file_data_t<T>::generate_next_(
		)
	{
		return false;
	}
	template<
		typename T
	>
	__constexpr_imp
		void
		file_data_t<T>::increment_using_additional_data_(
			const std::size_t _a_mode,
			const std::string_view _a_additional_data
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
		typename T
	>
	__constexpr_imp
		gen_data_ptr_t<T>
		file_data(
			const utility::io::file_name_t _a_filename
		)
	{
		using namespace std;
		return gen_data_ptr_t<T>(new file_data_t<T>(_a_filename));
	}

	template<
		typename T,
		typename R
	>
	__constexpr_imp
		gen_data_ptr_t<T>
		file_data(
			const utility::io::file_name_t _a_filename,
			R&& _a_init_elements
		)
	{
		using namespace std;
		return gen_data_ptr_t<T>(new file_data_t<T>(
			utility::io::file_rw_info_t<T>(_a_filename), forward<R>(_a_init_elements)));
	}
	__constexpr_imp
		const std::string_view
		file_data_extension(
		) noexcept
	{
		return global::get_global_test_options()._m_file_data_extension;
	}
	_END_ABC_NS
		_END_NS