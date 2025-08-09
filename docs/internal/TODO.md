# To-do List for abc_test #

## Focus for Version 0.0.2 ##

1 - Testing. A lot of the core features in abc_test lack testing. Without tests, code changes can (and have) broken features - it is in essence a case-study as to why testing is necessary. We are adding tests to abc_test, the tests themselves written using abc_test - in effect, test bootstrapping. However, to write tests for every part of abc_test will take some time. For the next release, we want to have in place some basic tests for data generators, repetition configurations, assertions, macros and logging. These tests will provide us with a safety-net when making further changes to the code.
2 - Lack of documentation. We have some Doxygen generated documentation and some user-written documentation. However the Doxygen documentation is incomplete for many functions, and the user-written documentation is for an older version of abc_test. We have kept it as we believe it will be helpful when writing the new documentation, and could help users wanting to learn more about abc_test. For version 0.0.2, we would like the core documentation regarding test declarations, assertions and data generators to be written to a high quality.
3 - Code clean-up. While we are happy with a lot of the code, there are some portions which are of a lower quality, written with little regard for maintainability or readability. We make no promises for version 0.0.2 regarding how this will be improved, however it will be something we will make an attempt to tackle.
4 - abc_test makes heavy use of generics and template specializations - for example, default_printer_t and default_parser_t specializations are necessary when working with the file-reading data generator, default_enumeration_t specializations are necessary when working with the enumeration data generator, and default_random_generator_t specializations are necessary when working with the random data generator. In version 0.0.1, many of these specializations are inconsistent, missing, or defined incorrectly. For version 0.0.2 we want to have addressed this, and hopefully have specializations defined for all of the core types in the C++ standard library.
5 - The default console and file output requires some improvement. Specifically, some output is poorly labelled, some fields are not being printed correctly, and some fields are missing. For version 0.0.2 we want to have much clearer default output for abc_test.
6 - In abc_test many of the functions which work with strings use a mixture of std::string and std::u8string. For version 0.0.2 we want to provide the user with overloads for each function and type of string.

### Specific Long-form List of to-do's for 0.0.2 ###

18 - Brainstorm writing tests for the logging functions.
19 - Organize inner documentation so there is a landing page for those looking for user-written documentation.
20 - Work on organization of test declarations documentation.
21 - Work on organization of test assertions documentation.
22 - Work on organization of data generators documentation.
23 - Write list of all objects for which we would like to have specialisations for in abc_test. 
24 - Work out how we want to include/exclude specializations. Either we have specific files for each specialization. Or we have a compiler on/off switch which allows/disallows these entities to be compiled.
25 - Write list of all types of specialization which we would require. From here, we should have a 2d matrix of all specialisations that need to be written. And adding them to a big to-do list should be the next step.
26 - Perform an overview of the output from a generic run of abc_test, and come up with a list of changes and improvements which need to be made. From there, add them to the to-do list.
27 - Go through each of our examples and test files. Make a note of where strings are used, and determine whether there are u8string overloads for them. Once that is done, come up with a list of all functions which require a u8string or std::string overload. Add those functions which need to be written to the to-do list.
29 - Write repetition config test for enum generator.
30 - Write repetition config test for random generator.
31 - matcher_default_comparable_t needs to be had a look at, and made consistent across all the data types we want to write instances for.
33 - Write a test for a _CHECK macro on its own. 
34 - Write a test for a _CHECK_EXPR macro on its own.
35 - Write a test for a _REQUIRE macro on its own.
36 - Write a test for a _REQUIRE_EXPR macro on its own.
37 - Write a test for an && in a _CHECK macro on its own.
38 - Write a test for a || in a _CHECK macro on its own.
39 - Write a test for a ! in a _CHECK macro on its own.
40 - Write a test for an && in a _REQUIRE macro on its own.
41 - Write a test for a || in a _REQUIRE macro on its own.
42 - Write a test for a ! in a _REQUIRE macro on its own.
43 - Write a test for a _CHECK macro as part of a multi matcher. 
44 - Write a test for a _CHECK_EXPR macro on its own as part of a multi matcher. 
45 - Write a test for a _REQUIRE macro on its own as part of a multi matcher. 
46 - Write a test for a _REQUIRE_EXPR macro on its own as part of a multi matcher. 
47 - Write a test for an && in a _CHECK macro on its own as part of a multi matcher. 
48 - Write a test for a || in a _CHECK macro on its own as part of a multi matcher. 
49 - Write a test for a ! in a _CHECK macro on its own as part of a multi matcher. 
50 - Write a test for an && in a _REQUIRE macro on its own as part of a multi matcher. 
51 - Write a test for a || in a _REQUIRE macro on its own as part of a multi matcher. 
52 - Write a test for a ! in a _REQUIRE macro on its own as part of a multi matcher. 
53 - Write test for the _EXPR type where the internal type only has a == and a printer speciailization. It should default to default behaviour.
54 - Write a test for the _EXPR macro where the internal type has a specialised matcher_comparison specialization. Ensure it is followed.
55 - Write a test for a bespoke matcher. Ensure that what is printed is as stipulated by the user.
56 - Write a test for the true_matcher and false_matcher.
57 - Brainstorm writing tests for the exception macros. 
60 - Check incorrectly read data for GDF for a DGWFS is processed correctly. By this we mean, it fails gracefully.
62 - Check repetition configs are properly processed by data generators with file support. By this we mean, that a correct RC of mode 0 goes to the first file, then mode 1 to the underlying DG - and that it never goes to the other files (which are write-only currently).
63 - Check incorrectly read data for TDF for a DGWFS is processed correctly. By this we mean, it fails gracefully.

## Longer Term Focus ##

1 - Code documentation. See docs/internal/documentation.md for details.
2 - Tutorials and examples. See docs/internal/tutorials_and_examples.md for details.
3 - Tests. See docs/internal/tests.md for details.

## Not Currently a Focus ##

1 - Run clang-tidy on the code. Also consider other static analyzers, and add warnings when building from the three major compilers. Tried this in VS, got some odd results - specifically results from the fmt library. Think we'll have to try it under Ubuntu.
	- One of the issues appears to be that our dependencies are publicly available. This causes clang-tidy to run on fmt. To fix this, it would be best to only use fmt privately - so not exposed outside of our code. This would mean moving all fmt code to a cpp file. Instead we think we'll just ignore the fmt results for now.
	- We have a script which puts all the source files we use into a text file. A script is then run, running clang-tidy on all of the elements in that list. While a good start, we ran into issues running this on Windows - primarily the different clang-tidy versions in Cygwin and VS causes issues. We couldn't run clang-tidy directly from VS as it won't let us put our headers in.
	- Our next effort was a VM, running Ubuntu. We got that working, however clang-tidy was then throwing errors complaining about files it could not find.
	- We were able to get it working on the code we wanted under Ubuntu. However, as fmt was included from files, the errors were still being reported from it. 
	- Even questioning whether clang-tidy is worth the effort it appears I have to go through to get it working the way I want it to.
4 - Want to set up automatic builds for Windows and Linux. Specifically we want one script which can be ran, which builds using g++, clang and msvc (depending on environment). It then runs the linters+compilers on all the code, and tells the user what passed and what failed. This way, we can go backwards to older versions of the compilers, finding any errors. The code should also run the test suite of our code, ensuring that it all works correctly.
5 - Address how some of our tests fail in linux due to our files for wchar_t assuming UTF16, when it is UTF32 on linux.
6 - Add the functionality which allows the user to set an options object for a test. Either it can be set via an object or macro (e.g. _SET_OPTIONS(...)) where the argument is an object or a string, and it sets it for that object. Or, we can set it in the list of options associated with a test case (either an object or a string). An object can allow us to set the data manually. A string can allow us to refer to some pre-loaded named option. This can allow us to set the same test options across multiple tests.
	- This does raise some questions regarding regression testing. If our options change, does this make our tests harder or impossible to repeat? Do we memoize a failed test's options? Or we assume it won't change.
	- Some options are things which we won't want to change. Going down this route would mean re-writing our current test_options_t object, to separate the components which would change on a per-test basis.
	- Practically, we should separate test_options_t into two sets of variables; test_options_unchanged_t and per_test_options_t. The former would be things which shouldn't be able to be set on a per-test basis - e.g. repetition_config. per_test_options_t would change things that can be set per test - e.g. number of random variables to be generated by default
	- One separated, test_options_unchanged_t should be a global variable. It should contain a default per_test_options_t value. There should then be a thread-local per_test_options_t, which can be set each time a test case is initialized. it will either point to the bespoke per_test_options_t, or the default.
7 - Add functionality to allow data generator's to filter their output. 
	- This could be through a simple binary test. Of course, it brings into question things regarding random generation, as there must be a cutoff when we stop assuming that eventually a filter will pass.
8 - Check CMakeLists to ensure that examples and tests subfolders are only being build when specified. Currently it appears that they are always built.
9 - Look into supporting older versions of C++, and older compiler versions.
10 - Combinatorics algorithm has some issues. Specifically, when re-generating values, we go through the list of combinations in order. This is because we use heap's algorithm for the permutation component of our combinatorics generator - and we are unsure of how to, when giving an integer, find the permutation corresponding to that integer. We could consider changing from that algorithm to a different one, however that algorithm only requires one change to move to the next permutation (its more effecient than the alternatives).
11 - We should have a document which documents our coding style.
12 - We should have a document which goes through the terminology we use, to ensure it is consistent, and explains exactly what we mean by a term.
13 - We should consider our testing terminology, as we have not checked we are using the correct terms e.g. test case, test suite, test executable, assertion.
14 - Consider an inline namespace containing the version. We removed this because instantiating specialised templates/structs for some of the derived gen_data classes, when using this keyword, wouldn't allow us to create them in named namespaces; we had to define them in a format similar to how fmt does it (see commented out example in examples/include/abc_test_examples/enumeration/enum_enumeration_examples.h. This may be something only done in visual studio, or may be part of the standard. We should investigate further to find the reasoning. Then decide whether including the versioned namespace is worth the trade off, or if there are any workarounds.
15 - In abc_test/gen_data/enumeration/specializations/fundamental_types.h, there are several specializations which cannot have separate definitions and implementations (as in, we cannot separate them without an error being encountered). We believe this to be an error with Visual Studio, however we will need to investigate further.
16 - Currently we print the seed from a test run in hex. I do not see the reason why we do this, it should be changed.
17 - Consider an alternate random generator, as std::mt19937_64, and the C++'s std way of working with random generators is quite poorly realized. Also, std::mt19937_64 is not at all space effecient.
18 - Change create_data_sequence in typeless_data_generator_collection_stack.h into two different functions; one memoized, one not. This will make the code clearer.
19 - We're using a function from Google Benchmark, who use an apache licence. Need to look into whether we need to include the original licence when providing our library.
20 - We should have a way to "turn off" computationally expensive tests. By this, we mean a way of stating that "this test or these values should only be generated if the test is not in testing mode". This requires more thought.
	- Perhaps _DEBUG can be level 1. All _RELEASE candidates are level 2 or above. Each test is defined with a number, we can then ensure that tests are only ran if the number is met. Or have some kind of way of querying what mode the test is in, so that in a test case values can be changed at run time. For example, we many enumerate all values at test level 2. But at test level 1, only randomly generate 100 values.
21 - Investigate whether its possible to write random generators in a similar manner to how fmt::format is used. Specifically, write code such as abc::random_generator("({0},_)",default_random_generator<int>(0,100)). This would create a tuple generator, with the first default generator replaced by the argument above. This would be very clear syntax compared to what we currently have.
22 - Have some user-defined file which defines a "type map" for logging. By this, we mean the file may be in the form of a map with one entry being "std::basic_string<char,allocator<char>>,std::string". Currently type_id<T> will just print a type. I'm proposing a second function called type_custom_id<T>. If it finds an entry for T in the type map file, it will use its value, otherwise its behaviour is just type_id. So type_custom_id<std::basic_string<char>>() returns "std::string". This can help with verbose types which have default type parameters we don't usually see. Using these will allow us to abbreviate logged message data.
23 - Give warning when empty file has been processed. e.g. when reading data from file that should contain unit tests, tell the user if a file has nothing in it. 
	- To do this, we need to check the file_reader_generator_t, and any object which is derived from data_generator_with_file_support_t<T,true>. 
24 - Consider a type of executable which tells the user when there are unused test files in a file hierarchy. By this we mean, scan the data folder a test executable points at, and run the test suite. Ignore test failures, only tick of files which are used (or not used), and report the results. It should have three modes; report, delete all unused, and delete empty files.
25 - Go through string-related utility functions and make them generic - able to return strings of any type, and able to take strings of any type.
26 - The way that data generators work currently, they automatically add their data to the log stack. We want to make it so that there are three additional features; 1) The user can turn this off either for an individual generator, or across all generators. 2) We provide data concerning the current stack of for loops - e.g. the index in the stack, as well as the iteration in the generator's for loop. 3) The source currently used is a placeholder. data generator's constructor should take a std::source_location = current_location(), so that we can use this ias the location from where the logged data comes from.
29 - When using CTest for testing construction of library, consider using FetchContent. We are unsure of how it will work with the private project, so this may not be possible..
30 - Some check is needed in CMake to ensure the correct c++ version is being used (c++23). 
31 - When using CTEst for testing construction of library, consider making a more verbose executable - one with actual tests in, instead of just a minimal example as we currently have.
32 - Try making fmt private again.
33 - CTest executable build location is incorrect. Needs to be in a different build directory.
34 - CTest executable creates list of clang tidy files, which it should not.
35 - Run CMakeLists.txt files through cmake linters, to check for any errors.
36 - Consider adding functionality so "find_package" works with CMake.
37 - Consider functionality for "install" being viable for abc_test from cmake.
38 - Consider new data generator for writing tests. It would be int he form

for (auto&& [validator, input] : data_validator<char,int>(gdf("hello"), random_generator<int>()))
{
	//validator would be an object in the form validator<char>
	char data; // do something to generate the correct value from the input parameter int.
	//validaotr would have a member function validate. If the entry exists in the file hello, it checks that the entry matches data.
	// otherwise, it writes that value to the file.
	_CHECK(validator.validate(data));
	
39 - Write comparison checks for generic ranges - so expressions like _CHECK_EXPR(vect == {1,2,3}) can be written.
40 - File tests with bootstrapped abc_test pose an issue. We want to ensure that any temporary files are deleted after they are created. We can either link every created file to a run of abc_test, and have an option which allows them to be deleted when the test_main is exited. Such an option could be useful for those running abc_test in production code; e.g. to check the changes that a run would make before the run is made.
41 - data generator's tertiary data needs to match exactly to what the internal printer will produce. For example, if we manually set the tertiary data of a file_data_generator to "(0,0)" it will be read correctly. But as it sets its current teritary data value using the print function for that data generator, "(0, 0)" will be produced. As they do not match, the linear search will fail to find the correct value, resulting in incorrect results. This should be addressed. Either normalization, or a "strict" version of the printer/parser combo which won't allow "(0,0)", only allowing "(0, 0"). The former is easier (parse then print again). The latter is less computationally expensive.
42 - file_data_generator, when working with tertiary data, currently works as follows: re-loads the correct file, skips lines to that index. It can be more efficient. 
43 - file_data_generator_with_file_support. Currently the list of files is processed like this; 1st file read and write to. All other files only write to. We should have the option to only read from too. We should NOT have the ability to read and write to the nth file. This is because doing so will just copy the initial file, causing an exponential increase in the number of entities each data generator creates each time its entered. 
44 - Want to have a mechanism which defines the "maximum" number of threads an instance of the test harness can have. This isn't the number of threads on the system, or the number of threads defined by the user, but rather some abstract notino of what the system can conceivably have as a maximum value. Most of the time this would be set to std::numeric_limits<size_t>::max(), however when bootstrapping tests, it would be set to the maximum number of threads the parent running-test was assigned. Currently, we can set this value to be very high and the system will just accept it - potentially allowing us to write some code which would break the system. This safety check wouldn't eliminate that (we can always write a test which spawns a billion threads), but it would give us a bit more safety - never a bad thing.
45 - Validation of rep configs needs to be looked at. Currently all we do is ensure it parses correctly. We could look at whether there exists a test which matches the repetition configuration's unique id. I don't think we can analyse a repetition configuration after its been ran on a test to discern whether its invalid. We can check for invalidly parsed tertiary data. We can also look at streamlining how the data is stored - e.g. we have whitespace characters in the string which aren't needed. Should we enforce an ordering on the rep config? e.g. for loop index 0 can't follow for loop index 1. But they would need an index, for when a rep config skips a for loop index.
46 - Should the string printers report specific string types - e.g. a u8 prefix when checking std::u8string types?

## Information

The todo list has been re-organised. There are now two sets of elements in the todo list; focus and non-focus. 

- Elements in the focus list are what we are currently working on.
- Elements in the non-focus list are what we are not currently working on.