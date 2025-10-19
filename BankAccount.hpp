#pragma once
#include <string>
#include <vector>

struct BankTransaction {
    std::string type;   // "Topup" atau "Withdraw"
    double amount;
    std::string date;   // Format: YYYY-MM-DD
};

class BankAccount {
private:
    std::string accountId;
    std::string ownerId; // ID user pemilik akun
    double balance;
    std::vector<BankTransaction> history;

public:
    BankAccount() = default;
    BankAccount(const std::string& accId, const std::string& ownerId);

    void topup(double amount);
    bool withdraw(double amount);
    void printCashflowToday() const;
    void printCashflowMonth() const;

    std::string serialize() const;
    void deserialize(const std::string& line);

    const std::string& getAccountId() const;
    const std::string& getOwnerId() const;
    double getBalance() const;
};
