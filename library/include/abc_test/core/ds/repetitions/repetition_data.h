#pragma once
#include <vector>
#include <tuple>

#include "fmt/format.h"
#include "abc_test/utility/internal/macros.h"

#include "abc_test/utility/internal/log.h"

_BEGIN_ABC_DS_NS
	/*!
	* Structure specificlaly designed to describe how a gen_data for loop has been navigated. 
	* 
	* It consists of the index in the for loop, the collection index (in case a collection of gen_datas was used)
	* and an additional data part, represented as a string.
	*/
	struct repetition_data_t
	{
	public:
		/*!
		* Default constructor. 
		*/
		__constexpr
			repetition_data_t(
			) noexcept;
		/*!
		* Generic constructor, usually used.
		*/
		__constexpr
			repetition_data_t(
				const size_t _a_for_loop_index,
				const size_t _a_generation_collection_index,
				const std::size_t _a_mode,
				const std::string_view _a_additional_data
			) noexcept;
		/*!
		* Equality function.
		*/
		__constexpr
			bool
			operator==(
				const repetition_data_t& _a_rhs
				) const noexcept;
		/*!
		* Spaceship operator function.
		*/
		__constexpr
			auto
			operator<=>(
				const repetition_data_t& _a_rhs
				) const noexcept;
		/*!
		* Gets the additional data part of the object.
		*/
		__constexpr
			const std::string_view
			additional_data(
			) const noexcept;
		/*!
		* Gets the generaiton collection index part of the object.
		*/
		__constexpr
			size_t
			generation_collection_index(
			) const noexcept;
		/*!
		* Gets the for lop index part of the object.
		*/
		__constexpr
			size_t
			for_loop_index(
			) const noexcept;
		/*!
		* Gets the mode of the object
		*/
		__constexpr
			std::size_t
			mode(
			) const noexcept;
		/*!
		* Changes the mode of the current object
		*/
		__constexpr
			void
			set_mode(
				const std::size_t _a_mode
			) noexcept; 
		/*!
		* Sets the for loop and generation collection index of the object
		*/
		__constexpr
			void
			set_for_loop_and_generation_collection_indexs(
				const std::size_t _a_for_loop_index,
				const std::size_t _a_generation_collection_index
			) noexcept;
		/*!
		* Sets the string of the object.
		*/
		__constexpr
			void
			set_string(
				const std::string_view _a_str
			) noexcept;
	private:
		std::size_t _m_for_loop_index;
		std::size_t _m_generation_collection_index;
		std::size_t _m_mode;
		std::string _m_additional_data;
	};
	using repetition_data_sequence_t = std::vector<repetition_data_t>;
	_END_ABC_DS_NS

template
<
>
struct fmt::formatter<abc::ds::repetition_data_t> : formatter<string_view>
{
	/*!
	* Provides a formatter for a poset_setup_test_data_t object
	*/
	__constexpr
		auto
		format(
			abc::ds::repetition_data_t _a_rd,
			format_context& _a_cxt
		) const
		->format_context::iterator;
};

	_BEGIN_ABC_DS_NS
	__constexpr_imp
		repetition_data_t::repetition_data_t(
		) noexcept
		: repetition_data_t(0, 0, 0, std::string_view{})
	{

	}
	__constexpr_imp
		repetition_data_t::repetition_data_t(
			const size_t _a_for_loop_index,
			const size_t _a_generation_collection_index,
			const std::size_t _a_mode,
			const std::string_view _a_additional_data
		) noexcept
		: _m_for_loop_index(_a_for_loop_index)
		, _m_generation_collection_index(_a_generation_collection_index)
		, _m_mode(_a_mode)
		, _m_additional_data(_a_additional_data)
	{

	}
	__constexpr_imp
		bool
		repetition_data_t::operator==(
			const repetition_data_t& _a_rhs
			) const noexcept
	{
		__cmp_test(_m_for_loop_index);
		__cmp_test(_m_generation_collection_index);
		__cmp_test(_m_additional_data);
		__cmp_test(_m_mode);
		return true;
	}
	__constexpr_imp
		auto
		repetition_data_t::operator<=>(
			const repetition_data_t& _a_rhs
			) const noexcept
	{
		using namespace std;
		return 
			tie(
				_m_for_loop_index,
				_m_generation_collection_index,
				_m_additional_data,
				_m_mode
			) 
			<=> 
			tie(
				_a_rhs._m_for_loop_index,
				_a_rhs._m_generation_collection_index,
				_a_rhs._m_additional_data,
				_a_rhs._m_mode
			);
	}
	__constexpr_imp
		const std::string_view
		repetition_data_t::additional_data(
		) const noexcept
	{
		return _m_additional_data;
	}
	__constexpr_imp
		size_t
		repetition_data_t::generation_collection_index(
		) const noexcept
	{
		return _m_generation_collection_index;
	}
	__constexpr_imp
		size_t
		repetition_data_t::for_loop_index(
		) const noexcept
	{
		return _m_for_loop_index;
	}
	__constexpr_imp
		std::size_t
		repetition_data_t::mode(
		) const noexcept
	{
		return _m_mode;
	}
	__constexpr_imp
		void
		repetition_data_t::set_mode(
			const std::size_t _a_mode
		) noexcept
	{
		_m_mode = _a_mode;
	}
	__constexpr_imp
		void
		repetition_data_t::set_for_loop_and_generation_collection_indexs(
			const std::size_t _a_for_loop_index,
			const std::size_t _a_generation_collection_index
		) noexcept
	{
		_m_for_loop_index = _a_for_loop_index;
		_m_generation_collection_index = _a_generation_collection_index;
	}
	__constexpr_imp
		void
		repetition_data_t::set_string(
			const std::string_view _a_str
		) noexcept
	{
		_m_additional_data = _a_str;
	}
	_END_ABC_DS_NS

__constexpr_imp
auto
fmt::formatter<abc::ds::repetition_data_t>::format(
	abc::ds::repetition_data_t _a_rd,
	format_context& _a_ctx
) const
-> format_context::iterator
{
	using namespace std;
	string _l_rv{ fmt::format(
		"repetition_data_t {{"
		"_m_for_loop_index = {0}, "
		"_m_generation_collection_index = {1}, "
		"_m_additional_data = {2}, "
		"_m_mode = {3}}}",
		_a_rd.for_loop_index(),
		_a_rd.generation_collection_index(),
		_a_rd.additional_data(),
		_a_rd.mode()
	) };
	return formatter<string_view>::format(_l_rv, _a_ctx);
}