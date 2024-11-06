# Overview of the Framework

This document provides an overview of the testing framework.

After the user has created their tests, they will want to run them. Generally the user will want to run their tests through the abc_test framework. This document details how this framework works, and how to interact with it.

# Running tests

It could be said that the ```test_main_t``` object is the primary way of interfacing with the abc_test library. It has one constructor, which takes an element of type ```const validated_test_options_t<T>&```, one public function ```run_tests```, which essentially runs the all the tests it was told to run.

`validated_test_options_t` is a templated object which can only be instantiated with a ```T```' which is either the same as ```test_options_base_t``` or derived from it. There is only one way to construct an entity of type ```validated_test_options_t```, through the function ```validate_test_options```.

Therefore, we would argue it is more accurate to say that the user interfaces with ```abc_test``` through ```test_main_t```, but how the tests are ran is described through ```test_options_base_t``` - or a derived class.

The following code is all that would be required to run a test suite.

```
run_tests(const test_options_base_t& _a_tests)
{
    validated_test_options_or_error_t _l_res{validate_test_options(_a_tests)};
	if (_l_res.has_value())
	{
	    test_main_t _l_test_main(_l_res.value());
		_l_test_main.run_tests();
	}
	
```

## test_options_base_t Overview

`test_options_base_t` allows the user to describe how their tests are ran. It has a set of public fields, described as follows:

| Field | Type | Default value | Description |
| --- | --- | --- | --- |
| ```path_delimiter``` | ```std::string_view``` | ```"::"``` | This is the ```string``` used to separate a test's ```path``` into its separate components. See XXX for more information regarding this. **Should not be an empty string.** |
| ```root_path``` | ```std::filesystem::path``` | ```std::filesystem::current_path()``` | ```abc_test``` has the ability to write files. Unless explicitly stated, it is assumed relative filenames are given by the user. It is assumed relative filenames have ```root_path``` as their root. **The root_path should exist before the test is ran**. |
| `global_seed` | `global_seed_t` | `global_seed_t()` | The global seed for this run of `abc_test`. See XXX for more information regarding this. |
| `number_of_integers_used_to_seed_random_generators` | `std::size_t` | `100` | The number of integers used to seed other random seeds in this run of `abc_test`. See XXX for more information regarding this. |
| `general_data_extension` | `std::string` | `"dg"` | The data extension used for general data files. See XXX for more information regarding this. |
| `tertiary_data_file_extension` | `std::string` | `"td"` | The data extension used for tertiary data files. See XXX for more information regarding this. |
| `comment_str` | `std::string` | `"#"` | The prefix string used to denote a comment line when reading files for this run of `abc_test`. |
| `write_data_to_files` | `bool` | `true` | Denotes whether this run of `abc_test` will write any data to files. This is a quick way to turn off all file writing. |
| `threads` | `std::size_t` | `std::thread::hardware_concurrency()` | The total number of threads made available to the system when running tests. See XXX for more information regarding this. |
| `map_of_unique_ids_and_for_loop_stack_tries` | `map_unique_id_to_tdg_collection_stack_trie_t` | `Default constructor` | The repetition map used with this run of `abc_test` See XXX for more information regarding this. |
| `use_global_test_list` | `bool` | `true` | Whether to use the GLOT as a source of tests when beginning this run of `abc_test`. |
| `test_reporters` | `vector<std::shared_ptr<test_reporter_t>>` | `Default constructor` | The set of `test_reporter_t` entities used to process test results. See XXX for more information. |
| `error_reporters` | `vector<std::shared_ptr<error_reporter_t>>` | `Default constructor` | The set of `error_reporter_t` entities used to process test errors. See XXX for more information. |
| `test_lists` | `vector<std::test_list_t>` | `Default constructor` | The set of `test_list_t` objects which are used with this run of `abc_test`. This is separate to the GLOT. Using this allows the user to programmatically interface with `abc_test'. |
| `force_run_all_tests` | `bool` | `false` | Usually whether tests are ran depends on the value in map_of_unique_ids_and_for_loop_stack_tries. By setting this to true, `abc_test` will run all tests; if the test has no entry in `map_of_unique_ids_and_for_loop_stack_tries` then the test is ran normally, and with that entry if there is an entry in `map_of_unique_ids_and_for_loop_stack_tries`. If set to false, if `map_of_unique_ids_and_for_loop_stack_tries` has no entries all tests are ran normally. If `map_of_unique_ids_and_for_loop_stack_tries` has any entries, only those tests with entries are ran. |

`included_instances_test_options_t` is an extension of `base_test_options_t`, which takes advantage of the additional mechanisms included in included_instances. The fields of `included_instances_test_options_t` are as follows:

| Field | Type | Default value | Description |
| --- | --- | --- | --- |
| `use_text_test_reporter_to_cout` | `bool` | `true` | If `true`, adds a `text_test_reporter_t` entity to the list of `test_reporters` that prints to `std::cout`. |
| `use_text_error_reporter_to_cout` | `bool` | `true` | If `true`, adds a `text_error_reporter_t` entity to the list of `test_errors` that prints to `std::cout`. |
| `text_test_reporter_file_names` | `std::vector<std::pair<std::filesystem::path, bool>>` | `Default constructor` | For each pair, adds a `text_test_reporter_t` to `test_reporters` |
| `text_error_reporter_file_names` | `bool` | `true` | For each pair, adds a `text_error_reporter_t` to `test_errors` |

# Command line argumnets

We have included a command line argument parser, pased off `CLI11`. It allows the user to parse c

# Global List of Tests (GLOT)

The GLOT is where we imagine the majority of users using this library will store their tests. The GLOT is a global variable which stores a list of test functions, together with their ```user_data_t``` objects.