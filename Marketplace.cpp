#include "marketplace.h"
#include <cmath>
#include <ctime>

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
    capacity = 10; // Initial capacity
    medicines = new Medicine *[capacity];
    x = 0;
    y = 0;
}

Shop::Shop(string shopName)
{
    name = shopName;
    medicineCount = 0;
    capacity = 10; // Initial capacity
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

    // Copy all medicines (deep copy)
    for (int i = 0; i < medicineCount; i++)
    {
        medicines[i] = new Medicine(*other.medicines[i]);
    }
}

Shop &Shop::operator=(const Shop &other)
{
    if (this != &other) // Avoid self-assignment
    {
        // Delete existing medicines
        for (int i = 0; i < medicineCount; i++)
        {
            delete medicines[i];
        }
        delete[] medicines;

        // Copy data
        name = other.name;
        medicineCount = other.medicineCount;
        capacity = other.capacity;
        medicines = new Medicine *[capacity];
        x = other.x;
        y = other.y;

        // Copy all medicines (deep copy)
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

        // Copy existing medicines
        for (int i = 0; i < medicineCount; i++)
        {
            newMedicines[i] = medicines[i];
        }

        delete[] medicines;
        medicines = newMedicines;
        capacity = newCapacity;
    }

    // Create appropriate medicine type based on type string
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
        return false; // Medicine not found
    }

    // Delete the medicine object
    delete medicines[index];

    // Shift remaining medicines to fill the gap
    for (int i = index; i < medicineCount - 1; i++)
    {
        medicines[i] = medicines[i + 1];
    }

    medicineCount--;
    return true;
}

// Getter methods for accessing medicines
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
    shops[0].setCoordinates(2, 4); // Hardcoded shop coordinates
    shops[0].addMedicine("Paracetamol", 5.50, 50, "Tablet");
    shops[0].addMedicine("Ibuprofen", 8.50, 30, "Tablet");

    shops[1] = Shop("HealthPlus");
    shops[1].setCoordinates(3, 5); // Hardcoded shop coordinates
    shops[1].addMedicine("Amoxicillin", 16.00, 25, "Tablet");
    shops[1].addMedicine("Cetirizine", 12.50, 20, "Tablet");

    shops[2] = Shop("QuickMeds");
    shops[2].setCoordinates(1, 3); // Hardcoded shop coordinates
    shops[2].addMedicine("Metformin", 19.00, 35, "Tablet");
    shops[2].addMedicine("Insulin", 45.00, 10, "General");
}

Marketplace::~Marketplace() {}

// MARK: Distance and Delivery Calculation
double Marketplace::calculateDistance(int x1, int y1, int x2, int y2) const
{
    // Euclidean distance: sqrt((x2-x1)² + (y2-y1)²)
    int dx = x2 - x1;
    int dy = y2 - y1;
    return sqrt(dx * dx + dy * dy);
}

double Marketplace::calculateDeliveryCharge(double distance) const
{
    // $0.5 per unit distance
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

    // Get current timestamp
    time_t now = time(0);
    char *dt = ctime(&now);
    string timestamp = dt;
    // Remove newline from timestamp
    if (!timestamp.empty() && timestamp.back() == '\n')
    {
        timestamp.pop_back();
    }

    // Write order details in a readable format
    logFile << "========================================\n";
    logFile << "Order Date: " << timestamp << "\n";
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
    market.shops[market.shopCount].setCoordinates(0, 0); // Default coordinates for new shops
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

    // Get customer coordinates
    std::cout << "Enter your location (x y): ";
    if (!(std::cin >> customerX >> customerY))
    {
        std::cout << "Invalid coordinates! Please enter two numbers.\n";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        return;
    }

    bool found = false;
    for (int i = 0; i < shopCount; i++)
    {
        int idx = shops[i].findMedicine(medName);
        if (idx != -1)
        {
            found = true;
            std::cout << "\nFound at " << shops[i].name << "\n";
            std::cout << "Price: $" << fixed << setprecision(2) << shops[i].getMedicine(idx)->getPrice() << " per unit\n";
            std::cout << "Available: " << shops[i].getMedicine(idx)->getQuantity() << " units\n";

            // Calculate distance and delivery charge
            int shopX = shops[i].getX();
            int shopY = shops[i].getY();
            double distance = calculateDistance(customerX, customerY, shopX, shopY);
            double deliveryCharge = calculateDeliveryCharge(distance);

            // Calculate base cost for logging
            double baseCost = shops[i].getMedicine(idx)->getPrice() * quantity;
            double totalCost = baseCost + deliveryCharge;

            std::cout << "Shop location: (" << shopX << ", " << shopY << ")\n";
            std::cout << "Your location: (" << customerX << ", " << customerY << ")\n";
            std::cout << "Distance: " << fixed << setprecision(2) << distance << " units\n";

            if (shops[i].buyMedicine(medName, quantity, deliveryCharge))
            {
                logOrder(medName, quantity, shops[i].name, customerX, customerY,
                         shopX, shopY, distance, baseCost, deliveryCharge, totalCost);
                return;
            }
        }
    }
    if (!found)
    {
        std::cout << "\nMedicine not found in shops.\n";
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
        return false; // Invalid shop index
    }
    return shops[shopIndex].removeMedicine(medName);
}

// MARK: File I/O + Composition interaction
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
            continue; // Skip empty lines

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
                continue; // Skip invalid entries

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
                    // Create new shop
                    shops[shopCount] = Shop(shopName);
                    shops[shopCount].setCoordinates(0, 0); // Default coordinates for shops loaded from file
                    shopIndex = shopCount;
                    shopCount++;
                }

                if (shopIndex != -1)
                {
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