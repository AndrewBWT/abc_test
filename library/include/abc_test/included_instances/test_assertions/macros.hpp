#pragma once
#include "abc_test/core/test_assertions/macros.hpp"

/*!
 * @brief Given a function as an argument, checks that the function does not
 * throw an exception, and wraps the result up in a matcher which is returned
 * from this macro.
 */
#define _MAKE_MATCHER_CHECKING_NO_EXCEPTION_THROWN(_a_function)     \
    [&]() -> abc::matcher_t                                         \
    {                                                               \
        abc::matcher_t _l_rv;                                       \
        __ABC_INTERNAL_BEGIN_NO_THROW_MATCHER(_l_rv);               \
        _a_funcion();                                               \
        __ABC_INTERNAL_END_NO_THROW_MATCHER(_l_rv);                 \
        _a_matcher_name.add_source_info(                            \
            "_MAKE_MATCHER_CHECKING_NO_EXCEPTION_THROWN",           \
            _ABC_NS_UTILITY_STR::mk_str_representing_function_args( \
                #_a_function                                        \
            ),                                                      \
            std::source_location::current()                         \
        );                                                          \
        return _l_rv;                                               \
    }()
/*!
 * Given a matcher to write to, is part of a pair of macros for checking that a
 * block of code does not throw an exception.
 *
 * The argument is the matcher to write to.
 */
#define _BEGIN_NO_THROW_MATCHER(_a_matcher_name)                \
    _a_matcher_name.add_source_info(                            \
        "_BEGIN_NO_THROW_MATCHER",                              \
        _ABC_NS_UTILITY_STR::mk_str_representing_function_args( \
            #_a_matcher_name                                    \
        ),                                                      \
        std::source_location::current()                         \
    );                                                          \
    __ABC_INTERNAL_BEGIN_NO_THROW_MATCHER(_a_matcher_name)
/*!
 * Given a matcher to write to, is part of a pair of macros for checking that a
 * block of code does not throw an exception.
 *
 * The argument is the matcher to write to.
 */
#define _END_NO_THROW_MATCHER(_a_matcher_name)                  \
    _a_matcher_name.add_source_info(                            \
        "_END_NO_THROW_MATCHER",                                \
        _ABC_NS_UTILITY_STR::mk_str_representing_function_args( \
            #_a_matcher_name                                    \
        ),                                                      \
        std::source_location::current()                         \
    );                                                          \
    __ABC_INTERNAL_END_NO_THROW_MATCHER(_a_matcher_name)

// Internal macro. Should only be used by developer.
_BEGIN_ABC_NS
template <bool Expected_Behaviour>
__no_constexpr_imp matcher_t
    no_exception_thrown() noexcept
{
    using namespace std;
    using namespace _ABC_NS_MATCHER;
    bool     _l_result{true};
    u8string _l_str;
    if constexpr (not Expected_Behaviour)
    {
        _l_result = false;
        _l_str    = u8"not ";
    }
    return mk_matcher_using_result(matcher_result_t(
        _l_result,
        matcher_result_infos_t(fmt::format(
            u8"No exception was thrown, which was {0}the expected behaviour.",
            _l_str
        ))
    ));
}

__no_constexpr_imp matcher_t
    unexpected_exception_thrown(
        const std::exception& _a_exception
    ) noexcept
{
    using namespace std;
    using namespace _ABC_NS_MATCHER;
    using namespace _ABC_NS_UTILITY_STR;
    return mk_matcher_using_result(matcher_result_t(
        false,
        matcher_result_infos_t(fmt::format(
            u8"An unexpected exception, of a type derived from {0} was thrown. "
            u8"The exception's what() function returned \"{1}\".",
            type_id<std::exception>(),
            cast_string_to_u8string(_a_exception.what())
        ))
    ));
}

__no_constexpr_imp matcher_t
    unexpected_exception_of_unknown_type_thrown() noexcept
{
    using namespace std;
    using namespace _ABC_NS_MATCHER;
    return mk_matcher_using_result(matcher_result_t(
        false,
        matcher_result_infos_t(fmt::format(
            u8"An unexpected exception of an unknown type was thrown."
        ))
    ));
}

_END_ABC_NS
#define __ABC_INTERNAL_BEGIN_NO_THROW_MATCHER(_a_matcher_name) \
    _a_matcher_name = abc::no_exception_thrown<true>();        \
                                                               \
    try                                                        \
    {
// Internal macro. Should only be used by developer.
#define __ABC_INTERNAL_END_NO_THROW_MATCHER(_a_matcher_name) \
    }                                                        \
    catch (const std::exception& _a_exception)               \
    {                                                        \
        _a_matcher_name.override_matcher(                    \
            unexpected_exception_thrown(_a_exception)        \
        );                                                   \
    }                                                        \
    catch (...)                                              \
    {                                                        \
        _a_matcher_name.override_matcher(                    \
            unexpected_exception_of_unknown_type_thrown()    \
        );                                                   \
    }

/*!
 * Macro for checking that a function will definitely throw an exception.
 *
 * The exception type is irrilevant, just that an exception is thrown.
 */
#define _MAKE_MATCHER_CHECKING_EXCEPTION_THROWN(_a_function)        \
    [&]() -> abc::matcher_t                                         \
    {                                                               \
        abc::matcher_t _l_rv;                                       \
        __ABC_INTERNAL_BEGIN_THROW_MATCHER(_l_rv);                  \
        _a_funcion();                                               \
        __ABC_INTERNAL_END_THROW_MATCHER(_l_rv);                    \
        _a_matcher_name.add_source_info(                            \
            "_MAKE_MATCHER_CHECKING_EXCEPTION_THROWN",              \
            _ABC_NS_UTILITY_STR::mk_str_representing_function_args( \
                #_a_function                                        \
            ),                                                      \
            std::source_location::current()                         \
        );                                                          \
        return _l_rv;                                               \
    }()
/*!
 * Given a matcher to write to, is part of a pair of macros for checking that a
 * block of code definitely throws an exception.
 *
 * The argument is the matcher to write to.
 */
#define _BEGIN_THROW_MATCHER(_a_matcher_name)         \
    __ABC_INTERNAL_BEGIN_THROW_MATCHER(               \
        _a_matcher_name, true, "_BEGIN_THROW_MATCHER" \
    )
/*!
 * Given a matcher to write to, is part of a pair of macros for checking that a
 * block of code definitely throws an exception.
 *
 * The argument is the matcher to write to.
 */
#define _END_THROW_MATCHER(_a_matcher_name)                     \
    _a_matcher_name.add_source_info(                            \
        "_END_THROW_MATCHER",                                   \
        _ABC_NS_UTILITY_STR::mk_str_representing_function_args( \
            #_a_matcher_name                                    \
        ),                                                      \
        std::source_location::current()                         \
    );                                                          \
    __ABC_INTERNAL_END_THROW_MATCHER(_a_matcher_name)
// Internal macro.
#define __ABC_INTERNAL_BEGIN_THROW_MATCHER(_a_matcher_name) \
    _a_matcher_name = abc::unexpected_no_exception();       \
    try                                                     \
    {
#define __ABC_INTERNAL_END_THROW_MATCHER(_a_matcher_name)              \
    }                                                                  \
    catch (const std::exception& _a_exception)                         \
    {                                                                  \
        _a_matcher_name.override_matcher(                              \
            expected_exception_thrown(_a_exception)                    \
        );                                                             \
    }                                                                  \
    catch (...)                                                        \
    {                                                                  \
        _a_matcher_name.override_matcher(expected_exception_thrown()); \
    }
/*!
 * Macro for checking that a function will throw an exception whose what()
 * function returns an exact message.
 *
 * It may not be obvious, but the exception returned must either be
 * std::exception, or use it as a base class.
 *
 */
#define _MAKE_MATCHER_CHECKING_EXCEPTION_MSG(_a_msg, _a_function)   \
    [&]() -> abc::matcher_t                                         \
    {                                                               \
        abc::matcher_t _l_rv;                                       \
        __ABC_INTERNAL_BEGIN_EXCEPTION_MSG_MATCHER(_l_rv, _a_msg);  \
        _a_funcion();                                               \
        __ABC_INTERNAL_END_EXCEPTION_MSG_MATCHER(_l_rv, _a_msg);    \
        _a_matcher_name.add_source_info(                            \
            "_MAKE_MATCHER_CHECKING_EXCEPTION_MSG",                 \
            _ABC_NS_UTILITY_STR::mk_str_representing_function_args( \
                #_a_msg, #_a_function                               \
            ),                                                      \
            std::source_location::current()                         \
        );                                                          \
        return _l_rv;                                               \
    }()
/*!
 * Given a matcher to write to, is part of a pair of macros for checking that a
 * block of code throws an exception with a specific message.
 *
 */
#define _BEGIN_EXCEPTION_MSG_MATCHER(_a_matcher_name, _a_msg)   \
    _a_matcher_name.add_source_info(                            \
        "_BEGIN_EXCEPTION_MSG_MATCHER",                         \
        _ABC_NS_UTILITY_STR::mk_str_representing_function_args( \
            #_a_matcher_name, #_a_msg                           \
        ),                                                      \
        std::source_location::current()                         \
    );                                                          \
    __ABC_INTERNAL_BEGIN_EXCEPTION_MSG_MATCHER(_a_matcher_name, _a_msg)
/*!
 * Given a matcher to write to, is part of a pair of macros for checking that a
 * block of code throws an exception with a specific message.
 *
 */
#define _END_EXCEPTION_MSG_MATCHER(_a_matcher_name, _a_msg)     \
    _a_matcher_name.add_source_info(                            \
        "_END_EXCEPTION_MSG_MATCHER",                           \
        _ABC_NS_UTILITY_STR::mk_str_representing_function_args( \
            #_a_matcher_name, #_a_msg                           \
        ),                                                      \
        std::source_location::current()                         \
    );                                                          \
    __ABC_INTERNAL_END_EXCEPTION_MSG_MATCHER(_a_matcher_name, _a_msg)
// Internal macro
#define __ABC_INTERNAL_BEGIN_EXCEPTION_MSG_MATCHER(_a_matcher_name, _a_msg) \
    _a_matcher_name = abc::unexpected_no_exception();                       \
    \                                                                       \
    try                                                                     \
    {
// Internal macro
#define __ABC_INTERNAL_END_EXCEPTION_MSG_MATCHER(_a_matcher_name, _a_msg) \
    \                                                                     \
    }                                                                     \
    catch (const std::exception& _a_exception)                            \
    {                                                                     \
        _a_matcher_name.override_matcher(                                 \
            expected_exception_thrown(_a_exception)                       \
        );                                                                \
        \                                                                 \
    }                                                                     \
    catch (...)                                                           \
    {                                                                     \
        _a_matcher_name.override_matcher(expected_exception_thrown());    \
        \                                                                 \
    }

/*!
 * Macro for checking that a function will throw an exception that is of a
 * specific type, or derived from a specific type.
 *
 *
 */
#define _MAKE_MATCHER_CHECKING_EXCEPTION_TYPE(_a_type, _a_function)  \
    [&]() -> abc::matcher_t                                          \
    {                                                                \
        abc::matcher_t _l_rv;                                        \
        __ABC_INTERNAL_BEGIN_EXCEPTION_TYPE_MATCHER(_l_rv, _a_type); \
        _a_function();                                               \
        __ABC_INTERNAL_END_EXCEPTION_TYPE_MATCHER(_l_rv, _a_type);   \
        _l_rv.add_source_info(                                       \
            "_MAKE_MATCHER_CHECKING_EXCEPTION_TYPE",                 \
            _ABC_NS_UTILITY_STR::mk_str_representing_function_args(  \
                #_a_type, #_a_function                               \
            ),                                                       \
            std::source_location::current()                          \
        );                                                           \
        return _l_rv;                                                \
    }()
/*!
 * Given a matcher to write to, is part of a pair of macros for checking that a
 * block of code throws an exception that is either of a specific type, or
 * derived from that type..
 *
 */
#define _BEGIN_EXCEPTION_TYPE_MATCHER(_a_matcher_name, _a_type) \
    _a_matcher_name.add_source_info(                            \
        "_BEGIN_EXCEPTION_TYPE_MATCHER",                        \
        _ABC_NS_UTILITY_STR::mk_str_representing_function_args( \
            #_a_matcher_name, #_a_type                          \
        ),                                                      \
        std::source_location::current()                         \
    );                                                          \
    __ABC_INTERNAL_BEGIN_EXCEPTION_TYPE_MATCHER(_a_matcher_name, _a_type)
/*!
 * Given a matcher to write to, is part of a pair of macros for checking that a
 * block of code throws an exception that is either of a specific type, or
 * derived from that type..
 */
#define _END_EXCEPTION_TYPE_MATCHER(_a_matcher_name, _a_type)   \
    _a_matcher_name.add_source_info(                            \
        "_END_EXCEPTION_TYPE_MATCHER",                          \
        _ABC_NS_UTILITY_STR::mk_str_representing_function_args( \
            #_a_matcher_name, #_a_type                          \
        ),                                                      \
        std::source_location::current()                         \
    );                                                          \
    __ABC_INTERNAL_END_EXCEPTION_TYPE_MATCHER(_a_matcher_name, _a_type)

_BEGIN_ABC_NS
template <typename T>
__no_constexpr_imp abc::matcher_t
                   unexpected_no_exception()
{
    using namespace std;
    using namespace _ABC_NS_MATCHER;
    using namespace _ABC_NS_UTILITY_STR;
    return mk_matcher_using_result(matcher_result_t(
        false,
        matcher_result_infos_t(fmt::format(
            u8"No exception was thrown, which was not the expected behaviour. "
            u8"Function expected an exception of type \"{0}\".",
            type_id<T>()
        ))
    ));
}

template <typename T>
__no_constexpr_imp abc::matcher_t
                   expected_exception_thrown(
                       const T& _a_exception
                   )
{
    using namespace std;
    using namespace _ABC_NS_MATCHER;
    using namespace _ABC_NS_UTILITY_STR;
    return mk_matcher_using_result(matcher_result_t(
        true,
        matcher_result_infos_t(fmt::format(
            u8"An exception of type \"{0}\" was thrown, which was the expected "
            u8"behaviour.",
            type_id<T>()
        ))
    ));
}

template <typename T>
__no_constexpr_imp abc::matcher_t
                   expected_exception_not_thrown(
                       const std::exception& _a_exception
                   )
{
    using namespace std;
    using namespace _ABC_NS_MATCHER;
    using namespace _ABC_NS_UTILITY_STR;
    return mk_matcher_using_result(matcher_result_t(
        false,
        matcher_result_infos_t(fmt::format(
            u8"Expected an exception of type \"{0}\".",
            cast_string_to_u8string(_a_exception.what())
        ))
    ));
}

template <typename T>
__no_constexpr_imp abc::matcher_t
                   expected_exception_not_thrown()
{
    using namespace std;
    using namespace _ABC_NS_MATCHER;
    using namespace _ABC_NS_UTILITY_STR;
    return mk_matcher_using_result(matcher_result_t(
        false,
        matcher_result_infos_t(fmt::format(
            u8"Expected an exception of type \"{0}\".", type_id<T>()
        ))
    ));
}

_END_ABC_NS
// Internal macro
#define __ABC_INTERNAL_BEGIN_EXCEPTION_TYPE_MATCHER(_a_matcher_name, _a_type) \
    _a_matcher_name = abc::unexpected_no_exception<_a_type>();                \
    try                                                                       \
    {
// Internal macro
#define __ABC_INTERNAL_END_EXCEPTION_TYPE_MATCHER(_a_matcher_name, _a_type) \
    }                                                                       \
    catch (const _a_type& _a_exception)                                     \
    {                                                                       \
        _a_matcher_name.override_matcher(                                   \
            expected_exception_thrown(_a_exception)                         \
        );                                                                  \
    }                                                                       \
    catch (const std::exception& _a_exception)                              \
    {                                                                       \
        _a_matcher_name.override_matcher(                                   \
            expected_exception_not_thrown<_a_type>(_a_exception)            \
        );                                                                  \
    }                                                                       \
    catch (...)                                                             \
    {                                                                       \
        _a_matcher_name.override_matcher(                                   \
            expected_exception_not_thrown<_a_type>()                        \
        );                                                                  \
    }

/*!
 * Macro for checking that a function will throw an exception that is of a
 * specific type, or derived from a specific type.
 *
 *
 */
#define _MAKE_MATCHER_CHECKING_EXCEPTION_TYPE_AND_MSG(              \
    _a_exception_type, _a_msg, _a_function                          \
)                                                                   \
    [&]() -> abc::matcher_t                                         \
    {                                                               \
        abc::matcher_t _l_rv;                                       \
        __ABC_INTERNAL_BEGIN_EXCEPTION_TYPE_AND_MSG_MATCHER(        \
            _l_rv, _a_exception_type, _a_msg                        \
        );                                                          \
        _a_function();                                              \
        __ABC_INTERNAL_END_EXCEPTION_TYPE_AND_MSG_MATCHER(          \
            _l_rv, _a_exception_type, _a_msg                        \
        );                                                          \
        _l_rv.add_source_info(                                      \
            "_MAKE_MATCHER_CHECKING_EXCEPTION_TYPE_AND_MSG",        \
            _ABC_NS_UTILITY_STR::mk_str_representing_function_args( \
                #_a_exception_type, #_a_msg, #_a_function           \
            ),                                                      \
            std::source_location::current()                         \
        );                                                          \
        return _l_rv;                                               \
    }()
/*!
 * Given a matcher to write to, is part of a pair of macros for checking that a
 * block of code throws an exception that is either of a specific type, or
 * derived from that type..
 *
 */
#define _BEGIN_EXCEPTION_TYPE_AND_MSG_MATCHER(                  \
    _a_matcher_name, _a_exception_type, _a_msg                  \
)                                                               \
    _a_matcher_name.add_source_info(                            \
        "_BEGIN_EXCEPTION_TYPE_AND_MSG_MATCHER",                \
        _ABC_NS_UTILITY_STR::mk_str_representing_function_args( \
            #_a_matcher_name, #_a_exception_type, #_a_msg       \
        ),                                                      \
        std::source_location::current()                         \
    );                                                          \
    __ABC_INTERNAL_BEGIN_EXCEPTION_TYPE_AND_MSG_MATCHER(        \
        _a_matcher_name, _a_exception_type, _a_msg              \
    )
/*!
 * Given a matcher to write to, is part of a pair of macros for checking that a
 * block of code throws an exception that is either of a specific type, or
 * derived from that type..
 */
#define _END_EXCEPTION_TYPE_AND_MSG_MATCHER(                    \
    _a_matcher_name, _a_exception_type, _a_msg                  \
)                                                               \
    _a_matcher_name.add_source_info(                            \
        "_END_EXCEPTION_TYPE_AND_MSG_MATCHER",                  \
        _ABC_NS_UTILITY_STR::mk_str_representing_function_args( \
            #_a_matcher_name, #_a_exception_type, #_a_msg       \
        ),                                                      \
        std::source_location::current()                         \
    );                                                          \
    __ABC_INTERNAL_END_EXCEPTION_TYPE_AND_MSG_MATCHER(          \
        _a_matcher_name, _a_exception_type, _a_msg              \
    )

// Internal macro
_BEGIN_ABC_NS
template <typename T>
__no_constexpr_imp matcher_t
    no_exception_thrown(
        const std::u8string_view _a_msg
    ) noexcept
{
    using namespace std;
    using namespace _ABC_NS_MATCHER;
    using namespace _ABC_NS_UTILITY_STR;
    bool _l_result{true};
    return mk_matcher_using_result(matcher_result_t(
        _l_result,
        matcher_result_infos_t(fmt::format(
            u8"No exception was thrown, which is not the expected behaviour. "
            u8"The expected behaviour was that an exception of type \"{0}\" "
            u8"would be thrown, which would have a what() function that "
            u8"returned the std::u8string \"{1}\".",
            type_id<T>(),
            _a_msg
        ))
    ));
}

template <typename T>
__no_constexpr_imp matcher_t
    check_expected_exception(
        const T&                 _a_exception,
        const std::u8string_view _a_msg
    ) noexcept
{
    using namespace std;
    using namespace _ABC_NS_MATCHER;
    using namespace _ABC_NS_UTILITY_STR;
    if constexpr (derived_from<T, std::exception>)
    {
        auto _l_str_result{
            abc::matcher::matcher_default_comparable_t<
                abc::matcher::comparison_enum_t::EQ,
                u8string_view,
                u8string_view>{}
                .run(_a_msg, cast_string_to_u8string(_a_exception.what()))
        };
        if (_l_str_result.passed())
        {
            return mk_matcher_using_result(matcher_result_t(
                true,
                matcher_result_infos_t(fmt::format(
                    u8"An exception was thrown of the correct type, and whose "
                    u8"what() function returned the correct "
                    u8"std::u8string. This was the expected behaviour."
                ))
            ));
        }
        else
        {
            return mk_matcher_using_result(matcher_result_t(
                false,
                matcher_result_infos_t(
                    fmt::format(
                        u8"An exception was thrown of the correct type, "
                        u8"however "
                        u8"the exception's "
                        u8"what() function returned a std::u8string which did "
                        u8"not "
                        u8"match the expected std::u8string. The discrepencies "
                        u8"are "
                        u8"shown below. This was not the expected behaviour."
                    ),
                    std::vector<std::u8string>{},
                    std::vector<matcher_result_infos_t::tree_structure_t>(
                        1,
                        make_tuple(
                            u8"Child",
                            u8"C",
                            make_shared<matcher_result_infos_t>(_l_str_result
                                                                    .str())
                        )
                    )
                )
            ));
        }
    }
    else
    {
        return mk_matcher_using_result(matcher_result_t(
            false,
            matcher_result_infos_t(fmt::format(
                u8"An exception was thrown of the correct type. However "
                u8"the exception, of type \"{0}\", is not a derived class "
                u8"of "
                u8"\"{1}\". Therefore, it does not have a what() function, "
                u8"and "
                u8"therefore its text output canno tbe compared with the "
                u8"expected text output \"{2}\". This is not the expected "
                u8"behaviour.",
                type_id<T>(),
                type_id<std::exception>(),
                _a_msg
            ))
        ));
    }
}

template <typename T, typename U>
__no_constexpr_imp matcher_t
    incorrect_exception(
        const U&                 _a_exception,
        const std::u8string_view _a_msg
    ) noexcept
{
    using namespace std;
    using namespace _ABC_NS_MATCHER;
    using namespace _ABC_NS_UTILITY_STR;
    auto _l_str_result{
        abc::matcher::matcher_default_comparable_t<
            abc::matcher::comparison_enum_t::EQ,
            u8string_view,
            u8string_view>{}
            .run(_a_msg, cast_string_to_u8string(_a_exception.what()))
    };
    if (_l_str_result.passed())
    {
        return mk_matcher_using_result(matcher_result_t(
            false,
            matcher_result_infos_t(fmt::format(
                u8"An exception was thrown of the incorrect type, "
                u8"however the exception was derived from \"{0}\", and its "
                u8"what() function returned the correct std::u8string "
                u8"This was not the expected behaviour."
            ))
        ));
    }
    else
    {
        return mk_matcher_using_result(matcher_result_t(
            false,
            matcher_result_infos_t(
                fmt::format(
                    u8"An exception was thrown of the correct type, "
                    u8"however "
                    u8"the exception's "
                    u8"what() function returned a std::u8string which did "
                    u8"not "
                    u8"match the expected std::u8string. The discrepencies "
                    u8"are "
                    u8"shown below. This was the expected behaviour."
                ),
                _l_str_result.str().get_vector()
            )
        ));
    }
}

template <typename T>
__no_constexpr_imp matcher_t
    incorrect_exception(
        const std::u8string_view _a_msg
    ) noexcept
{
    using namespace std;
    using namespace _ABC_NS_MATCHER;
    return mk_matcher_using_result(matcher_result_t(
        false,
        matcher_result_infos_t(
            fmt::format(u8"An exception was thrown of the correct type, "
                        u8"however "
                        u8"the exception's "
                        u8"what() function returned a std::u8string which did "
                        u8"not "
                        u8"match the expected std::u8string. The discrepencies "
                        u8"are "
                        u8"shown below. This was the expected behaviour.")
        )
    ));
}

_END_ABC_NS
#define __ABC_INTERNAL_BEGIN_EXCEPTION_TYPE_AND_MSG_MATCHER(               \
    _a_matcher_name, _a_exception_type, _a_msg                             \
)                                                                          \
    _a_matcher_name = abc::no_exception_thrown<_a_exception_type>(_a_msg); \
    try                                                                    \
    {
// Internal macro
#define __ABC_INTERNAL_END_EXCEPTION_TYPE_AND_MSG_MATCHER(               \
    _a_matcher_name, _a_exception_type, _a_msg                           \
)                                                                        \
    }                                                                    \
    catch (const _a_exception_type& _a_exception)                        \
    {                                                                    \
        _a_matcher_name.override_matcher(                                \
            check_expected_exception(_a_exception, _a_msg)               \
        );                                                               \
    }                                                                    \
    catch (const std::exception& _a_exception)                           \
    {                                                                    \
        _a_matcher_name.override_matcher(                                \
            incorrect_exception<_a_exception_type>(_a_exception, _a_msg) \
        );                                                               \
    }                                                                    \
    catch (...)                                                          \
    {                                                                    \
        _a_matcher_name.override_matcher(                                \
            incorrect_exception<_a_exception_type>(_a_msg)               \
        );                                                               \
    }
