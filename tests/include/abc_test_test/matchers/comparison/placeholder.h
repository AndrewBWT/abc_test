#pragma once
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>

#include "abc_test/matcher/comparison/placeholder.h"


TEST_CASE("Placeholder tests", "ab::test")
{
	using namespace ab::test::cmp;
	placeholder_t _l_default_pl;
	CHECK(placeholder_t() == _l_default_pl);
}