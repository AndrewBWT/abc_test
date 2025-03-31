#pragma once
#include "abc_test/core/ds/source/single_source.hpp"
#include "abc_test/core/errors/test_library_exception.hpp"
#include "abc_test/included_instances/reporters/text_test_reporter/enum_print_data.hpp"
#include "abc_test/utility/internal/macros.hpp"

#include <string>
#include <variant>
#include <vector>
#include "abc_test/utility/io/threaded_text_output_reporter.hpp"

_BEGIN_ABC_REPORTERS_NS

template <typename T, typename Formattable_Identifier, typename Print_Config>
struct list_formattable_t
{
public:
    __constexpr virtual void
        get_data(
            const Formattable_Identifier&                       _a_collection,
            const T&                                            _a_element,
            const Print_Config&                                 _a_pc,
            const utility::io::threated_text_output_reporter_t& _a_ttor,
            const std::size_t _a_idx
        ) const
        = 0;
    __constexpr void
        process_all_data(
            const enum_print_pair_collection_t<Formattable_Identifier>&
                                _a_collection,
            const T&            _a_element,
            const Print_Config& _a_pc,
            const abc::utility::io::threated_text_output_reporter_t& _a_ttor
        ) const noexcept;
protected:
    __constexpr virtual bool
        check_data(const Formattable_Identifier& _a_fid, const T& _a_element)
            const
        = 0;
};

template <typename T, typename Formattable_Identifier, typename Print_Config>
struct list_vertical_formattable_t
{
public:
    __constexpr virtual std::pair<std::u8string, std::vector<std::u8string>>
        get_data(
            const Formattable_Identifier& _a_collection,
            const T&                      _a_element,
            const Print_Config&           _a_pc
        ) const
        = 0;

    __constexpr_imp void
        process_all_data(
            const enum_print_pair_collection_t<Formattable_Identifier>&
                                _a_collection,
            const T&            _a_element,
            const Print_Config& _a_pc,
            const abc::utility::io::threated_text_output_reporter_t& _a_ttor
        ) const noexcept
    {
        using namespace std;
        vector<pair<u8string, vector<u8string>>> _l_strs;
        size_t                       _l_biggest_first_element{0};
        for (const enum_print_pair_t<Formattable_Identifier>& _l_pair :
             _a_collection)
        {
            if (always_print(_l_pair.second)
                || (constexual_print(_l_pair.second)
                    && check_data(_l_pair.first, _a_element)))
            {
                const pair<u8string, vector<u8string>> _l_str_pair{ get_data(_l_pair.first, _a_element, _a_pc) };
                _l_biggest_first_element
                    = std::max(_l_str_pair.first.size(), _l_biggest_first_element);
                _l_strs.push_back(_l_str_pair);
            }
        }
        for (const auto& [_l_field, _l_vect_data] : _l_strs)
        {
            for (size_t _l_idx{ 0 }; const auto& _l_data: _l_vect_data)
            {
                if (_l_idx++ == 0)
                {
                    _a_ttor.write(fmt::format(
                        u8"{0}{1}{2}",
                        _l_field,
                        u8string(_l_biggest_first_element - _l_field.size(), char8_t(' ')),
                        _l_data
                    ));
                }
                else
                {
                    _a_ttor.write(fmt::format(
                        u8"{0}{1}",
                        u8string(_l_biggest_first_element, char8_t(' ')),
                        _l_data
                    ));
                }
            }
        }
    }
protected:
    __constexpr virtual bool
        check_data(const Formattable_Identifier& _a_fid, const T& _a_element)
            const
        = 0;
};

/*template <
    typename Formattable_Identifier,
    typename T,
    typename Print_Config,
    typename Class>
__constexpr_imp std::vector<std::string>
                get_all_data_as_single_vector(
                    const enum_print_pair_collection_t<Formattable_Identifier>&
                                                                             _a_collection,
                    const T& _a_element, const Print_Config& _a_pc, const Class&
_a_class, const abc::utility::io::threated_text_output_reporter_t& _a_ttor
                );
template <
    typename Formattable_Identifier,
    typename T,
    typename Print_Config,
    typename Class>
__constexpr_imp

    std::vector<std::vector<std::string>>
    get_all_data_as_2d_vector(
        const enum_print_pair_collection_t<Formattable_Identifier>&
                                                                 _a_collection,
        const T&                                                 _a_element,
        const Print_Config&                                      _a_pc,
        const Class&                                             _a_class,
        const abc::utility::io::threated_text_output_reporter_t& _a_ttor
    );*/
_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS
template <typename T, typename Formattable_Identifier, typename Print_Config>
__constexpr_imp void
    list_formattable_t<T, Formattable_Identifier, Print_Config>::
        process_all_data(
            const enum_print_pair_collection_t<Formattable_Identifier>&
                                _a_collection,
            const T&            _a_element,
            const Print_Config& _a_pc,
            const abc::utility::io::threated_text_output_reporter_t& _a_ttor
        ) const noexcept
{
    for (size_t _l_idx{ 0 }; const enum_print_pair_t<Formattable_Identifier>& _l_pair :
         _a_collection)
    {
        if (always_print(_l_pair.second)
            || (constexual_print(_l_pair.second)
                && check_data(_l_pair.first, _a_element)))
        {
            get_data(_l_pair.first, _a_element, _a_pc, _a_ttor, _l_idx);
        }
        ++_l_idx;
    }
}

/**template <
    typename Formattable_Identifier,
    typename T,
    typename Print_Config,
    typename Class>
__constexpr_imp std::vector<std::string>
                get_all_data_as_single_vector(
                    const enum_print_pair_collection_t<Formattable_Identifier>&
                                                                             _a_collection,
                    const T& _a_element, const Print_Config& _a_pc, const Class&
_a_class, const abc::utility::io::threated_text_output_reporter_t& _a_ttor
                )
{
    using RV = std::vector<std::string>;
    using namespace std;
    RV _l_rv{};

    for (const enum_print_pair_t<Formattable_Identifier>& _l_pair :
         _a_collection)
    {
        if (always_print(_l_pair.second)
            || (constexual_print(_l_pair.second)
                && _a_class.check_data(_l_pair.first, _a_element)))
        {
            _l_rv.append_range(
                _a_class.get_data(_l_pair.first, _a_element, _a_pc, _a_ttor)
            );
        }
    }
    return _l_rv;
}

template <
    typename Formattable_Identifier,
    typename T,
    typename Print_Config,
    typename Class>
__constexpr_imp std::vector<std::vector<std::string>>
                get_all_data_as_2d_vector(
                    const enum_print_pair_collection_t<Formattable_Identifier>&
                                                                             _a_collection,
                    const T& _a_element, const Print_Config& _a_pc, const Class&
_a_class, const abc::utility::io::threated_text_output_reporter_t& _a_ttor
                )
{
    using RV = std::vector<std::vector<std::string>>;
    using namespace std;
    RV _l_rv{};

    for (const enum_print_pair_t<Formattable_Identifier>& _l_pair :
         _a_collection)
    {
        if (always_print(_l_pair.second)
            || (constexual_print(_l_pair.second)
                && _a_class.check_data(_l_pair.first, _a_element)))
        {
            _l_rv.push_back(
                _a_class.get_data(_l_pair.first, _a_element, _a_pc, _a_ttor)
            );
        }
    }
    return _l_rv;
}*/

_END_ABC_REPORTERS_NS