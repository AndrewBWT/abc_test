# To-do List for abc-test

**This document should be revisited when some of the higher priority tasks have been completed**

## Highest Priority

**It is imperative these issues are resolved as soon as possible**

## Very High Priority

**These issues should be addressed before the repository is open publicly**

- Ensure that the code is re-buildable using the CMake file.
- Make tutorials and examples. While this may seem like low-hanging fruit, we have found these very useful in directing the design of the library. Much of this code is confusing and commented out in places. We should build an examples folder, which we can also use to test the output, to ensure it is correct.
- Make a simple README.md which tells the reader what this library is for.

## High Priority

**After all other issues are dealt with, these should be next**

- Documentation. Currently we are lacking it. When we are feature complete, we should go through each function and provide some direction as to what it does.
- Combinatorics library. We want to augment utility/combinations to become a sub-class of gen_data. This will allow us to create combinations/permutations and subsets from arbitrary data sets. We also want to be able to randomly probe sets of combinations, as they can be very large. Random probing will allow the user to quickly generate random data.
- Finish off random gen_data objects, so that it can be used easily with arrays/vectors and enums.
- Check that code compiles using g++ and clang.
- Run clang-tidy, clang-format on the code.
- Ensure all gen_data_base_t derived objects named constructors return a gen_data_collection_t unary object, as this allows us to create singleton for loop elements.

## Medium Priority

**These issues are important, but right now they are not a priority**

- Add functionality allowing global options object to be changed on a per-test basis.
- Add functionality allowing gen_data_collection<T> objects to have their data filtered.
- Revisit how data is stored in files. For random_data_t and enumerate_data_t it is not necessary to store the failed data using parsers and printers. We still want to allow the user to use this functionality if they want to, however we also want to be able to store data which won't require the user to make fmt/scn specialised structs. 
- Investigate adding functionality for an "always on" file. This would automatically store the failed data from a test, and automatically re-run it when the program is ran again. 
- Augment main CMakeLists.txt file so that the examples subproject is only built when specified - that is, include a new option to allow it to be built.

## Low priority

- The terminology we use should be looked at, standardised, and improved. E.g. "data from a failed test" can encompass many different things. We need consistent terms when discussing things like this.
- Check whether we can re-build library using lower/higher versions of CMake. Currently we have only used what works, and not attempted to target any particular version.
- Migrate to older versions of C++; is it possible to use C++20, 17, 14, 11?
-- std::filesystem could be replaced with boost::filesystem for 14,11.
-- std::expected could have bespoke version built.
-- Concepts would be an issue, these are used heavily in the code. Moving to concept-less code is possible.
- Consider readding back inline namespace containing the version. We removed this because instantiating specialised templates/structs for some of the derived gen_data classes, when using this keyword, wouldn't allow us to create them in named namespaces; we had to define them in a format similar to how fmt does it (see commented out example in examples/include/abc_test_examples/enumeration/enum_enumeration_examples.h. This may be something only done in visual studio, or may be part of the standard. We should investigate further to find the reasoning. Then decide whether including the versioned namespace is worth the trade off, or if there are any workarounds.
- In abc_test/gen_data/enumeration/specializations/fundamental_types.h, there are several specializations which cannot have separate definitions and implementations. We believe this to be an error with Visual Studio, however we will need to investigate further.