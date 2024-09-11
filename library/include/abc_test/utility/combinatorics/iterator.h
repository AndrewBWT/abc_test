#pragma once

#include <cstddef>
#include <vector>
#include "abc_test/utility/combinatorics.h"
#include "abc_test/utility/internal/macros.h"

_BEGIN_ABC_UTILITY_NS
    template<
        typename T
    >
	struct combinatorics_iterator
	{
    public:
        using difference_type = std::ptrdiff_t;
        using value_type = combinatorics<T>::return_container;
        using pointer = void;
        using reference = const value_type&;
        using iterator_category = std::random_access_iterator_tag;
        constexpr
            combinatorics_iterator(
                const combinatorics<T>& _a_combinatorics
            ) noexcept;
	};
_END_ABC_NS
_END_NS