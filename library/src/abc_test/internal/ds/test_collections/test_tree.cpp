#include "abc_test/internal/ds/test_collections/test_tree.hpp"
#include "abc_test/internal/ds/test_collections/test_tree_iterator.hpp"

_BEGIN_ABC_DS_NS
__no_constexpr_or_inline_imp
	test_tree_iterator_t
	test_tree_t::begin(
	) const noexcept
{
	return test_tree_iterator_t(std::ref(*this));
}
__no_constexpr_or_inline_imp
test_tree_iterator_sentinel_t
	test_tree_t::end(
	) const noexcept
{
	return test_tree_iterator_sentinel_t();
}
_END_ABC_DS_NS