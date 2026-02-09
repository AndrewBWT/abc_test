# To-do List for testing in abc_test #

This file contains a list of all the things we want to do in regards to testing abc_test.

## No Particular Order ##

1 - Testing the parsing of repetition configurations should test the following:
  1 - Unable to find the correct number of colons.
  2 - Unable to parse the compressed str
  3 - Unable to parse the name of the function.
  4 - Internal map already contains an element with the given UUID.
2 - Component tests for combinatorics data generator. Specifically:
  1 - Check permutations
  2 - Check combinations
  3 - Check both together.
  4 - Check RC works correctly.
3 - Add rng test for rvalue constructor. There was a bug in it which we didn't catch - and our tests should have done.
4 - Add checks for when tests use the same file names. We recently had to fix a bug that only occurred when using mult-threading, as the same file names were being used. This should be allowed behaviour that fails gracefully - currently it will throw a segmentation fault if the files contain unparsable types.
5 - Add test for "--help" (or "-h") command line option, to ensure it prints out the correct information.