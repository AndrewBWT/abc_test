#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"

_TEST_CASE(
    abc::test_case_t({.name = "Simple data generators"})
)
{
    using namespace abc;
    // This for loop will produce 3 int values; 1, 2 and 3. It does this using a
    // static data generator.
    _BEGIN_MULTI_ELEMENT_BBA(check_numbs, "Checks the data generator's output");
    for (const int& integer : iterate_over<int>({1, 2, 3}))
    {
        // The element produced by the range-based for loop is a cref. All of
        // the for loops in this test case produce int crefs.
        check_numbs += _BLOCK_CHECK(_EXPR(integer <= 2));
    }
    // This for loop will read int values from the file "integer_file". It does
    // this using a file data generator.
    for (auto& integer : read_data_from_file<int>("integer_file"))
    {
        check_numbs += _BLOCK_CHECK(_EXPR(integer <= 2));
    }
    // This for loop will produce random int values. It does this using a random
    // data generator.
    for (auto& integer : generate_data_randomly<int>())
    {
        check_numbs += _BLOCK_CHECK(_EXPR(integer <= 2));
    }
    _END_BBA_CHECK(check_numbs);
}

_TEST_CASE(
    abc::test_case_t({.name = "Simple data generators, part 2"})
)
{
    using namespace abc;
    // This for loop will navigate through the data generators in the order they
    // are given; firstly it will produce 1, 2 and 3, then it will produce
    // random values, finally it will read data from the file "int_data".
    _BEGIN_MULTI_ELEMENT_BBA(check_numbs, "Checks the data generator's output");
    for (auto& integer : data_generator_collection<int>(
             iterate_over<int>({1, 2, 3}),
             generate_data_randomly<int>(),
             read_data_from_file<int>("int_data")
         ))
    {
        check_numbs += _BLOCK_CHECK(_EXPR(integer <= 2));
    }
    // The data_generator_collection_t object created from the for loop below is
    // identical to the one created from the data_generator_collection function
    // above.
    for (auto& integer : iterate_over<int>({1, 2, 3})
                             & generate_data_randomly<int>()
                             & read_data_from_file<int>("int_data"))
    {
        check_numbs += _BLOCK_CHECK(_EXPR(integer <= 2));
    }
    _END_BBA_CHECK(check_numbs);
}

_TEST_CASE(
    abc::test_case_t({.name = "Printers in abc_test"})
)
{
    using namespace abc;
    using namespace std;
    // As we cannot create instantiations of base_printer_t, we will show the
    // user the included subclasses which use it as their base class.

    // The first of these is default_printer_t.
    utility::printer::default_printer_t<int> printer_1;
    _CHECK_EXPR(printer_1.run_printer(123) == "123");

    // We recomend that the user use the function default_printer in the abc
    // namespace.

    // It returns a printer_t type, which is a type synonym for a
    // std::shared_ptr.
    auto printer_2 = utility::printer::default_printer<int>();
    _CHECK_EXPR(printer_2->run_printer(123) == "123");


    // We advice that the user use the print function usually.
    _CHECK_EXPR(print<int>(123, printer_2) == "123");

    // We can elevate printer_1 to a printer_t type using the function
    // mk_printer
    _CHECK_EXPR(print<int>(123, mk_printer(printer_1)) == "123");

    // print has a default parameter for its second argument, allowing the user
    // to write.
    _CHECK_EXPR(print<int>(123) == "123");

    // abc_test includes specializations for more complicated types, such as
    // std::tuple and std::vector.
    using tuple_type = tuple<int, string, vector<bool>>;
    tuple_type tuple_1{
        123, "h3llo"s, vector<bool>{true, false, true}
    };
    _CHECK_EXPR(print(tuple_1) == "(123, \"h3llo\", {true, false, true})");

    // Here is an example using function_printer_t. We first create a function
    // which prints a bool
    auto bool_printer = [](const bool& _a_bool) -> std::string
    {
        return _a_bool ? "1" : "0";
    };
    // We can now use it to create a bool printer...
    abc::utility::printer::function_printer_t<bool, decltype(bool_printer)> fp1(
        bool_printer
    );
    _CHECK_EXPR(fp1.run_printer(true) == "1");

    // There is a print function which takes a function_printer_t argument.
    _CHECK_EXPR(print(true, bool_printer) == "1");

    // Or if we wanted, we could use the function function_printer to elevate
    // it to being a printer_t, and then use the other print function.
    _CHECK_EXPR(print(true, function_printer<bool>(bool_printer)) == "1");

    // Using bool_printer, we can use the constructor for the specialization of
    // std::tuple and std::vector to change how tuple_1 is printed.
    {
        using namespace utility::printer;
        // This type is quite complicated, so we use the above namespace to
        // reduce text.
        _CHECK_EXPR(
            print(
                // Printing tuple_1
                tuple_1,
                // Make a printer using the default type of the tuple.
                default_printer<tuple_type>(
                    // But initialized with our own printers for each of the
                    // tuple's types.
                    default_printer<int>(),
                    default_printer<string>(),
                    // The last type is changed. It uses the default_printer_t
                    // constructor for vector.
                    default_printer<vector<bool>>(
                        // But it uses its own printer_t type for bool, created
                        // using function_printer.
                        function_printer<bool>(bool_printer)
                    )
                )
            )
            == "(123, \"h3llo\", {1, 0, 1})"
        );
        // An easier way to get the same effect is as follows: firstly create a
        // default printer. Note we don't use the default_printer function here,
        // we use the default_printer_t constructor instead.
        auto tuple_printer{default_printer_t<tuple_type>()};
        // Then change the third field.
        std::get<2>(tuple_printer.get_printers_ref()
        ) = default_printer<vector<bool>>(function_printer<bool>(bool_printer));
        // Then run the printer using mk_printer to elevate tuple_printer to a
        // printer_t type.
        _CHECK_EXPR(
            print(tuple_1, mk_printer(tuple_printer))
            == "(123, \"h3llo\", {1, 0, 1})"
        );
    }
}

// This is a user-defined object.
struct S
{
    int int_1;
    int int_2;
};

// This is how we create a default_printer_t instance for S.
template <>
struct abc::utility::printer::default_printer_t<S>
    : public abc::utility::printer::printer_base_t<S>
{
    inline std::string
        run_printer(
            const S& _a_object
        ) const noexcept
    {
        // We use the object object_printer_parser_t and the function
        // object_printer. We give it the default object_printer_parser_t
        // object, a string which is prepended to the beginning of the list of
        // data fields, the names of the data fields, and then the fields
        // themselves
        return object_printer_with_field_names(
            object_printer_parser_t{},
            "S",
            {"int_1", "int_2"},
            _a_object.int_1,
            _a_object.int_2
        );
    }
};

// This is a bespoke printer_base_t instance for int. It prints the data out in
// hex.
struct int_printer_t : abc::utility::printer::printer_base_t<int>
{
    inline std::string
        run_printer(
            const int& _a_object
        ) const noexcept
    {
        return fmt::format("{:x}", _a_object);
    }
};

// This is a bespoke printer for S.
struct s_printer : public abc::utility::printer::printer_base_t<S>
{
    abc::utility::printer::printer_t<int> m_int_printer
        = abc::utility::printer::default_printer<int>();
    s_printer() = default;

    inline s_printer(
        const abc::utility::printer::printer_t<int>& int_printer
    )
        : m_int_printer(int_printer)
    {}

    inline std::string
        run_printer(
            const S& _a_object
        ) const noexcept
    {
        // This printer uses the function object_printer_with_custom_printers
        // with a bespoke object_printer_parser_t object.
        // object_printer_with_custom_printers takes a tuple of bespoke printers
        // for each field it is provided.
        return object_printer_with_custom_printers(
            abc::utility::object_printer_parser_t{
                .begin_char                             = '(',
                .end_char                               = ')',
            },
            "S",
            std::make_tuple(m_int_printer, m_int_printer),
            _a_object.int_1,
            _a_object.int_2
        );
    }
};

_TEST_CASE(
    abc::test_case_t({.name = "Custom printer"})
)
{
    using namespace abc;
    S s1{10'000, 20'000};
    // This will call the default printer on S.
    _CHECK_EXPR(print(s1) == "S {int_1 = 10000, int_2 = 20000}");

    // This printer uses the bespoke printer s_printer.
    _CHECK_EXPR(print(s1, mk_printer(s_printer())) == "S (10000, 20000)");

    // This also uses the bespoke printer s_printer, but we use int_printer_t to
    // print S's integers.
    _CHECK_EXPR(
        print(s1, mk_printer(s_printer(mk_printer(int_printer_t()))))
        == "S (2710, 4e20)"
    );
}

_TEST_CASE(
    abc::test_case_t({.name = "Printer object"})
)
{
    using namespace abc;
    using namespace std;
    // int_printer_1 is created using a bespoke function to print int objects.
    auto int_printer_func = [](const int& i)
    {
        const size_t buf_size = 10;
        char         buf[buf_size]{};
        to_chars(buf, buf + buf_size, i);
        return string(buf);
    };
    _CHECK_EXPR(abc::print(123, int_printer_func) == "123");
    _CHECK_EXPR(abc::print(123) == "123");

    // s_printer_1 is created using a bespoke function to print S objects.
    auto s_printer_func = [](const S& s)
    {
        return fmt::format("S {{{0}, {1}}}", s.int_1, s.int_2);
    };
    _CHECK_EXPR(abc::print(S{1, 2}, s_printer_func) == "S {1, 2}");
    _CHECK_EXPR(abc::print(S{1, 2}) == "S {int_1 = 1, int_2 = 2}");
}

constexpr bool
    operator==(
        const S& lhs,
        const S& rhs
    )
{
    return std::make_pair(lhs.int_1, lhs.int_2)
           == std::make_pair(rhs.int_1, rhs.int_2);
}

_BEGIN_ABC_UTILITY_PARSER_NS

template <>
struct default_parser_t<S> : public parser_base_t<S>
{
    __constexpr virtual parse_result_t<S>
        run_parser(
            parser_input_t& _a_parse_input
        ) const
    {
        return object_parser<S, int, int>(
            {"int_1", "int_2"}, "S", '{', '}', _a_parse_input
            // std::make_tuple(parser_t<int>(), parser_t<int>())
        );
    }
};

_END_ABC_UTILITY_PARSER_NS

_BEGIN_ABC_UTILITY_PRINTER_NS
/*template <>
struct default_printer_t<S> : public printer_base_t<S>
{
    __constexpr virtual std::string
        run_printer(
            const S& _a_parse_input
        ) const
    {
        return object_printer(
            { "int_1", "int_2" },
            "S",
            '{',
            '}',
            _a_parse_input.int_1, _a_parse_input.int_2
            // std::make_tuple(parser_t<int>(), parser_t<int>())
        );
    }
};*/
_END_ABC_UTILITY_PRINTER_NS

/*template <>
struct scn::scanner<S>
{
    template <typename ParseContext>
    auto
        parse(
            ParseContext& pctx
        ) -> scan_expected<typename ParseContext::iterator>
    {
        return pctx.begin();
    }

    template <typename Context>
    auto
        scan(
            S&       val,
            Context& ctx
        ) -> scan_expected<typename Context::iterator>
    {
        auto result = scn::scan<std::pair<int, int>>(ctx.range(), "{}");
        if (! result)
        {
            return unexpected(result.error());
        }
        else
        {
            val.int_1 = result->value().first;
            val.int_2 = result->value().second;
            return result->begin();
        }
    }
};*/

_TEST_CASE(
    abc::test_case_t({.name = "Parser object"})
)
{
    using namespace abc;
    using namespace std;
    // int_printer_1 is created using a bespoke function to print int objects.
    auto int_parser_func = [](abc::utility::parser::parser_input_t& str)
    {
        int               result{};
        const string_view _l_str{str.sv()};
        auto [ptr, ec]
            = from_chars(_l_str.data(), _l_str.data() + _l_str.size(), result);

        if (ec == std::errc())
        {
            str.advance(ptr - _l_str.data());
            return expected<int, string>(result);
        }
        else
        {
            return expected<int, string>(
                unexpected(fmt::format("Could not parse \"{0}\"", _l_str))

            );
        }
    };
    // int_printer_2 is created using int's fmt::format definition.
    abc::utility::parser::parser_t<int> int_parser_2
        = abc::utility::parser::default_parser<int>();

    _CHECK_EXPR(parse<int>("123", int_parser_func) == 123);
    _CHECK_EXPR(parse("123", int_parser_2) == 123);

    // s_printer_1 is created using a bespoke function to print S objects.
    auto s_parser_func = [](abc::utility::parser::parser_input_t& _a_str)
    {
        auto res = scn::scan<int, int>(_a_str.sv(), "S {{{0}, {1}}}");
        if (res.has_value())
        {
            auto [arg1, arg2] = res->values();
            _a_str.advance(_a_str.sv().size());
            return expected<S, string>(S{arg1, arg2});
        }
        else
        {
            return expected<S, string>(
                unexpected(fmt::format("Could not parse \"{0}\"", _a_str.sv()))
            );
        }
    };
    _CHECK_EXPR(parse<S>("S {1, 2}", s_parser_func) == (S{1, 2}));
    _CHECK_EXPR(parse<S>("S {int_1 = 1, int_2 = 2}") == (S{1, 2}));
}

_TEST_CASE(
    abc::test_case_t({.name = "Strings and str_t's"})
)
{
    using namespace abc;
    using namespace std;
    using namespace abc::utility;
    // Below we show how the default printer and parser work.
    _CHECK_EXPR(abc::print(make_tuple("123"s, "456"s)) == "(\"123\", \"456\")");
    // abc::utility::printer::printer_t<std::string> ti;
    // abc::utility::printer::printer_t<std::tuple<std::string>> ti2(ti);
    _CHECK_EXPR(abc::print("123") == "\"123\"");
    _CHECK_EXPR(parse<string>("\"123\"") == "123");
    _CHECK_EXPR(
        (parse<pair<string, string>>("(\"123\", \"456\")"))
        == make_pair("123"s, "456"s)
    );
    // This printer/parser pair is not able to parse the argument correctly.
    // This is because the parser will terminate either at the first whitespace
    // character, or at the end of the string. The printed pair becomes "{123,
    // 456}". It is parsed as {"{123," "456}"}.
    pair<int, int> arg_1 = make_pair(123, 456);
    _CHECK_EXPR((parse<pair<int, int>>(abc::print(arg_1))) == arg_1);
    const auto arg_2 = make_pair("\"123\""s, "\"456\""s);
    // The user could surround the strings by quotes, and assume that the parser
    // would recognise them as escape characters. However this requires
    // scn::scan to be formatted correctly, which isn't how the default parser
    // is defined. So the following won't work.
    _CHECK_EXPR((parse<pair<string, string>>(abc::print(arg_2))) != arg_2);
}

// Here is another entity called T.
struct T
{
    int                                     m1;
    std::string                             m2;
    std::vector<std::string>                m3;
    std::tuple<int, std::string, char, int> m4;
};

constexpr bool
    operator==(
        const T& lhs,
        const T& rhs
    )
{
    return std::make_tuple(lhs.m1, lhs.m2, lhs.m3, lhs.m4)
           == std::make_tuple(rhs.m1, rhs.m2, rhs.m3, rhs.m4);
}

_TEST_CASE(
    abc::test_case_t({.name = "Strings and str_t's"})
)
{
    using namespace abc;
    using namespace std;
    using namespace abc::utility;
    T object_1 = T{
        500, "501"s, {"502"s, "503"s, "504"s},
          {505, "506"s, 'c', 508}
    };
    // This pair of printer/parser is able to parse the argument correctly.
    // _CHECK_EXPR(
    //    default_parser<T>().run_parser_with_exception(
    //        default_printer<T>().run_printer(object_1)
    //    )
    //   == object_1
    // );
}

_TEST_CASE(
    abc::test_case_t({.name = "Static generator examples"})
)
{
    using namespace abc;
    using namespace std;
    auto fib_func = [](const size_t max_index)
    {
        vector<uint64_t> rv;
        for (size_t idx{0}; idx <= max_index; ++idx)
        {
            if (idx == 0)
            {
                rv.push_back(0);
            }
            else if (idx == 1)
            {
                rv.push_back(1);
            }
            else
            {
                auto itt_end{rv.end() - 1};
                rv.push_back(*itt_end + *(itt_end - 1));
            }
        }
        return rv;
    };
    optional<uint64_t> last;
    _BEGIN_MULTI_ELEMENT_BBA(
        fibonacci, "Test for overflow in uint64_t-based fibonacci function"
    );
    // This data generator stores the first 100 fibonacci numbers. The test
    // checks for overflow.
    for (auto& integer : iterate_over<uint64_t>(fib_func(100)))
    {
        if (last.has_value())
        {
            fibonacci += _BLOCK_CHECK(_EXPR(last.value() <= integer));
        }
        last = integer;
    }
    _END_BBA_CHECK(fibonacci);

    _BEGIN_MULTI_ELEMENT_BBA(to_string_check, "Test std::to_string function");
    // This data generator should probably not be encoded using a static data
    // generator. If the user wants to add more test data, then they will need
    // to re-compile the executable.
    using test_data_1 = pair<int, string>;
    for (auto& [integer, str] : iterate_over<test_data_1>({
             {1,   "1"  },
             {2,   "2"  },
             {100, "102"}
    }))
    {
        to_string_check += _BLOCK_CHECK(_EXPR(std::to_string(integer) == str));
    }
    _END_BBA_CHECK(to_string_check);
}

_TEST_CASE(
    abc::test_case_t(
        {.name = "General data examples", .path = "abc::general_data"}
    )
)
{
    // Assume the root path is set at "root_path".
    // Assume the default general data file extension "gd" has been used
    using namespace std::filesystem;
    using namespace abc;
    using namespace std;
    // s1 = "root_path/abc/general_data/general\ data\ examples/hello.gd
    string s1 = gdf("hello").path().string();
    // s2 = "root_path/abc/special_folder/hello2.gd.
    string s2 = gdf("hello", "abc/special_folder").path().string();
    // s3 = "special_root/hello/three/hello3.gdf
    string s3
        = gdf_from_path("special_root/hello/three/hello3.gdf").path().string();
}