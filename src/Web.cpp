#include "Connection.h"
#include "Utils.h"
#include <argparse/argparse.hpp>
#include <cstdio>
#include <filesystem>
#include <format>
#include <functional>
#include <iostream>
#include <string>
#include <string_view>
#include <toml++/toml.hpp>
#include <vector>

namespace Web {
namespace Render {

struct MenuOption {
    const char *Desc;
    std::function<void()> Select;
};
struct Menu {
    const char *Title;
    const std::vector<MenuOption> Options;
};

template <typename... Args>
void print(std::string_view rt_fmt_str, Args &&...args) {
    std::cout << std::vformat(rt_fmt_str, std::make_format_args(args...))
              << "\n";
}

bool try_parse(std::string &input, int &output) {
    try {
        output = std::stoi(input);
    } catch (std::invalid_argument) {
        return false;
    }
    return true;
}

static int get_int(int min, int max) {
    std::cout << "> ";

    std::string input;
    int out = -1;

    std::getline(std::cin, input);
    while (!try_parse(input, out) || out > max || out < min) {
        std::cout << "Bad Entry: ";
        getline(std::cin, input);
    }

    return out;
}

static void render_menu(Menu menu) {
    print("[== {} ==]", menu.Title);

    short range = 0;
    for (auto const &option : menu.Options) {
        print(" [{}] {}", range++, option.Desc);
    }

    auto selected_option = get_int(0, range - 1);
    menu.Options.at(selected_option).Select();
}

} // namespace Render

class App {
  public:
    static void default_config(std::ostream &out) {
        out << "# Web : SSH Manager, config file" << std::endl;
        out << "# Define entries like this" << std::endl;
        out << "# [[strings]]" << std::endl;
        out << "# name=\"My Name\"" << std::endl;
        out << "# type=\"ssh\"" << std::endl;
        out << "# address=\"example.com\"" << std::endl;
        out << "# user=\"root\"" << std::endl;
        out << std::endl;

        out << "[[strings]]" << std::endl;
        out << "name=\"My Ubuntu Box\"" << std::endl;
        out << "type=\"ssh\"" << std::endl;
        out << "address=\"example-servers.com\"" << std::endl;
        out << "user=\"me\"" << std::endl;
    }

    App() {
        const auto config_path = Web::Utils::get_web_config_file();

        // Create default config if one is not found
        if (!Utils::file_exists(config_path.c_str())) {
            Render::print("Config file not found, creating one at {}",
                          config_path.c_str());
            std::ofstream outfile(config_path);

            default_config(outfile);

            outfile.close();
        }

        // Parse Config
        config = toml::parse_file(config_path.c_str());
    }

    std::vector<std::shared_ptr<String>> parse_strings() {
        std::vector<std::shared_ptr<String>> strings = {};

        if (toml::array *arr = config["strings"].as_array()) {
            arr->for_each([&strings](auto &&el) {
                toml::table *string_data = el.as_table();
                strings.emplace_back(String::Parse(string_data));
            });
        }

        return strings;
    }

    void menu_main() {
        if (!config["strings"]) {
            std::cout << "Found 0 configured strings" << std::endl;
            return;
        }
        const auto strings = parse_strings();
        std::vector<Render::MenuOption> options{};

        for (const auto &string : strings) {
            options.emplace_back(
                Render::MenuOption{string->GetName().c_str(),
                                   [string]() -> void { string->connect(); }});
        }

        Web::Render::render_menu({"Web : SSH Manager", {options}});
    }

  private:
    toml::table config;
};

} // namespace Web

int main(int argc, char *argv[]) {
    // Handle Arguments
    argparse::ArgumentParser program("web", WEB_VERSION);

    /// Alternative Run Types, mutualyl exclusive
    auto &group = program.add_mutually_exclusive_group();
    group.add_argument("--config")
        .help("prints config and config path, and exits")
        .default_value(false)
        .implicit_value(true);
    group.add_argument("--default-config")
        .help("prints the defualt config, and exits")
        .default_value(false)
        .implicit_value(true);

    // Parse
    try {
        program.parse_args(argc, argv);
    } catch (const std::exception &err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        return 1;
    }

    // Alternate run states or stuff to do before main app
    if (program["--config"] == true) {
        const auto config_path = Web::Utils::get_web_config_file();

        std::cout << "Config path: " << config_path.c_str() << std::endl;

        if (Web::Utils::file_exists(config_path.c_str())) {
            std::ifstream f(config_path.c_str());
            if (f.is_open()) {
                std::cout << f.rdbuf();
                std::cout << std::endl << "^ Config File ^" << std::endl;
                f.close();
            } else {
                std::cerr << "Failed to open config file!" << std::endl;
                return -1;
            }
        } else {
            std::cout << "Config file not found, creating one" << std::endl;
            std::ofstream outfile(config_path.c_str());

            Web::App::default_config(outfile);
            Web::App::default_config(std::cout);
            std::cout << std::endl << "^ Config File ^" << std::endl;

            outfile.close();
        }

        return 0;
    } else if (program["--default-config"] == true) {
        Web::App::default_config(std::cout);
        return 0;
    }

    // Main App
    auto app = Web::App();
    app.menu_main();

    return 0;
}
