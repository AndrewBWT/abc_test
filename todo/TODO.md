# To-do List for abc-test #

**Update 31/10/24: TODO**

Entities in the todo list are organised by priority from 1 to 5
- 1 is the highest priority, and must be rectified ASAP.
- 2 is high priority. There should be a plan for working towards addressing it.
- 3 is medium priority. It should be planned for, however it is only actively worked on when there is nothing of a higher priority.
- 4 low priority. It is not planned for, and it being addressed will realistically not change the trajectory of the project. However, it should be addressed at some point.
- 5 lowest priority. These issues would be nice, and will help the project. However generally low-hanging fruit goes here.

Each todo item has a date associated with it. We will try to address the elements in date order, though this may not always be possible.

## Priority 1

No items.

## Priority 2

Added 31/12/24:
- [x] Fix parsing of seeds from the command line/configuration files.
- [ ] Fix parsing function for repetition map elements.
- [ ] Create enumeration examples. 
- [ ] Create manual for loop examples.
- [ ] Create examples showing tests being re-ran with seeds.
- [ ] Work on README.md, showing an introduction to abc_test.

## Priority 3

Added 31/12/24:
- [ ] Documentation of code. Currently generally we have a good percentage of the code commented. However it has not been proof-read, and there are parts of it whose names have changed.
- [ ] Add combinatorics generator. 
- [ ] Add examples showing use of combinatorics generator.
- [ ] Add random generator using enumeration.
- [ ] Add examples showing random generator using enumeration.
- [ ] Add random_generator_object specialisations for arrays, vectors and enums.
- [ ] Run clang-tidy on the code.
- [ ] Check if code compiles using g++ and clang.

## Priority 4

Added 31/12/24:
- [ ] Add functionality for per-test options object. It should be able to be set via either a name (e.g. string), an object or a file.
- [ ] Add functionality allowing data_generator_t objects to have their code filtered.
- [ ] Investigate use of configuration files through CLI11, or our own type.
- [ ] Change master CMakeLists.txt file so that examples subfolder is only built when specified. 


## Priority 5

Added 31/12/24:
- [ ] Terminology and documentation is inconsistent and confusing. It needs to be improved drasticly.
- [ ] Consider supporting different versions of CMake, VS, C++ etc.
- [ ] Consider an inline namespace containing the version. We removed this because instantiating specialised templates/structs for some of the derived gen_data classes, when using this keyword, wouldn't allow us to create them in named namespaces; we had to define them in a format similar to how fmt does it (see commented out example in examples/include/abc_test_examples/enumeration/enum_enumeration_examples.h. This may be something only done in visual studio, or may be part of the standard. We should investigate further to find the reasoning. Then decide whether including the versioned namespace is worth the trade off, or if there are any workarounds.
- [ ] In abc_test/gen_data/enumeration/specializations/fundamental_types.h, there are several specializations which cannot have separate definitions and implementations (as in, we cannot separate them without an error being encountered). We believe this to be an error with Visual Studio, however we will need to investigate further.
- [ ] Change text_test_reporter_t and its associated printers to not print information that is not important.
- [ ] Consider alternate, more space-effecient methods for storing seeds. Hex seems a bit wasteful.

## Completed

Priority 2:
31/10/24:
- Change repetition map to allow key of EITHER unique id OR a source_location. This can allow us to circumvent the issues we are having in running code using repetitions when we may be adding/removing elements from the code base.
- Add configuration file-loading using CLI11 to library.