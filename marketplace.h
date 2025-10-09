#pragma once

#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>

using namespace std;
// MARK: Encapsulation
class Medicine
{
private:
    string name;
    double price;
    int quantity;

public:
    // MARK: Constructor/Destructor
    Medicine(string n = "", double p = 0, int q = 0);
    ~Medicine();

    string getName() const;
    double getPrice() const;
    int getQuantity() const;

    bool canBuy(int amount) const;
    void reduceStock(int amount);

    // MARK: Friend + Operator Overload
    friend void display(const Medicine &m);
    friend ostream &operator<<(ostream &os, const Medicine &m);
};

void display(const Medicine &m);

class Shop
{
public:
    string name;
    Medicine medicines[50];
    int medicineCount;

    Shop();
    Shop(string shopName);
    ~Shop();

    void addMedicine(string medName, double price, int quantity = 1);
    int findMedicine(string medName) const;
    void showInventory() const;
    bool buyMedicine(string medName, int quantity = 1);
};

// MARK: Abstraction
class Feature
{
public:
    ~Feature() {}
    void run(class Marketplace &market);
};

// MARK: Composition (has-a Shops) + Encapsulation
class Marketplace
{
private:
    Shop shops[5];
    int shopCount;

public:
    Marketplace();
    ~Marketplace();

    void orderMedicine();
    void searchMedicine() const;
    void showAllShops() const;

    void addMedicineAndPersist(int shopIndex, string medName, double price, int quantity, const string &filePath = "inventory.csv");
    void loadInventoryFromFile(const string &filePath = "inventory.csv");

    // MARK: Accessors
    int getShopCount() const { return shopCount; }
    const Shop &getShop(int i) const { return shops[i]; }

    // MARK: Friend Function (external addShop can access private state)
    friend void addShop(Marketplace &market, const string &shopName);
};