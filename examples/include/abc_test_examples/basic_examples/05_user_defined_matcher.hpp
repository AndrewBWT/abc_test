#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"

#include <numeric>
#include <ranges>
#include <vector>

namespace test
{
namespace
{
    template<typename T>
    constexpr abc::matcher::matcher_result_t
        run(
            const std::vector<T>& _a_vector
        )
    {
        using namespace abc;
        using namespace std;
        using namespace abc::matcher;
        T      _l_bad_element{};
        size_t _l_bad_idx{0};
        bool   _l_all_same{true};
        if (_a_vector.size() > 0)
        {
            const T& _l_check{ _a_vector[0]};
            for (size_t _l_idx{1}; _l_idx < _a_vector.size(); ++_l_idx)
            {
                if (_a_vector[_l_idx] != _l_check)
                {
                    _l_all_same    = false;
                    _l_bad_idx     = _l_idx;
                    _l_bad_element = _a_vector[_l_idx];
                    break;
                }
            }
        }
        if (_l_all_same)
        {
            return matcher_result_t{true, ""};
            //	fmt::format("Vector {0} contains all the same element.",
            //		_m_vector) };
        }
        else
        {
            return matcher_result_t{
                false,
                fmt::format(
                    "Vector {0} does not contain all same element."
                    "First element which is not the same as all the previous "
                    "is at index {1}, "
                    "which contains the element {2}.",
                    _a_vector,
                    _l_bad_idx,
                    _l_bad_element
                )
            };
        }
    }
}
template <typename T>
class list_same_matcher_t : public abc::matcher::matcher_base_t
{
public:
    constexpr list_same_matcher_t(
        const std::vector<T>& _a_vector
    )
        : abc::matcher::matcher_base_t(run(_a_vector))
    {}
};

template <typename T>
abc::matcher_t
    list_same_matcher(
        const std::vector<T>& _a_vect
    ) noexcept
{
    return abc::matcher::make_matcher(new test::list_same_matcher_t(_a_vect));
}
} // namespace test

_TEST_CASE(
    abc::test_case_t(
        {.name        = "file_05_example_01",
         .description = "User-defined matchers",
         .path        = "examples::basic_examples::05_user_defined_matcher"}
    )
)
{
    using namespace abc;
    /*!
     * Matchers are designed to be user-definable. In fact, we believe that a
     * users will find writing their own matchers to be much more beneficial to
     * reducing code bloat than using those we provide. Above we show an example
     * of a matcher which takes a list of arbitrary values and ensures they are
     * all equal.
     *
     * The return value from run must be in the form matcher_result_t. These
     * objects contain three fields; whether they have been ran (should always
     * be true), whether they passed, and what the string describing the
     * matcher's result is.
     *
     * We deliverabtely left the matcher passing state as having an empty
     * string, as usually the user will only care about failing instances.
     *
     * Note that we use a named matcher. This is an asthetic choice, as we don't
     * particualrly like using the new keyword when there is no need for it.
     *
     * Below we show some uses of it.
     */
    using namespace std;
    using namespace test;
    vector<int> _l_x{1, 1, 1, 1, 1, 1};
    _CHECK(list_same_matcher(_l_x));
    _l_x = {1, 1, 1, 1, 1, 2};
    _CHECK(list_same_matcher(_l_x));
    _l_x = {1, 1, 1, 1, 1, 1};
    _CHECK(list_same_matcher(_l_x));
    /*!
     * Here are three more examples of valid code using this new matcher.
     */
    _CHECK(make_matcher(new list_same_matcher_t(_l_x)));
    _CHECK(matcher_t(new list_same_matcher_t(_l_x)));
    _CHECK(matcher_t(list_same_matcher(_l_x)));
}

_TEST_CASE(
    abc::test_case_t(
        {.name        = "file_05_example_02",
         .description = "User-defined matchers using function_wrapper",
         .path        = "examples::basic_examples::05_user_defined_matcher"}
    )
)
{
    using namespace abc;
    /*!
     * Sometimes it may not be convenient, or appropriate, to create a generic
     * matcher class.
     *
     * Perhaps the user just wants a one-time matcher for a specific test.
     *
     * The included function_wrapper matcher is designed for these situations.
     *
     * The user provides an std::function<abc::matcher_result_t(void)> object,
     * which is called by the run function. Below we show its use
     */
    using namespace std;
    using namespace test;
    vector<int>                               _l_x{1, 1, 1, 1, 1, 1};
    abc::matcher::function_wrapper_internal_t _l_f1 = [&]()
    {
        using namespace abc::matcher;
        if (_l_x.size() == 0)
        {
            return matcher_result_t(true, "Vector is empty");
        }
        else
        {
            return matcher_result_t(false, "Vector is not empty");
        }
    };
    _CHECK(matcher_t(new abc::matcher::function_wrapper_matcher_t(_l_f1)));
    _CHECK(matcher_t(function_wrapper(_l_f1)));
    _CHECK(make_matcher(new abc::matcher::function_wrapper_matcher_t(_l_f1)));
    _CHECK(function_wrapper(_l_f1));
    /*!
     * However, this is very brittle; it literally works on a single element,
     * defined by the user, which has to stay in scope or when the function is
     * ran, an error will be thrown.
     *
     * A better approach is to use the function_wrapper_matcher_t class to
     * create a runnable function, which can be called with
     */
}

namespace testing
{

template <typename T>
constexpr abc::matcher_t
    is_empty(
        std::vector<T>& _a_vect
    ) noexcept
{
    using namespace std;
    using namespace abc;
    using namespace _ABC_NS_MATCHER;
    return matcher_t(matcher_base_ptr_t(new function_wrapper_matcher_t(
        [&]()
        {
            return matcher_result_t(
                (_a_vect.size() == 0),
                fmt::format(
                    "{0} is{1} empty",
                    _a_vect,
                    (_a_vect.size() == 0) ? "" : " not"
                )
            );
        }
    )));
}
} // namespace testing

_TEST_CASE(
    abc::test_case_t(
        {.name        = "file_05_example_03",
         .description = "A second user-defined matchers using function_wrapper",
         .path        = "examples::basic_examples::05_user_defined_matcher"}
    )
)
{
    using namespace abc;
    /*!
     * The above example use of a function wrapper is very brittle; it literally
     * works on a single element, defined by the user, which has to stay in
     * scope or when the function is ran, an error will be thrown.
     *
     * A better approach is to use the function_wrapper_matcher_t class to
     * create a runnable function, which can then be called with the argumtns as
     * needed.
     *
     * Above we have done just that.
     */
    using namespace std;
    vector<int>  x1{1, 2, 3};
    vector<char> x2;
    _CHECK(testing::is_empty(x1));
    _CHECK(testing::is_empty(x2));
}