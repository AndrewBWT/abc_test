#pragma once

#include "abc_test/core/data_generator/data_generator.hpp"
#include "abc_test/core/test_runner.hpp"
#include "abc_test/included_instances/data_generator/random_generator/default_random_generator.hpp"
#include "abc_test/utility/io/file/file_line_reader.hpp"
#include "abc_test/utility/io/file/file_line_writer.hpp"
#include "abc_test/utility/io/file/file_name.hpp"
#include "abc_test/utility/str/rw_info.hpp"

#include <concepts>
#include <random>

_BEGIN_ABC_DG_NS

template <typename T>
class random_data_generator_t
{
public:
    using generator_type = T;
    using tertiary_type
        = std::tuple<std::size_t, std::size_t, utility::rng_counter_t>;
    __constexpr const tertiary_type&
        tertiary_data() const;
    __constexpr void
        set_generator_using_tertiary_data(const tertiary_type& _a_tertiary_data
        );
    __constexpr void
        reset();
    __constexpr
    random_data_generator_t(const random_generator_t<T>& _a_rnd);
    __constexpr bool
        has_current_element() const;
    __constexpr bool
        generate_next();
    __constexpr const T&
        current_element() const;
private:
    tertiary_type          _m_random_calls_before_after;
    random_generator_t<T>  _m_random_generator;
    utility::rng_counter_t _m_elemnets_to_randomly_generate{
        utility::rng_counter_t(10)
    };
    utility::rng_counter_t _m_elements_generated{utility::rng_counter_t(0)};
    T                      _m_element;
};

_END_ABC_DG_NS

_BEGIN_ABC_NS

template <typename T, typename... Args>
__constexpr_imp _ABC_NS_DG::data_generator_collection_t<T>
                generate_data_randomly(
                    const _ABC_NS_DG::random_generator_t<T>& _a_rnd,
                    Args... _a_file_reader_writers
                );
template <typename T, typename... Args>
__constexpr_imp _ABC_NS_DG::data_generator_collection_t<T>
                generate_data_randomly(Args... _a_file_reader_writers);

template <typename T>
__constexpr_imp _ABC_NS_DG::data_generator_collection_t<T>
    generate_data_randomly(const _ABC_NS_DG::random_generator_t<T>& _a_rnd);
template <typename T>
__constexpr_imp _ABC_NS_DG::data_generator_collection_t<T>
                generate_data_randomly();

_END_ABC_NS

_BEGIN_ABC_DG_NS
template <typename T>
__constexpr const random_data_generator_t<T>::tertiary_type&
                  random_data_generator_t<T>::tertiary_data() const
{
    return _m_random_calls_before_after;
}

template <typename T>
__constexpr void
    random_data_generator_t<T>::set_generator_using_tertiary_data(
        const tertiary_type& _a_tertiary_data
    )
{
    _m_random_calls_before_after = _a_tertiary_data;
}

template <typename T>
__constexpr void
    random_data_generator_t<T>::reset()
{
    _m_elements_generated = 0;
}

template <typename T>
__constexpr
random_data_generator_t<T>::random_data_generator_t(
    const random_generator_t<T>& _a_rnd
)
    : _m_random_generator(_a_rnd)

{
    get<0>(_m_random_calls_before_after
    ) = global::get_this_threads_current_test().get_random_generator().calls();
    get<2>(_m_random_calls_before_after) = _m_elements_generated;
    _m_element                           = (*_m_random_generator)(
        global::get_this_threads_current_test().get_random_generator(),
        _m_elements_generated
    );
    get<1>(_m_random_calls_before_after
    ) = global::get_this_threads_current_test().get_random_generator().calls();
    _m_elements_generated++;
}

template <typename T>
__constexpr bool
    random_data_generator_t<T>::has_current_element() const
{
    return _m_elements_generated < _m_elemnets_to_randomly_generate;
}

template <typename T>
__constexpr bool
    random_data_generator_t<T>::generate_next()
{
    if (_m_elements_generated + 1 < _m_elemnets_to_randomly_generate)
    {
        get<0>(_m_random_calls_before_after)
            = global::get_this_threads_current_test()
                  .get_random_generator()
                  .calls();
        get<2>(_m_random_calls_before_after) = _m_elements_generated;
        _m_element                           = (*_m_random_generator)(
            global::get_this_threads_current_test().get_random_generator(),
            _m_elements_generated
        );
        get<1>(_m_random_calls_before_after)
            = global::get_this_threads_current_test()
                  .get_random_generator()
                  .calls();
        _m_elements_generated++;
        return true;
    }
    else
    {
        return false;
    }
}

template <typename T>
__constexpr const T&
    random_data_generator_t<T>::current_element() const
{
    return _m_element;
}

_END_ABC_DG_NS

_BEGIN_ABC_NS
template <typename T, typename... Args>
__constexpr_imp _ABC_NS_DG::data_generator_collection_t<T>
                generate_data_randomly(
                    const _ABC_NS_DG::random_generator_t<T>& _a_rnd,
                    Args... _a_file_reader_writers
                )
{
    using namespace _ABC_NS_DG;
    return make_data_generator_with_file_support<random_data_generator_t<T>>(
        random_data_generator_t<T>(_a_rnd), _a_file_reader_writers...
    );
}

template <typename T, typename... Args>
__constexpr_imp _ABC_NS_DG::data_generator_collection_t<T>
                generate_data_randomly(
                    Args... _a_file_reader_writers
                )
{
    using namespace _ABC_NS_DG;
    using namespace std;
    return generate_data_randomly<T>(
        default_random_generator<T>(), _a_file_reader_writers...
    );
}

template <typename T>
__constexpr_imp _ABC_NS_DG::data_generator_collection_t<T>
                generate_data_randomly(
                    const _ABC_NS_DG::random_generator_t<T>& _a_rnd
                )
{
    using namespace _ABC_NS_DG;
    return make_data_generator_with_file_support<random_data_generator_t<T>>(
        random_data_generator_t<T>(_a_rnd)
    );
}

template <typename T>
__constexpr_imp _ABC_NS_DG::data_generator_collection_t<T>
                generate_data_randomly()
{
    using namespace _ABC_NS_DG;
    using namespace std;
    return generate_data_randomly<T>(default_random_generator<T>());
}

_END_ABC_NS