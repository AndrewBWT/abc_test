# Matchers

This page contains documentation concerning matchers in `abc_test`. It covers the following topics:

- A Proper Introduction To Matchers
- Basic matchers.
- Comparison operators.
- Expression-encoded Matchers.
- Included matchers.
- Annotating Matchers.
- Matchers and logic operators.
- Simulating lazily evaluated logic operators.
- Defining your own matchers.
- The Design of Matchers.

All of the functionality shown in this document  requires the following include directive.

```cpp
#include "abc_test/core.h"
```

# A Proper Introduction To Matchers

In [this document about test assertions]() we provided the reader with a brief introduction to matchers. In this section we provide a re-introduction to matchers.

A matcher in `abc_test` is an object. It has a type of `matcher_t`. In `abc_test` matchers are generic objects which perform some logical check on some program entity. They are passed to assertions, which take the result of the matcher and interacts with the `abc` test framework to report their result.

As matchers are objects, they can be passed around to objects, stored in containers, and generally used as any programmatical construct. On their own they won't "do" anything - the user cannot terminate a test using one, they cannot register a matcher's result with the `abc` test framework.

In the remainder of this document we introduce the reader to the core concepts concerning matchers. In [this document]() provides the documentation for all the defined matchers included with this library - for example, matchers which operate on ranges.

# Basic Matchers

A `matcher_t` is the basic type of object we include. In reality, it is a type synonym for an unannotated `matcher_wrapper_t`. For more information regarding the design of matchers, see [this section]().


