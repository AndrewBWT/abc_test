#pragma once

#include "abc_test/utility/rng/complete_global_seed.hpp"
#include "abc_test/utility/rng/inner_rng.hpp"

#include <memory>
#include <random>
#include <scn/scan.h>


_BEGIN_ABC_UTILITY_NS

/*!
 * @brief The type which represents a random number generator in abc_test.
 *
 * Internally, it is designed so that the use is able to define their own rng
 * types, which extend the inner_rng_t type. The rng_t class can only return one
 * type of random number, that of the std::mt19937_64::result_type type.
 */
struct rng_t
{
public:
    /*!
     * @brief The internal result_t type, which is returned frmo the () operator
     * from rng_t.
     */
    using result_type_t = std::mt19937_64::result_type;
    __constexpr
    rng_t() noexcept
        = delete;
    /*!
     * @brief Creates an rng_t object, using this rng_t object to seed the new
     * object.
     * @param _a_n_elements_to_take The number of elements the seed_t value will
     * have.
     * @return rng_t The created rng_t object.
     *
     * The reader should note that the inner_rng_t object in the returned object
     * is created using the virtual function deep_copy.
     */
    /*
     * Cannot be constexpr due to calling private rng_t constructor, which is
     * not constexpr.
     */
    __no_constexpr rng_t
        make_rng(const std::size_t _a_n_elements_to_take) noexcept;
    /*!
     * @brief Factory function for creating rng_t objects. One of only two ways
     * of creating an rng_t value without any previous rng_t value.
     * @tparam Rng The type of the internal inner_rng_t object.
     * @param _a_global_seed complete_global_seed_t object used to create the
     * seed_t object, used to create the returned object.
     * @param _a_seed_size The number of values to take from _a_global_seed to
     * seed the created rng_t object. See complete_global_seed_t for more
     * information regarding this argument.
     * @return The rng_t object created from the parameters.
     */
    template <typename Rng>
    requires std::derived_from<Rng, inner_rng_t>
    __constexpr static rng_t
        make_rng(
            const complete_global_seed_t& _a_global_seed,
            const std::size_t             _a_seed_size
        ) noexcept;
    /*!
     * @brief Factory function for creating rng_t objects. One of only two ways
     * of creating an rng_t value without any previous rng_t value.
     * @tparam Rng The type of the internal inner_rng_t object.
     * @param _a_seed seed_t object representing the seed of the created object.
     * @return The rng_t object created from the parameters.
     */
    template <typename Rng>
    requires std::derived_from<Rng, inner_rng_t>
    __constexpr static rng_t
        make_rng(const seed_t& _a_seed) noexcept;
    /*!
     * @brief This function progresses the object's internal RNG. It can also
     * turn it back, though this can be computationally expensive.
     * @param _a_expected_calls The set number of calls to place the internal
     * RNG at.
     *
     * Note that, if _a_expected_calls is less than the current number of calls,
     * then the entire object is reset and the internal RNG progressed to the
     * correct value. For high _a_expected_calls values, this can be
     * computationally expensive.
     */
    __constexpr void
        progress(const std::size_t _a_expected_calls) noexcept;
    /*!
     * @brief Returns a randomly generated result_type_t type.
     * @return The randomly generated result_type_t type.
     */
    __constexpr result_type_t
        operator()();
    /*!
     * @brief Tells the caller the number of times the internal RNG has been
     * called.
     * @return The number of times the internal RNG has been called.
     */
    __constexpr size_t
        calls() const noexcept;
    __constexpr rng_t(const rng_t& _a_rng)
        : _m_rng(_a_rng._m_rng->deep_copy())
        , _m_calls(_a_rng._m_calls)
    {

    }
private:
    using inner_rng_ptr_t = std::unique_ptr<inner_rng_t>;
    inner_rng_ptr_t _m_rng;
    seed_t                       _m_seed;
    size_t                       _m_calls{0};
    /*
     * Cannot be constexpr due to use of shared_ptr copy constructor.
     */
    __no_constexpr
        rng_t(
            inner_rng_ptr_t& _a_rng,
            const utility::seed_t&              _a_seed
        );
    __constexpr
        rng_t(
            inner_rng_ptr_t&& _a_rng,
            const utility::seed_t& _a_seed
        );
};

_END_ABC_UTILITY_NS

_BEGIN_ABC_UTILITY_NS

__no_constexpr_imp rng_t
    rng_t::make_rng(
        const std::size_t _a_n_elements_to_take
    ) noexcept
{
    using namespace std;
    if (_m_rng == nullptr)
    {
        std::unreachable();
    }
    else
    {
        inner_rng_t&            _l_rng{*_m_rng};
        inner_rng_ptr_t _l_rng_cpy{_l_rng.deep_copy()};
        if (_l_rng_cpy == nullptr)
        {
            std::unreachable();
        }
        else
        {
            inner_rng_t&     _l_rng_copy_ref{*_l_rng_cpy};
            seed_t           _l_seed(_a_n_elements_to_take);
            seed_t::iterator _l_end{end(_l_seed)};
            for (seed_t::iterator _l_itt{begin(_l_seed)}; _l_itt != _l_end;
                 ++_l_itt)
            {
                *_l_itt = _l_rng();
            }
            _l_rng_copy_ref.set_seed(_l_seed);
            return rng_t(_l_rng_cpy, _l_seed);
        }
    }
}

template <typename Rng>
requires std::derived_from<Rng, inner_rng_t>
__constexpr_imp rng_t
    rng_t::make_rng(
        const complete_global_seed_t& _a_global_seed,
        const std::size_t             _a_seed_size
    ) noexcept
{
    return make_rng<Rng>(
        seed_t(_a_global_seed.create_seed_from_global_seed(_a_seed_size))
    );
}

template <typename Rng>
requires std::derived_from<Rng, inner_rng_t>
__constexpr_imp rng_t
    rng_t::make_rng(
        const seed_t& _a_seed
    ) noexcept
{
    using namespace std;
    return rng_t(std::make_unique<Rng>(_a_seed), _a_seed);
}

__constexpr_imp void
    rng_t::progress(
        const size_t _a_expected_calls
    ) noexcept
{
    if (_m_calls != _a_expected_calls)
    {
        if (_m_rng == nullptr)
        {
            std::unreachable();
        }
        else
        {
            inner_rng_t& _l_rng{*_m_rng};
            if (_m_calls > _a_expected_calls)
            {
                // Basically been reset to zero.
                _l_rng.set_seed(_m_seed);
                _l_rng.progress(_m_calls - _a_expected_calls);
            }
            else
            {
                // _a_expected_calls <= _m_calls. _m_alls - _a_expected_calls >
                // 0.
                _l_rng.progress(_a_expected_calls);
            }
            _m_calls = _a_expected_calls;
        }
    }
}

__constexpr_imp rng_t::result_type_t
                rng_t::operator()()
{
    ++_m_calls;
    if (_m_rng == nullptr)
    {
        std::unreachable();
    }
    else
    {
        inner_rng_t& _l_rng{*_m_rng};
        return _l_rng();
    }
}

__constexpr_imp size_t
    rng_t::calls() const noexcept
{
    return _m_calls;
}

__no_constexpr_imp
    rng_t::rng_t(
        inner_rng_ptr_t& _a_rng,
        const seed_t&                       _a_seed
    )
    : _m_rng(std::move(_a_rng)), _m_seed(_a_seed)
{}
__constexpr
rng_t::rng_t(
    inner_rng_ptr_t&& _a_rng,
    const utility::seed_t& _a_seed
)
    : _m_rng(std::move(_a_rng)), _m_seed(_a_seed)
{

}

_END_ABC_UTILITY_NS