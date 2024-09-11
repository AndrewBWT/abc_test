#pragma once
#include "abc_test/matcherS/generic_matcher.h"
#include "abc_test/core/errors/test_library_exception.h"

_BEGIN_ABC_MATCHER_NS
	using matcher_internal_ptr_t = std::shared_ptr<generic_matcher_t>;
	using matcher_internal_ptr_const_ref_t = const matcher_internal_ptr_t&;
	struct matcher_t : public generic_matcher_t
	{
	public:
		__constexpr
			matcher_t(
			) noexcept;
		__constexpr
			matcher_t(
				matcher_internal_ptr_t _a_matcher_internal
			) noexcept;
		__constexpr
			matcher_internal_ptr_const_ref_t
			internal_matcher(
			) const noexcept;
	private:
		matcher_internal_ptr_t _m_matcher_internal;
	private:
		__constexpr
			virtual
			matcher_result_t
			run(
				test_runner_ref_t
			) override;
	};
	_END_ABC_NS
		_END_NS

		_BEGIN_ABC_MATCHER_NS
	__constexpr_imp
		matcher_result_t
		matcher_t::run(
			test_runner_ref_t _a_test_runner
		)
	{
		using namespace std;
		using namespace errors;
		if (_m_matcher_internal.get() != nullptr)
		{
			_m_matcher_internal->run_test(_a_test_runner);
			return _m_matcher_internal->test_result();
		}
		else
		{
			throw test_library_exception_t(
				"_m_matcher_internal contains a nullptr, meaning that it contains no internal matcher. "
				"Therefore, the test cannot be run as it is invalid."
			);
		}
	}
	__constexpr_imp
		matcher_t::matcher_t(
		) noexcept
		: _m_matcher_internal(matcher_internal_ptr_t(nullptr))
	{}
	__constexpr_imp
		matcher_t::matcher_t(
			matcher_internal_ptr_t _a_matcher_internal
		) noexcept
		: _m_matcher_internal(_a_matcher_internal)
	{

	}
	__constexpr_imp
		matcher_internal_ptr_const_ref_t
		matcher_t::internal_matcher(
		) const noexcept
	{
		return _m_matcher_internal;
	}
_END_ABC_NS
_END_NS