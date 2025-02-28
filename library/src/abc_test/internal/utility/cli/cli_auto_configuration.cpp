#include "abc_test/internal/utility/cli/cli_auto_configuration.hpp"

/*#include "abc_test/included_instances/options/option_config.hpp"
#include "abc_test/internal/options/test_options_base.hpp"
#include "abc_test/internal/utility/printers/specializations/fundamental_types.hpp"
#include "abc_test/internal/utility/printers/specializations/stl_11.hpp"
#include "abc_test/internal/utility/printers/specializations/stl_98.hpp"

#include <fstream>

_BEGIN_ABC_NS
__no_constexpr_or_inline_imp void
    cli_auto_configuration_t::setup_next_file(
        const bool                 _a_test_success,
        const test_options_base_t& _a_options
    ) const noexcept
{
    switch (_m_rep_write_data_type)
    {
    case rep_write_data_type_t::ALWAYS_WRITE:
        prepare_file(_a_test_success, _a_options);
        break;
    case rep_write_data_type_t::AUTO:
        if ((not this->_m_loaded_configuration.has_value() && not _a_test_success) ||
            (_m_loaded_configuration.has_value() && _a_test_success))
        {
            prepare_file(_a_test_success, _a_options);
        }
    default:
        break;
    }
}

void
    cli_auto_configuration_t::prepare_file(
        bool                       _a_test_success,
        const test_options_base_t& _a_options
    ) const noexcept
{
    using namespace std;
    ofstream _l_output;
    size_t   _l_next_index;
    if (_m_last_config_info.has_value())
    {
        _l_next_index = _m_last_config_info.value().last_index + 1;
        if (_l_next_index > _m_last_config_info.value().max_index)
        {
            const filesystem::path _l_new_path{
                filesystem::path(_m_repetition_folder)
                    .append(fmt::format(
                        "{0}_{1}_{2}",
                        global::get_global_test_options().autofile_name,
                        _m_last_config_info.value().max_index + 1,
                        _m_last_config_info.value().max_index
                            + global::get_global_test_options().autofile_size
                    ))
            };
            _l_output.open(_l_new_path, ios::app);
            // Create new file.
        }
        else
        {
            _l_output.open(this->_m_last_config_info.value().file, ios::app);
        }
    }
    else
    {
        _l_next_index = 1;
        const filesystem::path _l_new_path{
            filesystem::path(_m_repetition_folder)
                .append(fmt::format(
                    "{0}_{1}_{2}",
                    global::get_global_test_options().autofile_name,
                    1,
                    +global::get_global_test_options().autofile_size
                ))
        };
        _l_output.open(_l_new_path, ios::app);
    }
    using namespace detail;
    _l_output << "metadata_"
              << abc::utility::printer::default_printer_t<pair<bool, size_t>>{}
                     .run_printer(make_pair(_a_test_success, _l_next_index))
              << std::endl;
    _l_output << get<0>(_s_autofile_name) << " = " << _a_options.autofile_name
              << std::endl;
    _l_output << get<0>(_s_autofile_size) << " = " << _a_options.autofile_size
              << std::endl;
    _l_output << get<0>(_s_comment_str) << " = " << _a_options.comment_str
              << std::endl;

    _l_output << get<0>(_s_force_run_all_tests) << " = "
              << (_a_options.force_run_all_tests ? "true" : "false")
              << std::endl;
    _l_output << get<0>(_s_general_data_file_extension) << " = "
              << _a_options.general_data_extension << std::endl;
    _l_output << get<0>(_s_global_seed) << " = " <<

        (_a_options.global_seed.complete_global_seed()
                 .value()
                 .integer()
                 .has_value()
             ? fmt::format(
                   "{0}",
                   _a_options.global_seed.complete_global_seed()
                       .value()
                       .integer()
                       .value()
               )
             : fmt::format(
                   "{0}",
                   _a_options.global_seed.complete_global_seed()
                       .value()
                       .vector_of_integers()
                       .value()
               ))
              << std::endl;
    _l_output
        << get<0>(_s_repetition_config) << " = "
        << ds::print_compressed_map_of_unique_ids_to_tdg_collection_stack_tries(
               _a_options.map_of_unique_ids_and_for_loop_stack_tries
           )
        << std::endl;
    _l_output << get<0>(_s_integers_used_for_rng_generation) << " = "
              << _a_options.number_of_integers_used_to_seed_random_generators
              << std::endl;
    _l_output << get<0>(_s_path_delimiter) << " = " << _a_options.path_delimiter
              << std::endl;
    _l_output << get<0>(_s_repetition_file_metadata_string) << " = "
              << _a_options.autofile_metadata_string << std::endl;
    _l_output << get<0>(_s_root_path) << " = " << _a_options.root_path.string()
              << std::endl;
    _l_output << get<0>(_s_show_configuration_explanations) << " = "
              << (_a_options.show_configuration_explanations ? "true" : "false")
              << std::endl;
    _l_output << get<0>(_s_tertiary_data_file_extension) << " = "
              << _a_options.tertiary_data_file_extension << std::endl;
    _l_output << get<0>(_c_test_paths_to_run) << " = ";
    for (size_t _l_idx{ 0 }; const string_view _l_str : _a_options.test_paths_to_run)
    {
        if (_l_str == "")
        {
            continue;
        }
        _l_output << _l_str;
        if (_l_idx + 1 < _a_options.test_paths_to_run.size())
        {
            _l_output << " ";
        }
        ++_l_idx;
    }
    _l_output << endl;
    _l_output << get<0>(_s_threads) << " = " << _a_options.threads << std::endl;
    _l_output << get<0>(_s_global_test_list) << " = "
              << (_a_options.use_global_test_list ? "true" : "false")
              << std::endl;
    _l_output << get<0>(_s_write_data_to_files) << " = "
              << (_a_options.write_data_to_files ? "true" : "false")
              << std::endl;
    _l_output.close();
}

_END_ABC_NS*/