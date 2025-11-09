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
    string type; // Tablet, Syrup, General

public:
    // MARK: Constructor/Destructor
    Medicine(string n = "", double p = 0, int q = 0, string t = "General");
    ~Medicine();

    string getName() const;
    double getPrice() const;
    int getQuantity() const;
    string getType() const { return type; }

    bool canBuy(int amount) const;
    void reduceStock(int amount);

    virtual void displayInfo() const;
    virtual string getCategory() const;

    friend void display(const Medicine &m);
    friend ostream &operator<<(ostream &os, const Medicine &m);
};

class Tablet : public Medicine
{
public:
    Tablet(const string &n = "", double p = 0.0, int q = 0);
    virtual ~Tablet();
    virtual void displayInfo() const override;
    virtual string getCategory() const override;
};

class Syrup : public Medicine
{
public:
    Syrup(const string &n = "", double p = 0.0, int q = 0);
    virtual ~Syrup();
    virtual void displayInfo() const override;
    virtual string getCategory() const override;
};

void display(const Medicine &m);

class Shop
{
private:
    Medicine **medicines; // Array of pointers for polymorphism
    int capacity;
    int medicineCount;
    int x; // Shop x coordinate
    int y; // Shop y coordinate

public:
    string name;

    Shop();
    Shop(string shopName);
    Shop(const Shop &other);            // Copy constructor
    Shop &operator=(const Shop &other); // Assignment operator
    ~Shop();

    void addMedicine(string medName, double price, int quantity = 1, string type = "General");
    int findMedicine(string medName) const;
    void showInventory() const;
    bool buyMedicine(string medName, int quantity = 1, double deliveryCharge = 0.0);
    bool removeMedicine(string medName);

    // Getter methods for accessing medicines
    const Medicine *getMedicine(int index) const;
    int getMedicineCount() const;

    // Getter methods for coordinates
    int getX() const { return x; }
    int getY() const { return y; }
    void setCoordinates(int xCoord, int yCoord)
    {
        x = xCoord;
        y = yCoord;
    }
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

    // Order logging function
    void logOrder(const string &medName, int quantity, const string &shopName,
                  int customerX, int customerY, int shopX, int shopY,
                  double distance, double baseCost, double deliveryCharge, double totalCost) const;

public:
    Marketplace();
    ~Marketplace();

    void orderMedicine();
    void searchMedicine() const;
    void showAllShops() const;

    // Distance and delivery calculation functions
    double calculateDistance(int x1, int y1, int x2, int y2) const;
    double calculateDeliveryCharge(double distance) const;

    // Retailer functions
    void listShopsForRetailer() const;
    bool removeMedicineFromShop(int shopIndex, string medName);

    void addMedicineAndPersist(int shopIndex, string medName, double price, int quantity, const string &type, const string &filePath = "inventory.csv");
    void loadInventoryFromFile(const string &filePath = "inventory.csv");

    // MARK: Accessors
    int getShopCount() const { return shopCount; }
    const Shop &getShop(int i) const { return shops[i]; }

    // MARK: Friend Function (external addShop can access private state)
    friend void addShop(Marketplace &market, const string &shopName);
};