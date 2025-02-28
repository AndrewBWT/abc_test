#include "abc_test/internal/ds/test_data/finalised_test_set_data.hpp"
#include "abc_test/internal/options/test_options_base.hpp"

/*_BEGIN_ABC_DS_NS
template<typename T>
__no_constexpr_or_inline_imp
T finalised_test_set_data_t::get_re_run_test_options() const noexcept
{
    T _l_opt{ global::get_global_test_options() };
    _l_opt.force_run_all_tests = false;
    _l_opt.global_seed = global::get_global_seed();
    _l_opt.map_of_unique_ids_and_for_loop_stack_tries = this->_m_map_ids_to_tdg_collection_stack_tries;
    _l_opt.write_data_to_files = false;
    return _l_opt;
}
_END_ABC_DS_NS*/