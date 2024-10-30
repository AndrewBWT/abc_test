#pragma once

#include "abc_test/included_instances/gen_data/random/base.h"
#include "abc_test/included_instances/gen_data/random_data.h"
#include "abc_test/included_instances/gen_data/random_data_description.h"
#include "abc_test/included_instances/gen_data/static_data.h"
#include "abc_test/internal/data_generator/data_generator.h"
#include "abc_test/internal/data_generator/data_generator_with_file_and_element_support.h"
#include "abc_test/internal/test_runner.h"
#include "abc_test/internal/utility/io/file/file_name.h"
#include "abc_test/internal/utility/io/file/file_reader.h"
#include "abc_test/internal/utility/io/file/file_writer.h"
#include "abc_test/internal/utility/str/rw_info.h"

#include <concepts>
#include <random>

_BEGIN_ABC_DG_NS

template <typename T>
class random_data_t
{
public:
    using generator_type = T;
    using tertiary_type  = std::pair<std::size_t, std::size_t>;

    __constexpr const tertiary_type&
        tertiary_data() const
    {
        return _m_random_calls_before_after;
    }

    __constexpr void
        set_generator_using_tertiary_data(
            const tertiary_type& _a_tertiary_data
        )
    {
        _m_random_calls_before_after = _a_tertiary_data;
    }

    __constexpr void
        reset()
    {
        _l_elements_generated = 0;
        //! The number of times the generator has been called before the current
        //! element was created
        // std::size_t _m_random_gen_calls_before;
        //! The number of times the generator has been called after the element
        //! was created.
        // std::size_t _m_random_gen_calls_after;
        std::pair<std::size_t, std::size_t> _m_random_calls_before_after;
        T                                   _m_element;
    }

    __constexpr
    random_data_t(
        const std::shared_ptr<random_generator_t<T>>& _a_rnd
    )
        : _m_random_generator(_a_rnd)

    {}

    __constexpr bool
        has_current_element() const
    {
        return _l_elements_generated < _m_elemnets_to_randomly_generate;
    }

    __constexpr bool
        generate_next()
    {
        if (_l_elements_generated + 1 < _m_elemnets_to_randomly_generate)
        {
            _m_element = (*_m_random_generator)(
                global::get_this_threads_current_test().get_random_generator()
            );
            _l_elements_generated++;
            return true;
        }
        else
        {
            return false;
        }
    }

    __constexpr const T&
        current_element() const
    {
        return _m_element;
    }

    __constexpr const std::string
                      get_additional_data() const
    {
        return _m_rep_data.printer().run_printer(
            {_m_random_gen_calls_before, _m_random_gen_calls_after}
        );
    }
private:
    using random_rep_data_t = std::pair<std::size_t, std::size_t>;
    tertiary_type _m_random_calls_before_after;
    abc::utility::str::rw_info_t<random_rep_data_t> _m_rep_data;
    std::shared_ptr<random_generator_t<T>>          _m_random_generator;
    size_t      _m_elemnets_to_randomly_generate{10};
    std::size_t _l_elements_generated{0};
    //! The number of times the generator has been called before the current
    //! element was created
    std::size_t _m_random_gen_calls_before;
    //! The number of times the generator has been called after the element was
    //! created.
    std::size_t _m_random_gen_calls_after;
    T           _m_element;
};

_END_ABC_DG_NS

_BEGIN_ABC_NS
template <typename T>
__constexpr_imp _ABC_NS_DG::data_generator_collection_t<T, true>
                generate_data_randomly_internal(
                    std::shared_ptr<_ABC_NS_DG::random_generator_t<T>> _a_rnd_base,
                    const file_names_t<T>& _a_file_reader_writers
                    // const file_names_t<T>&             _a_file_reader_writers
                )
{
    using namespace _ABC_NS_DG;
    using namespace std;
    if (_a_file_reader_writers.size() == 0)
    {
		return unary_collection<T>(
			make_shared<data_generator_single_t<random_data_t<T>, false>>(
				random_data_t<T>(_a_rnd_base)
			)
		);
    }
	else if (_a_file_reader_writers.size() == 1)
	{
		return unary_collection<T>(
			make_shared<data_generator_single_t<random_data_t<T>, true>>(
				random_data_t<T>(_a_rnd_base), _a_file_reader_writers[0]
			)
		);
	}
    else
    {
        return unary_collection<T>(
            make_shared<data_generator_single_t<random_data_t<T>, true>>(
                random_data_t<T>(_a_rnd_base), _a_file_reader_writers[0],
				std::pair<typename file_names_t<T>::const_iterator,
				typename file_names_t<T>::const_iterator>(_a_file_reader_writers.begin() + 1,
					_a_file_reader_writers.end())
            )
        );
    }
}

template <typename T, typename... Args>
__constexpr void
process_args_internal(
	file_names_t<T>& _a_fns,
	const file_name_t<T>& _a_fn_no_type,
	Args... _a_elements
) noexcept
{
	_a_fns.push_back(_a_fn_no_type);
	process_args_internal<T>(_a_fns, _a_elements...);
}

template <typename T, typename... Args>
__constexpr void
process_args_internal(
	file_names_t<T>& _a_fns,
	const file_name_t<T>& _a_fn_no_type
) noexcept
{
	_a_fns.push_back(_a_fn_no_type);
}

template <typename T, typename... Args>
__constexpr void
process_args_internal(
	file_names_t<T>& _a_fns
) noexcept
{

}

template <typename T, typename... Args>
__constexpr file_names_t<T>
            process_args(
                Args... _a_elements
            ) noexcept
{
	file_names_t<T> _l_rv{};
	process_args_internal<T>(_l_rv, _a_elements...);
	return _l_rv;
}

template <typename T, typename... Args>
__constexpr_imp _ABC_NS_DG::data_generator_collection_t<T, true>
                generate_data_randomly(
                    std::shared_ptr<_ABC_NS_DG::random_generator_t<T>> _a_rnd_base,
                    Args... _a_file_reader_writers
                )
{
    return generate_data_randomly_internal(
        _a_rnd_base, process_args<T>(_a_file_reader_writers...)
    );
}

template <typename T, typename... Args>
__constexpr_imp _ABC_NS_DG::data_generator_collection_t<T, true>
                generate_data_randomly(
                    Args... _a_file_reader_writers
                )
{
    using namespace _ABC_NS_DG;
    using namespace std;
    return generate_data_randomly_internal<T>(
        (make_shared<random_generator_t<T>>()),
        process_args<T>(_a_file_reader_writers...)
    );
}

/*template <typename T>
__constexpr_imp _ABC_NS_DG::data_generator_collection_t<T, true>
                generate_data_randomly(
                    const file_names_t<T>& _a_file_reader_writers
                )
{
    using namespace _ABC_NS_DG;
    using namespace std;
    if (_a_file_reader_writers.size() > 0)
    {
        return unary_collection<T>(
            make_shared<data_generator_single_t<random_data_t<T>, true>>(
                random_data_t<T>(
                    make_shared<random_generator_t<T>>()
                ),
                _a_file_reader_writers
            )
        );
    }
    else
    {
        return unary_collection<T>(
            make_shared<data_generator_single_t<random_data_t<T>, false>>(
                random_data_t<T>(
                    make_shared<random_generator_t<T>>()
                )
            )
        );
    }
}

template <typename T>
__constexpr_imp _ABC_NS_DG::data_generator_collection_t<T, true>
                generate_data_randomly(
                    _ABC_NS_DG::random_generator_t<T>* _a_rnd_base
                )
{
    using namespace _ABC_NS_DG;
    return
unary_collection<T>(make_shared<data_generator_single_t<random_data_t<T>,
false>>( random_data_t<T>(_a_rnd_base)
    ));
}

template <typename T>
__constexpr_imp _ABC_NS_DG::data_generator_collection_t<T, true>
                generate_data_randomly()
{
    using namespace _ABC_NS_DG;
    using namespace std;
    return unary_collection<T>(
        make_shared<data_generator_single_t<random_data_t<T>, false>>(
            random_data_t<T>(make_shared<random_generator_t<T>>())
        )
    );
}*/

_END_ABC_NS

#if 0
_BEGIN_ABC_DG_NS
using rep_data_t = std::pair<std::size_t, std::size_t>;
/*!
* Object which contains randomly generated data. Also contains the mechanisms for
* writing failed cases to a file.
*/
template<
	typename T
>
struct random_data_t : public data_generator_with_file_support_and_element_storage_t<T, rep_data_t,true,true>
{
public:
	/*!
	* Constructor which takes a random generator function and a file name.
	* The file anme is where failing test cases are ran, and where they are read from.
	*/
	__constexpr
		random_data_t(
			random_generator_t<T>* _a_random_generator,
			const utility::io::opt_file_rw_info_t<T>& _a_templated_file_rw,
			const utility::io::opt_file_name_t& _a_enumerate_opt_data_file_name,
			const test_options_base_t* _a_test_options = &_ABC_NS_GLOBAL::get_global_test_options()
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
	data_generator_collection_t<T,true>
	random_data(
		random_generator_t<T>* _a_rnd_base = new random_generator_t<T>()
	);
template<
	typename T
>
__constexpr
	data_generator_collection_t<T,true>
	random_data(
		utility::io::file_name_t& _a_file_name,
		random_generator_t<T>* _a_rnd_base = new random_generator_t<T>()
	);
template<
	typename T
>
data_generator_collection_t<T,true>
	random_data(
		const utility::io::file_name_t& _a_file_name
	) noexcept
{
	return unary_collection<T>(nullptr);
}
template<
	typename T
>
data_generator_collection_t<T,true>
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
	data_generator_collection_t<T,true>
	random_data(
		random_generator_t<T>* _a_rnd_base,
		const std::optional<utility::io::file_rw_info_t<T>>& _a_opt_tfrw
	);
_END_ABC_DG_NS

_BEGIN_ABC_DG_NS
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
				this->_m_element = _m_random_generator->operator()(_m_rndgen);
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
		const utility::io::opt_file_rw_info_t<T>& _a_templated_file_rw,
		const utility::io::opt_file_name_t& _a_enumerate_opt_data_file_name,
		const test_options_base_t* _a_test_options
	) noexcept
	: data_generator_with_file_support_and_element_storage_t<T, rep_data_t,true,true>(0, _a_templated_file_rw, {},
		"clubs", {}, T{})
	//: gen_data_base_t<T>(0,
	//	_a_templated_file_rw,std::vector<T>())
	//, _m_random_generator(_a_random_generator)
	//, _m_elemnets_to_randomly_generate(
	//	this->_m_test_options->_m_default_numb_random_elements_to_generate)
	//, _m_element(T{})
	//, _m_random_gen_calls_before(0)
	//, _m_random_gen_calls_after(0)
	/*
				const size_t _a_elements_generated,
			const utility::io::opt_file_rw_info_t<T>& _a_core_data_rw_info,
			R&& _a_elements,
			const std::string_view _a_rep_data_file_extension,
			const utility::io::opt_file_rw_info_t<Rep_Data>& _a_rep_data_rw_info,
			const T& _a_element,
			const test_options_t* _a_test_options = global::get_global_test_options_ptr()
		) noexcept;
	*/
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
		this->_m_element = _m_random_generator->operator()(_m_rndgen);
		_m_random_gen_calls_after = _m_rndgen.calls();
		return true;
	}
}
template<
	typename T
>
__constexpr_imp
	data_generator_collection_t<T,true>
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
	data_generator_collection_t<T,true>
	random_data(
		random_generator_t<T>* _a_rnd_base,
		const std::optional<utility::io::file_rw_info_t<T>>& _a_tfrwi
	)
{
	return unary_collection<T>(new random_data_t<T>(_a_rnd_base, _a_tfrwi));
}
_END_ABC_DG_NS
#endif