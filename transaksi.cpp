#include "Transaksi.hpp"
#include <sstream>

Transaction::Transaction(const std::string& id, const std::string& buyerId, const std::string& sellerId,
                         const std::string& itemId, int quantity, double totalPrice,
                         const std::string& date, TransactionStatus status)
    : transactionId(id), buyerId(buyerId), sellerId(sellerId), itemId(itemId),
      quantity(quantity), totalPrice(totalPrice), date(date), status(status) {}

std::string Transaction::getId() const { return transactionId; }
std::string Transaction::getBuyerId() const { return buyerId; }
std::string Transaction::getSellerId() const { return sellerId; }
std::string Transaction::getItemId() const { return itemId; }
int Transaction::getQuantity() const { return quantity; }
double Transaction::getTotalPrice() const { return totalPrice; }
std::string Transaction::getDate() const { return date; }
TransactionStatus Transaction::getStatus() const { return status; }

std::string Transaction::getStatusString() const {
    switch (status) {
        case TransactionStatus::Paid: return "Paid";
        case TransactionStatus::Completed: return "Completed";
        case TransactionStatus::Canceled: return "Canceled";
    }
    return "Unknown";
}

void Transaction::setStatus(TransactionStatus newStatus) {
    status = newStatus;
}

std::string Transaction::serialize() const {
    std::ostringstream oss;
    oss << transactionId << "|" << buyerId << "|" << sellerId << "|" << itemId << "|"
        << quantity << "|" << totalPrice << "|" << date << "|"
        << getStatusString();
    return oss.str();
}

void Transaction::deserialize(const std::string& line) {
    std::istringstream iss(line);
    std::string qtyStr, priceStr, statusStr;
    std::getline(iss, transactionId, '|');
    std::getline(iss, buyerId, '|');
    std::getline(iss, sellerId, '|');
    std::getline(iss, itemId, '|');
    std::getline(iss, qtyStr, '|');
    std::getline(iss, priceStr, '|');
    std::getline(iss, date, '|');
    std::getline(iss, statusStr, '|');

    quantity = std::stoi(qtyStr);
    totalPrice = std::stod(priceStr);
    if (statusStr == "Paid") status = TransactionStatus::Paid;
    else if (statusStr == "Completed") status = TransactionStatus::Completed;
    else status = TransactionStatus::Canceled;
}
