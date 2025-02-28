# To-do List for abc_test #

## Current Focus

- Code documentation. See docs/internal/documentation.md for details.
- Tutorials and examples. See docs/internal/tutorials_and_examples.md for details.
- Tests. See docs/internal/tests.md for details.
- README. See docs/internal/readme.md for details.

## Not Currently a Focus

- Change internal subfolder to core.
- Move internal/utility to utility.
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

## information

The todo list has been re-organised. There are now two sets of elements in the todo list; focus and non-focus. 

- Elements in the focus list are what we are currently working on.
- Elements in the non-focus list are what we are not currently working on.
- Elements in the focus list may have sub-lists associated with them, especially if it is a big project. - We may use numbers to enforce ordering on lists or sub-lists of elements.

## Completed

31/10/24:
- Change repetition map to allow key of EITHER unique id OR a source_location. This can allow us to circumvent the issues we are having in running code using repetitions when we may be adding/removing elements from the code base.
-- This was changed to just allowing a UID, created from a test's name and path.
- Add configuration file-loading using CLI11 to library.
- Fix parsing of seeds from the command line/configuration files.
1/11/24:
- Fix parsing function for repetition map elements.
- Check repetition logic when using static_generator and file_generator to ascertain whether the incorrect repetition data is from repetition logic, or from random_data_generator_t. (It was the repetition logic).
- Investigate use of configuration files through CLI11, or our own type.
- Move processing file args from random to data_generator_with_file_support.
- Fix random data generator's seed values not being set correctly.
02/11/24:
- Rewrite enumeration code.
- Create enumeration examples. 
- Ensure manual data generator code works with updated code-base.
- Create manual for loop examples.
03/11/24:
- Work on README.md, showing an introduction to abc_test.
04/11/24:
- Add combinatorics generator. 
- Add examples showing use of combinatorics generator.
-5/11/24:
- Add enumeration constructor which takes a start, end, and size-like operator. The size-like operator denotes how many enumeration values should be produced. The start and end values are what the first and last values should be (though end value is not set in stone). The algorithm then computes how many jumps to perform per generate_next. This is so we can say "generate 7 values between 0 and 50". 50/(7-1) = 8R2. The values [0,8,16,24,32,40,48] are then produced. This could be difficult as we would need be able to calculate how many values are between two points. This isn't just straight subtraction as it will have to take into account any values in enumerate_object_t for that object. 
- Add random generator using enumeration.
- Add examples showing random generator using enumeration.
- Test combinatorics generator with file IO and reset - we haven't written the logic for this yet. Rewrote logic, works correctly though it is inefficient.
- Add random_generator_object specialisations for arrays, vectors and enums.
- Rename all header files (.h) to .hpp. This was seen on a tip regarding a code review; using this makes it easier to see that the language is C++.
27/2/25:
- Add different ways of creating data files; e.g. general_data should check for a file extension. If there isn't one, use the one given in the options. There should be a general_data_exact which provides an exact location, not using any part of the test options to initialise it.
- Implement the ability to write a seed to a file. Give it re-write mode or only write mode. Read only mode too.
- An idea worth investigating; a version where all assertions are built up as a tree. Assertions either have group nodes, or assertion children (assertion children can't have any more children). This has sort of been addressed with BBAs.