#pragma once

#include "abc_test/included_instances/data_generator/random_generator_object.h"
#include "abc_test/internal/data_generator/data_generator.h"
#include "abc_test/internal/test_runner.h"
#include "abc_test/internal/utility/io/file/file_line_reader.h"
#include "abc_test/internal/utility/io/file/file_line_writer.h"
#include "abc_test/internal/utility/io/file/file_name.h"
#include "abc_test/internal/utility/str/rw_info.h"

#include <concepts>
#include <random>

_BEGIN_ABC_DG_NS

template <typename T>
class random_data_generator_t
{
public:
    using generator_type = T;
    using tertiary_type  = std::pair<std::size_t, std::size_t>;
    __constexpr const tertiary_type&
        tertiary_data() const;
    __constexpr void
        set_generator_using_tertiary_data(const tertiary_type& _a_tertiary_data
        );
    __constexpr void
        reset();
    __constexpr
    random_data_generator_t(
        const std::shared_ptr<random_generator_object_t<T>>& _a_rnd
    );
    __constexpr bool
        has_current_element() const;
    __constexpr bool
        generate_next();
    __constexpr const T&
                      current_element() const;
    __constexpr const std::string
                      get_additional_data() const;
private:
    using random_rep_data_t = std::pair<std::size_t, std::size_t>;
    tertiary_type _m_random_calls_before_after;
    abc::utility::str::rw_info_t<random_rep_data_t> _m_rep_data;
    std::shared_ptr<random_generator_object_t<T>>   _m_random_generator;
    size_t      _m_elemnets_to_randomly_generate{10};
    std::size_t _l_elements_generated{0};
    T           _m_element;
};

_END_ABC_DG_NS

_BEGIN_ABC_NS

template <typename T, typename... Args>
__constexpr_imp _ABC_NS_DG::data_generator_collection_t<T, true>
                generate_data_randomly(
                    std::shared_ptr<_ABC_NS_DG::random_generator_object_t<T>> _a_rnd_base,
                    Args... _a_file_reader_writers
                );
template <typename T, typename... Args>
__constexpr_imp _ABC_NS_DG::data_generator_collection_t<T, true>
                generate_data_randomly(Args... _a_file_reader_writers);

namespace
{
template <typename T>
__constexpr_imp _ABC_NS_DG::data_generator_collection_t<T, true>
                generate_data_randomly_internal(
                    std::shared_ptr<_ABC_NS_DG::random_generator_object_t<T>> _a_rnd_base,
                    const utility::io::file_names_t<T>& _a_file_reader_writers
                );
template <typename T>
__constexpr_imp void
    process_args_internal(utility::io::file_names_t<T>& _a_fns) noexcept;
template <typename T, typename... Args>
__constexpr_imp void
    process_args_internal(
        utility::io::file_names_t<T>&      _a_fns,
        const utility::io::file_name_t<T>& _a_fn_no_type,
        Args... _a_elements
    ) noexcept;
template <typename T>
__constexpr_imp void
    process_args_internal(
        utility::io::file_names_t<T>&      _a_fns,
        const utility::io::file_name_t<T>& _a_fn_no_type
    ) noexcept;

template <typename T, typename... Args>
__constexpr utility::io::file_names_t<T>
            process_args(Args... _a_elements) noexcept;
} // namespace

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
    _l_elements_generated = 0;
}

template <typename T>
__constexpr
random_data_generator_t<T>::random_data_generator_t(
    const std::shared_ptr<random_generator_object_t<T>>& _a_rnd
)
    : _m_random_generator(_a_rnd)

{}

template <typename T>
__constexpr bool
    random_data_generator_t<T>::has_current_element() const
{
    return _l_elements_generated < _m_elemnets_to_randomly_generate;
}

template <typename T>
__constexpr bool
    random_data_generator_t<T>::generate_next()
{
    if (_l_elements_generated + 1 < _m_elemnets_to_randomly_generate)
    {
        _m_random_calls_before_after.first
            = global::get_this_threads_current_test()
                  .get_random_generator()
                  .calls();
        _m_element = (*_m_random_generator)(
            global::get_this_threads_current_test().get_random_generator()
        );
        _m_random_calls_before_after.second
            = global::get_this_threads_current_test()
                  .get_random_generator()
                  .calls();
        _l_elements_generated++;
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

template <typename T>
__constexpr const std::string
                  random_data_generator_t<T>::get_additional_data() const
{
    return _m_rep_data.printer().run_printer(_m_random_calls_before_after);
}

_END_ABC_DG_NS

_BEGIN_ABC_NS
template <typename T, typename... Args>
__constexpr_imp _ABC_NS_DG::data_generator_collection_t<T, true>
                generate_data_randomly(
                    std::shared_ptr<_ABC_NS_DG::random_generator_object_t<T>> _a_rnd_base,
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
        (make_shared<random_generator_object_t<T>>()),
        process_args<T>(_a_file_reader_writers...)
    );
}

namespace
{
template <typename T>
__constexpr_imp _ABC_NS_DG::data_generator_collection_t<T, true>
                generate_data_randomly_internal(
                    std::shared_ptr<_ABC_NS_DG::random_generator_object_t<T>> _a_rnd_base,
                    const utility::io::file_names_t<T>& _a_file_reader_writers
                    // const file_names_t<T>&             _a_file_reader_writers
                )
{
    using namespace _ABC_NS_DG;
    using namespace std;
    if (_a_file_reader_writers.size() == 0)
    {
        return unary_collection<T>(
            make_shared<data_generator_with_file_support_t<
                random_data_generator_t<T>,
                false>>(random_data_generator_t<T>(_a_rnd_base))
        );
    }
    else if (_a_file_reader_writers.size() == 1)
    {
        return unary_collection<T>(
            make_shared<data_generator_with_file_support_t<
                random_data_generator_t<T>,
                true>>(
                random_data_generator_t<T>(_a_rnd_base),
                _a_file_reader_writers[0]
            )
        );
    }
    else
    {
        return unary_collection<T>(
            make_shared<data_generator_with_file_support_t<
                random_data_generator_t<T>,
                true>>(
                random_data_generator_t<T>(_a_rnd_base),
                _a_file_reader_writers[0],
                std::pair<
                    typename utility::io::file_names_t<T>::const_iterator,
                    typename utility::io::file_names_t<T>::const_iterator>(
                    _a_file_reader_writers.begin() + 1,
                    _a_file_reader_writers.end()
                )
            )
        );
    }
}

template <typename T>
__constexpr_imp void
    process_args_internal(
        utility::io::file_names_t<T>& _a_fns
    ) noexcept
{}

template <typename T, typename... Args>
__constexpr_imp void
    process_args_internal(
        utility::io::file_names_t<T>&      _a_fns,
        const utility::io::file_name_t<T>& _a_fn_no_type,
        Args... _a_elements
    ) noexcept
{
    _a_fns.push_back(_a_fn_no_type);
    process_args_internal<T>(_a_fns, _a_elements...);
}

template <typename T>
__constexpr_imp void
    process_args_internal(
        utility::io::file_names_t<T>&      _a_fns,
        const utility::io::file_name_t<T>& _a_fn_no_type
    ) noexcept
{
    _a_fns.push_back(_a_fn_no_type);
}

template <typename T, typename... Args>
__constexpr_imp utility::io::file_names_t<T>
                process_args(
                    Args... _a_elements
                ) noexcept
{
    utility::io::file_names_t<T> _l_rv{};
    process_args_internal<T>(_l_rv, _a_elements...);
    return _l_rv;
}
} // namespace

_END_ABC_NS