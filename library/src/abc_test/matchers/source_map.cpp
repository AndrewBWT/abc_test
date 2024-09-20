#include "abc_test/matchers/source_map.h"
#include "abc_test/global.h"

_BEGIN_ABC_NS
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
				location_string(global::get_this_threads_test_runner_ptr()->most_recent_source()))
			;
	}
	else
	{
		using namespace std;
		string _l_rv{};
		for (const pair<string, vector<string>>& _l_element : _m_internal_map)
		{
			string _l_rv_element{ fmt::format("\n  Location: {}",_l_element.first) };
			for (const string& _l_str : _l_element.second)
			{
				_l_rv_element.append("\n    Expression: " + _l_str);
			}
			_l_rv.append(_l_rv_element);
		}
		return _l_rv;
	}
}

_END_ABC_NS