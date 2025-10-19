#include "BankAccount.hpp"
#include <sstream>
#include <iostream>
#include <ctime>

BankAccount::BankAccount(const std::string& accId, const std::string& ownerId)
    : accountId(accId), ownerId(ownerId), balance(0.0) {}

void BankAccount::topup(double amount) {
    balance += amount;
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buf[11];
    strftime(buf, sizeof(buf), "%Y-%m-%d", ltm);
    history.push_back({"Topup", amount, buf});
}

bool BankAccount::withdraw(double amount) {
    if (amount > balance) return false;
    balance -= amount;
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buf[11];
    strftime(buf, sizeof(buf), "%Y-%m-%d", ltm);
    history.push_back({"Withdraw", amount, buf});
    return true;
}

void BankAccount::printCashflowToday() const {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char today[11];
    strftime(today, sizeof(today), "%Y-%m-%d", ltm);
    std::cout << "Transaksi Hari Ini:\n";
    for (const auto& h : history) {
        if (h.date == today)
            std::cout << h.date << " - " << h.type << ": " << h.amount << "\n";
    }
}

void BankAccount::printCashflowMonth() const {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    int month = ltm->tm_mon + 1;
    int year = ltm->tm_year + 1900;
    std::cout << "Transaksi Bulan Ini:\n";
    for (const auto& h : history) {
        int y, m, d;
        sscanf(h.date.c_str(), "%d-%d-%d", &y, &m, &d);
        if (y == year && m == month)
            std::cout << h.date << " - " << h.type << ": " << h.amount << "\n";
    }
}

std::string BankAccount::serialize() const {
    std::ostringstream oss;
    oss << accountId << "|" << ownerId << "|" << balance << "|" << history.size();
    for (const auto& h : history)
        oss << "|" << h.type << "," << h.amount << "," << h.date;
    return oss.str();
}

void BankAccount::deserialize(const std::string& line) {
    std::istringstream iss(line);
    std::string countStr;
    std::getline(iss, accountId, '|');
    std::getline(iss, ownerId, '|');
    std::string balStr;
    std::getline(iss, balStr, '|');
    balance = std::stod(balStr);
    std::getline(iss, countStr, '|');
    int count = std::stoi(countStr);
    history.clear();
    for (int i = 0; i < count; ++i) {
        std::string hStr;
        std::getline(iss, hStr, '|');
        std::istringstream hss(hStr);
        std::string type, amtStr, date;
        std::getline(hss, type, ',');
        std::getline(hss, amtStr, ',');
        std::getline(hss, date, ',');
        history.push_back({type, std::stod(amtStr), date});
    }
}

const std::string& BankAccount::getAccountId() const { return accountId; }
const std::string& BankAccount::getOwnerId() const { return ownerId; }
double BankAccount::getBalance() const { return balance; }
