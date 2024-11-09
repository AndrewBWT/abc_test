# Declaring Tests
This page contains documentation concerning the declaration of tests in `abc_test`. It covers the following topics:

- The basic ways to declare tests.
- The `test_case_t` object, used to store user-defined data about tests.
- An overview of how the test declaration macros work.
- Alternate ways of declaring tests.

# Basic ways to declare tests

Tests in `abc_test` are free functions with a type signature of `void function();`. The user can declare a test with the macro `_TEST_CASE`. This macro also registers the test with the global list of tests (GLOT). We will delve into greater detail regarding what this means [in a later section](#GLOT).

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
We will go into greater detail regarding `test_case_t` in [the next section](#testcaset). However for now, the reader should be aware it is an aggregate class - that is to say, it has no constructors, and only public fields. Therefore, the only way to initialise it is using either an initializer list or a designated initializer.

The reader may notice that in the example above, there are an extra pair of brackets around the initializer list for the `test_case_t` entity. These extra brackets are needed due to how the pre-processor works. Without them, it would assume the commas in the initializer list were meant to separate arguments for the macro. In turn, the macro would be expanded incorrectly, and a compiler error would be raised.

To be clear, when using the `_TEST_CASE` macro,  there must be an extra pair of brackets surrounding its `test_case_t` argument if it uses an initializer list in its construction.

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

# The test_case_t Object{#testcaset}

The `test_case_t` object is used to store user-defined data about a test. `abc_test` has been designed in such a way that, every time a new test is declared, the user should fill out the fields of the test's `test_case_t` object, providing information such as a name and description for the test.

`test_case_t` only has public fields, and no constructors. It has been designed in this manner so that aggregate initialization can be used to create `test_case_t` objects.

Below we show how the public fields in `test_case_t` are defined. Note that this is the order they are given in the class, and therefore the order the user must use when initializing `test_case_t` elements.

```cpp
string_view           name             = "";
optional<string_view> description      = nullopt;
string_view           path             = "";
size_t                threads_required = 1;
```

Below we give a brief description of each field, and what it is used for in `abc_test`.

- `name`
    - This is the name of the test. It should provide some indication as to what the purpose of the test is.
    - A test's `name` forms part of a test's unique ID (see [here](#UID) for more information).
    - The `abc` test framework uses and creates folders and files based off a test's `name` and `path`. These files store test-specific data. It is possible that the `abc` test framework may try to create files or folders using the test's `name` as the file/folder's name. **Currently no checks are performed on a test's `name` to ensure that it can be used as a file or folder name. However the user should be aware of this functionality, as an incorrectly chosen `name` could result in unexpected errors when the `abc` test framework is running**.
- `description`
    - This is the user-supplied description of the test. It is not used in any other capacity other than to store information for the user.
- `path`
    - A test's `path` is primarily used to group together tests, in a similar way to how folders can be seen as a way of grouping together files. See [here](#testspath) for more information about a test's `path`.
    - While the user supplies a `string_view` to represent the test's `path`, the actual run-time representation of a test's `path` is a collection of `string`s. This collection is created by splitting the user-supplied `string_view` using a delimiter.
        - For example, if the user-supplied `string_view` is `"aa::bb::cc"`, and the delimiter used is `"::"`, then the run-time representation of the
        `path` is `{"aa","bb","cc"}`.
        - The delimiter used is the global `base_test_options_t`'s member variable `path_delimiter`. It has a default value of `"::"`, but can be changed by the user.
    - A test's `path` forms part of a test's unique ID (see [here](#UID) for more information).
    - The `abc` test framework uses and creates folders and files based off a test's `name` and `path`. These files store test-specific data. It is possible that the `abc` test framework may try to create folders using the test's `path` as the folder's name. **Currently no checks are performed on a test's `path` to ensure that it can be used as a folder name. However the user should be aware of this functionality, as an incorrectly chosen `path` could result in unexpected errors when the `abc` test framework is running**.
- `threads_required`
    - This represents how many threads the test requires when it is ran. The `abc` test framework will ensure this many threads are allocated to the test at run-time. See [this page](@ref threads) for more information regarding multi-threaded tests in `abc_test`.

Here is an example of a test being declared using all of the fields in the `test_case_t` object.


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

## Field Requirements

When the `abc` test framework processes a `test_case_t` component, it checks its fields to ensure it does not contain any prohibited values. 

Some of these checks may be performed at compile time, if it is possible to do so. These are as follows:

- The `name` variable is checked to ensure it does not equal `""` - empty `name`s are **not** allowed.
- The `threads_required` variable is checked to ensure it is `> 0` - it cannot equal `0`.

When using the macros  `_TEST_CASE` or `_NAMED_TEST_CASE` to declare tests, the above checks are guaranteed to be performed at compile time. If any of them fail, then a compilation error is raised.

In addition, the following check is performed at runtime by the `abc` test framework when a test-suite is being processed:
- Each test's unique ID must be unique among all the tests that are going to be ran. See [here](#UID) for more information about unique IDs.

In [the next section](#howmacroswork) we describe how the test macros work. The compile-time checks are performed when the function `checked_user_defined_test_data_t::test_data` is useed on the `test_case_t` object provided by the user.

If the user is creating tests at run-time and these checks fail, then `checked_user_defined_test_data_t::test_data` will throw an `abc::test_library_exception_t` exception.

In [this document](#overview_of_framework) we explain how the `abc` test framework works in detail. The run-time check which checks that unique identifiers are unique is performed at step 5 in the `test_main_t` object's function `run_tests`, when the `registered_test_data_t` elements are taken from the `base_test_options_t` object and the GLOT, and added to the local `test_collection_t` variable.

## Test's path{#testspath}
In previous sections in this document we have referred to a test's `path`. In this subsection we explain explicitly what a test's `path` is, and conceptually how it can be used to group tests together.

Given a set of tests $S$, each with a `path` $p$, and another path `p`, we can use the function $path(p',S)$ to return a subset of $S$ containing tests whose `path` $p$ contains a prefix of $p'$.

For example, consider the a set of tests with the paths `{a,b,c},{},{a,b},{a,b,c},{a,b,c,d},{b,c,d}`. The following table shows which elements are returned from calling $path$ using this set and  different path arguments.

| Input Path | Output Tests (represented as paths)|
|--|--|
| `{}` | `{{a,b,c},{},{a,b},{a,b,c},{a,b,c,d},{b,c,d}}` |
| `{a}` | `{{a,b,c},{a,b},{a,b,c},{a,b,c,d}}` |
| `{b}` | `{{b,c,d}` |
| `{a,b}` | `{{a,b,c},{a,b},{a,b,c},{a,b,c,d}}` |
| `{a,b,c}` | `{{a,b,c},{a,b,c},{a,b,c,d}}` |
| `{a,b,c,d}` | `{{a,b,c,d}}` |
| `{d}` | `{}` |

The `abc` framework includes some functionality to only run tests which are returned if $path$ returns them when ran with the given argument. See [this document](#overview_of_framework) for more information regarding this.

## UID{#UID}

At several points in this document we have referred to the unique identifier (or UID) of a test. In this subsection we explain explicitly what a test's UID is.

The UID is not a field itself, rather it is produced by the `abc` test framework when processing a test. It is a string composed of a test's `name`, the global `base_test_options_t` member variable `path_delimiter` and the test's `path` variable, in the form


```path + path_delimiter + name ```

For example, when using the `path_delimiter` `"::"` with a test that has the `name` `"hello"` and `path` `"one::two::three"`, the test's UID would be `"one::two::three::hello"`.

Due to the requirements we have put on a test's `name` and `path`, a test's UID should be unique among all other test's being ran. This allows us to refer to a test using the UID.


In the previous section we discussed the test's `path`, which we said is used to group together tests. In this subsection we discuss how this works

A test's `path` is represented by the user as a `string_view`, however when processed by the `abc` test framework, it is processed as a collection of `string`s. This processing works by using a delimiter. The `abc` test framework uses the global `base_test_options_t` member variable `path_delimiter` to do this.

Given a set of tests $S$, when getting all the tests in $S$ using the path $p$, we write $path(p,S)$. $path$ works by filtering the tests in $S$. Any test which has a path $p'$ that has a prefix of $p$ are returned. 

For example, given a set of tests $S$ which have `path`s of `"a::b::c","a::b","","a::b::c::d","b::c"}`, $path("a",S)$ will return the first 

Conceptually, given a set of tests $S$, we say that the result of getting all tests 

Tests can have the same path as one another. If two tests in a set of tests $S$ have a `path` $p$, getting the set of tests which 

- Tests can have the same path as each other.
- Tests can have an empty path.

 Here we provide the reader with more details about the `path` variable.

- The `abc` test framework uses the `base_test_options_t` field `path_delimiter` as the delimiter when creating the collection of `string`s which represent the path.
- If two test's have the same `path`, using that `path` to refer to a group of tests will definitely include both tests.
- If test A has a `path` pathA, which is a prefix of test B's path pathB, using pathA will definitely include both test A and B, while using pathB will only refer to path B. 
    - Using the file path analgy from before, file paths can be considered recursive; they get all files in sub-paths.

- The way the `abc` test framework processes the original `string` is by splitting it up, using the `base_test_options_t` field `path_delimiter` as the delimiter.

Here is a final example using all the fields from `test_case_t`.


# How the Macros Work{#howmacroswork}
In the previous section we showed how tests are declared. In this section we show abstractly how the macros work.

Both the `_TEST_CASE` and `_NAMED_TEST_CASE` macros work almost identically; the only difference is that `_TEST_CASE` first creates a random name for its test. Abstractly this can be represented using the following pseudocode.

```cpp
_TEST_CASE(user_data)
  _NAMED_TEST_CASE(make_unique_name(),user_data)
```

Here, `make_unique_mname` is a function which generates a random function name.

`_NAMED_TEST_CASE` works by forward declaring the test function, then passing its data to a processing function `process`. Finally it begins defining the test function, allowing the user to write the body after the macro is used. This is shown in the following pseudocode.

```cpp
_NAMED_TEST_CAES(test_name,user_data)
  //Source information is generated.
  source_info_t source_info{make_source_info()};
  //The test is forward declared.
  static void test_name();
  //When given to process, test_name is the reference to the created function.
  process(test_name,user_data,source_info);
  static void test_name()
```

In the above pseudocode, the `source_info_t` object is an abstract representation of the source of the test declaration. The system uses it to notify the user of where a failed test case originated from.

Finally, all that remains to be declared is the function `process`. Below is some pseudocode doing so.

```cpp
process(tests_function_reference,user_data,source_data)
  checked_user_defined_test_data_t checked_user_data =
    checked_user_defined_test_data_t::test_data(user_data);
  // Create a registered test_data_t object. These are the entities which store the actual test.
  registered_test_data_t rtd(abc::mk_function(tests_function_reference),
    checked_user_data,
    source_data);
  global::add_test(rtd);
```

This pseudocode can be explained as follows:

Firstly an object of type `checked_user_defined_test_data_t` is created using the function `checked_user_defined_test_data_t::test_data`. It is this function which performs the compile-time checks on the `test_case_t` object.

Then, an object of type `registered_test_data_t` is created. `registered_test_data_t` are the objects which contain all the relevant test information. 

## The GLOT{#GLOT}

In the previous section, we described how the macros used to declare tests work. We noted that the tests are passed to the global list of tests (or GLOT). In this subsection we give more information about the GLOT, and how it interacts with the `abc` test framework.

The GLOT is an abstract list of tests. There are only two ways to edit it, and one way to access it. 

```cpp
//Gets a cref to the GLOT.
const test_list& get_test_list() noexcept;
//Adds an entity to the GLOT.
void add_test(const registered_test_data_t& _a_rtd) noexcept;
//Clears the GLOT.
void clear_list() noexcept;
```

The GLOT is used to populate `test_main_t` only if the member variable of `base_test_options_t` `use_global_list` is set to true. Otherwise, it is not used. See XXX for more inforamtion regarding this.

# Alternate ways of declaring tests
In the previous section we described how the test declaration macros worked. In this section we show other ways of declaring tests, and how to have them passed to `test_main_t` so that they are processed by the `abc` test framework.

Assuming that the included `abc` test framework is being used, the user at some point will instantiate `test_main_t` with a `base_test_options_t` object.

As discussed previously, to use the GLOT, the field `xxx` must be set to true. 

However, there are also lists of `test_list_t` entities which can be used to pass tests to the `abc` test framework. It is these which the user should populate with their programatically created `registered_test_data_t` elements.



```
ds::registered_test_data_t _l_test = /*...*/
//Adds the test to the global list of tests
global::add_test(_l_test);
//Clears the global list of tests
global::clear_test_list();
//Create own list of tests to be processed by the framework later
vector<ds::registered_test_data_t> _l_tests;
_l_tests.push_back(_l_test);
```

Creating a `registered_test_data_t` element is relatively easy. It consists of three components:
- A `test_function_t` object. The function `mk_function` can be used to create such an object, if the argument is a `std::callable` object.
- A `checked_user_defined_test_data_t` object. This is checked at compile time.
- A `single_source_t` element. 

```
    std::function<void()> _l_test1;
    abc::global::add_test(ds::registered_test_data_t(
        mk_function(_l_test1),
        ds::checked_user_defined_test_data_t::test_data(test_case_t{}),
        ds::single_source_t{}
    ));
```
