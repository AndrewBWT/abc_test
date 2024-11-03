# abc test #

abc test (or abc_test) is a C++ testing library. 

abc test's core features are:
- It is a matcher-based testing library (like GTest or Catch2). abc test's version of the matcher can be composed in logical statements, and extended by the user to write their own.
- abc test uses data generators to loop through sets of data, which allows testing code to be re-used.
- By building on scn and fmt, data can be easily streamed to and from files, allowing the user to write their testing code in a file rather than in their source code.
- We provide data generators which can enumerate over a data type, randomly generate data for a data type, iterate over static data in a collection, and stream data from a file.
- Many of our data generators have the functionality to automatically retain problematic data. For example, when generating data randomly the user knows that any problematic value can be retained, and know that if it causes isseus later, it can be revisited easily.
- This way, the user can write their tests in a unit test-like manner, a property-based like-manner, or even a fuzzy-test like-manner. 
- We allow the user to test their code in parallel.

This is a passion/personal project currently, and I would really appreciate any feedback. If this has piqued your interest, please see below for some examples!

# Basic example

This is the most basic example, and shows an example of a classic unit test.
```
#include "abc_test/core.h"
#include "abc_test/included_instances.h"
constexpr f1(int i)
{
    return i*2;
}

_TEST_CASE(
    abc::test_data_t{
        .name        = "Example 1",
        .path        = "examples::basic_examples::001"
    }
)
{
    _CHECK_EXPR(f1(2) == 4);
	_CHECK_EXPR(f1(4) == 7);
}

```
We can see that tests are defined through the object test_data_t. This object allows us to describe the fields of the test; the name and path. The path can be thought of as a folder for the test.

The code itself will check if f1(2) == 4, and if f1(4) == 7.