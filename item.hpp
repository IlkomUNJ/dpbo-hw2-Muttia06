#pragma once
#include <string>

class Item {
private:
    std::string itemId;
    std::string name;
    double price;
    int stock;
    std::string sellerId;

public:
    Item() = default;
    Item(const std::string& id, const std::string& name, double price, int stock, const std::string& sellerId);

    std::string getId() const;
    std::string getName() const;
    double getPrice() const;
    int getStock() const;
    std::string getSellerId() const;

    void reduceStock(int quantity);
    void increaseStock(int quantity);
    void setPrice(double newPrice);

    std::string serialize() const;
    void deserialize(const std::string& line);
};
