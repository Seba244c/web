#include "Connection.h"
#include <format>
#include <memory>
using namespace std::string_view_literals;

namespace Web {
std::shared_ptr<String> String::Parse(toml::table *string_data) {
    const auto &string_type = (*string_data)["type"].value_or(""sv);
    if (string_type.starts_with("ssh"))
        return SSHString::Parse(string_data);
    return nullptr;
}

void SSHString::connect() const {
    std::system(std::format("ssh {}@{}", m_User, m_Addres).c_str());
}

std::shared_ptr<SSHString> SSHString::Parse(toml::table *string_data) {
    const auto address = (*string_data)["address"].value_or(""sv);
    const auto user = (*string_data)["user"].value_or(""sv);
    const auto name = (*string_data)["name"].value_or(""sv);
    return (std::make_shared<SSHString>(
        (std::string(name)), (std::string(address)), (std::string(user))));
}
} // namespace Web
