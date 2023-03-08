#include <libsauros/sauros.hpp>

#include <filesystem>
#include <iostream>
#include <string>

namespace {
std::shared_ptr<sauros::environment_c> env =
    std::make_shared<sauros::environment_c>();

sauros::file_executor_c *file_executor{nullptr};

} // namespace

void run_file(const std::string &file) {
  if (!std::filesystem::is_regular_file(file)) {
    std::cerr << "Given item `" << file << "` is not a file" << std::endl;
    std::exit(1);
  }

  file_executor = new sauros::file_executor_c(env);
  file_executor->run(file);

  // Indicate that we are about to quit
  file_executor->finish();
}

int main(int argc, char** argv) {

  if (argc != 2) {
    std::cerr << "Expected file to run!" << std::endl;
    return 1;
  }

  std::filesystem::path target(argv[1]);

  if (!std::filesystem::exists(target)) {
    std::cerr << "Given target: " << target << " does not exist" << std::endl;
    return 1;
  }

  run_file(argv[1]);
  return 0;
}