# To-do List for abc_test #

## Current Focus

- Code documentation. See docs/internal/documentation.md for details.
- Tutorials and examples. See docs/internal/tutorials_and_examples.md for details.
- Tests. See docs/internal/tests.md for details.
- README. See docs/internal/readme.md for details.

## Not Currently a Focus

- Run clang-tidy on the code.
- Check if code compiles using g++ and clang.
- Add some functionality which would allow the user to set an options object per test. Either we set it via a name, via an object, or via a file.
	- Such functionality would raise questions regarding memoized data; either we store each test's options file, or we assume it won't change.
	- Some options are not things which we would want to change per-test. It may be that actually we want the functionality to change certain things in a test - e.g. number of random elements generated from a data generator.
- Add functionality allowing data_generator_t objects to have their elements filtered.
- Change master CMakeLists.txt file so that examples subfolder is only built when specified. 
- Improve combinatorics algorithm for dealing with file IO; at the moment we just go through the elements in order. We want an algorithm which can hone in on the exact value in an O(log n)-like method. This would mean being able to calculate the size of permutations/combinations, as well as partial-combinations/permutations (e.g. the size of we leave the first element in its place). This may get particularly difficult for permutations, as we used an algorithm which only makes a single change for that one.
	- Alternative is to use a simpler permutation algorithm.
- We should have a document which describes our coding style - e.g. making as much constexpr as possible.
- Terminology and documentation is inconsistent and confusing. It needs to be improved drastically.
- Consider supporting different versions of CMake, VS, C++ etc.
- Consider an inline namespace containing the version. We removed this because instantiating specialised templates/structs for some of the derived gen_data classes, when using this keyword, wouldn't allow us to create them in named namespaces; we had to define them in a format similar to how fmt does it (see commented out example in examples/include/abc_test_examples/enumeration/enum_enumeration_examples.h. This may be something only done in visual studio, or may be part of the standard. We should investigate further to find the reasoning. Then decide whether including the versioned namespace is worth the trade off, or if there are any workarounds.
- In abc_test/gen_data/enumeration/specializations/fundamental_types.h, there are several specializations which cannot have separate definitions and implementations (as in, we cannot separate them without an error being encountered). We believe this to be an error with Visual Studio, however we will need to investigate further.
- Change text_test_reporter_t and its associated printers to not print information that is not important.
- Consider alternate, more space-efficient methods for storing seeds. Hex seems a bit wasteful.
- Change create_data_sequence in typeless_data_generator_collection_stack.h into two different functions; one memoized, one not. This will make the code clearer.
- Write examples showing how logging works.
- Should we perform checks on a test's name to ensure it can be used as a file/folder name.
- Add functionality to allow the user to filter tests based on a path. When this is addressed, update DECLARING_TESTS.md.
- Check that same pairs of test paths and test names are dealt with properly. Update DECLARING_TESTS.md when this is addressed.
- We're using a function from Google Benchmark, who use an apache licence. Need to look into whether we need to include the original licence when providing our library.
- We should have a way to "turn off" computationally expensive tests.
- Investigate whether its possible to write random generators in a similar manner to how fmt::format is used. Specifically, write code such as abc::random_generator("({0},_)",default_random_generator<int>(0,100)). This would create a tuple generator, with the first default generator replaced by the argument above. This would be very clear syntax compared to what we currently have.
- Add a way of automatically adding generated data to the current log stack. Make it able to be turned off.
- Have some "type-map" available to log data, which would turn complicated types into simpler ones. e.g. "std::vector<int,std::allocator..." could become "vector<int>". This would have to be user-defined, and loaded at runtime.
- Give warning when empty file has been processed. e.g. when reading data from file that should contain unit tests, tell the user if a file has nothing in it. 
- Consider a type of executable which tells the user when there are unused test files in a file hierarchy.
- Go through string-related utility functions and make them generic - able to return strings of any type, and able to take strings of any type.

## Information

The todo list has been re-organised. There are now two sets of elements in the todo list; focus and non-focus. 

- Elements in the focus list are what we are currently working on.
- Elements in the non-focus list are what we are not currently working on.
- Elements in the focus list may have sub-lists associated with them, especially if it is a big project. - We may use numbers to enforce ordering on lists or sub-lists of elements.