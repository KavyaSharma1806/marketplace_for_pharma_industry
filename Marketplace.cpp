#include "marketplace.h"
#include <cmath>

using namespace std;

// MARK: Constructor/Destructor
Medicine::Medicine(string n, double p, int q, string t) : name(n), price(p), quantity(q), type(t) {}
Medicine::~Medicine() {}
string Medicine::getName() const { return name; }
double Medicine::getPrice() const { return price; }
int Medicine::getQuantity() const { return quantity; }
bool Medicine::canBuy(int amount) const { return quantity >= amount; }
void Medicine::reduceStock(int amount) { quantity -= amount; }

// MARK: Operator Overloading
ostream &operator<<(ostream &os, const Medicine &m)
{
    os << m.name << " - $" << fixed << setprecision(2) << m.price
       << " - Stock: " << m.quantity << " - Type: " << m.type;
    return os;
}

void display(const Medicine &m)
{
    std::cout << m << "\n";
}

void Medicine::displayInfo() const
{

    std::cout << *this << "\n";
}

string Medicine::getCategory() const
{
    return "Medicine";
}

Tablet::Tablet(const string &n, double p, int q) : Medicine(n, p, q, "Tablet") {}
Tablet::~Tablet() {}
void Tablet::displayInfo() const
{
    cout << "[Tablet] ";

    Medicine::displayInfo();
}
string Tablet::getCategory() const { return "Tablet"; }

// MARK: SYRUP
Syrup::Syrup(const string &n, double p, int q) : Medicine(n, p, q, "Syrup") {}
Syrup::~Syrup() {}
void Syrup::displayInfo() const
{
    cout << "[Syrup]  ";

    Medicine::displayInfo();
}
string Syrup::getCategory() const { return "Syrup"; }

// MARK: Constructor/Destructor
Shop::Shop()
{
    name = "";
    medicineCount = 0;
    capacity = 10;
    medicines = new Medicine *[capacity];
    x = 0;
    y = 0;
}

Shop::Shop(string shopName)
{
    name = shopName;
    medicineCount = 0;
    capacity = 10;
    medicines = new Medicine *[capacity];
    x = 0;
    y = 0;
}

Shop::Shop(const Shop &other)
{
    name = other.name;
    medicineCount = other.medicineCount;
    capacity = other.capacity;
    medicines = new Medicine *[capacity];
    x = other.x;
    y = other.y;

    for (int i = 0; i < medicineCount; i++)
    {
        medicines[i] = new Medicine(*other.medicines[i]);
    }
}

Shop &Shop::operator=(const Shop &other)
{
    if (this != &other)
    {

        for (int i = 0; i < medicineCount; i++)
        {
            delete medicines[i];
        }
        delete[] medicines;

        name = other.name;
        medicineCount = other.medicineCount;
        capacity = other.capacity;
        medicines = new Medicine *[capacity];
        x = other.x;
        y = other.y;

        for (int i = 0; i < medicineCount; i++)
        {
            medicines[i] = new Medicine(*other.medicines[i]);
        }
    }
    return *this;
}

Shop::~Shop()
{
    for (int i = 0; i < medicineCount; i++)
    {
        delete medicines[i];
    }
    delete[] medicines;
}

void Shop::addMedicine(string medName, double price, int quantity, string type)
{
    if (medicineCount >= capacity)
    {
        int newCapacity = capacity * 2;
        Medicine **newMedicines = new Medicine *[newCapacity];

        for (int i = 0; i < medicineCount; i++)
        {
            newMedicines[i] = medicines[i];
        }

        delete[] medicines;
        medicines = newMedicines;
        capacity = newCapacity;
    }

    if (type == "Tablet")
        medicines[medicineCount] = new Tablet(medName, price, quantity);
    else if (type == "Syrup")
        medicines[medicineCount] = new Syrup(medName, price, quantity);
    else
        medicines[medicineCount] = new Medicine(medName, price, quantity, type);

    medicineCount++;
}

int Shop::findMedicine(string medName) const
{
    for (int i = 0; i < medicineCount; i++)
    {
        if (medicines[i]->getName() == medName)
            return i;
    }
    return -1;
}

void Shop::showInventory() const
{
    std::cout << "\n=== " << name << " ===\n";
    for (int i = 0; i < medicineCount; i++)
    {
        std::cout << "  ";
        medicines[i]->displayInfo();
    }
}

bool Shop::buyMedicine(string medName, int quantity, double deliveryCharge)
{
    int index = findMedicine(medName);
    if (index != -1 && medicines[index]->canBuy(quantity))
    {
        double baseCost = medicines[index]->getPrice() * quantity;
        double totalCost = baseCost + deliveryCharge;

        std::cout << "Base Cost: $" << fixed << setprecision(2) << baseCost << "\n";
        if (deliveryCharge > 0)
        {
            std::cout << "Delivery Charge: $" << fixed << setprecision(2) << deliveryCharge << "\n";
        }
        std::cout << "Total Cost: $" << fixed << setprecision(2) << totalCost << "\n";
        std::cout << "Confirm purchase? (y/n): ";
        char choice;
        std::cin >> choice;
        if (choice == 'y' || choice == 'Y')
        {
            medicines[index]->reduceStock(quantity);
            std::cout << "Purchase successful!\n";
            return true;
        }
    }
    return false;
}

bool Shop::removeMedicine(string medName)
{
    int index = findMedicine(medName);
    if (index == -1)
    {
        return false;
    }

    delete medicines[index];

    for (int i = index; i < medicineCount - 1; i++)
    {
        medicines[i] = medicines[i + 1];
    }

    medicineCount--;
    return true;
}

const Medicine *Shop::getMedicine(int index) const
{
    return medicines[index];
}

int Shop::getMedicineCount() const
{
    return medicineCount;
}

// MARK: Constructor/Destructor
Marketplace::Marketplace()
{
    shopCount = 3;

    shops[0] = Shop("MediCare");
    shops[0].setCoordinates(2, 4);
    shops[0].addMedicine("Paracetamol", 5.50, 50, "Tablet");
    shops[0].addMedicine("Ibuprofen", 8.50, 30, "Tablet");

    shops[1] = Shop("HealthPlus");
    shops[1].setCoordinates(3, 5);
    shops[1].addMedicine("Amoxicillin", 16.00, 25, "Tablet");
    shops[1].addMedicine("Cetirizine", 12.50, 20, "Tablet");

    shops[2] = Shop("QuickMeds");
    shops[2].setCoordinates(1, 3);
    shops[2].addMedicine("Metformin", 19.00, 35, "Tablet");
    shops[2].addMedicine("Insulin", 45.00, 10, "General");
}

Marketplace::~Marketplace() {}

// MARK: Distance and Delivery Calculation
double Marketplace::calculateDistance(int x1, int y1, int x2, int y2) const
{

    int dx = x2 - x1;
    int dy = y2 - y1;
    return sqrt(dx * dx + dy * dy);
}

double Marketplace::calculateDeliveryCharge(double distance) const
{

    return distance * 0.5;
}

// MARK: Order Logging
void Marketplace::logOrder(const string &medName, int quantity, const string &shopName,
                           int customerX, int customerY, int shopX, int shopY,
                           double distance, double baseCost, double deliveryCharge, double totalCost) const
{
    ofstream logFile("log.txt", ios::app);
    if (!logFile)
    {
        return;
    }

    logFile << "========================================\n";
    logFile << "Order Recorded In: log.txt\n";
    logFile << "Medicine: " << medName << "\n";
    logFile << "Quantity: " << quantity << "\n";
    logFile << "Shop: " << shopName << "\n";
    logFile << "Customer Location: (" << customerX << ", " << customerY << ")\n";
    logFile << "Shop Location: (" << shopX << ", " << shopY << ")\n";
    logFile << "Distance: " << fixed << setprecision(2) << distance << " units\n";
    logFile << "Base Cost: $" << fixed << setprecision(2) << baseCost << "\n";
    logFile << "Delivery Charge: $" << fixed << setprecision(2) << deliveryCharge << "\n";
    logFile << "Total Cost: $" << fixed << setprecision(2) << totalCost << "\n";
    logFile << "========================================\n\n";

    logFile.close();
}

// MARK: Friend Function
void addShop(Marketplace &market, const string &shopName)
{
    if (market.shopCount >= 5)
    {
        cout << "Cannot add more shops. Limit reached." << "\n";
        return;
    }
    market.shops[market.shopCount] = Shop(shopName);
    market.shops[market.shopCount].setCoordinates(0, 0);
    market.shopCount++;
    cout << "Shop added: " << shopName << "\n";
}

// MARK: Behavior
void Marketplace::orderMedicine()
{
    string medName;
    int quantity;
    int customerX, customerY;

    std::cout << "\nEnter medicine name: ";
    std::cin.ignore();
    getline(std::cin, medName);
    std::cout << "Enter quantity: ";
    std::cin >> quantity;

    if (quantity <= 0)
    {
        std::cout << "Invalid quantity! Please enter a positive number.\n";
        return;
    }

    // MARK: Coordinates
    std::cout << "Enter your location (x y): ";
    if (!(std::cin >> customerX >> customerY))
    {
        std::cout << "Invalid coordinates! Please enter two numbers.\n";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        return;
    }

    int availableShops[5];
    int availableCount = 0;
    double baseCosts[5];
    double deliveryCharges[5];
    double totalCosts[5];
    double distances[5];

    for (int i = 0; i < shopCount; i++)
    {
        int idx = shops[i].findMedicine(medName);
        if (idx != -1 && shops[i].getMedicine(idx)->canBuy(quantity))
        {

            int shopX = shops[i].getX();
            int shopY = shops[i].getY();
            double distance = calculateDistance(customerX, customerY, shopX, shopY);
            double deliveryCharge = calculateDeliveryCharge(distance);
            double baseCost = shops[i].getMedicine(idx)->getPrice() * quantity;
            double totalCost = baseCost + deliveryCharge;

            availableShops[availableCount] = i;
            baseCosts[availableCount] = baseCost;
            deliveryCharges[availableCount] = deliveryCharge;
            totalCosts[availableCount] = totalCost;
            distances[availableCount] = distance;
            availableCount++;
        }
    }

    if (availableCount == 0)
    {
        double lowestPrice = -1.0;
        bool medicineExists = false;

        for (int i = 0; i < shopCount; i++)
        {
            int idx = shops[i].findMedicine(medName);
            if (idx != -1)
            {
                medicineExists = true;
                double price = shops[i].getMedicine(idx)->getPrice();
                if (lowestPrice < 0 || price < lowestPrice)
                {
                    lowestPrice = price;
                }
            }
        }

        double baseMarketPrice = 0.0;

        if (medicineExists)
        {
            baseMarketPrice = lowestPrice;
            std::cout << "\nMedicine \"" << medName << "\" not found in any shop with sufficient stock.\n";
        }
        else
        {
            std::cout << "\nMedicine \"" << medName << "\" not found in any shop.\n";
        }

        std::cout << "Not available in the market .However, we can arrange it from a wholesaler at 2x the market price.\n\n";

        double wholesalerPricePerUnit = 100;
        double wholesalerBaseCost = wholesalerPricePerUnit * quantity;

        int wholesalerX = 0;
        int wholesalerY = 0;
        double wholesalerDistance = calculateDistance(customerX, customerY, wholesalerX, wholesalerY);
        double wholesalerDeliveryCharge = calculateDeliveryCharge(wholesalerDistance);
        double wholesalerTotalCost = wholesalerBaseCost + wholesalerDeliveryCharge;

        std::cout << "=== Wholesaler Option ===\n";
        std::cout << "Medicine: " << medName << "\n";
        std::cout << "Quantity: " << quantity << "\n";
        if (medicineExists)
        {
            std::cout << "Market price (lowest found): $" << fixed << setprecision(2) << baseMarketPrice << " per unit\n";
        }
        else
        {
            std::cout << "Estimated market price: $" << fixed << setprecision(2) << baseMarketPrice << " per unit\n";
        }
        std::cout << "Wholesaler price: $" << fixed << setprecision(2) << wholesalerPricePerUnit << " per unit (2x market price)\n";
        std::cout << "Base cost: $" << fixed << setprecision(2) << wholesalerBaseCost << "\n";
        std::cout << "Delivery charge: $" << fixed << setprecision(2) << wholesalerDeliveryCharge << "\n";
        std::cout << "Total cost: $" << fixed << setprecision(2) << wholesalerTotalCost << "\n\n";

        std::cout << "Would you like to purchase from the wholesaler? (y/n): ";
        char confirm;
        std::cin >> confirm;

        if (confirm == 'y' || confirm == 'Y')
        {
            std::cout << "\n=== Processing order from Wholesaler ===\n";
            std::cout << "Base Cost: $" << fixed << setprecision(2) << wholesalerBaseCost << "\n";
            if (wholesalerDeliveryCharge > 0)
            {
                std::cout << "Delivery Charge: $" << fixed << setprecision(2) << wholesalerDeliveryCharge << "\n";
            }
            std::cout << "Total Cost: $" << fixed << setprecision(2) << wholesalerTotalCost << "\n";
            std::cout << "Purchase successful!\n";

            logOrder(medName, quantity, "Wholesaler", customerX, customerY,
                     wholesalerX, wholesalerY, wholesalerDistance, wholesalerBaseCost,
                     wholesalerDeliveryCharge, wholesalerTotalCost);
        }
        else
        {
            std::cout << "Purchase cancelled.\n";
        }
        return;
    }

    std::cout << "\n=== Available Options for \"" << medName << "\" (Quantity: " << quantity << ") ===\n";
    std::cout << "Your location: (" << customerX << ", " << customerY << ")\n\n";

    for (int i = 0; i < availableCount; i++)
    {
        int shopIdx = availableShops[i];
        int medIdx = shops[shopIdx].findMedicine(medName);

        std::cout << i + 1 << ". " << shops[shopIdx].name << "\n";
        std::cout << "   Price per unit: $" << fixed << setprecision(2)
                  << shops[shopIdx].getMedicine(medIdx)->getPrice() << "\n";
        std::cout << "   Available stock: " << shops[shopIdx].getMedicine(medIdx)->getQuantity() << " units\n";
        std::cout << "   Shop location: (" << shops[shopIdx].getX() << ", "
                  << shops[shopIdx].getY() << ")\n";
        std::cout << "   Distance: " << fixed << setprecision(2) << distances[i] << " units\n";
        std::cout << "   Base cost: $" << fixed << setprecision(2) << baseCosts[i] << "\n";
        std::cout << "   Delivery charge: $" << fixed << setprecision(2) << deliveryCharges[i] << "\n";
        std::cout << "   Total cost: $" << fixed << setprecision(2) << totalCosts[i] << "\n\n";
    }

    std::cout << "Select a shop (1-" << availableCount << ") or 0 to cancel: ";
    int choice;
    if (!(std::cin >> choice) || choice < 0 || choice > availableCount)
    {
        std::cout << "Invalid selection! Order cancelled.\n";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        return;
    }

    if (choice == 0)
    {
        std::cout << "Order cancelled.\n";
        return;
    }

    int selectedShopIdx = availableShops[choice - 1];
    int shopX = shops[selectedShopIdx].getX();
    int shopY = shops[selectedShopIdx].getY();

    std::cout << "\n=== Processing order from " << shops[selectedShopIdx].name << " ===\n";

    if (shops[selectedShopIdx].buyMedicine(medName, quantity, deliveryCharges[choice - 1]))
    {
        logOrder(medName, quantity, shops[selectedShopIdx].name, customerX, customerY,
                 shopX, shopY, distances[choice - 1], baseCosts[choice - 1],
                 deliveryCharges[choice - 1], totalCosts[choice - 1]);
    }
    else
    {
        std::cout << "Purchase cancelled.\n";
    }
}

// MARK: Behavior (const)
void Marketplace::searchMedicine() const
{
    string medName;
    std::cout << "\nEnter medicine name to search: ";
    std::cin.ignore();
    getline(std::cin, medName);

    std::cout << "\nSearch Results:\n";
    bool found = false;
    for (int i = 0; i < shopCount; i++)
    {
        int index = shops[i].findMedicine(medName);
        if (index != -1)
        {
            found = true;
            std::cout << shops[i].name << " - $" << fixed << setprecision(2)
                      << shops[i].getMedicine(index)->getPrice()
                      << " - Stock: " << shops[i].getMedicine(index)->getQuantity() << "\n";
        }
    }
    if (!found)
    {
        std::cout << "Medicine not found in shops.\n";
    }
}

void Marketplace::showAllShops() const
{
    for (int i = 0; i < shopCount; i++)
    {
        shops[i].showInventory();
    }
}

// MARK: Retailer Functions
void Marketplace::listShopsForRetailer() const
{
    std::cout << "\n=== Available Shops ===\n";
    for (int i = 0; i < shopCount; i++)
    {
        std::cout << i << ". " << shops[i].name << " (Location: " << shops[i].getX() << ", " << shops[i].getY() << ")\n";
    }
    std::cout << "======================\n";
}

bool Marketplace::removeMedicineFromShop(int shopIndex, string medName)
{
    if (shopIndex < 0 || shopIndex >= shopCount)
    {
        return false;
    }
    return shops[shopIndex].removeMedicine(medName);
}

// MARK: File I/O
void Marketplace::addMedicineAndPersist(int shopIndex, string medName, double price, int quantity, const string &type, const string &filePath)
{
    if (shopIndex < 0 || shopIndex >= shopCount)
    {
        cout << "Invalid shop index." << "\n";
        return;
    }

    shops[shopIndex].addMedicine(medName, price, quantity, type);

    ofstream out(filePath, ios::app);
    if (!out)
    {
        cout << "Failed to open " << filePath << " for writing." << "\n";
        return;
    }
    out << shops[shopIndex].name << "," << medName << "," << fixed << setprecision(2) << price << "," << quantity << "," << type << "\n";
    out.close();
    cout << "Item persisted to " << filePath << "\n";
}

void Marketplace::loadInventoryFromFile(const string &filePath)
{
    ifstream in(filePath);
    if (!in)
    {
        cout << "Could not open " << filePath << " for reading. Starting with default inventory." << "\n";
        return;
    }

    string line;
    while (getline(in, line))
    {
        if (line.empty())
            continue;

        stringstream ss(line);
        string shopName, medName, priceStr, qtyStr, typeStr;

        if (getline(ss, shopName, ',') &&
            getline(ss, medName, ',') &&
            getline(ss, priceStr, ',') &&
            getline(ss, qtyStr, ','))
        {
            // Optional type field (5th column)
            if (!getline(ss, typeStr))
            {
                typeStr = "General";
            }

            if (medName.empty() || priceStr.empty() || qtyStr.empty())
                continue;

            try
            {
                double price = stod(priceStr);
                int quantity = stoi(qtyStr);

                // Find or create shop
                int shopIndex = -1;
                for (int i = 0; i < shopCount; i++)
                {
                    if (shops[i].name == shopName)
                    {
                        shopIndex = i;
                        break;
                    }
                }

                if (shopIndex == -1 && shopCount < 5)
                {
                    shops[shopCount] = Shop(shopName);
                    shops[shopCount].setCoordinates(0, 0);
                    shopIndex = shopCount;
                    shopCount++;
                }

                if (shopIndex != -1)
                {
                    int existingMedIndex = shops[shopIndex].findMedicine(medName);
                    if (existingMedIndex != -1)
                    {
                        continue;
                    }
                    shops[shopIndex].addMedicine(medName, price, quantity, typeStr);
                }
            }
            catch (const exception &e)
            {
                cout << "Skipping invalid line: " << line << "\n";
            }
        }
    }
    in.close();
    cout << "Inventory loaded from " << filePath << "\n";
}