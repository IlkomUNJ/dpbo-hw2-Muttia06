#pragma once
#include <string>

enum class Role {
    Buyer,
    Seller
};

class User {
private:
    std::string id;
    std::string name;
    Role role;

public:
    User() = default;
    User(const std::string& id, const std::string& name, Role role);

    std::string getId() const;
    std::string getName() const;
    Role getRole() const;
    std::string getRoleString() const;

    std::string serialize() const;
    void deserialize(const std::string& line);
};
