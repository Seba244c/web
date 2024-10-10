#pragma once
#include <filesystem>

namespace Web::Utils {
std::filesystem::path get_web_config_file();
bool file_exists(const char *file_path);
} // namespace Web::Utils
