#include "abc_test/core/ds/test_data/test_set_data.hpp"
#include "abc_test/core/options/test_options_base.hpp"


_BEGIN_ABC_DS_NS

__no_constexpr_or_inline_imp void
test_set_data_t::process_invoked_test(
    ds::invoked_test_data_t& _a_invoked_test
) noexcept
{
    using namespace std;
    using enum enum_test_status_t;
    const u8string_view _l_delimiter{
        global::get_global_test_options().path_delimiter
    };
    const enum_test_status_t _l_test_status{ _a_invoked_test.test_status() };
    switch (_l_test_status)
    {
    case NO_TERMINATION_TEST_FAILED:
    case TERMINATION_OCCOURED_TEST_FAILED:
    case TERMINATION_OCCOURED_UNEXPECTED_THROW:
        _m_map_ids_to_tdg_collection_stack_tries.insert(
            string{ _a_invoked_test.post_setup_test_data().unique_id() },
            _a_invoked_test.for_loop_stack_trie()
        );
        break;
    default:
        break;
    }
    const bool               _l_test_passed{ test_passed(_l_test_status) };
    _m_total_tests_ran += 1;
    _m_total_tests_passed += _l_test_passed ? 1 : 0;
    _m_total_tests_failed += (not _l_test_passed) ? 1 : 0;
    _m_total_tests_failed_and_terminated
        += (_l_test_status == TERMINATION_OCCOURED_TEST_FAILED) ? 1 : 0;
    _m_total_tests_failed_but_not_terminated
        += (_l_test_status == NO_TERMINATION_TEST_FAILED) ? 1 : 0;
    _m_total_tests_designated_failure_due_to_exception
        += (_l_test_status == TERMINATION_OCCOURED_UNEXPECTED_THROW) ? 1 : 0;
    _m_total_assertions_ran += _a_invoked_test.assertions_recieved();
    _m_total_assertions_passed += _a_invoked_test.assertions_passed();
    _m_total_assertions_failed += _a_invoked_test.assertions_failed();
}

_END_ABC_DS_NS