#pragma once
#include "abc_test/utility/io/file/file_rw_info.h"

_BEGIN_ABC_UTILITY_IO_NS
	template<
		typename T
	>
	struct file_rw_t
	{
	public:
		/*!
		* Default deleted.
		*/
		//__constexpr
		//	file_rw_t(
		//	);
		/*!
		* Setup with object which has a range of elements to be written to the file
		*/
		__constexpr_imp
			file_rw_t(
				const file_rw_info_t<T>& _a_tfrwi
			)
		{

		}
		template<
			typename R = std::vector<T>
		>
		__constexpr
			file_rw_t(
				const file_rw_info_t<T>& _a_tfrwi,
				const std::string_view _a_extension,
				const test_options_t* _a_test_options,
				R&& _a_elements = {}
			);
		template<
			typename R
		>
		__constexpr
			file_rw_t(
				const file_rw_info_t<T>& _a_tfrwi,
				R&& _a_elements = {}
			);
		template<
			typename R
		>
		__constexpr
			file_rw_t(
				const file_rw_info_t<T>& _a_tfrwi,
				const test_options_t* _a_test_options_ptr,
				R&& _a_elements = {}
			);
		__constexpr
			bool
			read_all_elements(
			) const noexcept;
		__constexpr
			bool
			has_current_element(
			) const noexcept;
		__constexpr
			std::optional<std::string>
			write_element(
				const T& _a_element
			) noexcept;
		__constexpr
			bool
			read_next_element(
			);
		__constexpr
			const T&
			current_element(
			) const noexcept;
		__constexpr
			std::size_t
			elements_read(
			) const noexcept;
	private:
		T _m_element;
		file_name_with_extension_t _m_fnwe;
		utility::str::parser_t<T> _m_parser;
		utility::str::printer_t<T> _m_printer;
		mutable std::size_t _m_mode;
		file_line_reader_t _m_reader;
		file_line_writer_t _m_writer;
		std::size_t _m_elements_read;
		const test_options_t* _m_options;
		__constexpr
			void
			setup_writer(
			) noexcept;
		__constexpr
			void
			read_line(
			) noexcept;
	};
	template<
		typename T
	>
	using opt_file_rw_t = std::optional<file_rw_t<T>>;
	_END_ABC_UTILITY_IO_NS

_BEGIN_ABC_UTILITY_IO_NS
	template<
		typename T
	>
	template<
		typename R
	>
	__constexpr_imp
		file_rw_t<T>::file_rw_t(
			const file_rw_info_t<T>& _a_tfrwi,
			const std::string_view _a_extension,
			const test_options_t* _a_test_options,
			R&& _a_elements
		)
		: _m_fnwe(file_name_with_extension_t(
			_a_tfrwi.file_name(), _a_extension
			))
		, _m_parser(_a_tfrwi.parser())
		, _m_printer(_a_tfrwi.printer())
		, _m_mode(0)
		, _m_element(T{})
		, _m_reader(file_line_reader_t())
		, _m_elements_read(0)
		, _m_writer(file_line_writer_t())
		, _m_options(_a_test_options)
	{
		if (not _m_fnwe.exists())
		{
			file_line_writer_t _l_flw(_m_fnwe, _m_options);
			_l_flw.write_comment(typeid(T).name());
			for (auto&& _l_element : _a_elements)
			{
				_l_flw.write_line(_m_printer.run_printer(_l_element));
			}
		}
		_m_reader = file_line_reader_t(_m_fnwe, _a_test_options);
		if (_m_reader.has_current_line())
		{
			read_line();
		}
		else
		{
			setup_writer();
		}
	}
	template<
		typename T
	>
	template<
		typename R
	>
	__constexpr_imp
		file_rw_t<T>::file_rw_t(
			const file_rw_info_t<T>& _a_tfrwi,
			R&& _a_elements
		)
		: file_rw_t(_a_tfrwi,global::get_global_test_options_ptr(),_a_elements)
	{

	}
	template<
		typename T
	>
	template<
		typename R
	>
	__constexpr_imp
		file_rw_t<T>::file_rw_t(
			const file_rw_info_t<T>& _a_tfrwi,
			const test_options_t* _a_test_options_ptr,
			R&& _a_elements
		)
		: file_rw_t<T>(_a_tfrwi, _a_test_options_ptr->_m_general_data_extension, _a_test_options_ptr,_a_elements)
	{
	}
	template<
		typename T
	>
	__constexpr_imp
		bool
		file_rw_t<T>::read_all_elements(
		) const noexcept
	{
		using namespace std;
		using namespace utility;
		return _m_mode == 1;
	}
	template<
		typename T
	>
	__constexpr_imp
		bool
		file_rw_t<T>::has_current_element(
		) const noexcept
	{
		return _m_mode == 0;
	}
	template<
		typename T
	>
	__constexpr_imp
		std::optional<std::string>
		file_rw_t<T>::write_element(
			const T& _a_element
		) noexcept
	{
		using namespace std;
		++_m_elements_read;
		_m_writer.write_line(_m_printer.run_printer(_a_element));
		return optional<string>{};
	}
	template<
		typename T
	>
	__constexpr_imp
		bool
		file_rw_t<T>::read_next_element(
		)
	{
		if (_m_mode == 0)
		{
			const bool _l_line_read{ _m_reader.get_next_line() };
			if (_l_line_read)
			{
				read_line();
			}
			else
			{
				setup_writer();
			}
			return _l_line_read;
		}
		else
		{
			return false;
		}
	}
	template<
		typename T
	>
	__constexpr_imp
		const T&
		file_rw_t<T>::current_element(
		) const noexcept
	{
		return _m_element;
	}
	template<
		typename T
	>
	__constexpr_imp
		std::size_t
		file_rw_t<T>::elements_read(
		) const noexcept
	{
		return _m_elements_read;
	}
	template<
		typename T
	>
	__constexpr_imp
		void
		file_rw_t<T>::setup_writer(
		) noexcept
	{
		using namespace utility::io;
		_m_reader = file_line_reader_t();
		_m_writer = file_line_writer_t(_m_fnwe, _m_options);
		_m_mode = 1;
	}
	template<
		typename T
	>
	__constexpr_imp
		void
		file_rw_t<T>::read_line(
		) noexcept
	{
		++_m_elements_read;
		_m_element = _m_parser.run_parser_with_exception(_m_reader.current_line());
	}
	_END_ABC_UTILITY_IO_NS