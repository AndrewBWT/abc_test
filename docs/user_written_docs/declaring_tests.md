# Declaring Tests
This page contains documentation concerning the declaration of tests in `abc_test`. It covers the following topics:

- The basic ways to declare tests.
- The `test_case_t` object, used to store user-defined data about tests.
- An overview of how the test declaration macros work.
- Alternate ways of declaring tests.

All of the functionality shown in this document only requires the following include directive.

```cpp
#include "abc_test/core.h"
```

# Basic ways to declare tests

Tests in `abc_test` are free functions with a type signature of `void function();`. The user can declare a test with the macro `_TEST_CASE`. This macro also registers the test with the global list of tests. We will delve into greater detail regarding what this means [in a later section](#global-list-of-tests).

Abstractly, `_TEST_CASE` can be said to represent a function in the form

```cpp
void _TEST_CASE(const abc::test_case_t&)
```

The `test_case_t` argument contains user-defined information about the test.

After using the macro and setting its argument, the user then writes their test code. Below we show an example - the test code goes where we have written `//Testing code`.

```cpp
_TEST_CASE(
    abc::test_case_t({.name = "A simple test", .path = "simple_test_path"})
)
{
    // Testing code.
}
```
We will provide more detail regarding `test_case_t` in [the next section](#the-test_case_t-object). However for now, the reader should be aware that it is an aggregate class - that is to say, it has no constructors, and only public member variables. Therefore, the only way to initialise it is using either an initializer list or a designated initializer.

The reader may notice that in the example above, there are an extra pair of brackets around the designated initializer used to construct the `test_case_t` entity. These extra brackets are needed due to how the pre-processor works. Without them, the pre-processor would assume the commas in the designated initializer should be used to separate macro arguments. In turn, the macro would be expanded incorrectly, and a compiler error would be raised.

As a general rule, if the user declares a `test_case_t` entity using an aggregate initializer, contained within a macro, they should surround it with an extra set of brackets to ensure the macro won't be expanded incorrectly.

As an example, the following test declaration will not compile.
```cpp
_TEST_CASE(
    abc::test_case_t
        {.name             = "A simple test",
         .path             = "simple_test"}) 
{}
```

The `TEST_CASE` macro works by creating a function with a randomly generated name, which we will designate `f`. The user has no control over the name of `f`. However, we do include another macro, `_NAMED_TEST_CASE`, which allows the user to name the function being created. An example of its use is shown below, where the created function will be named `nameless_function`.

```cpp
_NAMED_TEST_CASE(
    nameless_function,
    abc::test_case_t({.name = "A named test"})
)
...
```

# The test_case_t Object

The `test_case_t` object is used to store user-defined data about a test. `abc_test` has been designed in such a way that, every time a new test is declared, the user should fill out the member variables of the test's `test_case_t` object, providing information such as a name and description for the test.

`test_case_t` only has public member variables, and no constructors. It has been designed in this manner so that aggregate initialization can be used to create `test_case_t` objects.

Below we show how the public member variables in `test_case_t` are defined. Note that this is the order they are given in the class, and therefore the order the user must use when initializing `test_case_t` elements.

```cpp
string_view           name             = "";
optional<string_view> description      = nullopt;
string_view           path             = "";
size_t                threads_required = 1;
```

Below we give a brief description of each member variable, and what it is used for in `abc_test`.

- `name`
    - This is the name of the test. It should provide some indication as to what the purpose of the test is.
    - A test's `name` forms part of a test's unique identifier (see [here](#tests-unique-identifier) for more information).
    - The `abc` test framework uses and creates folders and files based off a test's `name` and `path`. These files store test-specific data. It is possible that the `abc` test framework may try to create files or folders using the test's `name` as the file/folder's name. **Currently no checks are performed on a test's `name` to ensure that it can be used as a file or folder name. However the user should be aware of this functionality, as an incorrectly chosen `name` could result in unexpected errors when the `abc` test framework is running**.
- `description`
    - This is the user-supplied description of the test. It is not used in any other capacity other than to store information for the user.
- `path`
    - A test's `path` is primarily used to group together tests, in a similar way to how folders can be seen as a way of grouping together files. See [here](#tests-path) for more information about a test's `path`.
    - While the user supplies a `string_view` to represent the test's `path`, the actual run-time representation of a test's `path` is a collection of `string`s. This collection is created by splitting the user-supplied `string_view` using a delimiter.
        - For example, if the user-supplied `string_view` is `"aa::bb::cc"`, and the delimiter used is `"::"`, then the run-time representation of the
        `path` is `{"aa","bb","cc"}`.
        - The delimiter used is the global `base_test_options_t`'s member variable `path_delimiter`. It has a default value of `"::"`, but can be changed by the user.
    - A test's `path` forms part of a test's unique identifier (see [here](#tests-unique-identifier) for more information).
    - The `abc` test framework uses and creates folders and files based off a test's `name` and `path`. These files store test-specific data. It is possible that the `abc` test framework may try to create folders using the test's `path` as the folder's name. **Currently no checks are performed on a test's `path` to ensure that it can be used as a folder name. However the user should be aware of this functionality, as an incorrectly chosen `path` could result in unexpected errors when the `abc` test framework is running**.
- `threads_required`
    - This represents how many threads the test requires when it is ran. The `abc` test framework will ensure this many threads are allocated to the test at run-time. See [this page]((#/docs/user_written_docs/threading)) for more information regarding multi-threaded tests in `abc_test`.

Here is an example of a test being declared using all of the member variables in the `test_case_t` object.


```cpp
_TEST_CASE(
    abc::test_case_t(
        {.name        = "A simple test",
         .description = "A brilliant description, describing wonderful things",
         .path        = "simple_test::simplest_test::so_simple",
         .threads_required = 4}
    )
)
{
    // Testing code.
}
```

## test_case_t Requirements

When the `abc` test framework processes a `test_case_t` component, the framework checks the `test_case_t`'s variables to ensure they do not contain any prohibited values. 

Some of these checks may be performed at compile time, if it is possible to do so. These are as follows:

- The `name` variable is checked to ensure it does not equal `""` - empty `name`s are **not** allowed.
- The `threads_required` variable is checked to ensure it is `> 0` - it cannot equal `0`.

When using the macros  `_TEST_CASE` or `_NAMED_TEST_CASE` to declare tests, the above checks are guaranteed to be performed at compile time. If any of them fail, then a compilation error is raised.

The following checks are always performed at runtime by the `abc` test framework when a test-suite is being processed:
- A `test_case_t`'s `threads_required` variable cannot be greater than the number of threads allocated to the `abc` test framework.
- When a `test_case_t`'s `path` is broken up into a collection of `string`s, no `string` in the collection can equal `""`.
    - But the collection can be empty.
- Each test's unique identifier must be unique among all the tests that are going to be ran. See [here](#tests-unique-identifier) for more information about unique identifiers.

In [the next section](#how-the-macros-work) we describe how the test macros work. The compile-time checks are performed when the function `checked_user_defined_test_data_t::test_data` is useed on the `test_case_t` object provided by the user. 

If the user is creating tests at run-time and these checks fail, then `checked_user_defined_test_data_t::test_data` will throw an `abc::test_library_exception_t` exception.

In [this document](#/docs/user_written_docs/overview_of_framework) we explain how the `abc` test framework works in detail. The document also explains at what point in the `abc` test framework the runtime checks occour, why the checks are performed, and what happens if they fail.
## Test's Path
In this section we have introduced the notion of a test's `path`. We have discussed how the user provides a `string_view`, and the runtime system creates the actual `path`, a collection of `string`s, by splitting the input `string_view` using a delimiter. In this subsection we provide an exact definition as to what a `path` is, and show how groups of tests can be filtered using `path`s.

A test's `path` is a collection of `string`s. The collection can be empty, however each individual element cannot be an empty `string`. For a `path` $p$, we use the notation $size(p)$ to get the size of the collection of `string`s.

We say that `path` $a$ *matches* `path` $b$ iff the first $size(b)$ elements of `path` $a$ are identical to the elements in `path` $b$ - essentially that the initial elements in $a$ match $b$. From this definition, the reader should see that, for two arbitrary `path`s $p$ and $q$, if $p$ matches $q$ it does not mean that $q$ matches $p$.

Using this notion of matching, we can filter groups of tests using a single `path`. As an example, consider a set of tests with the paths `{{"a","b","c"},{},{"a","b"},{"a","b","c"},{"a","b","c","d"},{"b","c","d"}}`. The following table shows which elements match various input $p$ `path`s.

| $p$ | Test paths which match $p$|
|--|--|
| `{}` | `{{"a","b","c"},{},{"a","b"},{"a","b","c"},{"a","b","c","d"},{"b","c","d"}}` |
| `{"a"}` | `{{"a","b","c"},{"a","b"},{"a","b","c"},{"a","b","c","d"}}` |
| `{"b"}` | `{{"b","c","d"}}` |
| `{"a","b"}` | `{{"a","b","c"},{"a","b"},{"a","b","c"},{"a","b","c","d"}}` |
| `{"a","b","c"}` | `{{"a","b","c"},{"a","b","c"},{"a","b","c","d"}}` |
| `{"a","b","c","d"}` | `{{"a","b","c","d"}}` |
| `{d}` | `{}` |

The `abc` test framework allows the user to filter the set of tests to be ran using user-provided `path`s. See [this document](#/docs/user_written_docs/overview_of_framework) for more information regarding this.

## Test's Unique Identifier

At several points in this document we have referred to the unique identifier (or UID) of a test. In this subsection we explain explicitly what a test's UID is.

The UID is not a member variable itself, rather it is produced by the `abc` test framework when processing a test. It is a `string` composed of a test's `name`, the global `base_test_options_t` member variable `path_delimiter` and the test's `path` variable. It is created by composing each of these together, in the form

``` path + path_delimiter + name ```

For example, when using the default `path_delimiter` `"::"` with a test that has the `name` `"hello"` and `path` `"one::two::three"`, the test's UID would be `"one::two::three::hello"`.

One of the requirements for a test is that its UID should be unique among all other tests in its test-suite. This is checked at runtime. It allows the system to use a UID to refer to exactly one test.

# How the Macros Work
In the previous section we showed how tests are declared using the macros included with `abc_test`. In this section we show abstractly how these macros work.

Understanding how these macros work is not necessary for most users. However, if the user wishes to create their own tests programmatically at runtime (e.g. using `std::function`), then understanding how these macros work will help in this endevour.

Both the `_TEST_CASE` and `_NAMED_TEST_CASE` macros work almost identically; the only difference is that `_TEST_CASE` first creates a random name for its test. Abstractly this can be represented using the following pseudocode:

```cpp
_TEST_CASE(user_data)
  _NAMED_TEST_CASE(make_unique_name(),user_data)
```

Here, `make_unique_mname` is a function which generates a random function name.

Below is a pseudocode representation of how `_NAMED_TEST_CASE` could be defined.

```cpp
_NAMED_TEST_CASE(test_name,user_data)
  //Source information is generated.
  single_source_t source_info{make_source_info()};
  //The test is forward declared.
  static void test_name();
  //When given to process, test_name is the reference to the created function.
  process(test_name,user_data,source_info);
  static void test_name()
```

First, the source information, containing information about where `test_name` was declared, is generated and stored in a `single_source_t` variable. In reality, this information is generated from the macro itself, however for ease of understanding the user can assume it is generated here.

The test function `test_name` is then forward declared, before a reference to `test_name`, the user data, and the source information are passed to the function `process`. Finally the definition of `test_name` is started, allowing the user to write the body of the test function straight after the macro. 

Below is a representation of how the function `process` could be defined. 

```cpp
process(tests_function_reference,user_data,source_data)
  checked_user_defined_test_data_t checked_user_data =
    checked_user_defined_test_data_t::test_data(user_data);
  //Create an entity of type test_function_t
  test_function_t test{abc::mk_function(tests_function_reference)};
  // Create a registered test_data_t object. This is the type of entity which store the actual test.
  registered_test_data_t rtd(test,
    checked_user_data,
    source_data);
  global::add_test(rtd);
```

In the first line, an entity of type `checked_user_defined_test_data_t` is created using the function `checked_user_defined_test_data_t::test_data`. Entities of type `checked_user_defined_test_data_t` are essentially identical to entities of type `test_case_t`, except that they have had their member variables checked for prohibited values. It is in the function `checked_user_defined_test_data_t::test_data` that the compile time checks [discussed in the previous section](#the-test_case_t-object) are performed.

The function `mk_function` is referenced on the second line. This function takes any callable object which takes no arguments and returns `void`, and creates an entity of type `test_function_t`. Here, a function reference is used as the callable object. However, an entity of type `std::function<void()>` could just as easily be used.

An object of type `registered_test_data_t` is created on the third line, using one of its constructor which takes arguments of type `test_function_t`, `checked_user_defined_test_data_t` and `single_source_t`. `registered_test_data_t` is the object used to store tests throughout `abc_test`.

Finally, this `registered_test_data_t` entity is passed to the function `global::add_test`, which registers it with the global list of tests. In the next section, we explain what this means, and where the created test is now stored.

## Global List Of Tests

In the previous section, we described how the macros used to declare tests work. We noted that the tests are passed to the global list of tests (or GLOT). In this subsection we give more information about the GLOT, and how it interacts with the `abc` test framework.

The GLOT is simply a global `vector` of `registered_test_data_t` elements. There are only two ways to edit it, and one way to access it. Its API can be considered as follows.

```cpp
namespace global {
//Gets a cref to the GLOT.
const vector<registered_test_data_t>& get_test_list() noexcept;
//Adds an entity to the GLOT.
void add_test(const registered_test_data_t& _a_rtd) noexcept;
//Clears the GLOT.
void clear_list() noexcept;
}
```

The GLOT is populated using the test declaration macros `_TEST_CASE` and `_NAMED_TEST_CASE`. We will also see in the next section how the user can add tests to it manually. When the user sets up the `abc` test framework, one of the options available is to add all the tests in the GLOT to the framework's test-suite. The user can find more information regarding this in [this document](#/docs/user_written_docs/overview_of_framework). 

# Alternate ways of declaring tests
In the previous section we described to the user how the test declaration macros work. This section builds on that information, showing the user alternate ways of writing test declarations, and how to register them with the `abc` test framework.

We showed the user how the `abc::mk_function` was used to create an entity of type `test_funtion_t`. The `mk_function` function can take **any** callable type which returns `void` and takes no arguments.

Using this, the user is able to write runtime code which generates test functions, which can then be added to the `abc` test framework.

For example, the following code could be called before the `abc` test framework is setup and ran.

```cpp
std::function<void()> _l_test_function = []()
{
    //Test code
};
registered_test_data_t _l_test(mk_function(_l_test_function),
    checked_user_defined_test_data_t::test_data(test_case_t{/*test case info*/}));
global::add_test(_l_test);
```

This code would add the funcion `_l_test_function` to the GLOT, which would be processed by the `abc` test framework when it is set up.

The reader may notice that the constructor for `registered_test_data_t` is different to that used by the macros `_TEST_CASE` and `_NAMED_TEST_CASE`. This constructor does not include a `single_source_t` variable. We found that sometimes, when source information was included with a test created at runtime, this source information could be a point of confusion for the user; for example, tests created in loops would all have the same source information. However, the user is able to use the other constructor if they want to include some source information in their runtime generated tests.

Below is another example of code showing how runtime generated tests can be created. In this example, a loop is used to generate a hundred test cases.

```cpp
vector<registered_test_data_t> tests;
for (size_t idx{0}; idx < 100; ++idx)
{
    function<void()> f = []()
    {
        // Test code
    };
    tests.push_back(mk_function(f),
    checked_user_defined_test_data_t::test_data(test_case_t{/*test case info*/});
}
```

Rather than adding these tests to the GLOT, they have been stored in a local variable. The `abc` test framework has the ability to process user-defined collections of `registered_test_data_t` elements like the variable `tests` in the example above. More information regarding this functionality can be found in [this document](#/docs/user_written_docs/overview_of_framework).