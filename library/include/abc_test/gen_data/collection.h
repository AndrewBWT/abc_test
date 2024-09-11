#pragma once
#include <vector>
#include <memory>
#include "abc_test/gen_data/gen_data_base.h"
#include <cstdarg>

_BEGIN_ABC_GD_NS
	//Forward declaration
	template<
		typename T
	>
	struct gen_data_collection_iterator_t;
	/*!
	* Describes a collection of gen_data elements. This object contains the logic for allowing for loops
	* to work properly with a collection of gen_data elements.
	*/
	template<
		typename T
	>
	struct gen_data_collection_t
	{
	public:
		//! An individual gen_data object.
		using gen_data_element_t = gen_data_ptr_t<T>;
		//! The internal collection of gen_data_element_t objects
		using gen_data_collection_internal_t = std::vector<gen_data_element_t>;
		//! Iterator for the object
		using iterator = gen_data_collection_iterator_t<T>;
		//! Const iterator for the object
		using const_iterator = gen_data_collection_iterator_t<T>;
		/*!
		* Default constructor for hte object
		*/
		__constexpr
			gen_data_collection_t(
			) noexcept;
		/*!
		* Constructor taking arbitrary collection of gen_data objects.
		*/
		template<
			typename... Args
		>
		__constexpr
			gen_data_collection_t(
				Args ... _a_elements
			) noexcept;
		/*!
		* Gets begin iterator for object
		*/
		__constexpr
			iterator
			begin(
			);
		/*!
		* Gets end iterator for object
		*/
		__constexpr
			iterator
			end(
			);
		/*!
		* Gets const begin iterator for object
		*/
		__constexpr
			const_iterator
			begin(
			) const;
		/*!
		* Gets const end iterator for object
		*/
		__constexpr
			const_iterator
			end(
			) const;
		/*!
		* Gets const begin iterator for object
		*/
		__constexpr
			const_iterator
			cbegin(
			) const;
		/*!
		* Gets const end iterator for object
		*/
		__constexpr
			const_iterator
			cend(
			) const;
		/*!
		* Gets the internal collection of elements
		*/
		__constexpr
			const gen_data_collection_internal_t&
			get_elements(
			) const noexcept;
	private:
		gen_data_collection_internal_t _m_elements;
		__constexpr
			iterator
			gen_iterator(
				gen_data_collection_internal_t::iterator _a_iterator,
				const bool _a_add_repeatable_test_config,
				test_runner_t* _a_test_runner = global::get_this_threads_test_runner_ptr()
			);
		__constexpr
			iterator
			gen_iterator_begin(
			);
		__constexpr
			iterator
			gen_iterator_end(
			);
	};

	template<
		typename T
	>
	__constexpr
		gen_data_collection_t<T>
		unary_collection(
			gen_data_base_t<T>* _a_pointer
		) noexcept;
	namespace
	{
		template<
			typename T
		>
		__constexpr
			void
			process_elements(
				std::vector<gen_data_ptr_t<T>>& _l_vect,
				const gen_data_ptr_t<T>& _a_element
			) noexcept;
		template<
			typename T
		>
		__constexpr
			void
			process_elements(
				std::vector<gen_data_ptr_t<T>>& _l_vect,
				const gen_data_collection_t<T>& _a_gdc
			) noexcept;
		template<
			typename T,
			typename... Args
		>
		__constexpr
			void
			process_elements(
				std::vector<gen_data_ptr_t<T>>& _l_vect,
				const gen_data_ptr_t<T>& _a_element,
				Args... _a_elements
			) noexcept;
		template<
			typename T,
			typename... Args
		>
		__constexpr
			void
			process_elements(
				std::vector<gen_data_ptr_t<T>>& _l_vect,
				const gen_data_collection_t<T>& _a_gdc,
				Args... _a_elements
			) noexcept;
		template<
			typename T,
			typename... Args
		>
		__constexpr
			std::vector<gen_data_ptr_t<T>>
			process_elements(
				Args... _a_elements
			) noexcept;
	}
_END_ABC_NS
_END_NS

_BEGIN_ABC_GD_NS
	template<
		typename T
	>
	__constexpr_imp
		gen_data_collection_t<T>::gen_data_collection_t(
		) noexcept
	{

	}
	template<
		typename T
	>
	template<
		typename... Args
	>
	__constexpr_imp
		gen_data_collection_t<T>::gen_data_collection_t(
			Args ... _a_elements
		) noexcept
		: _m_elements(process_elements<T>(_a_elements...))
	{
	}
	template<
		typename T
	>
	__constexpr_imp
		gen_data_collection_t<T>::iterator
		gen_data_collection_t<T>::begin(
		)
	{
		return gen_iterator_begin();
	}
	template<
		typename T
	>
	__constexpr_imp
		gen_data_collection_t<T>::iterator
		gen_data_collection_t<T>::end(
		)
	{
		return gen_iterator_end();
	}
	template<
		typename T
	>
	__constexpr_imp
		gen_data_collection_t<T>::const_iterator
		gen_data_collection_t<T>::begin(
		) const
	{
		return gen_iterator_begin();
	}
	template<
		typename T
	>
	__constexpr_imp
		gen_data_collection_t<T>::const_iterator
		gen_data_collection_t<T>::end(
		) const
	{
		return gen_iterator_end();
	}
	template<
		typename T
	>
	__constexpr_imp
		gen_data_collection_t<T>::const_iterator
		gen_data_collection_t<T>::cbegin(
		) const
	{
		return _m_elements.cbegin();
	}
	template<
		typename T
	>
	__constexpr_imp
		gen_data_collection_t<T>::const_iterator
		gen_data_collection_t<T>::cend(
		) const
	{
		return _m_elements.cend();
	}
	template<
		typename T
	>
	__constexpr_imp
		const gen_data_collection_t<T>::gen_data_collection_internal_t&
		gen_data_collection_t<T>::get_elements(
		) const noexcept
	{
		return _m_elements;
	}
	template<
		typename T
	>
	__constexpr_imp
		gen_data_collection_t<T>::iterator
		gen_data_collection_t<T>::gen_iterator(
			gen_data_collection_internal_t::iterator _a_iterator,
			const bool _a_add_repeatable_test_config,
			test_runner_t* _a_test_runner
		)
	{
		return iterator(_m_elements.begin(), _m_elements.end(), _a_iterator, _a_add_repeatable_test_config,
			_a_test_runner);
	}
	template<
		typename T
	>
	__constexpr_imp
		gen_data_collection_t<T>::iterator
		gen_data_collection_t<T>::gen_iterator_begin(
		)
	{
		return gen_iterator(_m_elements.begin(), true);
	}
	template<
		typename T
	>
	__constexpr_imp
		gen_data_collection_t<T>::iterator
		gen_data_collection_t<T>::gen_iterator_end(
		)
	{
		return gen_iterator(_m_elements.end(), false);
	}
	template<
		typename T
	>
	__constexpr_imp
		gen_data_collection_t<T>
		unary_collection(
			gen_data_base_t<T>* _a_pointer
		) noexcept
	{
		using namespace std;
		return gen_data_collection_t<T>(gen_data_ptr_t<T>(_a_pointer));
	}

	namespace
	{
		template<
			typename T
		>
		__constexpr_imp
			void
			process_elements(
				std::vector<gen_data_ptr_t<T>>& _l_vect,
				const gen_data_ptr_t<T>& _a_element
			) noexcept
		{
			if (_a_element.get()->has_current_element())
			{
				_l_vect.push_back(_a_element);
			}
		}
		template<
			typename T
		>
		__constexpr_imp
			void
			process_elements(
				std::vector<gen_data_ptr_t<T>>& _l_vect,
				const gen_data_collection_t<T>& _a_gdc
			) noexcept
		{
			for (const gen_data_ptr_t<T> _a_ptr : _a_gdc.get_elements())
			{
				process_elements(_l_vect, _a_ptr);
			}
		}
		template<
			typename T,
			typename... Args
		>
		__constexpr_imp
			void
			process_elements(
				std::vector<gen_data_ptr_t<T>>& _l_vect,
				const gen_data_ptr_t<T>& _a_element,
				Args... _a_elements
			) noexcept
		{
			if (_a_element.get()->has_current_element())
			{
				_l_vect.push_back(_a_element);
			}
			process_elements(_l_vect, _a_elements...);
		}
		template<
			typename T,
			typename... Args
		>
		__constexpr_imp
			void
			process_elements(
				std::vector<gen_data_ptr_t<T>>& _l_vect,
				const gen_data_collection_t<T>& _a_gdc,
				Args... _a_elements
			) noexcept
		{
			for (const gen_data_ptr_t<T> _a_ptr : _a_gdc.get_elements())
			{
				process_elements(_l_vect, _a_ptr);
			}
			process_elements(_l_vect, _a_elements...);
		}
		template<
			typename T,
			typename... Args
		>
		__constexpr_imp
			std::vector<gen_data_ptr_t<T>>
			process_elements(
				Args... _a_elements
			) noexcept
		{
			using namespace std;
			std::vector<gen_data_ptr_t<T>> _l_rv{};
			process_elements<T>(_l_rv, _a_elements...);
			return _l_rv;
		}
	}
	_END_ABC_NS
		_END_NS