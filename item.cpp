#include "Item.hpp"
#include <sstream>

Item::Item(const std::string& id, const std::string& name, double price, int stock, const std::string& sellerId)
    : itemId(id), name(name), price(price), stock(stock), sellerId(sellerId) {}

std::string Item::getId() const { return itemId; }
std::string Item::getName() const { return name; }
double Item::getPrice() const { return price; }
int Item::getStock() const { return stock; }
std::string Item::getSellerId() const { return sellerId; }

void Item::reduceStock(int quantity) {
    if (quantity <= stock) stock -= quantity;
}

void Item::increaseStock(int quantity) {
    stock += quantity;
}

void Item::setPrice(double newPrice) {
    price = newPrice;
}

std::string Item::serialize() const {
    std::ostringstream oss;
    oss << itemId << "|" << name << "|" << price << "|" << stock << "|" << sellerId;
    return oss.str();
}

void Item::deserialize(const std::string& line) {
    std::istringstream iss(line);
    std::string priceStr, stockStr;
    std::getline(iss, itemId, '|');
    std::getline(iss, name, '|');
    std::getline(iss, priceStr, '|');
    std::getline(iss, stockStr, '|');
    std::getline(iss, sellerId, '|');
    price = std::stod(priceStr);
    stock = std::stoi(stockStr);
}
