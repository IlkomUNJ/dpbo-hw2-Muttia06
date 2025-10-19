#pragma once
#include "User.hpp"
#include "BankAccount.hpp"
#include "Item.hpp"
#include "Transaksi.hpp"
#include <vector>
#include <string>

class Database {
public:
    static std::vector<User> loadUsers(const std::string& filename);
    static void saveUsers(const std::vector<User>& users, const std::string& filename);

    static std::vector<BankAccount> loadBankAccounts(const std::string& filename);
    static void saveBankAccounts(const std::vector<BankAccount>& accounts, const std::string& filename);

    static std::vector<Item> loadItems(const std::string& filename);
    static void saveItems(const std::vector<Item>& items, const std::string& filename);

    static std::vector<Transaction> loadTransactions(const std::string& filename);
    static void saveTransactions(const std::vector<Transaction>& transactions, const std::string& filename);
};
