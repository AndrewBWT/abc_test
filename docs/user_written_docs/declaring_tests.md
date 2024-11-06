# Declaring Tests

Tests in abc_test are free functions with a type of

```void function();```

The user can declare a test function using the macro ```_TEST_CASE```. ```_TEST_CASE``` takes a single argument of type ```abc::test_data_t```. This argument contains user-defined information about the test itself.

The user then writes the test, where we have written ```//Testing code```.

```
_TEST_CASE(
    abc::test_data_t(
        {.name             = "A simple test",
         .path             = "simple_test"}
    )
)
{
	// Testing code.
}
```
The constructor above uses aggregate initialization, where each field is named.

The reader should note that in the example above we use a normal bracket then a curly bracket after writing ```abc_test_data_t```. This is because of the way that macros work in C++; we need to use an extra bracket to escape the macros scope, which allows the pre-processor to process the commas in the initializer as we want it to.

Below is an example where we do not include the extra bracket. The pre-processor would incorrectly process this, and the compiler would likely report an error.

```
_TEST_CASE(
    abc::test_data_t
        {.name             = "A simple test",
         .path             = "simple_test"}) 
...
```

When a test is declared using ```_TEST_CASE```, it is added to the global list of tests (GLOT). The name of the test function created is not shown to the user; see BELOW for an alternate way of writing a test function where the user gets to name the created function.

## test_data_t Fields ##

The only argument the user gives to tests is of type ```abc::test_data_t```. Below we provide an overview of each public field ```abc::test_data_t``` has. As we use an aggregate initialiser, these fields must be in the order given below. 

1. ```name```. This is the name of the test. 
	- Type = ```std::string_view```.
	- Default value = ``""``.
	- *abc_test has some functionality where it uses the name of a test as a file name to store data associated with that test. Currently we perform no checks on a test's name to ensure that it can be used as a file name. However, this may be something we revisit in a future version.*
2. ```description```. This is the description associated with a test. Here the user can enter relevant information about the test itself. 
	- Type = ```std::optional<std::string_view>```. 
	- Default value = ```std::optional<std::string_view>{}```.
3. ```path```. The path of a test acts like a folder path; it allows the user to organise a test, and to group it together with other tests. 
	- Type = ```std::string_view```. 
	- Default value = ```""```.
	- A test's path is processed by the system and broken up to produce a list of strings using some user-defined delimiter. For example, the string ```"aa::bb::cc"``` becomes ```{"aa","bb","cc"}``` when used with the delimiter ```"::"```. This list of strings is the test's path.
		- The default delimiter is set to ```"::"```.
	- *Currently the functionality to filter tests based on their path is not provided by the system.*
	- *The user should note that, like a test's name, a test's path is used to organise data into folders concerning a test. We do not perform any checks to ensure a path can be used as a folder. However, this may be something we revisit in a future version.*
4. ```threads_required```. The user can set how many thread resources should be used when running this test. abc_test library can run multiple tests at once. However, before a test can be ran the number of thread resources that test uses must be available. 
	- Type = ```std::size_t```. 
	- Default value = ```1```.
	
There are some compile-time checks performed on a ```abc::test_data_t``` element. These are:
- The ```name``` field of a test cannot be empty. Since the default value is an empty string, it must be set by the user.
- The ```threads_required``` field of a test must be ```> 0```.

The following check is not performed at compile-time, instead it is performed at run-time. The user should be aware of it when writing tests:
- In a given set of tests being ran by the testing library, every test's pair of ```name``` and ```path``` must be unique - **there cannot be duplicates**. If any are found, the system will alert the user.

The above may be confusing, so here are some saliant rules:
- The only required field is a test's ```name```. All other fields can be skipped.
- It is unwise to leave a test's ```path``` as blank.
- The fields must be written in the order:
	1. ```name```.
	2. ```description```.
	3. ```path```.
	4. ```threads_required```.

```
_TEST_CASE(
    abc::test_data_t(
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


# Other ways of declaring tests

There is another macro which can be used to create test functions, ```_NAMED_TEST_CASE```. This macro allows the user to name the created function, which contains the test code. The macro takes two arguments; the name of the test function, and the ```abc::user_data_t``` element.


```
_NAMED_TEST_CASE(
    named,
    abc::test_data_t({.name = "A named test"})
)
{}
```

This can be useful when the user wants to use the created functions themselves, not in the confines of abc_test.

# What the macros do

We do not want there to be any confusion regarding the macros we use in this library, so we try to explain how they work to the user. 

```_TEST_CASE``` is defined in terms of ```_NAMED_TEST_CASE```. It simply creates an arbitrary name, which shouldn't clash with other names. It can be thought of as being in the form

```TEST_CASE(user_data) = _NAMED_TEST_CASE(unique_name(),user_data)```

```_NAMED_TEST_CASE``` works as follows:

1. The test itself is forward declared.
2. The test is registered with the GLOT. 
3. The test definition is then started, to be completed by the user.

Programmatically it can be represented as:

```
static void unique_name();
abc::register_test(unique_name,user_data)
static void unique_name()
```

The user can declare their own tests, and register them programmatically through one of the interfaces to the ```test_main_t``` object. See XXX for more details.