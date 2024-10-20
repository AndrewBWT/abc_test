#include "abc_test/internal/matchers/source_map.h"
#include "abc_test/internal/global.h"

_BEGIN_ABC_MATCHER_NS
__no_constexpr_or_inline_imp
	std::string
	matcher_source_map_t::gather_list_of_sources_and_representations(
	) const noexcept
{
	using namespace utility::str;
	if (_m_internal_map.size() == 0)
	{
		return
			fmt::format("\n  None found. Check test output (failing binary expression will "
				"be in the form <false>. However, through logic operators this could be made to pass "
				"some overarching test). The last reported source code location was \"{0}\"."
				"We suggest checking the control flow of the program after this point. ",
				"location str")
				//location_string(global::get_this_threads_test_runner_ptr()->most_recent_source()))
			;
	}
	else
	{
		using namespace std;
		string _l_rv{};
		return _l_rv;
	}
}

_END_ABC_MATCHER_NS