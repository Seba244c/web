#include <array>
#include <cstdio>
#include <filesystem>
#include <format>
#include <functional>
#include <iostream>
#include <string>
#include <string_view>
#include <toml++/toml.hpp>
#include <vector>
using namespace std::string_view_literals;

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

class String {
  public:
    virtual void connect() const = 0;
    virtual ~String() = default;
    virtual const std::string &GetName() const = 0;
};

class SSHString : public String {
  public:
    SSHString(std::string name, std::string addres, std::string user)
        : m_Name(std::move(name)), m_Addres(std::move(addres)),
          m_User(std::move(user)) {}
    void connect() const override {
        std::system(std::format("ssh {}@{}", m_User, m_Addres).c_str());
    }

    const std::string &GetName() const override { return m_Name; }

  private:
    std::string m_Name;
    std::string m_Addres;
    std::string m_User;
};

class App {
  public:
    App() {
        std::filesystem::path config_path;
        const char *config_dir = std::getenv("XDG_CONFIG_HOME");

        if (!config_dir) {
            const char *home_dir = std::getenv("HOME");
            if (!home_dir) {
                std::cout << "Failed to find home directory" << std::endl;
                return;
            }

            config_path = home_dir;
            config_path += "/.config";
        } else {
            config_path += config_dir;
        }

        config_path += "/web.toml";

        std::FILE *file = fopen(config_path.c_str(), "r");
        if (file) {
            fclose(file);
        } else {
            Render::print("Config file not found, creating one at {}",
                          config_path.c_str());
            std::ofstream outfile(config_path);

            outfile << "# Web : SSH Manager, config file" << std::endl;
            outfile << "# Define entries like this" << std::endl;
            outfile << "# [[strings]]" << std::endl;
            outfile << "# name=\"My Name\"" << std::endl;
            outfile << "# type=\"ssh\"" << std::endl;
            outfile << "# address=\"example.com\"" << std::endl;
            outfile << "# user=\"root\"" << std::endl;
            outfile << std::endl;

            outfile << "[[strings]]" << std::endl;
            outfile << "name=\"My Ubuntu Box\"" << std::endl;
            outfile << "type=\"ssh\"" << std::endl;
            outfile << "address=\"example-servers.com\"" << std::endl;
            outfile << "user=\"me\"" << std::endl;

            outfile.close();
        }

        config = toml::parse_file(config_path.c_str());
    }

    std::vector<std::shared_ptr<String>> parse_strings() {
        std::vector<std::shared_ptr<String>> strings = {};

        if (toml::array *arr = config["strings"].as_array()) {
            arr->for_each([&strings](auto &&el) {
                toml::table *string_data = el.as_table();
                const auto &string_type = (*string_data)["type"].value_or(""sv);
                if (string_type.starts_with("ssh")) {
                    const auto address =
                        (*string_data)["address"].value_or(""sv);
                    const auto user = (*string_data)["user"].value_or(""sv);
                    const auto name = (*string_data)["name"].value_or(""sv);
                    strings.emplace_back(std::make_shared<SSHString>(
                        (std::string(name)), (std::string(address)),
                        (std::string(user))));
                }
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
    auto app = Web::App();
    app.menu_main();

    return 0;
}
