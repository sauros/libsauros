#ifndef SAUROS_SYSTEM_HPP
#define SAUROS_SYSTEM_HPP

#include <optional>
#include <string>

namespace sauros {
namespace system {

//! \brief Attempt to load the sauros home directory
//!        by looking for SAUROS_HOME. If this environment
//!        variable is not yet setup, then the user's home
//!        directory is scanned for a `.sauros` directory
//! \returns Filled optional with path to sauros directory
//!          iff a directory is found that can be used as
//!          the home directory
extern std::optional<std::string> get_sauros_home_directory();

} // namespace system
} // namespace sauros

#endif