#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"

_TEST_CASE(
    abc::test_case_t({.name = "Matcher examples"})
)
{
    using namespace abc;
    // matcher_t is the object which holds the result of a user-defined test. A
    // default matcher has a result value of true, and an empty result_str
    // value.
    matcher_t matcher_1;
    _CHECK(matcher_1);
    // The function bool_matcher creates a matcher_t whose result is the bool
    // argument given to bool_matcher. matcher_2 has a result value of true, and
    // a result_str value of "true".
    matcher_t matcher_2 = bool_matcher(true);
    // matcher_3 has a result value of false and a result_str value of "false".
    matcher_t matcher_3 = bool_matcher(false);
    // Below is an example showing how to annotate a matcher.
    annotated_matcher_t annotated_matcher_1(
        annotate(matcher_3, "Testing bool_matcher(false)")
    );
    _CHECK(matcher_2);
    _CHECK(matcher_3);
    _CHECK(annotated_matcher_1);
    int i               = 2;
    // Expressions can be encoded in matchers using the macro _EXPR. matcher_4
    // has a result value of true, as it checks if i==2 (which it does). Its
    // result_str value is "i == 2".
    matcher_t matcher_4 = _EXPR(i == 2);
    i                   = 8;
    // matcher_5 has a result value of false, as i<=7 is false. Its result_str
    // value is "i >= 7", showing the failure of the original expression.
    matcher_t matcher_5 = _EXPR(i <= 7);
    _CHECK(matcher_4);
    _CHECK(matcher_5);
    // Another example of annotating a matcher. Note the annotate function is
    // different to that used previously.
    annotated_matcher_t annotated_matcher_2(
        annotate(fmt::format("Checking i <= {}", 7), matcher_5)
    );
    _CHECK(annotated_matcher_2);
}

struct type_x
{
    int i;
    int j;

    inline bool
        operator==(
            const type_x& rhs
        ) const
    {
        return i == rhs.i && j == rhs.j;
    }
};

struct type_y
{
    std::vector<int> is;

    inline bool
        operator==(
            const type_x& rhs
        ) const
    {
        return is.size() == 2 && is[0] == rhs.i && is[1] == rhs.j;
    }

    inline bool
        operator==(
            const type_y& rhs
        ) const
    {
        return is == rhs.is;
    }
};

template <>
struct fmt::formatter<type_y> : formatter<string_view>
{
    inline auto
        format(
            type_y          arg_y,
            format_context& _a_ctx
        ) const -> format_context::iterator
    {
        using namespace std;
        const string _l_rv{fmt::format("y {{{0}}}", arg_y.is)};
        return formatter<string_view>::format(_l_rv, _a_ctx);
    }
};

_TEST_CASE(
    abc::test_case_t({.name = "X example"})
)
{
    type_x xi{1, 2};
    type_x xj{3, 4};
    _CHECK(_EXPR(xi == xj));
    type_y yi{
        {1, 2}
    };
    type_y yj{
        {4, 5, 6}
    };
    _CHECK(_EXPR(yi == yj));
    _CHECK(_EXPR(xi == yi));
}

_TEST_CASE(
    abc::test_case_t({.name = "Matcher examples, revisited"})
)
{
    using namespace abc;
    matcher_t matcher_1 = _MATCHER(matcher_t());
    _CHECK(matcher_1);
    matcher_t           matcher_2 = _MATCHER(bool_matcher(true));
    matcher_t           matcher_3 = _MATCHER(bool_matcher(false));
    annotated_matcher_t annotated_matcher_2(
        _MATCHER(annotate(matcher_3, "Testing bool_matcher(false)"))
    );
    _CHECK(matcher_2);
    _CHECK(matcher_3);
    _CHECK(annotated_matcher_2);
    int       i         = 2;
    matcher_t matcher_4 = _MATCHER(_EXPR(i == 2));
    i                   = 8;
    matcher_t matcher_5 = _MATCHER(_EXPR(i <= 7));
    _CHECK(matcher_4);
    _CHECK(matcher_5);
    annotated_matcher_t annotated_matcher_3(
        _MATCHER(annotate(fmt::format("Checking i <= {}", 7), matcher_5))
    );
    _CHECK(annotated_matcher_3);
}

_TEST_CASE(
    abc::test_case_t({.name = "Unrequired _MATCHER examples"})
)
{
    _CHECK(_MATCHER(_EXPR(1 == 2)));
    _CHECK(_MATCHER(_MATCHER(_EXPR(1 == 2))));
}

_TEST_CASE(
    abc::test_case_t({.name = "Matcher examples, revisited"})
)
{
    using namespace abc;
    matcher_t matcher_1 = _MATCHER(matcher_t());
    _CHECK(matcher_1);
    matcher_t           matcher_2 = _MATCHER(bool_matcher(true));
    matcher_t           matcher_3 = _MATCHER(bool_matcher(false));
    annotated_matcher_t annotated_matcher_2(
        _MATCHER(annotate(matcher_3, "Testing bool_matcher(false)"))
    );
    _CHECK(matcher_2);
    _CHECK(matcher_3);
    _CHECK(annotated_matcher_2);
    int       i         = 2;
    matcher_t matcher_4 = _MATCHER(_EXPR(i == 2));
    i                   = 8;
    matcher_t matcher_5 = _MATCHER(_EXPR(i <= 7));
    _CHECK(matcher_4);
    _CHECK(matcher_5);
    annotated_matcher_t annotated_matcher_3(
        _MATCHER(annotate(fmt::format("Checking i <= {}", 7), matcher_5))
    );
    _CHECK(annotated_matcher_3);
}

_TEST_CASE(
    abc::test_case_t({.name = "Annotation examples"})
)
{
    using namespace abc;
    matcher_t           m1  = _MATCHER(_EXPR(2 <= 3));
    annotated_matcher_t am1 = _MATCHER(annotate("Annotating 1", m1));
    annotated_matcher_t am2 = _MATCHER(annotate(m1, "Annotating 2"));
    _CHECK(am1);
    _CHECK(am2);
    _CHECK(annotate("Annotating a final time", _EXPR(2 <= 3)));
}

_TEST_CASE(
    abc::test_case_t({.name = "Logical operators"})
)
{
    using namespace abc;
    matcher_t m1 = _MATCHER(_EXPR(1 < 2));
    matcher_t m2 = _MATCHER(_EXPR(2 == 2));
    _CHECK(m1 && m2);
    _CHECK(m2 || m1);
    _CHECK(m1 || m2);
    matcher_t m3 = _MATCHER(m1 && m2);
    _CHECK(! m3);
    _CHECK(annotate(
        "Checking this vitally important thing",
        ! (_EXPR(1 < 2) && _EXPR(2 == 2))
    ));
}

_TEST_CASE(
    abc::test_case_t({.name = "A computationally expensive test"})
)
{
    using namespace abc;
    using namespace std;
    auto check_iota_results
        = [](const vector<int>& vect, const pair<int, int>& begin_and_end_ints)
    {
        matcher_t left_matcher{_EXPR(
            vect.size()
            == std::abs(begin_and_end_ints.second - begin_and_end_ints.first)
        )};
        matcher_t right_matcher{_EXPR(
            vect
            == ranges::to<vector<int>>(ranges::iota_view(
                begin_and_end_ints.first, begin_and_end_ints.second
            ))
        )};
        _CHECK(left_matcher && right_matcher);
    };
    check_iota_results(
        ranges::to<vector<int>>(ranges::iota_view(1, 10'000)), {1, 9'999}
    );
}

_TEST_CASE(
    abc::test_case_t({.name = "Simple simulation of logical operators"})
)
{
    using namespace abc;
    using namespace std;
    simulated_or_expr_t se_1;
    se_1.set_left_child(_MATCHER(_EXPR(1 == 1)));
    // This simulates a logical OR expression. As the only set child is
    // true, the if statemenet's body is not entered.
    if (se_1)
    {
        se_1.set_right_child(_MATCHER(_EXPR(2 == 2)));
    }
    _CHECK(se_1);

    simulated_and_expr_t se_2;
    se_2.set_left_child(_MATCHER(_EXPR(1 == 2)));
    // This simulates a logical AND expression. As the only set child is false,
    // the if statement's body is not entered.
    if (se_2)
    {
        se_2.set_right_child(_MATCHER(_EXPR(2 == 2)));
    }
    _CHECK(se_2);
}

_TEST_CASE(
    abc::test_case_t({.name = "A computationally expensive test, revisited"})
)
{
    using namespace abc;
    using namespace std;
    auto check_iota_results
        = [](const vector<int>& vect, const pair<int, int>& begin_and_end_ints)
    {
        simulated_and_expr_t simulated_expr;
        simulated_expr.set_left_child(_MATCHER(_EXPR(
            vect.size()
            == std::abs(begin_and_end_ints.second - begin_and_end_ints.first)
        )));
        if (simulated_expr)
        {
            simulated_expr.set_right_child(_MATCHER(_EXPR(
                vect
                == ranges::to<vector<int>>(ranges::iota_view(
                    begin_and_end_ints.first, begin_and_end_ints.second
                ))
            )));
        }
        _CHECK(simulated_expr);
    };
    check_iota_results(
        ranges::to<vector<int>>(ranges::iota_view(1, 10'000)), {1, 9'999}
    );
}

_TEST_CASE(
    abc::test_case_t(
        {.name = "Several assertions created using simulated_logic_expr_t"}
    )
)
{
    using namespace abc;
    // A default constructed simulated_logic_expr_t has no left or right element
    // set.
    simulated_and_expr_t m1;
    // The if statement's body will not be entered.
    if (m1)
    {
        _TERMINATE_WITH_MSG("Never entered");
    }
    // This assertion will fail. All simulated_logic_expr_t entities with no
    // elements set automatically fail.
    _CHECK(m1);

    simulated_or_expr_t m2;
    m2.set_left_child(_MATCHER(_EXPR(1 == 1)));
    m2.set_right_child(_MATCHER(_EXPR(1 == 1)));
    // As both children are set (it doesn't matter if they are true or false)
    // this if statement will not be entered.
    if (m2)
    {
        _TERMINATE_WITH_MSG("Never entered");
    }
    // This assertion will pass. In the results the reader will see that both
    // children are evaluated.
    _CHECK(m2);

    // This resets both elements to std::nullopt.
    m2 = simulated_or_expr_t();

    // We can choose to set either the left or the right child. As long as only
    // one child is set, then the if statement may be entered.
    m2.set_right_child(_MATCHER(_EXPR(1 == 2)));
    if (m2)
    {
        m2.set_left_child(_MATCHER(_EXPR(1 == 1)));
    }
    // This assertion will pass.
    _CHECK(m2);

    simulated_and_expr_t m3;
    // The two set_right_child functions below set the children of m3 using
    // m2 and m1, both simulated_logic_expr_t types.
    //  We can do this with or without the _MATCHER macro. Using it adds
    //  source information to the test result).
    m3.set_left_child(m2);
    if (m3)
    {
        m3.set_right_child(_MATCHER(m1));
    }
    _CHECK(m3);
}

template <typename T, std::size_t N>
inline abc::matcher_t
    arrays_equal(
        const std::array<T, N>& arr_l,
        const std::array<T, N>& arr_r
    )
{
    using namespace abc::matcher;
    using namespace std;
    optional<size_t> diverging_idx_opt;
    for (size_t idx{0}; idx < N; ++idx)
    {
        if (arr_l[idx] != arr_r[idx])
        {
            diverging_idx_opt = idx;
            break;
        }
    }
    if (diverging_idx_opt.has_value())
    {
        size_t diverging_idx{diverging_idx_opt.value()};
        return mk_matcher_using_result(matcher_result_t(
            false,
            fmt::format(
                "{0} != {1}. Arrays diverge at index {2}. Left array at index "
                "{2} = {3}, while right array at index {2} = {4}.",
                arr_l,
                arr_r,
                diverging_idx,
                arr_l[diverging_idx],
                arr_r[diverging_idx]
            )
        ));
    }
    else
    {
        return mk_matcher_using_result(
            matcher_result_t(true, fmt::format("{0} == {1}", arr_l, arr_r))
        );
    }
}

_TEST_CASE(
    abc::test_case_t({.name = "User-defined matcher"})
)
{
    using namespace std;
    _CHECK(arrays_equal(array<int, 3>{1, 2, 3}, array<int, 3>{1, 2, 3}));
    _CHECK(arrays_equal(array<int, 3>{1, 2, 3}, array<int, 3>{1, 2, 4}));
}
