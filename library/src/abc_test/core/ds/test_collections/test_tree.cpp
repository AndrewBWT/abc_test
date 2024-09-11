#include "abc_test/core/ds/test_collections/test_tree.h"
#include "abc_test/core/ds/test_collections/test_tree_iterator.h"

_BEGIN_ABC_NS
namespace ds
{
	__no_constexpr_or_inline_imp
		test_tree_iterator_t
		test_tree_t::begin(
		) const noexcept
	{
		return test_tree_iterator_t(this);
	}
	__no_constexpr_or_inline_imp
		test_tree_iterator_t
		test_tree_t::end(
		) const noexcept
	{
		return test_tree_iterator_t(nullptr);
	}
}
_END_ABC_NS