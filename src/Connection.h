#pragma once
#include <memory>
#include <string>
#include <toml++/toml.hpp>

namespace Web {
class String {
  public:
    virtual ~String() = default;

    virtual void connect() const = 0;
    virtual const std::string &GetName() const = 0;

    static std::shared_ptr<String> Parse(toml::table *string_data);
};

class SSHString : public String {
  public:
    SSHString(std::string name, std::string addres, std::string user)
        : m_Name(std::move(name)), m_Addres(std::move(addres)),
          m_User(std::move(user)) {}

    void connect() const override;

    const std::string &GetName() const override { return m_Name; }

    static std::shared_ptr<SSHString> Parse(toml::table *string_data);

  private:
    std::string m_Name;
    std::string m_Addres;
    std::string m_User;
};
} // namespace Web
