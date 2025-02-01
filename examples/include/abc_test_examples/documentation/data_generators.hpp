#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"
#if 0
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
    // The printer functionality is contained in the abc::utility::printer
    // namespace.
    using namespace abc::utility::printer;
    // As we cannot create instantiations of base_printer_t, we will show the
    // user the included subclasses which use it as their base class.

    // The first of these is default_printer_t.
    default_printer_t<int> printer_1;
    _CHECK_EXPR(printer_1.run_printer(123) == "123");

    // We recommend that the user use the function print when printing. It takes
    // two arguments; the first is the object to be printed, and the second is
    // the printer_t object. Any derived type from base_printer_t can be
    // elevated to this type using mk_printer.

    _CHECK_EXPR(print(123, mk_printer(printer_1)) == "123");

    // We can also use the specialised default_printer function, which will
    // create and wrap a default_printer_t in a printer_t object.
    _CHECK_EXPR(print(123, default_printer<int>()) == "123");

    // print has a default parameter for its second argument, which creates a
    // default_printer_t object. Therefore, to get the default behaviour, no
    // argument needs to be provided.
    _CHECK_EXPR(print<int>(123) == "123");

    // abc_test includes default_printer_t specializations for more complicated
    // types, such as std::tuple and std::vector.
    using tuple_type = tuple<int, string, vector<bool>>;
    tuple_type tuple_1{
        123, "h3llo"s, vector<bool>{true, false, true}
    };
    _CHECK_EXPR(print(tuple_1) == "(123, \"h3llo\", {true, false, true})");

    // A function_printer_t object allows the user to create a base_printer_t
    // object which can take any callable object which takes a const T& and
    // returns an std::string.

    // Here is a function which prints a bool.
    auto bool_printer = [](const bool& object) -> std::string
    {
        return object ? "1" : "0";
    };
    // We can now use it to create a function_printer_t object.
    function_printer_t<bool, decltype(bool_printer)> fp1(bool_printer);
    _CHECK_EXPR(print(true, mk_printer(fp1)) == "1");

    // We can use the auxillery function function_printer, which removes some of
    // the boilerplate code required.
    _CHECK_EXPR(print(true, function_printer<bool>(bool_printer)) == "1");

    // Using bool_printer, we can use the specialized default_printer_t
    // constructors for the types std::tuple and std::vector to
    // change how tuple_1 is printed.
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
                // The last type is changed. It uses the specialized
                // default_printer_t constructor for vector.
                default_printer<vector<bool>>(
                    // But it uses its own printer_t type for bool, created
                    // using function_printer.
                    function_printer<bool>(bool_printer)
                )
            )
        )
        == "(123, \"h3llo\", {1, 0, 1})"
    );
    // An easier way to get the same effect is as follows: firstly
    // create a default printer for tuple_type.
    default_printer_t<tuple_type> tuple_printer;
    // Then change the third field.
    std::get<2>(tuple_printer.get_printers_ref())
        = default_printer<vector<bool>>(function_printer<bool>(bool_printer));
    // Then run the printer using mk_printer to elevate tuple_printer to
    // a printer_t type.

    _CHECK_EXPR(
        print(tuple_1, mk_printer(tuple_printer))
        == "(123, \"h3llo\", {1, 0, 1})"
    );
}
#endif
// This is a user-defined object.
struct S
{
    int int_1;
    int int_2;
};
#if 0
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
#endif

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
#if 0

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
                .begin_char = '(',
                .end_char   = ')',
            },
            "S",
            std::make_tuple(m_int_printer, m_int_printer),
            _a_object.int_1,
            _a_object.int_2
        );
    }
};

_TEST_CASE(
    abc::test_case_t({.name = "Custom printers"})
)
{
    using namespace abc;
    using namespace abc::utility::printer;
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

constexpr bool
    operator==(
        const S& lhs,
        const S& rhs
    )
{
    return std::make_pair(lhs.int_1, lhs.int_2)
           == std::make_pair(rhs.int_1, rhs.int_2);
}

_TEST_CASE(
    abc::test_case_t({.name = "Parsers in abc_test"})
)
{
    using namespace abc;
    using namespace std;
    // The parser functionality is contained in the abc::utility::parser
    // namespace.
    using namespace abc::utility::parser;
    // As we cannot create instantiations of base_parser_t, we will show the
    // user the included subclasses which use it as their base class.

    // The first of these is default_printer_t.
    default_parser_t<int> parser_1;
    // We can elevate it to the parser_t type using mk_parser.
    _CHECK_EXPR(parse("123", mk_parser(parser_1)) == parse_result_t<int>(123));
    // For the rest of the assertions in this test case, we will use the
    // function parse_with_exception. This removes the need for the
    // parse_result_t type.
    _CHECK_EXPR(parse_with_exception<int>("123", mk_parser(parser_1)) == 123);

    // We can also use the default_parser function instead of using mk_parser.
    _CHECK_EXPR(parse_with_exception<int>("123", default_parser<int>()) == 123);

    // As parse_with_exception (and parse) have their second argument defaulted
    // to a default parser, we can just write.
    _CHECK_EXPR(parse_with_exception<int>("123") == 123);


    // abc_test includes default_parser_t specializations for more complicated
    // types, such as std::tuple and std::vector.
    using tuple_type = tuple<int, string, vector<bool>>;
    tuple_type tuple_1{
        123, "h3llo"s, vector<bool>{true, false, true}
    };
    _CHECK_EXPR(
        parse_with_exception<tuple_type>("(123, \"h3llo\", {true, false, true})"
        )
        == tuple_1
    );

    // A function_parser_t object allows the user to create a base_parser_t
    // object which can take any callable object which takes a parser_input_t&
    // and returns a parse_result<T>.

    // Here is a function which parses a bool represented as a 0 or 1.
    auto bool_parser = [](parser_input_t& input) -> parse_result_t<bool>
    {
        if (*input == '1')
        {
            input.advance(1);
            return parse_result_t<bool>(true);
        }
        else if (*input == '0')
        {
            input.advance(1);
            return parse_result_t<bool>(false);
        }
        else
        {
            return parse_error<bool>(
                "Could not recognise character. Should be 0 or 1"
            );
        }
    };
    // We can now use it to create a function_parser_t object.
    function_parser_t<bool, decltype(bool_parser)> fp1(bool_parser);
    _CHECK_EXPR(parse_with_exception<bool>("1", mk_parser(fp1)) == true);

    // We can use the auxillery function function_parser, which removes some of
    // the boilerplate code required.
    _CHECK_EXPR(
        parse_with_exception<bool>("1", function_parser<bool>(bool_parser))
        == true
    );

    // Using bool_parser, we can use the specialized default_parser_t
    // constructors for the types std::tuple and std::vector to
    // change how tuple_1 is printed.
    _CHECK_EXPR(
        parse_with_exception(
            // Printing the following string
            "(123, \"h3llo\", {1, 0, 1})",
            // Make a parser using the default type of the tuple.
            default_parser<tuple_type>(
                // But initialized with our own parsers for each of the
                // tuple's types.
                default_parser<int>(),
                default_parser<string>(),
                // The last type is changed. It uses the specialized
                // default_parser_t constructor for vector.
                default_parser<vector<bool>>(
                    // But it uses its own parser_t type for bool, created
                    // using function_parser.
                    function_parser<bool>(bool_parser)
                )
            )
        )
        == tuple_1
    );
    // An easier way to get the same effect is as follows: firstly
    // create a default printer for tuple_type.
    default_parser_t<tuple_type> tuple_parser;
    // Then change the third field.
    std::get<2>(tuple_parser.get_parsers_ref())
        = default_parser<vector<bool>>(function_parser<bool>(bool_parser));
    // Then run the printer using mk_printer to elevate tuple_printer to
    // a printer_t type.

    _CHECK_EXPR(
        parse_with_exception(
            "(123, \"h3llo\", {1, 0, 1})", mk_parser(tuple_parser)
        )
        == tuple_1
    );
}

// This is how we create a default_printer_t instance for S.
template <>
struct abc::utility::parser::default_parser_t<S>
    : public abc::utility::parser::parser_base_t<S>
{
    inline parse_result_t<S>
        run_parser(
            parser_input_t& _a_input
        ) const noexcept
    {
        // We use the object object_printer_parser_t and the function
        // object_printer. We give it the default object_printer_parser_t
        // object, a string which is prepended to the beginning of the list of
        // data fields, the names of the data fields, and then the fields
        // themselves
        return abc::utility::parser::
            object_parser_with_field_names<S, int, int>(
                object_printer_parser_t{}, "S", {"int_1", "int_2"}, _a_input
            );
    }
};
#endif
// This is a bespoke printer_base_t instance for int. It prints the data out in
// hex.
struct int_parser_t : abc::utility::parser::parser_base_t<int>
{
    inline abc::utility::parser::parse_result_t<int>
        run_parser(
            abc::utility::parser::parser_input_t& _a_input
        ) const noexcept
    {
        using namespace std;
        using namespace abc::utility::parser;
        string_view sv{_a_input.sv()};
        size_t      pos{sv.find_first_not_of("0123456789abcdef")};
        if (pos == string::npos)
        {
            return parse_error<int>("Could not parse");
        }
        else
        {
            string sv2{sv.substr(0, pos)};
            int    n = stoul(sv2.c_str(), nullptr, 16);
            _a_input.advance(pos);
            return parse_result_t<int>(n);
        }
    }
};
#if 0
// This is a bespoke printer for S.
struct s_parser : public abc::utility::parser::parser_base_t<S>
{
    abc::utility::parser::parser_t<int> m_int_parser
        = abc::utility::parser::default_parser<int>();
    s_parser() = default;

    inline s_parser(
        const abc::utility::parser::parser_t<int>& int_parser
    )
        : m_int_parser(int_parser)
    {}

    inline abc::utility::parser::parse_result_t<S>
        run_parser(
            abc::utility::parser::parser_input_t& _a_input
        ) const
    {
        // This printer uses the function object_printer_with_custom_printers
        // with a bespoke object_printer_parser_t object.
        // object_printer_with_custom_printers takes a tuple of bespoke printers
        // for each field it is provided.
        return abc::utility::parser::
            object_printer_with_custom_parsers<S, int, int>(
                abc::utility::object_printer_parser_t{
                    .begin_char = '(',
                    .end_char   = ')',
                },
                "S",
                std::make_tuple(m_int_parser, m_int_parser),
                _a_input
            );
    }
};
_TEST_CASE(
    abc::test_case_t({.name = "Custom parsers"})
)
{
    using namespace abc;
    using namespace abc::utility::parser;
    S s1{10'000, 20'000};
    // This will call the default printer on S.
    _CHECK_EXPR(
        parse_with_exception<S>("S {int_1 = 10000, int_2 = 20000}") == s1
    );

    // This printer uses the bespoke printer s_printer.
    _CHECK_EXPR(
        parse_with_exception<S>("S (10000, 20000)", mk_parser(s_parser())) == s1
    );

    // This also uses the bespoke printer s_printer, but we use int_printer_t to
    // print S's integers.
    _CHECK_EXPR(
        parse_with_exception<S>(
            "S (2710, 4e20)", mk_parser(s_parser(mk_parser(int_parser_t())))
        )
        == s1
    );
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
#endif

_TEST_CASE(
    abc::test_case_t({.name = "File data generator examples"})
)
{
    using namespace abc;
    using namespace std;
    _BEGIN_MULTI_ELEMENT_BBA(
        to_string_check, "Testing std::to_string function"
    );
    // The type we will use in this test case.
    using test_type_1 = pair<int, string>;
    // The file "integer_file" contains the following data:
    // (1, "1")
    // (2, "2")
    // (100, "102")
    for (auto& [integer, str] :
         read_data_from_file<test_type_1>("integer_file"))
    {
        to_string_check += _BLOCK_CHECK(_EXPR(to_string(integer) == str));
    }
    // This is another way of accessing "integer_file".
    for (auto& [integer, str] :
         read_data_from_file<test_type_1>(gdf("integer_file")))
    {
        to_string_check += _BLOCK_CHECK(_EXPR(to_string(integer) == str));
    }
    // This accesses a differnet file, in special_folder. special_folder must
    // exist before this data generator is ran. Otherwise, the test framework
    // will throw an exception and terminate.
    // The file, if it doesn't already exist, is initialised using the data
    // shown.
    for (auto& [integer, str] : read_data_from_file<test_type_1>(
             gdf("special_folder", "integer_file"),
             {
                 {10,  "10" },
                 {20,  "20" },
                 {300, "301"}
    }
         ))
    {
        to_string_check += _BLOCK_CHECK(_EXPR(to_string(integer) == str));
    }
    // Another example. This one has an exact file where the data is located.
    for (auto& [integer, str] : read_data_from_file<test_type_1>(
             gdf_from_path(
                 "../../examples/data/test_data/unique/path/integer_file.gd"
             ),
             {
                 {1'000,  "1000" },
                 {2'000,  "2000" },
                 {30'000, "30100"}
    }
         ))
    {
        to_string_check += _BLOCK_CHECK(_EXPR(to_string(integer) == str));
    }
    // This one uses a custom printer and parser, which reads the int data in
    // hex.
    for (auto& [integer, str] :
         read_data_from_file<test_type_1>(general_data_file(
             gdf("integer_file"),
             utility::str::rw_info_t<test_type_1>{
                 .internal_parser
                 = utility::parser::default_parser<test_type_1>(
                     utility::parser::mk_parser(int_parser_t()),
                     utility::parser::default_parser<string>()
                 ),
                 .internal_printer
                 = utility::printer::default_printer<test_type_1>(
                     utility::printer::mk_printer(int_printer_t()),
                     utility::printer::default_printer<string>()
                 )
             }
         )))
    {
        to_string_check += _BLOCK_CHECK(_EXPR(to_string(integer) == str));
    }
    _END_BBA_CHECK(to_string_check);
}

struct int_generator_t : public abc::data_gen::random_generator_base_t<int>
{
    int_generator_t(
        int multiplier = 10
    )
        : m_multiplier(multiplier)
    {}

    inline int
        operator()(
            abc::utility::rng& _a_rnd_generator,
            const std::size_t  _a_index
        )
    {
        using namespace std;
        if (_a_index == 0)
        {
            return 0;
        }
        size_t limit{_a_index * m_multiplier};
        return _a_rnd_generator() % limit;
    }
private:
    int m_multiplier;
};

template <>
struct abc::data_gen::default_random_generator_t<S>
    : public abc::data_gen::random_generator_base_t<S>
{
    inline default_random_generator_t(
        abc::data_gen::random_generator_t<int> int_generator
    )
        : m_int_generator(int_generator)
    {}

    inline S
        operator()(
            abc::utility::rng& _a_rnd_generator,
            const std::size_t  _a_index
        )
    {
        using namespace std;
        return make_random_object<S>(
            _a_rnd_generator,
            _a_index,
            make_tuple(m_int_generator, m_int_generator)
        );
    }

    abc::data_gen::random_generator_t<int> m_int_generator
        = default_random_generator<int>();
};

_TEST_CASE(
    abc::test_case_t({ .name = "Enumeration data generator examples" })
)
{
    using namespace abc;
    using namespace std;
    _BEGIN_MULTI_ELEMENT_BBA(
        character_tester, "Testing character values"
    );
    // This random generator uses the default_random_generator_t instance for
    // int.
  //  for (auto character : enumerate_data<char>())
  //  {
  //      character_tester
  //          += _BLOCK_CHECK(_EXPR(character != 'd'));
 //   }
    for (const std::vector<char>& character : enumerate_data<std::vector<char>>(
        from_min_to_val<std::vector<char>>({'a'})))
    {
        character_tester
            += _BLOCK_CHECK(_EXPR(character != std::vector<char>()));
    }
    _END_BBA_CHECK(character_tester);
}

_TEST_CASE(
    abc::test_case_t({.name = "Random data generator examples"})
)
{
    using namespace abc;
    using namespace std;
    _BEGIN_MULTI_ELEMENT_BBA(
        check_integer_overflow, "Testing integers do not overflow"
    );
    // This random generator uses the default_random_generator_t instance for
    // int.
    for (auto integer : generate_data_randomly<int>())
    {
        check_integer_overflow
            += _BLOCK_CHECK(_EXPR(integer + integer > integer));
    }
    // There are two other overloads of generate_data_randomly. The first of
    // these takes an unbounded list of files to write failing data to. As with
    // the majority of data generators which use file data, only the first file
    // will have data read from it. All files in the list will have failing data
    // written to it.
    // This data file only has a single tertiary data file which failing data is
    // read and written to.
    for (auto integer : generate_data_randomly<int>(tdf("integer_file")))
    {
        check_integer_overflow
            += _BLOCK_CHECK(_EXPR(integer + integer > integer));
    }
    // This one reads data from a different tertiay data file. Failing test
    // cases are also written to gdf_1 and special_folder/gdf_2.
    for (auto integer : generate_data_randomly<int>(
             tdf("integer_file2"), gdf("gdf_1"), gdf("special_folder", "gdf_2")
         ))
    {
        check_integer_overflow
            += _BLOCK_CHECK(_EXPR(integer + integer > integer));
    }
    // This random data generator has an upper and lower bound between 500 and
    // 1000.
    for (auto integer :
         generate_data_randomly<int>(default_random_generator<int>(500, 1'000)))
    {
        check_integer_overflow
            += _BLOCK_CHECK(_EXPR(integer + integer > integer));
    }
    // This does the same as above, but writes the failing output to the
    // specified files.
    for (auto integer : generate_data_randomly<int>(
             default_random_generator<int>(500, 1'000),
             tdf("integer_file3"),
             gdf("gdf_1"),
             gdf("special_folder", "gdf_2")
         ))
    {
        check_integer_overflow
            += _BLOCK_CHECK(_EXPR(integer + integer > integer));
    }
    for (auto integer :
        generate_data_randomly<int>(mk_random_generator(int_generator_t(100))))
    {
    }
    _END_BBA_CHECK(check_integer_overflow);

}