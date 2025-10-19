#pragma once
#include <string>

enum class TransactionStatus {
    Paid,
    Completed,
    Canceled
};

class Transaction {
private:
    std::string transactionId;
    std::string buyerId;
    std::string sellerId;
    std::string itemId;
    int quantity;
    double totalPrice;
    std::string date; // YYYY-MM-DD
    TransactionStatus status;

public:
    Transaction() = default;
    Transaction(const std::string& id, const std::string& buyerId, const std::string& sellerId,
                const std::string& itemId, int quantity, double totalPrice, const std::string& date,
                TransactionStatus status);

    std::string getId() const;
    std::string getBuyerId() const;
    std::string getSellerId() const;
    std::string getItemId() const;
    int getQuantity() const;
    double getTotalPrice() const;
    std::string getDate() const;
    TransactionStatus getStatus() const;
    std::string getStatusString() const;

    void setStatus(TransactionStatus newStatus);

    std::string serialize() const;
    void deserialize(const std::string& line);
};
