#pragma once
#include "abc_test/core.hpp"
#include "abc_test/included_instances.hpp"

_TEST_CASE(
    abc::test_case_t(
        {.name = "Fuzzy tester for user_defined_test_data_t",
         .description
         = "Checks that each of the functions for user_defined_test_data_t are "
           "able to process randomised data",
         .path = "abc_test::internal::ds::test_data::user_defined_test_data",
         .threads_required = 1}
    )
)
{
    using namespace abc;
    using namespace std;
    using namespace abc::ds;
    using test_data_t = tuple<string, optional<string>, string, size_t>;
    for (auto& _l_test_name : generate_data_randomly<string>())
    {
        for (auto& _l_test_description :
             generate_data_randomly<optional<string>>())
        {
            for (auto& _l_path : generate_data_randomly<string>())
            {
                for (auto& _l_n_threads : generate_data_randomly<std::size_t>())
                {
                    user_defined_test_data_t _l_udtd{
                        _l_test_name, _l_test_description, _l_path, _l_n_threads
                    };
                    user_defined_test_data_t _l_udtd2{
                        _l_test_name, _l_test_description, _l_path, _l_n_threads
                    };
                    do_not_optimise(_l_udtd == _l_udtd2);
                    do_not_optimise(fmt::format("{}", _l_udtd));
                }
            }
        }
    }
}

/*
static void const volatile* volatile global_force_escape_pointer;

// FIXME: Verify if LTO still messes this up?
void UseCharPointer(char const volatile* const v) {
  // We want to escape the pointer `v` so that the compiler can not eliminate
  // computations that produced it. To do that, we escape the pointer by storing
  // it into a volatile variable, since generally, volatile store, is not
  // something the compiler is allowed to elide.
  global_force_escape_pointer = reinterpret_cast<void const volatile*>(v);
}

inline BENCHMARK_ALWAYS_INLINE void DoNotOptimize(Tp const& value) {
  internal::UseCharPointer(&reinterpret_cast<char const volatile&>(value));
}
*/