# Naming Convention #

In abc_test (or abc-test - this confusion is an issue), we are experimenting with a testing framework where the testing paradigm and the testing framework are decoupled. What we mean by this is, rather than having a unit testing framework and a property checking framework, our goal is to try to build a testing framework which provides you with the tools to utilise both paradigms using almost identical code.

This document is used to collect terms, which allows us to remain consistent with how we describe concepts in the library. The document does not flow well, but rather serves as the beginnings of our documentation, which allows us to organise them in the future.

## Data Generators ##

In the framework we have built, one of the core concepts is of the data generator. Nearly all exposed library objects which provide some generalisation over large amounts of data is a subclass of the object gen_data_with_repetition_type_t, which itself is a subclass of gen_data_base_t. gen_data_base_t generalises over a template T, while gen_data_with_repetition_type_t generalises over two templates T and Rep_Data. As all instantiations of gen_data_with_repetition_type_t name a specific Rep_Data type, we can have collections of gen_data_t objects with the same T, but different Rep_Data objects.

For example, we include the static_data_t object. It is written as follows

template<typename T> class static_data_t : public gen_data_with_repetition_type_t<T,std::size_t>

Another included object is random_data_t. It is defined as

template<typename T> class random_data_t : public gen_data_with_repetition_type_t<T,std::pair<std::size_t,std::size_t>>

We can have a collection of gen_data_t objects, which include instantiations of static_data_t and random_data_t objects.

gen_data_with_repetition_type_t comes built in with some generalisations regarding writing data to and from files, as well as reading and writing repetition data for re-running tests.

### Writing Files ###

gen_data_with_repetition_type_t comes with two file writing mechanisms. We show how the file writing mechanisms can be used in the examples sections, here we simply discuss what is provided, and what terms we use to refer to these.

### General Data ###

gen_data_with_repetition_type_t, templated over type T, allows elements of T to be written to and read from a file. To use this functionality, a printer and a parser must be provided. Though it should be noted, if they are not, it defaults to fmt::format for the printer, and scn::scan for the parser. These functions are in the form:

printer(const T&) -> std::string
parser(const std::string_view) -> std::optional<T>

We call this general data, as it is assumed it is encoded in a specific format for that T. The file extension is called "gd". 

When a data generator is iterated through, general data is the first thing outputted from the generator. 

The user should note that some instantiations of gen_data_with_repetition_type_t do not have the ability to read or write general data to and from a file. The creator of these objects can turn off this functionality. For example, static_data_t has no ability to read or write general data. 

### Rep Data ###

Repetition Data (or Rep Data) is a data type defined by a subclass C of gen_data_with_repetition_type_t. To be clear, it is not a type parameter to C, but a type parameter chosen when creating C. While the use of rep data is similar to general data, its use is more nuanced.

It offers the ability to read data from a file. However, the printer and parser it needs are in the following form:

printer(const Rep_Data&) -> std::string
parser(const std::string_view) -> std::optional<Rep_Data>

Each subclass C of gen_data_with_repetition_type_t must include a function subclass_set_data_using_mode_and_repetition_data(const Rep_Data&) which puts the instantiation of C in a specific state corresponding with this Rep_Data.

This is a very generic description, so consider an example. random_data_t's Rep_Data is in the form std::pair<std::size_t,std::size_t>. This corresponds to data regarding how to setup the random seed as it was to get this repeat data. 

Another example is static_data_t, which is simply an integer corresponding to which elemnet of the set of static data to refer to.

The reader may think that this is unnecessary when compared to general data. However, this functionality doesn't require the user to write their own code to get this file IO working.

In addition to this, this functionality is exactly what is required for understanding repetition configuration's, which are used to repeat parts of tests. A repetition configuration's leaves contain repetition data. The machinery to understand these things are all contained in the above code.

# Repetition Configuration #

Other testing frameworks may use a random seed as a way of describing the specific configuration of a test failure. These may be together with a list of tests to run.

abc_test allows tests to be re-run with the exact same configuration in a similar manner. For an entire set of tests, a test-suite repetition configuration is described as a tuple containing

- A random seed.
- A set of pairs of test identifiers and a test's repetition configuration.

A test's repetition configuration is a collection of tree-like structures. In each tree, the root contains a specific integer referred to as the for loop index. Every other node contains a for loop index, a generation collection index, and a string.

So, a test configuration could look like this:

+ 0
|-- (1,0,"1")
|---- (0,0,"0")
|---- (0,0,"1")
|-- (1,0,"2")

## How these work ##

The reader may be confused reading these. Consider the following test code. In this example, example_data.gd is in the form:

7
3

1  _TEST_CASE("A simple example", "abc::examples")
2  {
3     for (auto&& _l_int : iterate_over({1,2,3}) //will go through 1,2,3
4     {
5         _CHECK_EXPR(_l_int < 4);
6     }
7 	  for (auto&& _l_int1 : enumerate(from_m_to_n(10,1)) //10,9,8,7,6,5,4,3,2,1
8 	  {
9		  _CHECK_EXPR(_l_int1 != 9);
10		  _CHECK_EXPR(_l_int1 != 8);
11		  if (_l_int1 == 9)
12		  {
13			  for (auto&& _l_int2 : from_file("example_data") //7,3
14			  {
15			  	_CHECK_EXPR(_l_int2 == 4);
16			  }
17		  }
18	  }
19  }

The above code should be relatively easy to understand: 

- the code will fail on line 9 and 10 once (when _l_int1 == 9 and _l_int == 8, on the second and third iteration of the loop).
- the code will fail on line 14 two times, on the first and second iteration of the inner loop when _l_int1 == 9.

We can encode the data for each of these failures as lists, with each element describing at which point in the for loop it was at when it failed. Putting all these failures together, we get a tree which describes the complete set of test assertion failures in a piece of code.

This is, in essence, what a repetition configuration is; when the repetition configuration described above is used with this code, it will only re-run the parts of the tests which failed, skipping over all other parts of the test.

If you don't want to include a repetition configuration, or perhaps there are parts of your tests which means they wouldn't be appropriate, simply not including one will mean that the test is re-ran in its entirety.

This type of logic is quite confusing. While we provide the facility to enter a repetition configuration manually, we doubt it will be used in such a manner. Instead, a repetition configuration is provided when a test fails, and the user and quickly and easily re-run the part of the test that failed.