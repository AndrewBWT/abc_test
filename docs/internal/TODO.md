# To-do List for abc_test #

## Current Focus

- Code documentation. See docs/internal/documentation.md for details.
- Tutorials and examples. See docs/internal/tutorials_and_examples.md for details.
- Tests. See docs/internal/tests.md for details.
- README. See docs/internal/readme.md for details.

## Not Currently a Focus

- Run clang-tidy on the code. Also consider other programs, and add warnings when building from the three major compilers.
- Create tests which use CMake to build a project, using abc_test to create the testing library. Not sure how these will be created - perhaps github actions. We are testing that cmake can fetch the content, build an executable, and a simple "hello world" can be made using abc_test in this manner.
- Our CMakeLists.txt sets some variables globally. This needs to be amended, otherwise we may be polluting other's CMake build.
- Want to set up automatic builds for Windows and Linux. Specifically we want one script which can be ran, which builds using g++, clang and msvc (depending on enviornment). It then runs the linters+compilers on all the code, and tells the user what passed and what failed. This way, we can go backwards to older versions of the compilers, finding any errors. The code should also run the test suite of our code, ensuring that it all works correctly.
- Address how some of our tests fail in linux due to our files for wchar_t assuming UTF16, when it is UTF32 on linux.
- Add the functionality which allows the user to set an options object for a test. Either it can be set via an object or macro (e.g. _SET_OPTIONS(...)) where the argument is an object or a string, and it sets it for that object. Or, we can set it in the list of options associated with a test case (either an object or a string). An object can allow us to set the data manually. A string can allow us to refer to some pre-loaded named option. This can allow us to set the same test options across multiple tests.
	- This does raise some questions regarding regression testing. If our options change, does this make our tests harder or impossible to repeat? Do we memoize a failed test's options? Or we assume it won't change.
	- Some options are things which we won't want to change. Going down this route would mean re-writing our current test_options_t object, to separate the components which would change on a per-test basis.
- Add functionality to allow data generator's to filter their output. 
	- This could be through a simple binary test. Of course, it brings into question things regarding random generation, as there must be a cutoff when we stop assuming that eventually a filter will pass.
- Check CMakeLists to ensure that examples and tests subfolders are only being build when specified. Currently it appears that they are always built.
- Look into supporting older versions of C++, and older compiler versions.
- Combinatorics algorithm has some issues. Specifically, when re-generating values, we go through the list of combinations in order. This is because we use heap's algorithm for the permutation component of our combinatorics generator - and we are unsure of how to, when giving an integer, find the permutation corresponding to that integer. We could consider changing from that algorithm to a different one, however that algorithm only requires one change to move to the next permutation. 
- We should have a document which documents our coding style.
- We should have a document which goes through the terminology we use, to ensure it is consistent, and explains exactly what we mean by a term.
- We should consider our testing terminology, as we have not checked we are using the correct terms e.g. test case, test suite, test executable, assertion.
- Consider an inline namespace containing the version. We removed this because instantiating specialised templates/structs for some of the derived gen_data classes, when using this keyword, wouldn't allow us to create them in named namespaces; we had to define them in a format similar to how fmt does it (see commented out example in examples/include/abc_test_examples/enumeration/enum_enumeration_examples.h. This may be something only done in visual studio, or may be part of the standard. We should investigate further to find the reasoning. Then decide whether including the versioned namespace is worth the trade off, or if there are any workarounds.
- In abc_test/gen_data/enumeration/specializations/fundamental_types.h, there are several specializations which cannot have separate definitions and implementations (as in, we cannot separate them without an error being encountered). We believe this to be an error with Visual Studio, however we will need to investigate further.
- Currently we print the seed from a test run in hex. I do not see the reason why we do this, it should be changed.
- Consider an alternate random generator, as std::mt19937_64, and the C++'s std way of working with random generators is quite poorly realized. Also, std::mt19937_64 is not at all space effecient.
- Change create_data_sequence in typeless_data_generator_collection_stack.h into two different functions; one memoized, one not. This will make the code clearer.
- We're using a function from Google Benchmark, who use an apache licence. Need to look into whether we need to include the original licence when providing our library.
- We should have a way to "turn off" computationally expensive tests. By this, we mean a way of stating that "this test or these values should only be generated if the test is not in testing mode". This requires more thought.
	- Perhaps _DEBUG can be level 1. All _RELEASE candidates are level 2 or above. Each test is defined with a number, we can then ensure that tests are only ran if the number is met. Or have some kind of way of querying what mode the test is in, so that in a test case values can be changed at run time. For example, we many enumerate all values at test level 2. But at test level 1, only randomly generate 100 values.
- Investigate whether its possible to write random generators in a similar manner to how fmt::format is used. Specifically, write code such as abc::random_generator("({0},_)",default_random_generator<int>(0,100)). This would create a tuple generator, with the first default generator replaced by the argument above. This would be very clear syntax compared to what we currently have.
- Have some user-defined file which defines a "type map" for logging. By this, we mean the file may be in the form of a map with one entry being "std::basic_string<char,allocator<char>>,std::string". Currently type_id<T> will just print a type. I'm proposing a second function called type_custom_id<T>. If it finds an entry for T in the type map file, it will use its value, otherwise its behaviour is just type_id. So type_custom_id<std::basic_string<char>>() returns "std::string". This can help with verbose types which have default type paramters we don't usually see. Using these will allow us to abbreviate logged message data.
- Give warning when empty file has been processed. e.g. when reading data from file that should contain unit tests, tell the user if a file has nothing in it. 
- Consider a type of executable which tells the user when there are unused test files in a file hierarchy. By this we mean, scan the data folder a test executable points at, and run the test suite. Ignore test failures, only tick of files which are used (or not used), and report the results. It should have three modes; report, delete all unused, and delete empty files.
- Go through string-related utility functions and make them generic - able to return strings of any type, and able to take strings of any type.
- The way that data generators work currently, they automatically add their data to the log stack. We want to make it so that there are three additional features; 1) The user can turn this off either for an individual generator, or across all generators. 2) We provide data concerning the current stack of for loops - e.g. the index in the stack, as well as the iteration in the generator's for loop. 3) The source currently used is a placeholder. data generator's constructor should take a std::source_location = current_location(), so that we can use this ias the location from where the logged data comes from.

## Information

The todo list has been re-organised. There are now two sets of elements in the todo list; focus and non-focus. 

- Elements in the focus list are what we are currently working on.
- Elements in the non-focus list are what we are not currently working on.
- Elements in the focus list may have sub-lists associated with them, especially if it is a big project. - We may use numbers to enforce ordering on lists or sub-lists of elements.