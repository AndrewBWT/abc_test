# To-do List for abc-test #

**Update 31/10/24: TODO**

Entities in the todo list are organised by priority from 1 to 5
- 1 is the highest priority, and must be rectified ASAP.
- 2 is high priority. There should be a plan for working towards addressing it.
- 3 is medium priority. It should be planned for, however it is only actively worked on when there is nothing of a higher priority.
- 4 low priority. It is not planned for, and it being addressed will realistically not change the trajectory of the project. However, it should be addressed at some point.
- 5 lowest priority. These issues would be nice, and will help the project. However generally low-hanging fruit goes here.
- U Elements which have not had a priority set to them. We found that we were either adding elements to priority 2 because it was required for what we are working towards, or priority 5 because we didn't think the issue needed to be done now. By having a "U" group (short for unorganised), we can store new entities. At a later point we can find the correct place for them in the priority list.

Each list of todo's is given in the order which represents when they are to be completed.

## Priority 1

None

## Priority 2

- None

## Priority 3

- Documentation of code. Currently generally we have a good percentage of the code commented. However it has not been proof-read, and there are parts of it where the names of classes or the nomenclature we use has changed.
- Run clang-tidy on the code.
- Check if code compiles using g++ and clang.
- We have moved away from testing, as the code was changing quickly. We should think about revisiting it, as certain parts of the code-base are becoming more static, and we are moving towards sharing the library with others. We want to do our due dilligence when it comes to providing a library that works as we describe.
- Go through examples folder and ensure correctness. Also ensure all features are being shown off completely.
- Go through README.md and check spelling, tidy up code, potentially highlight additional features.

## Priority 4

- [ ] Add functionality for per-test options object. It should be able to be set via either a name (e.g. string), an object or a file.
- [ ] Add functionality allowing data_generator_t objects to have their elements filtered.
- [ ] Change master CMakeLists.txt file so that examples subfolder is only built when specified. 
- Investigate test_reporter which can be used with data generators to ONLY provide feedback after all elements have been generated. In a similar fashion to how quickcheck reports 98/100 tests passed. This is because some tests become unwieldly very quickly, due to the amount of output produced.
- Improve combinatorics algorithm for dealing with file IO; at the moment we just go through the elements in order. We want an algorithm which can hone in on the exact value in an O(log n)-like method. This would mean being able to calculate the size of permutations/combinations, as well as partial-combinations/permutations (e.g. the size of we leave the first element in its place). This may get particularly difficult for permutations, as we used an algorithm which only makes a single change for that one.

## Priority 5

- [ ] Terminology and documentation is inconsistent and confusing. It needs to be improved drastically.
- [ ] Consider supporting different versions of CMake, VS, C++ etc.
- [ ] Consider an inline namespace containing the version. We removed this because instantiating specialised templates/structs for some of the derived gen_data classes, when using this keyword, wouldn't allow us to create them in named namespaces; we had to define them in a format similar to how fmt does it (see commented out example in examples/include/abc_test_examples/enumeration/enum_enumeration_examples.h. This may be something only done in visual studio, or may be part of the standard. We should investigate further to find the reasoning. Then decide whether including the versioned namespace is worth the trade off, or if there are any workarounds.
- [ ] In abc_test/gen_data/enumeration/specializations/fundamental_types.h, there are several specializations which cannot have separate definitions and implementations (as in, we cannot separate them without an error being encountered). We believe this to be an error with Visual Studio, however we will need to investigate further.
- [ ] Change text_test_reporter_t and its associated printers to not print information that is not important.
- [ ] Consider alternate, more space-efficient methods for storing seeds. Hex seems a bit wasteful.
- [ ] Check through CLI11 docs, and look for way of displaying user-defined error message when a string fails to parse.
- [ ] Rename all header files (.h) to .hpp. This was seen on a tip regarding a code review; using this makes it easier to see that the language is C++.
- [ ] Change create_data_sequence in typeless_data_generator_collection_stack.h into two different functions; one memoized, one not. This will make the code clearer.

## Unorganised (Priority U)

- Add different ways of creating data files; e.g. general_data should check for a file extension. If there isn't one, use the one given in the options. There should be a general_data_exact which provides an exact location, not using any part of the test options to initialise it.
- Write examples showing how logging works.

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