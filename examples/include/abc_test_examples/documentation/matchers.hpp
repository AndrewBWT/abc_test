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
    annotated_matcher_t annotated_matcher_2(
        annotate(matcher_3, "Testing bool_matcher(false)")
    );
    _CHECK(matcher_2);
    _CHECK(matcher_3);
    _CHECK(annotated_matcher_2);
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
    annotated_matcher_t annotated_matcher_3(
        annotate(fmt::format("Checking i <= {}", 7), matcher_5)
    );
    _CHECK(annotated_matcher_3);
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
    abc::test_case_t({ .name = "Simulating logical operators" })
)
{
    using namespace abc;
    simulated_and_expr_t m1(_MATCHER(_EXPR(1 < 2)));
    if (m1)
    {
        matcher_t m2 = _MATCHER(_EXPR(2 == 2));
        m1.set_right_child(_MATCHER(m2));
    }
    _CHECK(m1);
    simulated_or_expr_t se1 = _MATCHER(_EXPR(1 < 2));
    if (se1)
    {
        matcher_t m2 = _MATCHER(_EXPR(2 == 2));
        se1.set_right_child(_MATCHER(m2));
    }
    _CHECK(se1);
    m1 = _MATCHER(_EXPR(1 > 2));
    if (m1)
    {
        m1 = _MATCHER(_EXPR(1 == 2));
    }
    _CHECK(m1);
}