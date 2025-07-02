#pragma once

#include "abc_test/utility/internal/unreachable.hpp"
#include "abc_test/utility/rng/inner_rng.hpp"

#include <memory>
#include <random>


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
     * The inner_rng_t object in the returned object
     * is created using the virtual function deep_copy.
     */
    __constexpr rng_t
        make_rng(const std::size_t _a_n_elements_to_take)
            noexcept(unreachable_does_not_throw_exception);
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
        ) noexcept(unreachable_does_not_throw_exception);
    /*!
     * @brief Factory function for creating rng_t objects. One of only two ways
     * of creating an rng_t value without any previous rng_t value.
     * @tparam Rng The type of the internal inner_rng_t object.
     * @param _a_seed seed_t An rvalue object representing the seed of the
     * created object.
     * @return The rng_t object created from the parameters.
     */
    template <typename Rng>
    requires std::derived_from<Rng, inner_rng_t>
    __constexpr static rng_t
        make_rng(seed_t&& _a_seed) noexcept;
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
        progress(const std::size_t _a_expected_calls)
            noexcept(unreachable_does_not_throw_exception);
    /*!
     * @brief Returns a randomly generated result_type_t type.
     * @return The randomly generated result_type_t type.
     */
    __constexpr result_type_t
        operator()() noexcept(unreachable_does_not_throw_exception);
    /*!
     * @brief Tells the caller the number of times the internal RNG has been
     * called.
     * @return The number of times the internal RNG has been called.
     */
    __constexpr size_t
        calls() const noexcept;
    /*!
     * @brief Constructor which takes an rvalue argument.
     * @param _a_rng The rvalue _a_rng argument.
     */
    __constexpr
    rng_t(rng_t&& _a_rng) noexcept;
    /*!
     * @brief A constructor which takes a lvalue argument.
     * @param _a_rng The lvalue in question.
     * @return
     */
    __constexpr
    rng_t(const rng_t& _a_rng) noexcept(unreachable_does_not_throw_exception);

    __constexpr rng_t&
        operator=(const rng_t& _a_arg);
    __constexpr rng_t&
        operator=(rng_t&& _a_arg) noexcept
        = default;
#if _TESTING_BUILD
    /*! Constructor for testing when inner_rng_t is a nullptr. It shouldn't be
     * able to happen, but we want to check that if it does, then the code will
     * throw a breakpoint.
     */
    template <typename Rng>
    requires std::derived_from<Rng, inner_rng_t>
    __constexpr static rng_t
        make_default_rng() noexcept;
#endif
private:
    using inner_rng_ptr_t = std::unique_ptr<inner_rng_t>;
    inner_rng_ptr_t _m_rng;
    seed_t          _m_seed;
    size_t          _m_calls{0};
    __constexpr
    rng_t(inner_rng_ptr_t&& _a_rng, seed_t&& _a_seed);
    __constexpr
    rng_t(inner_rng_ptr_t&& _a_rng, const seed_t& _a_seed);
    __constexpr inner_rng_t&
        get_inner_rng() const noexcept(unreachable_does_not_throw_exception);
    static constexpr std::u8string_view _s_error_msg
        = u8"rng_t's inner random generator contains a nullptr. This is an "
          u8"invalid state for the rng_t object, and is undefined behaviour.";
};

_END_ABC_UTILITY_NS

_BEGIN_ABC_UTILITY_NS

__constexpr_imp rng_t
    rng_t::make_rng(
        const std::size_t _a_n_elements_to_take
    )
        noexcept(
            unreachable_does_not_throw_exception
        )
{
    using namespace std;
    inner_rng_t&     _l_rng{get_inner_rng()};
    seed_t           _l_seed(_a_n_elements_to_take);
    seed_t::iterator _l_end{end(_l_seed)};
    for (seed_t::iterator _l_itt{begin(_l_seed)}; _l_itt != _l_end; ++_l_itt)
    {
        *_l_itt = (*this)();
    }
    return rng_t(_l_rng.make_rng(_l_seed), _l_seed);
}

template <typename Rng>
requires std::derived_from<Rng, inner_rng_t>
__constexpr_imp rng_t
    rng_t::make_rng(
        const complete_global_seed_t& _a_global_seed,
        const std::size_t             _a_seed_size
    )
        noexcept(
            unreachable_does_not_throw_exception
        )
{
    using namespace std;
    const auto _l_index{_a_global_seed.index()};
    switch (_l_index)
    {
    case 0:
    {
        seed_t           _l_seed(_a_seed_size);
        seed_t::iterator _l_end{end(_l_seed)};
        srand(get<0>(_a_global_seed));
        for (seed_t::iterator _l_itt{begin(_l_seed)}; _l_itt != _l_end;
             ++_l_itt)
        {
            *_l_itt = rand();
        }
        return make_rng<Rng>(std::move(_l_seed));
    }
    case 1:
        return make_rng<Rng>(get<1>(_a_global_seed));
    default:
        abc::unreachable_variant<decltype(_a_global_seed)>(u8"make_rng");
        // This code will never be reached.
        return make_rng<Rng>(get<1>(_a_global_seed));
    }
}

template <typename Rng>
requires std::derived_from<Rng, inner_rng_t>
__constexpr_imp rng_t
    rng_t::make_rng(
        seed_t&& _a_seed
    ) noexcept
{
    using namespace std;
    return rng_t(make_unique<Rng>(_a_seed), std::move(_a_seed));
}

template <typename Rng>
requires std::derived_from<Rng, inner_rng_t>
__constexpr_imp rng_t
    rng_t::make_rng(
        const seed_t& _a_seed
    ) noexcept
{
    using namespace std;
    return rng_t(make_unique<Rng>(_a_seed), _a_seed);
}

__constexpr_imp void
    rng_t::progress(
        const size_t _a_expected_calls
    )
        noexcept(
            unreachable_does_not_throw_exception
        )
{
    if (_m_calls != _a_expected_calls)
    {
        inner_rng_t& _l_rng{get_inner_rng()};
        if (_m_calls > _a_expected_calls)
        {
            // Basically been reset to zero.
            _l_rng.set_seed(_m_seed);
            _l_rng.progress(_a_expected_calls);
        }
        else
        {
            _l_rng.progress(_a_expected_calls - _m_calls);
        }
        _m_calls = _a_expected_calls;
    }
}

__constexpr_imp rng_t::result_type_t
                rng_t::operator()() noexcept(
        unreachable_does_not_throw_exception
    )
{
    inner_rng_t& _l_rng{get_inner_rng()};
    ++_m_calls;
    return _l_rng();
}

__constexpr_imp size_t
    rng_t::calls() const noexcept
{
    return _m_calls;
}

__constexpr_imp
    rng_t::rng_t(
        rng_t&& _a_rng
    ) noexcept
    : _m_rng(std::move(_a_rng._m_rng)), _m_calls(_a_rng._m_calls)
{}

__constexpr_imp
    rng_t::rng_t(
        const rng_t& _a_rng
    )
        noexcept(
            unreachable_does_not_throw_exception
        )
    : _m_rng(
          _a_rng._m_rng == nullptr
              ?
#if _TESTING_BUILD
              throw abc::unreachable_exception_t(rng_t::_s_error_msg)
#else
              inner_rng_ptr_t{}
#endif
              : _a_rng._m_rng->deep_copy()
      )
    , _m_calls(_a_rng._m_calls)
{}

__constexpr_imp rng_t&
    rng_t::operator=(
        const rng_t& _a_arg
    )
{
    if (this != &_a_arg)
    {
        _m_rng   = _a_arg.get_inner_rng().deep_copy();
        _m_seed  = _a_arg._m_seed;
        _m_calls = _a_arg._m_calls;
    }
    return *this;
}


#if _TESTING_BUILD
template <typename Rng>
requires std::derived_from<Rng, inner_rng_t>
__constexpr rng_t
    rng_t::make_default_rng() noexcept
{
    return rng_t(inner_rng_ptr_t{}, seed_t{});
}
#endif
__constexpr
rng_t::rng_t(
    inner_rng_ptr_t&& _a_rng,
    seed_t&&          _a_seed
)
    : _m_rng(std::move(_a_rng)), _m_seed(std::move(_a_seed))
{}

__constexpr
rng_t::rng_t(
    inner_rng_ptr_t&& _a_rng,
    const seed_t&     _a_seed
)
    : _m_rng(std::move(_a_rng)), _m_seed(_a_seed)
{}

__constexpr inner_rng_t&
    rng_t::get_inner_rng() const noexcept(
        unreachable_does_not_throw_exception
    )
{
    if (_m_rng == nullptr)
    {
        _ABC_UNREACHABLE(rng_t::_s_error_msg);
    }
    else
    {
        return *_m_rng;
    }
}

_END_ABC_UTILITY_NS