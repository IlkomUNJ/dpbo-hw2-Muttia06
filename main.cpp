#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include "User.hpp"
#include "BankAccount.hpp"
#include "Item.hpp"
#include "Transaksi.hpp"
#include "Store.hpp"
#include "Database.hpp"

std::string getTodayDate() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buf[11];
    strftime(buf, sizeof(buf), "%Y-%m-%d", ltm);
    return std::string(buf);
}

void menuBuyer(User& buyer, Store& store, std::vector<BankAccount>& banks, std::vector<Transaction>& transactions);
void menuSeller(User& seller, Store& store, std::vector<BankAccount>& banks, std::vector<Transaction>& transactions);
void menuAdmin(Store& store, std::vector<BankAccount>& banks);
BankAccount* findBank(std::vector<BankAccount>& banks, const std::string& ownerId) {
    for (auto& b : banks)
        if (b.getOwnerId() == ownerId)
            return &b;
    return nullptr;
}


int main() {
    std::vector<User> users = Database::loadUsers("data/users.txt");
    std::vector<BankAccount> banks = Database::loadBankAccounts("data/bank.txt");
    std::vector<Item> items = Database::loadItems("data/items.txt");
    std::vector<Transaction> transactions = Database::loadTransactions("data/transactions.txt");

    Store store;
    store.loadItems(items);
    store.loadTransactions(transactions);

    while (true) {
        std::cout << "\n=== MENU UTAMA ===\n";
        std::cout << "1. Daftar\n2. Login Buyer\n3. Login Seller\n4. Login Admin\n5. Tampilkan Semua User\n6. Keluar\nPilih: ";
        int choice; std::cin >> choice;

        if (choice == 1) {
            std::string id, name, roleStr;
            std::cout << "ID: "; std::cin >> id;
            std::cout << "Nama: "; std::cin.ignore(); std::getline(std::cin, name);
            std::cout << "Role (B/S): "; std::cin >> roleStr;
            Role role = (roleStr == "B") ? Role::Buyer : Role::Seller;
            users.emplace_back(id, name, role);
            banks.emplace_back("bank_" + id, id);
            std::cout << "Registrasi berhasil.\n";
        } else if (choice == 2 || choice == 3) {
            std::string id;
            std::cout << "ID: "; std::cin >> id;
            auto it = std::find_if(users.begin(), users.end(), [&](const User& u) {
                return u.getId() == id && ((choice == 2 && u.getRole() == Role::Buyer) || (choice == 3 && u.getRole() == Role::Seller));
            });
            if (it != users.end()) {
                if (choice == 2) menuBuyer(*it, store, banks, transactions);
                else menuSeller(*it, store, banks, transactions);
            } else std::cout << "User tidak ditemukan.\n";
        } else if (choice == 4) {
            std::string id;
            std::cout << "ID Admin: "; std::cin >> id;
            if (id == "admin") menuAdmin(store, banks);
            else std::cout << "ID admin salah.\n";
        } else if (choice == 5) {
            for (const auto& u : users)
                std::cout << u.getId() << " - " << u.getName() << " - " << u.getRoleString() << "\n";
        } else if (choice == 6) {
            Database::saveUsers(users, "data/users.txt");
            Database::saveBankAccounts(banks, "data/bank.txt");
            Database::saveItems(store.getAllItems(), "data/items.txt");
            Database::saveTransactions(store.getAllTransactions(), "data/transactions.txt");
            std::cout << "Data disimpan. Keluar...\n";
            break;
        } else std::cout << "Pilihan tidak valid.\n";
    }

    return 0;
}

void menuBuyer(User& buyer, Store& store, std::vector<BankAccount>& banks, std::vector<Transaction>& transactions) {
    while (true) {
        std::cout << "\n=== MENU BUYER ===\n";
        std::cout << "1. Lihat Semua Barang\n2. Beli Barang\n3. Riwayat Transaksi\n4. Menu Perbankan\n5. Logout\nPilih: ";
        int choice; std::cin >> choice;

        if (choice == 1) {
            for (const auto& item : store.getAllItems()) {
                std::cout << item.getId() << " - " << item.getName()
                          << " - Rp" << item.getPrice()
                          << " - Stok: " << item.getStock()
                          << " - Seller: " << item.getSellerId() << "\n";
            }
        } else if (choice == 2) {
            std::string itemId;
            int qty;
            std::cout << "ID Barang: "; std::cin >> itemId;
            std::cout << "Jumlah: "; std::cin >> qty;

            BankAccount* bank = findBank(banks, buyer.getId());
            if (!bank) { std::cout << "Akun bank tidak ditemukan.\n"; continue; }

            Item target;
            bool found = false;
            for (const auto& item : store.getAllItems()) {
                if (item.getId() == itemId) {
                    target = item;
                    found = true;
                    break;
                }
            }
            if (!found) { std::cout << "Barang tidak ditemukan.\n"; continue; }

            double total = target.getPrice() * qty;
            if (bank->getBalance() < total) {
                std::cout << "Saldo tidak cukup.\n";
                continue;
            }

            std::string trxId = "trx" + std::to_string(transactions.size() + 1);
            std::string date = getTodayDate();
            Transaction trx;
            if (store.purchaseItem(buyer.getId(), itemId, qty, trxId, date, trx)) {
                bank->withdraw(total);
                transactions.push_back(trx);
                std::cout << "Pembelian berhasil. Status: Paid\n";
            } else {
                std::cout << "Pembelian gagal. Stok tidak cukup.\n";
            }

        } else if (choice == 3) {
            auto list = store.getTransactionsByBuyer(buyer.getId());
            for (const auto& trx : list) {
                std::cout << trx.getDate() << " - " << trx.getItemId()
                          << " x" << trx.getQuantity()
                          << " - Rp" << trx.getTotalPrice()
                          << " - Status: " << trx.getStatusString() << "\n";
            }
        } else if (choice == 4) {
            BankAccount* bank = findBank(banks, buyer.getId());
            if (!bank) { std::cout << "Akun bank tidak ditemukan.\n"; continue; }

            std::cout << "\n=== MENU BANK BUYER ===\n";
            std::cout << "1. Topup\n2. Withdraw\n3. Cashflow Hari Ini\n4. Cashflow Bulan Ini\n5. Kembali\nPilih: ";
            int b; std::cin >> b;
            if (b == 1) {
                double amt; std::cout << "Jumlah Topup: "; std::cin >> amt;
                bank->topup(amt);
            } else if (b == 2) {
                double amt; std::cout << "Jumlah Withdraw: "; std::cin >> amt;
                if (!bank->withdraw(amt)) std::cout << "Saldo tidak cukup.\n";
            } else if (b == 3) {
                bank->printCashflowToday();
            } else if (b == 4) {
                bank->printCashflowMonth();
            }
        } else if (choice == 5) {
            break;
        } else {
            std::cout << "Pilihan tidak valid.\n";
        }
    }
}
void menuSeller(User& seller, Store& store, std::vector<BankAccount>& banks, std::vector<Transaction>& transactions) {
    while (true) {
        std::cout << "\n=== MENU SELLER ===\n";
        std::cout << "1. Lihat Stok Saya\n2. Tambah Item\n3. Hapus Item\n4. Laporan Penjualan\n5. Menu Perbankan\n6. Logout\nPilih: ";
        int choice; std::cin >> choice;

        if (choice == 1) {
            auto myItems = store.getItemsBySeller(seller.getId());
            for (const auto& item : myItems) {
                std::cout << item.getId() << " - " << item.getName()
                          << " - Rp" << item.getPrice()
                          << " - Stok: " << item.getStock() << "\n";
            }
        } else if (choice == 2) {
            std::string itemId, name;
            double price;
            int stock;
            std::cout << "ID Item: "; std::cin >> itemId;
            std::cout << "Nama Item: "; std::cin.ignore(); std::getline(std::cin, name);
            std::cout << "Harga: "; std::cin >> price;
            std::cout << "Stok: "; std::cin >> stock;
            store.addItem(Item(itemId, name, price, stock, seller.getId()));
            std::cout << "Item berhasil ditambahkan.\n";
        } else if (choice == 3) {
            std::string itemId;
            std::cout << "ID Item yang ingin dihapus: "; std::cin >> itemId;
            if (store.removeItem(itemId, seller.getId()))
                std::cout << "Item berhasil dihapus.\n";
            else
                std::cout << "Item tidak ditemukan atau bukan milik Anda.\n";
        } else if (choice == 4) {
            auto list = store.getTransactionsBySeller(seller.getId());
            for (const auto& trx : list) {
                std::cout << trx.getDate() << " - " << trx.getItemId()
                          << " x" << trx.getQuantity()
                          << " - Rp" << trx.getTotalPrice()
                          << " - Status: " << trx.getStatusString()
                          << " - Buyer: " << trx.getBuyerId() << "\n";
            }
        } else if (choice == 5) {
            BankAccount* bank = findBank(banks, seller.getId());
            if (!bank) { std::cout << "Akun bank tidak ditemukan.\n"; continue; }

            std::cout << "\n=== MENU BANK SELLER ===\n";
            std::cout << "1. Topup\n2. Withdraw\n3. Cashflow Hari Ini\n4. Cashflow Bulan Ini\n5. Kembali\nPilih: ";
            int b; std::cin >> b;
            if (b == 1) {
                double amt; std::cout << "Jumlah Topup: "; std::cin >> amt;
                bank->topup(amt);
            } else if (b == 2) {
                double amt; std::cout << "Jumlah Withdraw: "; std::cin >> amt;
                if (!bank->withdraw(amt)) std::cout << "Saldo tidak cukup.\n";
            } else if (b == 3) {
                bank->printCashflowToday();
            } else if (b == 4) {
                bank->printCashflowMonth();
            }
        } else if (choice == 6) {
            break;
        } else {
            std::cout << "Pilihan tidak valid.\n";
        }
    }
}
void menuAdmin(Store& store, std::vector<BankAccount>& banks) {
    while (true) {
        std::cout << "\n=== MENU ADMIN TOKO ===\n";
        std::cout << "1. Laporan Item Terlaris\n2. Pembeli Teraktif\n3. Penjual Teraktif\n4. Laporan Topup Hari Ini\n5. Kembali\nPilih: ";
        int choice; std::cin >> choice;

        if (choice == 1) {
            int k;
            std::cout << "Tampilkan top berapa item? "; std::cin >> k;
            auto topItems = store.getTopItems(k);
            std::cout << "\nItem Terlaris:\n";
            for (const auto& [itemId, count] : topItems)
                std::cout << itemId << " - Terjual: " << count << "\n";
        } else if (choice == 2) {
            int k;
            std::cout << "Tampilkan top berapa buyer? "; std::cin >> k;
            auto topBuyers = store.getTopBuyers(k);
            std::cout << "\nPembeli Teraktif:\n";
            for (const auto& [buyerId, count] : topBuyers)
                std::cout << buyerId << " - Transaksi: " << count << "\n";
        } else if (choice == 3) {
            int k;
            std::cout << "Tampilkan top berapa seller? "; std::cin >> k;
            auto topSellers = store.getTopSellers(k);
            std::cout << "\nPenjual Teraktif:\n";
            for (const auto& [sellerId, count] : topSellers)
                std::cout << sellerId << " - Transaksi: " << count << "\n";
        } else if (choice == 4) {
            std::string today = getTodayDate();
            std::cout << "\nTopup Hari Ini:\n";
            for (const auto& bank : banks) {
                double total = 0;
                for (const auto& line : bank.serialize()) {
                    // parsing manual karena kita tidak expose history langsung
                    // alternatif: tambahkan fungsi getTopupTodayAmount() di BankAccount
                }
                // Untuk sekarang, kita tampilkan cashflow hari ini
                if (bank.getBalance() > 0) {
                    std::cout << "Akun: " << bank.getOwnerId() << "\n";
                    bank.printCashflowToday();
                }
            }
        } else if (choice == 5) {
            break;
        } else {
            std::cout << "Pilihan tidak valid.\n";
        }
    }
}
