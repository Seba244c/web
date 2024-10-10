#include "Utils.h"
#include <iostream>

namespace Web::Utils {
std::filesystem::path get_web_config_file() {
    std::filesystem::path config_path;
    const char *config_dir = std::getenv("XDG_CONFIG_HOME");

    if (!config_dir) {
        const char *home_dir = std::getenv("HOME");
        if (!home_dir) {
            std::cerr << "Failed to find home directory" << std::endl;
            return "";
        }

        config_path = home_dir;
        config_path += "/.config";
    } else {
        config_path += config_dir;
    }

    config_path += "/web.toml";

    return config_path;
}
bool file_exists(const char *file_path) {
    std::FILE *file = fopen(file_path, "r");
    if (file) {
        fclose(file);
        return true;
    }
    return false;
}
} // namespace Web::Utils
