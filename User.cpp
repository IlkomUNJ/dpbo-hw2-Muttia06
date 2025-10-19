#include "User.hpp"
#include <sstream>

User::User(const std::string& id, const std::string& name, Role role)
    : id(id), name(name), role(role) {}

std::string User::getId() const { return id; }
std::string User::getName() const { return name; }
Role User::getRole() const { return role; }

std::string User::getRoleString() const {
    return (role == Role::Buyer) ? "Buyer" : "Seller";
}

std::string User::serialize() const {
    std::ostringstream oss;
    oss << id << "|" << name << "|" << (role == Role::Buyer ? "B" : "S");
    return oss.str();
}

void User::deserialize(const std::string& line) {
    std::istringstream iss(line);
    std::string roleStr;
    std::getline(iss, id, '|');
    std::getline(iss, name, '|');
    std::getline(iss, roleStr, '|');
    role = (roleStr == "B") ? Role::Buyer : Role::Seller;
}
