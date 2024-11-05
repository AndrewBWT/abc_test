# Ideas for abc-test #

## Name ##
- Currently we use abc-test for our name. We also use "abc_test" for the folder name. We should be more consistent with this, as it could lead us to a point where people use "abc_test" or "abc-test", and using the wrong one in the wrong place could cause errors for the user.
- Further to this, is "abc test" the best name for this project? Originally we used "ab test" because I have used the moniker "ab" for my personal projects for a while now. "abc test" is designed to tell the user it is "easy" to use. But in reality, the goals of this project are far broader than making an easy to use testing library.

## Features ##
- We should warn the user when we are opening a file in more than one location at the same time. We should differentiate between when this occurs because the user is running tests in parallel (is there even a solution to this which doesn't involve blocking?), and when the user opens the same file in a hierarchy of gen_data for loops. In that instance, it is an error and program should terminate. 
- Combinatorics gen_data_t object. We want the user to be easily able to create combinations/permutations/sub sets or combinations of all of the above from a collection of data. We also want them to be able to randomly probe this set (as it can be massive). For this random probing, we will need a "BigInteger" class.
- A "test mode" which automatically collects errors and when the executable is re-run, runs those errors (if the program wasn't re-compiled since then?).
- Allow the temporary changing of the global test_options_t object for individual tests. 
- Allow the definition of specific test_options_t for groups of tests.