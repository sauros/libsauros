#ifndef SAUROS_PROCESSOR_HPP
#define SAUROS_PROCESSOR_HPP

#include "libsauros/builtin_encodings.hpp"
#include "libsauros/cell.hpp"
#include "libsauros/environment.hpp"
#include "libsauros/exceptions.hpp"

#include "libsauros/types.hpp"
#include <array>
#include <exception>
#include <functional>
#include <optional>
#include <set>
#include <tuple>
#include <vector>

namespace sauros {

//! \brief A list processor
class processor_c {
public:
  //! \brief Construct the processor
  processor_c();

  //! \brief Destruct the processor
  ~processor_c();

  //! \brief Process a cell (generated from parser, or otherwise)
  //! \param cell The cell to process
  //! \param global_env The environment to use
  //! \returns Optional cell. If a cell is returned, no errors arose
  //! \note Upon an error occuring `runtime_exception_c` will be thrown
  cell_t process_cell(cell_t cell, env_ptr env);

  //! \brief Convert a cell to a string - Something that can be printed to the
  //! screen \param out[out] The string to populate \param cell[in] The cell to
  //! string \param env The environment to pull cell information from \param
  //! show_space Defaults to true, inserts spaces between items
  void cell_to_string(std::string &out, cell_t cell, env_ptr,
                      bool show_space = false);

  //! \brief Reset the processor
  void reset();

  //! \brief Load something that may or may not be a variable
  //! \param source The raw cell that holds variable data
  //! \param env The environment to look in
  //! \note This helps load variables that may or may not
  //!       contain accessors (this.that.inner)
  cell_t load_potential_variable(cell_t source, env_ptr env);

private:
  std::array<cell_t, BUILTIN_ENTRY_COUNT> _builtins;

  void populate_standard_builtins();

  void quote_cell(std::string &out, cell_t cell, env_ptr env);

  cell_t process_list(cells_t &cells, env_ptr env);

  cell_t process_lambda(cell_t cell, cells_t &cells, env_ptr env);

  cell_t clone_box(cell_t cell);

  void load_package(cell_t cell, location_s *location, env_ptr env);

  static std::tuple<cell_t, std::string, env_ptr>
  retrieve_box_data(cell_t &cell, std::shared_ptr<environment_c> &env);

  bool _break_loop{false};
  cell_t _yield_cell{nullptr};
  processor_c *_sub_processor{nullptr};
};

} // namespace sauros

#endif