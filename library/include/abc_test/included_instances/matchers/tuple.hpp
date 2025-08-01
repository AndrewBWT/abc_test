#pragma once


#include "abc_test/core/matchers/comparison/comparison_enum.hpp"
#include "abc_test/core/matchers/comparison/comparison_functions.hpp"
#include "abc_test/core/matchers/matcher_wrapper.hpp"

_BEGIN_ABC_NS
_END_ABC_NS
_BEGIN_ABC_MATCHER_NS

template <comparison_enum_t Cmp, typename... Ts>
struct matcher_default_comparable_t<Cmp, std::tuple<Ts...>, std::tuple<Ts...>>
{
public:
    static constexpr bool is_specialized{true};

    template <std::size_t I>
    __constexpr void
        run_internal(
            std::vector<matcher_result_t>& _a_rv,
            const std::tuple<Ts...>&       _a_arg1,
            const std::tuple<Ts...>&       _a_arg2
        ) const
    {
        using namespace std;
        const auto& _l_arg1{get<I>(_a_arg1)};
        const auto& _l_arg2{get<I>(_a_arg2)};
        _a_rv.push_back(
            matcher_default_comparable_t<
                Cmp,
                std::remove_cvref_t<decltype(_l_arg1)>,
            std::remove_cvref_t<decltype(_l_arg2)>>{}.run(_l_arg1, _l_arg2)
        );
        if constexpr (I + 1 < tuple_size<std::tuple<Ts...>>{})
        {
            return run_internal<I + 1>(_a_rv, _a_arg1, _a_arg2);
        }
    }

    __constexpr virtual matcher_result_t
        run(
            const std::tuple<Ts...>& _a_arg1,
            const std::tuple<Ts...>& _a_arg2
        ) const

    {
        using namespace std;
        vector<matcher_result_t> _l_results;
        run_internal<0>(_l_results, _a_arg1, _a_arg2);
        bool _l_passed{true};
        for (const auto& _l_result : _l_results)
        {
            if (not _l_result.passed())
            {
                _l_passed = false;
                break;
            }
        }
        auto _l_comparison_expr_to_string = [&]()
        {
            using namespace abc::utility::printer;
            const u8string _l_cmp_str{
                (_l_passed) ? matcher::cmp_str<Cmp>()
                            : matcher::not_cmp_str<Cmp>()
            };
            return fmt::format(
                u8"{0} {1} {2}",
                default_printer_t<tuple<Ts...>>{}.run_printer(_a_arg1),
                _l_cmp_str,
                default_printer_t<tuple<Ts...>>{}.run_printer(_a_arg2)
            );
        };
        u8string _l_primary_str;
        if (not _l_passed)
        {
            vector<size_t> _l_failed;
            for (size_t _l_idx{0}; auto& _l_result : _l_results)
            {
                if (not _l_result.passed())
                {
                    _l_failed.push_back(_l_idx);
                }
                _l_idx++;
            }
            u8string _l_elements_suffix{_l_failed.size() == 1 ? u8"s" : u8""};
            u8string _l_str_list;
            const size_t _l_failed_size{_l_failed.size()};
            for (size_t _l_idx{0}; auto& _l_failed_n : _l_failed)
            {
                _l_str_list.append(
                    abc::utility::str::positive_integer_to_placement(_l_failed_n
                    )
                );
                if (_l_idx + 2 < _l_failed_size)
                {
                    _l_str_list.append(u8", ");
                }
                else if (_l_idx + 1 < _l_failed_size)
                {
                    _l_str_list.append(u8" and ");
                }
                ++_l_idx;
            }
            _l_primary_str = fmt::format(
                u8"Tuple element{0}{1} failed. Matcher information shown below",
                _l_elements_suffix,
                _l_str_list
            );
        }
        return matcher_result_t(
            _l_passed,
            matcher_result_infos_t(
                _l_comparison_expr_to_string(), {_l_primary_str}
            )
        );
    }
};

_END_ABC_MATCHER_NS