#pragma once

#include <initializer_list>
#include <vector>
#include "abc_test/core/test_runner.h"
#include "abc_test/gen_data/collection.h"

_BEGIN_ABC_GD_NS
	template<
		typename T
	>
	struct static_data_t : public gen_data_base_t<T>
	{
	public:
		/*!
		* Default constructor which uses an empty list
		*/
		__constexpr
			static_data_t(
			) noexcept;
		/*!
		* Constructor which takes a range.
		*/
		template<
			typename R
		>
			requires std::ranges::sized_range<R>&& std::same_as<std::ranges::range_value_t<R>, T>
		__constexpr
			static_data_t(
				R&& _a_init_list
			) noexcept;
		/*!
		* Destructor which destroys the internal array.
		*/
		__constexpr
			~static_data_t(
			);
		/*!
		* Overridden function which gets the string form of the relevant data. Specifically in
		* this case its the index of the elemnet.
		*/
		__constexpr
			virtual
			std::string
			get_additional_string_data_(
			) const noexcept;
		/*!
		* Determines whether there is a current element in the collection.
		*/
		__constexpr
			virtual
			bool
			has_current_element_(
			) const noexcept;
		/*!
		* Returns a const reference to the current element.
		*/
		__constexpr
			virtual
			const T&
			current_element_(
			) const noexcept;
		/*!
		* Increments the object, getting the next element in the internal collection.
		*/
		__constexpr
			virtual
			bool
			generate_next_(
			);
		/*!
		* Increments the internal mechanisms using a string to represent additinoal data.
		*/
		__constexpr
			virtual
			void
			increment_using_additional_data_(
				const std::size_t _a_mode,
				const std::string_view _a_additional_data
			);
		/*!
		* Determines whether failed values can be written to a file. For this class, they cannot.
		*/
		__constexpr
			virtual
			bool
			are_failed_values_written_to_files(
			) const noexcept;
	private:
		T* _m_elements;
		std::size_t _m_elements_size;
	};
	/*!
	* Constructor which should be used. It creates a singleton collection.
	*/
	template<
		typename R
	>
	__constexpr
		gen_data_collection_t<std::ranges::range_value_t<R>>
		iterate_over(
			R&& _a_init_list
		);
_END_ABC_NS
_END_NS

_BEGIN_ABC_GD_NS
	template<
		typename T
	>
	__constexpr_imp
		static_data_t<T>::static_data_t(
		) noexcept
		: gen_data_base_t<T>(0, std::optional<utility::io::file_rw_info_t<T>>{})
		, _m_elements(nullptr)
		, _m_elements_size(0)
	{

	}
	template<
		typename T
	>
	template<
		typename R
	>
	requires std::ranges::sized_range<R> && std::same_as<std::ranges::range_value_t<R>,T>
	__constexpr_imp
		static_data_t<T>::static_data_t(
			R&& _a_init_list
		) noexcept
		: gen_data_base_t<T>(0, std::optional<utility::io::file_rw_info_t<T>>{}, "", std::vector<T>())
		, _m_elements(std::ranges::size(_a_init_list) == 0 ?
			nullptr : 
			new T[std::ranges::size(_a_init_list)])
		, _m_elements_size(_a_init_list.size())
	{
		for (std::size_t _l_idx{ 0 }; auto&& _l_element : _a_init_list)
		{
			_m_elements[_l_idx++] = _l_element;
		}
	}
	template<
		typename T
	>
	__constexpr_imp
		static_data_t<T>::~static_data_t(
		)
	{
		if (_m_elements_size > 0)
		{
			delete _m_elements;
		}
	}
	template<
		typename T
	>
	__constexpr_imp
		std::string
		static_data_t<T>::get_additional_string_data_(
		) const noexcept
	{
		using namespace std;
		return to_string(this->_m_elements_generated);
	}
	template<
		typename T
	>
	__constexpr_imp
		bool
		static_data_t<T>::has_current_element_(
		) const noexcept
	{
		return this->_m_elements_generated < _m_elements_size;
	}
	template<
		typename T
	>
	__constexpr_imp
		const T&
		static_data_t<T>::current_element_(
		) const noexcept
	{
		return _m_elements[this->_m_elements_generated];
	}
	template<
		typename T
	>
	__constexpr_imp
		bool
		static_data_t<T>::generate_next_(
		)
	{
		this->_m_elements_generated++;
		return has_current_element_();
	}
	template<
		typename T
	>
	__constexpr_imp
		void
		static_data_t<T>::increment_using_additional_data_(
			const std::size_t _a_mode,
			const std::string_view _a_additional_data
		)
	{
		using namespace utility::str;
		using namespace errors;
		using namespace std;
		if (_a_mode == 1)
		{
			this->_m_elements_generated = parser_t<size_t>().run_parser_with_exception(_a_additional_data);
			if (this->_m_elements_generated >= _m_elements_size)
			{
				throw test_library_exception_t(
					fmt::format(
						"Could not instantiate static_data_t element at index {0} "
						"as size of static_data_t array is {1}.",
						this->_m_elements_generated,
						_m_elements_size));
			}
		}
		else
		{
			throw unsupported_mode_exception("static_data_t", _a_mode, { 0,1 });
		}
	}
	template<
		typename T
	>
	__constexpr_imp
		bool
		static_data_t<T>::are_failed_values_written_to_files(
		) const noexcept
	{
		return false;
	}
	template<
		typename R
	>
	__constexpr_imp
		gen_data_collection_t<std::ranges::range_value_t<R>>
		iterate_over(
			R&& _a_init_list
		)
	{
		using namespace std;
		using T = ranges::range_value_t<R>;
		return unary_collection<T>(new static_data_t<T>(forward<R>(_a_init_list)));
	}
_END_ABC_NS
_END_NS