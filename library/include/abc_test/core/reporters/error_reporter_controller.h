#pragma once
#include "abc_test/core/reporters/error_reporter.h"
#include "abc_test/utility/ptr_utility.h"

_BEGIN_ABC_REPORTERS_NS
	using error_reporters_t = std::vector<utility::shared_and_raw_ptr<error_reporter_t>>;
	struct error_reporter_controller_t
	{
	public:
		/*!
		* Default constructor, contains an empty error_reporter_controller_t
		*/
		__constexpr
			error_reporter_controller_t(
			) noexcept;
		__constexpr
			void
			set_test_options(
				const test_options_t& _a_options
			) noexcept;
		/*!
		* Adds a collectin of setup_tests_log_reporter_t to the object.
		*/
		__constexpr
			void
			add_reporters(
				const error_reporters_t& _a_reporters
			) noexcept;
		/*!
		* Adds a collectin of setup_tests_log_reporter_t to the object.
		*/
		__constexpr
			void
			add_reporters(
				error_reporters_t&& _a_reporters
			) noexcept;
		/*!
		* Allows an error to be processed by all the setup_tests_log_reporter_t elements.
		*/
		__no_constexpr
			void
			report_error(
				const errors::setup_error_t& _a_error
			) noexcept;
		/*!
		* Goes through each setup_tests_log_reporter_t and sends the given message to each.
		* 
		* No termination occours or preparation occours. This function exists for things like warnings.
		*/
		__constexpr
			void
			report_information(
				const std::string_view _a_str
			) noexcept;
		/*!
		* Goes through each internal error_reporter_t and calls its exist function.
		* 
		* Then std::exit is called with an exit code of -1.
		*/
		__constexpr
			void
			hard_exit(
			) noexcept;
		/*!
		* Queries the internal errors, and returns true if the object has been signalled that it
		* should terminate. 
		*/
		__constexpr
			bool
			should_exit(
			) noexcept;
		/*!
		* Checks whether the object should exit, and calls exit if it does. This is a so-called "soft exit".
		* 
		* Returns true if the object has signified that it should terminate, false otherwise. 
		*/
		__constexpr
			bool
			soft_exit(
			) noexcept;
		/*!
		* Returns the number of times a catastrophic error has been encountered. 
		*/
		__constexpr
			std::size_t
			catastrophic_errors(
			) noexcept;
	private:
		error_reporters_t _m_error_reporters;
		const test_options_t* _m_test_options;
		bool _m_should_exit;
		std::size_t _m_catastrophic_errors;
		std::mutex _m_errors_mutex;
		__constexpr
			error_reporter_controller_t(
				const test_options_t* _a_options
			) noexcept;
		template<
			typename T
		>
		__constexpr
			void
			add_reporters_internal(
				T&& _a_reporters
			) noexcept;
		__constexpr
			void
			exit_no_lock(
			) noexcept;
	};
	_END_ABC_REPORTERS_NS

_BEGIN_ABC_REPORTERS_NS
	__constexpr_imp
		error_reporter_controller_t::error_reporter_controller_t(
		) noexcept
		: error_reporter_controller_t(nullptr)
	{

	}
	__constexpr_imp
		void
		error_reporter_controller_t::set_test_options(
			const test_options_t& _a_test_options
		) noexcept
	{
		_m_test_options = &_a_test_options;
	}
	__constexpr_imp
		void
		error_reporter_controller_t::add_reporters(
			const error_reporters_t& _a_reporters
		) noexcept
	{
		add_reporters_internal(std::forward<const error_reporters_t&>(_a_reporters));
	}
	__constexpr_imp
		void
		error_reporter_controller_t::add_reporters(
			error_reporters_t&& _a_reporters
		) noexcept
	{
		add_reporters_internal(std::forward<error_reporters_t&&>(_a_reporters));
	}

	__no_constexpr_imp
		void
		error_reporter_controller_t::report_error(
			const errors::setup_error_t& _a_error
		) noexcept
	{
		using namespace std;
		using namespace utility;
		unique_lock _l_error_reporter_unique_lock(_m_errors_mutex);
		if (_a_error.has_error())
		{
			for (const shared_and_raw_ptr<error_reporter_t>& _l_reporter : _m_error_reporters)
			{
				error_reporter_t* _l_reporter_ptr{ get_ptr(_l_reporter) };
				_l_reporter_ptr->report_error(_a_error);
			}
			if (_a_error.unrecoverable_error())
			{
				_m_catastrophic_errors++;
				_m_should_exit = true;
			}
		}
	}
	__no_constexpr_imp
		void
		error_reporter_controller_t::report_information(
			const std::string_view _a_str
		) noexcept
	{
		using namespace std;
		using namespace utility;
		unique_lock _l_error_reporter_unique_lock(_m_errors_mutex);
		for (const shared_and_raw_ptr<error_reporter_t>& _l_reporter : _m_error_reporters)
		{
			error_reporter_t* _l_reporter_ptr{ get_ptr(_l_reporter) };
			_l_reporter_ptr->report_information(_a_str);
		}
	}
	__constexpr_imp
		void
		error_reporter_controller_t::hard_exit(
		) noexcept
	{
		using namespace std;
		unique_lock _l_error_reporter_unique_lock(_m_errors_mutex);
		exit_no_lock();
		_m_errors_mutex.unlock();
		std::exit(-1);
	}
	__constexpr_imp
		bool
		error_reporter_controller_t::should_exit(
		) noexcept
	{
		using namespace std;
		unique_lock _l_error_reporter_unique_lock(_m_errors_mutex);
		return _m_should_exit;
	}
	__constexpr_imp
		bool
		error_reporter_controller_t::soft_exit(
		) noexcept
	{
		using namespace std;
		unique_lock _l_error_reporter_unique_lock(_m_errors_mutex);
		if (_m_should_exit)
		{
			exit_no_lock();
		}
		return _m_should_exit;
	}
	__constexpr_imp
		std::size_t
		error_reporter_controller_t::catastrophic_errors(
		) noexcept
	{
		using namespace std;
		unique_lock _l_error_reporter_unique_lock(_m_errors_mutex);
		return _m_catastrophic_errors;
	}
	__constexpr_imp
		error_reporter_controller_t::error_reporter_controller_t(
			const test_options_t* _a_options
		) noexcept
		: _m_error_reporters(error_reporters_t())
		, _m_test_options(_a_options)
		, _m_should_exit(false)
		, _m_catastrophic_errors(0)
		, _m_errors_mutex(std::mutex())
	{

	}
	template<
		typename T
	>
	__constexpr_imp
		void
		error_reporter_controller_t::add_reporters_internal(
			T&& _a_reporters
		) noexcept
	{
		_m_error_reporters.insert(_m_error_reporters.end(),
			_a_reporters.begin(), _a_reporters.end());
	}
	__constexpr_imp
		void
		error_reporter_controller_t::exit_no_lock(
		) noexcept
	{
		using namespace std;
		using namespace utility;
		for (const shared_and_raw_ptr<error_reporter_t>& _l_reporter : _m_error_reporters)
		{
			error_reporter_t* _l_reporter_ptr{ utility::get_ptr(_l_reporter) };
			_l_reporter_ptr->exit();
		}
	}
	_END_ABC_REPORTERS_NS