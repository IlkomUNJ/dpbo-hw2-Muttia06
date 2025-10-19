#include "Store.hpp"
#include <algorithm>
#include <map>

void Store::loadItems(const std::vector<Item>& loadedItems) {
    items = loadedItems;
}

void Store::loadTransactions(const std::vector<Transaction>& loadedTxs) {
    transactions = loadedTxs;
}

const std::vector<Item>& Store::getAllItems() const {
    return items;
}

const std::vector<Transaction>& Store::getAllTransactions() const {
    return transactions;
}

std::vector<Item> Store::getItemsBySeller(const std::string& sellerId) const {
    std::vector<Item> result;
    for (const auto& item : items)
        if (item.getSellerId() == sellerId)
            result.push_back(item);
    return result;
}

std::vector<Transaction> Store::getTransactionsByBuyer(const std::string& buyerId) const {
    std::vector<Transaction> result;
    for (const auto& trx : transactions)
        if (trx.getBuyerId() == buyerId)
            result.push_back(trx);
    return result;
}

std::vector<Transaction> Store::getTransactionsBySeller(const std::string& sellerId) const {
    std::vector<Transaction> result;
    for (const auto& trx : transactions)
        if (trx.getSellerId() == sellerId)
            result.push_back(trx);
    return result;
}

bool Store::purchaseItem(const std::string& buyerId, const std::string& itemId, int quantity,
                         const std::string& trxId, const std::string& date, Transaction& outTrx) {
    for (auto& item : items) {
        if (item.getId() == itemId && item.getStock() >= quantity) {
            item.reduceStock(quantity);
            double total = item.getPrice() * quantity;
            outTrx = Transaction(trxId, buyerId, item.getSellerId(), itemId, quantity, total, date, TransactionStatus::Paid);
            transactions.push_back(outTrx);
            return true;
        }
    }
    return false;
}

void Store::addItem(const Item& item) {
    items.push_back(item);
}

bool Store::removeItem(const std::string& itemId, const std::string& sellerId) {
    auto it = std::remove_if(items.begin(), items.end(), [&](const Item& i) {
        return i.getId() == itemId && i.getSellerId() == sellerId;
    });
    if (it != items.end()) {
        items.erase(it, items.end());
        return true;
    }
    return false;
}

std::vector<std::pair<std::string, int>> Store::getTopItems(int k) const {
    std::map<std::string, int> freq;
    for (const auto& trx : transactions)
        if (trx.getStatus() == TransactionStatus::Completed)
            freq[trx.getItemId()] += trx.getQuantity();

    std::vector<std::pair<std::string, int>> sorted(freq.begin(), freq.end());
    std::sort(sorted.begin(), sorted.end(), [](auto& a, auto& b) {
        return b.second < a.second;
    });
    if (sorted.size() > k) sorted.resize(k);
    return sorted;
}

std::vector<std::pair<std::string, int>> Store::getTopBuyers(int k) const {
    std::map<std::string, int> count;
    for (const auto& trx : transactions)
        count[trx.getBuyerId()]++;

    std::vector<std::pair<std::string, int>> sorted(count.begin(), count.end());
    std::sort(sorted.begin(), sorted.end(), [](auto& a, auto& b) {
        return b.second < a.second;
    });
    if (sorted.size() > k) sorted.resize(k);
    return sorted;
}

std::vector<std::pair<std::string, int>> Store::getTopSellers(int k) const {
    std::map<std::string, int> count;
    for (const auto& trx : transactions)
        count[trx.getSellerId()]++;

    std::vector<std::pair<std::string, int>> sorted(count.begin(), count.end());
    std::sort(sorted.begin(), sorted.end(), [](auto& a, auto& b) {
        return b.second < a.second;
    });
    if (sorted.size() > k) sorted.resize(k);
    return sorted;
}
