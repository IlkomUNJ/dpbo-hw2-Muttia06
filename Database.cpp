#include "Database.hpp"
#include <fstream>

template<typename T>
std::vector<T> loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<T> list;
    std::string line;
    while (std::getline(file, line)) {
        T obj;
        obj.deserialize(line);
        list.push_back(obj);
    }
    return list;
}

template<typename T>
void saveToFile(const std::vector<T>& list, const std::string& filename) {
    std::ofstream file(filename);
    for (const auto& obj : list)
        file << obj.serialize() << "\n";
}

std::vector<User> Database::loadUsers(const std::string& filename) {
    return loadFromFile<User>(filename);
}

void Database::saveUsers(const std::vector<User>& users, const std::string& filename) {
    saveToFile<User>(users, filename);
}

std::vector<BankAccount> Database::loadBankAccounts(const std::string& filename) {
    return loadFromFile<BankAccount>(filename);
}

void Database::saveBankAccounts(const std::vector<BankAccount>& accounts, const std::string& filename) {
    saveToFile<BankAccount>(accounts, filename);
}

std::vector<Item> Database::loadItems(const std::string& filename) {
    return loadFromFile<Item>(filename);
}

void Database::saveItems(const std::vector<Item>& items, const std::string& filename) {
    saveToFile<Item>(items, filename);
}

std::vector<Transaction> Database::loadTransactions(const std::string& filename) {
    return loadFromFile<Transaction>(filename);
}

void Database::saveTransactions(const std::vector<Transaction>& transactions, const std::string& filename) {
    saveToFile<Transaction>(transactions, filename);
}
