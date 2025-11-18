#pragma once

#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>

using namespace std;

// Base class representing a medicine item in our inventory
class Medicine
{
private:
    string name;
    double price;
    int quantity;
    string type; // Can be Tablet, Syrup, or General

public:
    Medicine(string n = "", double p = 0, int q = 0, string t = "General");
    virtual ~Medicine();

    // Basic getters
    string getName() const;
    double getPrice() const;
    int getQuantity() const;
    string getType() const { return type; }

    // Check if we have enough stock and reduce it after purchase
    bool canBuy(int amount) const;
    void reduceStock(int amount);

    // Virtual functions so derived classes can override them
    virtual void displayInfo() const;
    virtual string getCategory() const;

    // Friend functions for flexible output
    friend void display(const Medicine &m);
    friend ostream &operator<<(ostream &os, const Medicine &m);
};

// Tablet is a specific type of medicine
class Tablet : public Medicine
{
public:
    Tablet(const string &n = "", double p = 0.0, int q = 0);
    virtual ~Tablet();
    void displayInfo() const override;
    string getCategory() const override;
};

// Syrup is another medicine type
class Syrup : public Medicine
{
public:
    Syrup(const string &n = "", double p = 0.0, int q = 0);
    virtual ~Syrup();
    void displayInfo() const override;
    string getCategory() const override;
};

void display(const Medicine &m);

// Shop class manages a collection of medicines
class Shop
{
private:
    Medicine **medicines; // Using pointers so we can store different medicine types
    int capacity;
    int medicineCount;
    int x, y; // Shop location on a 2D grid

public:
    string name;

    Shop();
    Shop(string shopName);
    Shop(const Shop &other);            
    Shop &operator=(const Shop &other); 
    ~Shop();

    // Core shop operations
    void addMedicine(string medName, double price, int quantity = 1, string type = "General");
    bool refillStock(string medName, int additionalQuantity);
    int findMedicine(string medName) const;
    void showInventory() const;
    bool buyMedicine(string medName, int quantity = 1, double deliveryCharge = 0.0);
    bool removeMedicine(string medName);

    // Access medicines in the inventory
    const Medicine *getMedicine(int index) const;
    int getMedicineCount() const;

    // Location management
    int getX() const { return x; }
    int getY() const { return y; }
    void setCoordinates(int xCoord, int yCoord)
    {
        x = xCoord;
        y = yCoord;
    }
};

// Marketplace manages multiple shops
class Marketplace
{
private:
    Shop shops[5];
    int shopCount;

    // Helper to log completed orders to a file
    void logOrder(const string &medName, int quantity, const string &shopName,
                  int customerX, int customerY, int shopX, int shopY,
                  double distance, double baseCost, double deliveryCharge, double totalCost) const;

public:
    Marketplace();
    ~Marketplace();

    // Customer-facing functions
    void orderMedicine();
    void searchMedicine() const;
    void showAllShops() const;

    // Calculate distance and delivery costs
    double calculateDistance(int x1, int y1, int x2, int y2) const;
    double calculateDeliveryCharge(double distance) const;

    // Functions for retailers to manage inventory
    void listShopsForRetailer() const;
    bool removeMedicineFromShop(int shopIndex, string medName);
    bool refillStockInShop(int shopIndex, string medName, int additionalQuantity);

    // Persistence functions for saving/loading data
    void addMedicineAndPersist(int shopIndex, string medName, double price, int quantity, const string &type, const string &filePath = "inventory.csv");
    void refillStockAndPersist(int shopIndex, string medName, int additionalQuantity, const string &filePath = "inventory.csv");
    void loadInventoryFromFile(const string &filePath = "inventory.csv");

    // Accessors for external code
    int getShopCount() const { return shopCount; }
    const Shop &getShop(int i) const { return shops[i]; }
    Shop &getShop(int i) { return shops[i]; }

    // Friend function for adding new shops
    friend void addShop(Marketplace &market, const string &shopName);
};