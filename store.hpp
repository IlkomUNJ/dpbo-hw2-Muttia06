#pragma once
#include "Item.hpp"
#include "Transaksi.hpp"
#include <vector>
#include <string>

class Store {
private:
    std::vector<Item> items;
    std::vector<Transaction> transactions;

public:
    Store() = default;

    void loadItems(const std::vector<Item>& loadedItems);
    void loadTransactions(const std::vector<Transaction>& loadedTxs);
    const std::vector<Item>& getAllItems() const;
    const std::vector<Transaction>& getAllTransactions() const;

    std::vector<Item> getItemsBySeller(const std::string& sellerId) const;
    std::vector<Transaction> getTransactionsByBuyer(const std::string& buyerId) const;
    std::vector<Transaction> getTransactionsBySeller(const std::string& sellerId) const;

    bool purchaseItem(const std::string& buyerId, const std::string& itemId, int quantity,
                      const std::string& trxId, const std::string& date, Transaction& outTrx);

    void addItem(const Item& item);
    bool removeItem(const std::string& itemId, const std::string& sellerId);

    std::vector<std::pair<std::string, int>> getTopItems(int k) const;
    std::vector<std::pair<std::string, int>> getTopBuyers(int k) const;
    std::vector<std::pair<std::string, int>> getTopSellers(int k) const;
};
