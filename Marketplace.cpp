#include "marketplace.h"
#include <cmath>
#include <vector>

using namespace std;

// ==================== Medicine Class ====================
Medicine::Medicine(string n, double p, int q, string t) 
    : name(n), price(p), quantity(q), type(t) {}

Medicine::~Medicine() {}

string Medicine::getName() const { return name; }
double Medicine::getPrice() const { return price; }
int Medicine::getQuantity() const { return quantity; }

bool Medicine::canBuy(int amount) const 
{ 
    return quantity >= amount; 
}

void Medicine::reduceStock(int amount) 
{ 
    if (amount <= quantity) {
        quantity -= amount; 
    }
}

ostream &operator<<(ostream &os, const Medicine &m)
{
    os << m.name << " - $" << fixed << setprecision(2) << m.price
       << " - Stock: " << m.quantity << " - Type: " << m.type;
    return os;
}

void display(const Medicine &m)
{
    cout << m << "\n";
}

void Medicine::displayInfo() const
{
    cout << *this << "\n";
}

string Medicine::getCategory() const
{
    return "Medicine";
}

// ==================== Tablet Class ====================
Tablet::Tablet(const string &n, double p, int q) 
    : Medicine(n, p, q, "Tablet") {}

Tablet::~Tablet() {}

void Tablet::displayInfo() const
{
    cout << "[Tablet] ";
    Medicine::displayInfo();
}

string Tablet::getCategory() const { return "Tablet"; }

// ==================== Syrup Class ====================
Syrup::Syrup(const string &n, double p, int q) 
    : Medicine(n, p, q, "Syrup") {}

Syrup::~Syrup() {}

void Syrup::displayInfo() const
{
    cout << "[Syrup]  ";
    Medicine::displayInfo();
}

string Syrup::getCategory() const { return "Syrup"; }

// ==================== Shop Class ====================
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
    x = other.x;
    y = other.y;
    medicines = new Medicine *[capacity];

    for (int i = 0; i < medicineCount; i++)
    {
        // Deep copy each medicine based on its type
        if (other.medicines[i]->getCategory() == "Tablet") {
            medicines[i] = new Tablet(*dynamic_cast<Tablet*>(other.medicines[i]));
        } else if (other.medicines[i]->getCategory() == "Syrup") {
            medicines[i] = new Syrup(*dynamic_cast<Syrup*>(other.medicines[i]));
        } else {
            medicines[i] = new Medicine(*other.medicines[i]);
        }
    }
}

Shop &Shop::operator=(const Shop &other)
{
    if (this != &other)
    {
        // Clean up existing memory
        for (int i = 0; i < medicineCount; i++)
        {
            delete medicines[i];
        }
        delete[] medicines;

        // Copy data from other
        name = other.name;
        medicineCount = other.medicineCount;
        capacity = other.capacity;
        x = other.x;
        y = other.y;
        medicines = new Medicine *[capacity];

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
    // First check if medicine already exists
    int existingIndex = findMedicine(medName);
    if (existingIndex != -1)
    {
        cout << "Medicine '" << medName << "' already exists in inventory!\n";
        cout << "Current stock: " << medicines[existingIndex]->getQuantity() << " units\n";
        cout << "Use 'Refill Stock' option to add more quantity.\n";
        return;
    }

    // Expand capacity if needed
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

    // Create the right type of medicine
    if (type == "Tablet")
        medicines[medicineCount] = new Tablet(medName, price, quantity);
    else if (type == "Syrup")
        medicines[medicineCount] = new Syrup(medName, price, quantity);
    else
        medicines[medicineCount] = new Medicine(medName, price, quantity, type);

    medicineCount++;
}

bool Shop::refillStock(string medName, int additionalQuantity)
{
    int index = findMedicine(medName);
    if (index == -1)
    {
        cout << "Medicine '" << medName << "' not found in inventory!\n";
        cout << "Please add it as a new medicine first.\n";
        return false;
    }

    if (additionalQuantity <= 0)
    {
        cout << "Invalid quantity! Please enter a positive number.\n";
        return false;
    }

    // Access private quantity through a public method
    // We need to add the quantity to existing stock
    int currentStock = medicines[index]->getQuantity();
    
    // We'll need to update the Medicine class to support adding stock
    // For now, we can work around by creating a new medicine with updated quantity
    string name = medicines[index]->getName();
    double price = medicines[index]->getPrice();
    string type = medicines[index]->getType();
    string category = medicines[index]->getCategory();
    int newQuantity = currentStock + additionalQuantity;

    // Delete old medicine and create new one with updated quantity
    delete medicines[index];
    
    if (category == "Tablet")
        medicines[index] = new Tablet(name, price, newQuantity);
    else if (category == "Syrup")
        medicines[index] = new Syrup(name, price, newQuantity);
    else
        medicines[index] = new Medicine(name, price, newQuantity, type);

    cout << "Stock refilled successfully!\n";
    cout << "Previous stock: " << currentStock << " units\n";
    cout << "Added: " << additionalQuantity << " units\n";
    cout << "New stock: " << newQuantity << " units\n";
    
    return true;
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
    cout << "\n=== " << name << " ===\n";
    for (int i = 0; i < medicineCount; i++)
    {
        cout << "  ";
        medicines[i]->displayInfo();
    }
}

bool Shop::buyMedicine(string medName, int quantity, double deliveryCharge)
{
    int index = findMedicine(medName);
    if (index == -1 || !medicines[index]->canBuy(quantity))
    {
        return false;
    }

    double baseCost = medicines[index]->getPrice() * quantity;
    double totalCost = baseCost + deliveryCharge;

    cout << "Base Cost: $" << fixed << setprecision(2) << baseCost << "\n";
    if (deliveryCharge > 0)
    {
        cout << "Delivery Charge: $" << fixed << setprecision(2) << deliveryCharge << "\n";
    }
    cout << "Total Cost: $" << fixed << setprecision(2) << totalCost << "\n";
    cout << "Confirm purchase? (y/n): ";
    
    char choice;
    cin >> choice;
    
    if (choice == 'y' || choice == 'Y')
    {
        medicines[index]->reduceStock(quantity);
        cout << "Purchase successful!\n";
        return true;
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

    // Shift remaining medicines
    for (int i = index; i < medicineCount - 1; i++)
    {
        medicines[i] = medicines[i + 1];
    }

    medicineCount--;
    return true;
}

const Medicine *Shop::getMedicine(int index) const
{
    if (index >= 0 && index < medicineCount) {
        return medicines[index];
    }
    return nullptr;
}

int Shop::getMedicineCount() const
{
    return medicineCount;
}

// ==================== Marketplace Class ====================
Marketplace::Marketplace()
{
    shopCount = 3;

    // Initialize default shops with some basic inventory
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

void Marketplace::logOrder(const string &medName, int quantity, const string &shopName,
                           int customerX, int customerY, int shopX, int shopY,
                           double distance, double baseCost, double deliveryCharge, double totalCost) const
{
    ofstream logFile("log.txt", ios::app);
    if (!logFile)
    {
        cerr << "Warning: Could not write to log file.\n";
        return;
    }

    logFile << "========================================\n";
    logFile << "Order Details\n";
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

void addShop(Marketplace &market, const string &shopName)
{
    if (market.shopCount >= 5)
    {
        cout << "Cannot add more shops. Maximum limit reached.\n";
        return;
    }
    market.shops[market.shopCount] = Shop(shopName);
    market.shops[market.shopCount].setCoordinates(0, 0);
    market.shopCount++;
    cout << "Shop '" << shopName << "' added successfully.\n";
}

void Marketplace::orderMedicine()
{
    string medName;
    int quantity;
    int customerX, customerY;

    cout << "\nEnter medicine name: ";
    cin.ignore();
    getline(cin, medName);
    cout << "Enter quantity: ";
    cin >> quantity;

    if (quantity <= 0)
    {
        cout << "Invalid quantity! Please enter a positive number.\n";
        return;
    }

    cout << "Enter your location (x y): ";
    if (!(cin >> customerX >> customerY))
    {
        cout << "Invalid coordinates! Please enter two numbers.\n";
        cin.clear();
        cin.ignore(10000, '\n');
        return;
    }

    // Find all shops that have this medicine in stock
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

    // Handle case where no shop has the medicine
    if (availableCount == 0)
    {
        double lowestPrice = -1.0;
        bool medicineExists = false;

        // Check if the medicine exists anywhere
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

        if (medicineExists)
        {
            cout << "\nMedicine \"" << medName << "\" found but not enough stock in any shop.\n";
        }
        else
        {
            cout << "\nMedicine \"" << medName << "\" not available in any shop.\n";
        }

        cout << "We can arrange it from a wholesaler at 2x the market price.\n\n";

        double wholesalerPricePerUnit = medicineExists ? (lowestPrice * 2) : 100;
        double wholesalerBaseCost = wholesalerPricePerUnit * quantity;

        int wholesalerX = 0;
        int wholesalerY = 0;
        double wholesalerDistance = calculateDistance(customerX, customerY, wholesalerX, wholesalerY);
        double wholesalerDeliveryCharge = calculateDeliveryCharge(wholesalerDistance);
        double wholesalerTotalCost = wholesalerBaseCost + wholesalerDeliveryCharge;

        cout << "=== Wholesaler Option ===\n";
        cout << "Medicine: " << medName << "\n";
        cout << "Quantity: " << quantity << "\n";
        if (medicineExists)
        {
            cout << "Market price: $" << fixed << setprecision(2) << lowestPrice << " per unit\n";
        }
        cout << "Wholesaler price: $" << fixed << setprecision(2) << wholesalerPricePerUnit << " per unit\n";
        cout << "Base cost: $" << fixed << setprecision(2) << wholesalerBaseCost << "\n";
        cout << "Delivery charge: $" << fixed << setprecision(2) << wholesalerDeliveryCharge << "\n";
        cout << "Total cost: $" << fixed << setprecision(2) << wholesalerTotalCost << "\n\n";

        cout << "Would you like to order from the wholesaler? (y/n): ";
        char confirm;
        cin >> confirm;

        if (confirm == 'y' || confirm == 'Y')
        {
            cout << "\n=== Processing wholesaler order ===\n";
            cout << "Base Cost: $" << fixed << setprecision(2) << wholesalerBaseCost << "\n";
            if (wholesalerDeliveryCharge > 0)
            {
                cout << "Delivery Charge: $" << fixed << setprecision(2) << wholesalerDeliveryCharge << "\n";
            }
            cout << "Total Cost: $" << fixed << setprecision(2) << wholesalerTotalCost << "\n";
            cout << "Order placed successfully!\n";

            logOrder(medName, quantity, "Wholesaler", customerX, customerY,
                     wholesalerX, wholesalerY, wholesalerDistance, wholesalerBaseCost,
                     wholesalerDeliveryCharge, wholesalerTotalCost);
        }
        else
        {
            cout << "Order cancelled.\n";
        }
        return;
    }

    // Display available options
    cout << "\n=== Available Options for \"" << medName << "\" (Quantity: " << quantity << ") ===\n";
    cout << "Your location: (" << customerX << ", " << customerY << ")\n\n";

    for (int i = 0; i < availableCount; i++)
    {
        int shopIdx = availableShops[i];
        int medIdx = shops[shopIdx].findMedicine(medName);

        cout << i + 1 << ". " << shops[shopIdx].name << "\n";
        cout << "   Price per unit: $" << fixed << setprecision(2)
             << shops[shopIdx].getMedicine(medIdx)->getPrice() << "\n";
        cout << "   Available stock: " << shops[shopIdx].getMedicine(medIdx)->getQuantity() << " units\n";
        cout << "   Shop location: (" << shops[shopIdx].getX() << ", "
             << shops[shopIdx].getY() << ")\n";
        cout << "   Distance: " << fixed << setprecision(2) << distances[i] << " units\n";
        cout << "   Base cost: $" << fixed << setprecision(2) << baseCosts[i] << "\n";
        cout << "   Delivery charge: $" << fixed << setprecision(2) << deliveryCharges[i] << "\n";
        cout << "   Total cost: $" << fixed << setprecision(2) << totalCosts[i] << "\n\n";
    }

    cout << "Select a shop (1-" << availableCount << ") or 0 to cancel: ";
    int choice;
    if (!(cin >> choice) || choice < 0 || choice > availableCount)
    {
        cout << "Invalid selection! Order cancelled.\n";
        cin.clear();
        cin.ignore(10000, '\n');
        return;
    }

    if (choice == 0)
    {
        cout << "Order cancelled.\n";
        return;
    }

    int selectedShopIdx = availableShops[choice - 1];
    int shopX = shops[selectedShopIdx].getX();
    int shopY = shops[selectedShopIdx].getY();

    cout << "\n=== Processing order from " << shops[selectedShopIdx].name << " ===\n";

    if (shops[selectedShopIdx].buyMedicine(medName, quantity, deliveryCharges[choice - 1]))
    {
        logOrder(medName, quantity, shops[selectedShopIdx].name, customerX, customerY,
                 shopX, shopY, distances[choice - 1], baseCosts[choice - 1],
                 deliveryCharges[choice - 1], totalCosts[choice - 1]);
    }
    else
    {
        cout << "Purchase cancelled.\n";
    }
}

void Marketplace::searchMedicine() const
{
    string medName;
    cout << "\nEnter medicine name to search: ";
    cin.ignore();
    getline(cin, medName);

    cout << "\nSearch Results:\n";
    bool found = false;
    for (int i = 0; i < shopCount; i++)
    {
        int index = shops[i].findMedicine(medName);
        if (index != -1)
        {
            found = true;
            cout << shops[i].name << " - $" << fixed << setprecision(2)
                 << shops[i].getMedicine(index)->getPrice()
                 << " - Stock: " << shops[i].getMedicine(index)->getQuantity() << "\n";
        }
    }
    if (!found)
    {
        cout << "Medicine not found in any shop.\n";
    }
}

void Marketplace::showAllShops() const
{
    for (int i = 0; i < shopCount; i++)
    {
        shops[i].showInventory();
    }
}

void Marketplace::listShopsForRetailer() const
{
    cout << "\n=== Available Shops ===\n";
    for (int i = 0; i < shopCount; i++)
    {
        cout << i << ". " << shops[i].name 
             << " (Location: " << shops[i].getX() << ", " << shops[i].getY() << ")\n";
    }
    cout << "======================\n";
}

bool Marketplace::removeMedicineFromShop(int shopIndex, string medName)
{
    if (shopIndex < 0 || shopIndex >= shopCount)
    {
        return false;
    }
    return shops[shopIndex].removeMedicine(medName);
}

bool Marketplace::refillStockInShop(int shopIndex, string medName, int additionalQuantity)
{
    if (shopIndex < 0 || shopIndex >= shopCount)
    {
        cout << "Invalid shop index.\n";
        return false;
    }
    return shops[shopIndex].refillStock(medName, additionalQuantity);
}

void Marketplace::addMedicineAndPersist(int shopIndex, string medName, double price, 
                                        int quantity, const string &type, const string &filePath)
{
    if (shopIndex < 0 || shopIndex >= shopCount)
    {
        cout << "Invalid shop index.\n";
        return;
    }

    // Check if medicine already exists
    int existingIndex = shops[shopIndex].findMedicine(medName);
    if (existingIndex != -1)
    {
        cout << "Medicine '" << medName << "' already exists in this shop!\n";
        cout << "Use 'Refill Stock' option instead to add more quantity.\n";
        return;
    }

    shops[shopIndex].addMedicine(medName, price, quantity, type);

    ofstream out(filePath, ios::app);
    if (!out)
    {
        cout << "Failed to open " << filePath << " for writing.\n";
        return;
    }
    out << shops[shopIndex].name << "," << medName << "," 
        << fixed << setprecision(2) << price << "," 
        << quantity << "," << type << "\n";
    out.close();
    cout << "Medicine added and saved to " << filePath << "\n";
}

void Marketplace::refillStockAndPersist(int shopIndex, string medName, int additionalQuantity, const string &filePath)
{
    if (shopIndex < 0 || shopIndex >= shopCount)
    {
        cout << "Invalid shop index.\n";
        return;
    }

    // Check if medicine exists
    int medIndex = shops[shopIndex].findMedicine(medName);
    if (medIndex == -1)
    {
        cout << "Medicine '" << medName << "' not found in this shop!\n";
        cout << "Please add it as a new medicine first.\n";
        return;
    }

    // Refill the stock
    if (!shops[shopIndex].refillStock(medName, additionalQuantity))
    {
        return;
    }

    // Update the CSV file by rewriting it
    // First, read all current data
    ifstream inFile(filePath);
    vector<string> allLines;
    string line;
    
    if (inFile.is_open())
    {
        while (getline(inFile, line))
        {
            allLines.push_back(line);
        }
        inFile.close();
    }

    // Now rewrite the file with updated quantities
    ofstream outFile(filePath);
    if (!outFile)
    {
        cout << "Warning: Could not update " << filePath << "\n";
        return;
    }

    for (const string &csvLine : allLines)
    {
        stringstream ss(csvLine);
        string shopName, med, priceStr, qtyStr, typeStr;
        
        if (getline(ss, shopName, ',') &&
            getline(ss, med, ',') &&
            getline(ss, priceStr, ',') &&
            getline(ss, qtyStr, ','))
        {
            if (!getline(ss, typeStr))
            {
                typeStr = "General";
            }

            // Check if this is the line we need to update
            if (shopName == shops[shopIndex].name && med == medName)
            {
                // Get the updated quantity
                const Medicine *updatedMed = shops[shopIndex].getMedicine(medIndex);
                outFile << shopName << "," << med << "," 
                       << fixed << setprecision(2) << updatedMed->getPrice() << "," 
                       << updatedMed->getQuantity() << "," << typeStr << "\n";
            }
            else
            {
                // Write the original line
                outFile << csvLine << "\n";
            }
        }
    }
    
    outFile.close();
    cout << "Stock updated in " << filePath << "\n";
}

void Marketplace::loadInventoryFromFile(const string &filePath)
{
    ifstream in(filePath);
    if (!in)
    {
        cout << "Could not open " << filePath << ". Using default inventory.\n";
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

                // Find the shop or create it
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
                    // Check for duplicates
                    int existingMedIndex = shops[shopIndex].findMedicine(medName);
                    if (existingMedIndex == -1)
                    {
                        shops[shopIndex].addMedicine(medName, price, quantity, typeStr);
                    }
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