#pragma once

#include "abc_test/gen_data/gen_data_base.h"
#include "abc_test/core/test_runner.h"
#include <random>
#include "abc_test/gen_data/static_data.h"
#include <concepts>
#include "abc_test/utility/io/file/file_reader.h"
#include "abc_test/utility/io/file/file_writer.h"
#include "abc_test/utility/io/file/file_name.h"
#include "abc_test/gen_data/gen_data_with_repetition_type.h"
#include "abc_test/gen_data/random/base.h"
#include "abc_test/gen_data/random_data_description.h"
#include "abc_test/utility/str/rw_info.h"

_BEGIN_ABC_NS
using rep_data_t = std::pair<std::size_t, std::size_t>;
/*!
* Object which contains randomly generated data. Also contains the mechanisms for
* writing failed cases to a file.
*/
template<
	typename T
>
struct random_data_t : public gen_data_with_repetition_type_t<T, rep_data_t>
{
public:
	/*!
	* Constructor which takes a random generator function and a file name.
	* The file anme is where failing test cases are ran, and where they are read from.
	*/
	__constexpr
		random_data_t(
			random_generator_t<T>* _a_rnd_base = new random_generator_t<T>(),
			const utility::io::opt_file_rw_info_t<T>& _a_templated_file_rw = utility::io::opt_file_rw_info_t<T>{}
		) noexcept;
	/*
	* Checks if there is a current element.
	*/
	__constexpr
		virtual
		bool
		subclass_has_current_element(
		) const noexcept;
	/*!
	* Returns the current element.
	*/
	__constexpr
		virtual
		const T&
		subclass_current_element(
		) const noexcept override;
	/*!
	* Generates the next element in the series.
	*/
	__constexpr
		virtual
		bool
		subclass_generate_next(
		);
	/*!
	* Gets the additional string data. This is an override for random_data.
	*/
	__constexpr
		virtual
		rep_data_t
		subclass_get_repetition_data(
		) const noexcept override;
	/*!
	* Additional data is used to store a state of where a specific gen_data element is.
	* 
	* This funciton takes that additional data and puts the random_data object into the state described
	* in the string given.
	*/
	__constexpr
		virtual
		void
		subclass_set_data_using_mode_and_repetition_data(
			const std::size_t _a_mode,
			const rep_data_t _a_additional_data
		);
	__constexpr
		virtual
		bool
		are_failed_values_written_to_files(
		) const noexcept;
	__constexpr
		virtual 
		void
		subclass_reset_data(
		) noexcept final
	{

	}
private:
	random_generator_t<T>* _m_random_generator;
	size_t _m_elemnets_to_randomly_generate;
	T _m_element;
	//! The number of times the generator has been called before the current element was created
	std::size_t _m_random_gen_calls_before;
	//! The number of times the generator has been called after the element was created.
	std::size_t _m_random_gen_calls_after;
	__constexpr
		bool
		generate_data(
		) noexcept;
};
/*!
* Useful function which generates a random_data object in the required pointer form.
*/
template<
	typename T
>
__constexpr
	gen_data_collection_t<T>
	random_data(
		random_generator_t<T>* _a_rnd_base = new random_generator_t<T>()
	);
template<
	typename T
>
__constexpr
	gen_data_collection_t<T>
	random_data(
		utility::io::file_name_t& _a_file_name,
		random_generator_t<T>* _a_rnd_base = new random_generator_t<T>()
	);
template<
	typename T
>
gen_data_collection_t<T>
	random_data(
		const utility::io::file_name_t& _a_file_name
	) noexcept
{
	return unary_collection<T>(nullptr);
}
template<
	typename T
>
gen_data_collection_t<T>
	random_data(
		random_generator_t<T>* _a_rnd_base,
		const utility::str::rw_info_t<T>& _a_rw_info
	) noexcept
{
	return unary_collection<T>(new random_data_t<T>(_a_rnd_base));
}
/*!
* Useful function which generates a random_data object in the required pointer form.
*/
template<
	typename T
>
__constexpr
	gen_data_collection_t<T>
	random_data(
		random_generator_t<T>* _a_rnd_base,
		const std::optional<utility::io::file_rw_info_t<T>>& _a_opt_tfrw
	);
_END_ABC_NS

_BEGIN_ABC_NS
template<
	typename T
>
__constexpr_imp
	bool
	random_data_t<T>::subclass_has_current_element(
	) const noexcept
{
	return (
		this->_m_elements_generated <= _m_elemnets_to_randomly_generate
		); 
}
template<
	typename T
>
__constexpr_imp
	const T&
	random_data_t<T>::subclass_current_element(
	) const noexcept
{
	return _m_element;
}
template<
	typename T
>
__constexpr_imp
	bool
	random_data_t<T>::subclass_generate_next(
	)
{
	return generate_data();
}
template<
	typename T
>
__constexpr_imp
rep_data_t
	random_data_t<T>::subclass_get_repetition_data(
	) const noexcept
{
	return { _m_random_gen_calls_before, _m_random_gen_calls_after };
}
template<
	typename T
>
__constexpr_imp
	void
	random_data_t<T>::subclass_set_data_using_mode_and_repetition_data(
		const std::size_t _a_mode,
		const rep_data_t _a_additional_data2
	)
{
	using namespace std;
	using namespace scn;
	using namespace errors;
	using namespace utility;
	std::string _a_additional_data;
	if (_a_mode == 1)
	{
		auto _l_scn_pair{ scan<pair<size_t,size_t>>(_a_additional_data, "{}") };
		if (_l_scn_pair.has_value())
		{
			size_t _l_calls_before{ _l_scn_pair->value().first };
			size_t _l_calls_after{ _l_scn_pair->value().second };
			if (_l_calls_before < _m_random_gen_calls_before ||
				_l_calls_after < _m_random_gen_calls_after)
			{
				throw test_library_exception_t("");
			}
			else if (_l_calls_before > _m_random_gen_calls_before &&
				_l_calls_after > _m_random_gen_calls_after)
			{
				rng& _m_rndgen{ this->_m_test_runner->current_test().get_random_generator() };
				_m_rndgen.progress(_l_calls_before);
				_m_random_gen_calls_before = _l_calls_before;
				_m_element = _m_random_generator->operator()(_m_rndgen);
				_m_random_gen_calls_after = _l_calls_after;
			}
		}
		else
		{
			throw test_library_exception_t(
				fmt::format("Could not parse \"{0}\" to {1}.",
					_a_additional_data, typeid(pair<size_t, size_t>).name()));
		}
	}
	else
	{
		throw test_library_exception_t("");
	}
}
template<
	typename T
>
__constexpr_imp
	bool
	random_data_t<T>::are_failed_values_written_to_files(
	) const noexcept
{
	return true;
	//return this->_m_test_options->_m_write_random_data_to_file;
}
template<
	typename T
>
__constexpr_imp
	random_data_t<T>::random_data_t(
		random_generator_t<T>* _a_random_generator,
		const utility::io::opt_file_rw_info_t<T>& _a_templated_file_rw
	) noexcept
	: gen_data_with_repetition_type_t<T, rep_data_t>(0, _a_templated_file_rw, {},
		{})
	//: gen_data_base_t<T>(0,
	//	_a_templated_file_rw,std::vector<T>())
	//, _m_random_generator(_a_random_generator)
	//, _m_elemnets_to_randomly_generate(
	//	this->_m_test_options->_m_default_numb_random_elements_to_generate)
	//, _m_element(T{})
	//, _m_random_gen_calls_before(0)
	//, _m_random_gen_calls_after(0)
{
	generate_data();
}
template<
	typename T
>
__constexpr_imp
	bool
	random_data_t<T>::generate_data(
	) noexcept
{
	++this->_m_elements_generated;
	if (this->_m_elements_generated > _m_elemnets_to_randomly_generate)
	{
		return false;
	}
	else
	{
		utility::rng& _m_rndgen{ this->_m_test_runner->current_test().get_random_generator() };
		_m_random_gen_calls_before = _m_rndgen.calls();
		_m_element = _m_random_generator->operator()(_m_rndgen);
		_m_random_gen_calls_after = _m_rndgen.calls();
		return true;
	}
}
template<
	typename T
>
__constexpr_imp
	gen_data_collection_t<T>
	random_data(
		random_generator_t<T>* _a_rnd_base
	)
{
	return unary_collection<T>(new random_data_t<T>(_a_rnd_base));
}
template<
	typename T
>
__constexpr_imp
	gen_data_collection_t<T>
	random_data(
		random_generator_t<T>* _a_rnd_base,
		const std::optional<utility::io::file_rw_info_t<T>>& _a_tfrwi
	)
{
	return unary_collection<T>(new random_data_t<T>(_a_rnd_base, _a_tfrwi));
}
_END_ABC_NS