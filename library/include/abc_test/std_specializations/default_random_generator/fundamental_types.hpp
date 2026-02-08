#pragma once

#include "abc_test/included_instances/data_generator/random_generator/random_generator_base.hpp"
#include "abc_test/utility/bounds.hpp"
#include "abc_test/utility/enum.hpp"
#include "abc_test/utility/limits/max_value_concept.hpp"
#include "abc_test/utility/limits/min_value_concept.hpp"
#include "abc_test/utility/str/unicode.hpp"
#include "concepts"

#include <memory>
#include <optional>


_BEGIN_ABC_DG_NS

/*template <>
struct default_random_generator_t<bool> : public random_generator_base_t<bool>
{
    __no_constexpr virtual bool
        operator()(
            utility::rng_t&               _a_rnd_generator,
            const utility::rng_counter_t& _a_index
        );
};*/

template <typename T>
requires std::integral<T>
struct integral_random_generator_t : public random_generator_base_t<T>
{
    __constexpr
    integral_random_generator_t(
        const T& _a_arg1,
        const T& _a_arg2
    )
        : integral_random_generator_t(utility::bounds_t<T>(_a_arg1, _a_arg2))
    {}

    __constexpr
    integral_random_generator_t()
        : integral_random_generator_t(utility::bounds_t<T>(
              std::numeric_limits<T>::min(),
              std::numeric_limits<T>::max()
          ))
    {}

    // __constexpr
    // integral_random_generator_t(
    //     const std::pair<T, T>& _a_args...
    // )
    //     : integral_random_generator_t(
    //           utility::bounds_t<T>(_a_args.first, _a_args.second)...
    //       )
    // {}
    template <typename... Ts>
    requires std::same_as<Ts..., utility::bounds_t<T>>
    __constexpr
    integral_random_generator_t(
        Ts&&... _a_args
    )

    {
        // Normalise the bounds objects
        using namespace std;
        using namespace abc::utility;
        vector<bounds_t<T>> _l_original_bounds{std::move(_a_args...)};
        if (_l_original_bounds.empty())
        {
            _m_limit = static_cast<size_t>(numeric_limits<T>::max());
        }
        else
        {
            // Ensure that the bounds do not intersect.
            // Begin by sorting. O(n log n).
            std::ranges::sort(
                _l_original_bounds,
                [](auto& _a_l, auto& _a_r)
                {
                    return _a_l.lower() < _a_r.lower();
                }
            );
            vector<bounds_t<T>> _l_new_bounds({_l_original_bounds[0]});
            // Go through each element.
            for (auto _l_itt{std::begin(_l_original_bounds) + 1};
                 _l_itt != std::end(_l_original_bounds);
                 ++_l_itt)
            {
                auto& _l_last_element{_l_new_bounds.back()};
                auto& _l_current{*_l_itt};
                // If the lower of the current element is within the bounds of
                // the previous added element.
                if (_l_current.lower() < _l_last_element.higher())
                {
                    // Set the upper of that last added element to the higher of
                    // the two values.
                    _l_last_element.set_upper(
                        std::max(_l_last_element.higher(), _l_current.higher())
                    );
                }
                else
                {
                    // Else, just add it as there are no issues.
                    _l_new_bounds.push_back(_l_current);
                }
            }
            for (auto _l_itt{std::begin(_l_new_bounds)};
                 _l_itt != std::end(_l_new_bounds);
                 ++_l_itt)
            {
                _m_limit += (*_l_itt).difference();
            }
            _m_bounds = std::move(_l_new_bounds);
        }
    }

    __constexpr_imp virtual T
        operator()(
            utility::rng_t&               _a_rnd_generator,
            const utility::rng_counter_t& _a_index
        )
    {
        using namespace std;
        if (_m_bounds.size() == 0)
        {
            return static_cast<T>(uintmax_t_rng(_a_rnd_generator));
        }
        else
        {
            // 0 and 10. 40 and 50.
            // 0 and 20. hit 15.
            uintmax_t _l_rnd_val{
                _m_limit == 0 ? 0 : uintmax_t_rng(_a_rnd_generator)
            };
            for (auto&& _l_bound : _m_bounds)
            {
                if (_l_bound.difference() >= _l_rnd_val)
                {
                    return _l_bound.lower() + static_cast<T>(_l_rnd_val);
                }
                else
                {
                    _l_rnd_val -= _l_bound.difference();
                }
            }
            return _l_rnd_val;
        }
    }
private:
    std::vector<utility::bounds_t<T>> _m_bounds;
    std::uintmax_t                    _m_limit{0};

    __constexpr                       std::size_t
                                      uintmax_t_rng(
                                          utility::rng_t& _a_rnd_generator
                                      )
    {
        return _a_rnd_generator() % _m_limit;
    }
};

template <typename T>
requires std::integral<T> && (not abc::utility::str::is_char_type_c<T>)
struct default_random_generator_t<T> : public integral_random_generator_t<T>
{
public:
    using integral_random_generator_t<T>::integral_random_generator_t;
};

template <typename T>
requires abc::utility::str::is_char_type_c<T>
struct default_random_generator_t<T> : public integral_random_generator_t<T>
{
public:
    using integral_random_generator_t<T>::integral_random_generator_t;

    __constexpr
    default_random_generator_t(
        const bool _a_only_valid_characters
    ) noexcept

    {}
};

_END_ABC_DG_NS

_BEGIN_ABC_DG_NS
/*__no_constexpr_imp bool
    default_random_generator_t<bool>::operator()(
        utility::rng_t&               _a_rnd_generator,
        const utility::rng_counter_t& _a_index
    )
{
    using namespace std;
    size_t _l_val{_a_rnd_generator() % 2};
    return _l_val == 0 ? false : true;
}
*/
_END_ABC_DG_NS