# Documentation

This is the main documentation page for `abc_test`, a C++ testing library.

We provide hand-written documentation and Doxygen generated documentation to the reader. We envisage that the majority of readers will only need the user-written documentation. Below we provide a brief overview of each of the hand-written documents, which should help the reader understand which file they need to read to find the answer to their current question.

Finally, if the reader is looking to learn how to use `abc_test`, we point the reader to the folder `tutorials_and_examples`.

## General Framework
[This document](@ref overview_of_framework) provides a high-level overview of how `abc_test` works.

# Organisation of `abc_test`

`abc_test` has three high-level folders:

- `external`. This folder contains external libraries not part of `abc_test` which we have decided not to include using `cmake`.
- `internal`. This folder contains the core functionality of `abc_test`. If you are willing (or want to) write your own data generators and matchers, you would only need to use the entities in this folder. The abstract constructs are contained in this folder.
- `included_instances`. This folder contains the useful entities; matchers, data generators, command line interfaces, test and error reporters.

# Overview of documentation

There is a lot of other documentation with `abc_test`. There is the automatically doxygen generated documentation, which contains information about specific functions - both internal and external. Regarding the project at large; generally a good rule-of-thumb is that any entity in the `abc` namespace is designed to be used by users. Everything else by developers.

We also have hand-written documentation as well. Below is an overview of each file of hand-written documentation. This should help the user figure out what they want to know. 

If the user is looking for tutorials, see the `examples_and_tutorials` subfolder.

# General Framework

This document describes, from a high-level viewpoint, how `abc_test` works. We discuss the core components of the framework; test registration, test preparation, test running and termination. This is not a tutorial on the internal components of the system, rather we introduce terminology which should help the reader to understand other parts of the system.

# Declaring Tests

Each test ran by `abc_test` must be defined in some form, registered with `abc_test` and ran. In this file, we show every way that a test can be registered with the overarching system.

# Test Assertions

In this file we discuss each way that an assertion can be written in a test. We also discuss the result of a test. And exception handling and blocks.

# Abstract Data Generators

About abstract data generators.

# Static Data Generators

About the included static data generators.

# File based data generators

About the file based ones.

# Random based

# Enumeration

# Combinatorics

# Manual

# Logging

In this file we discuss how logging is performed using `abc_test`.

# Test Reporters

Documentation regarding test reporters. We also include documentation for the included reporte rtext_test_reporter.

# Error reporters
Docs for abstract error reporters. We also include docs for the included error_reporter text_error_reporter.

# Repetition Data

Information about repetition data.

# Test options

The front-end to the library.

# Random generators

Information about the random generators used.

# Threading

Information about threading.

# Command Line Interface

About the command line interface.

