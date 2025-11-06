#include "marketplace.h"

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
    if (!medicines)
    {
        cout << "Memory allocation failed for medicines array\n";
        capacity = 0;
    }
}

Shop::Shop(string shopName)
{
    name = shopName;
    medicineCount = 0;
    capacity = 10; // Initial capacity
    medicines = new Medicine *[capacity];
    if (!medicines)
    {
        cout << "Memory allocation failed for medicines array\n";
        capacity = 0;
    }
}

Shop::Shop(const Shop &other)
{
    name = other.name;
    medicineCount = other.medicineCount;
    capacity = other.capacity;
    medicines = new Medicine *[capacity];
    if (!medicines)
    {
        cout << "Memory allocation failed for medicines array\n";
        capacity = 0;
        medicineCount = 0;
        return;
    }

    // Copy all medicines (deep copy)
    for (int i = 0; i < medicineCount; i++)
    {
        medicines[i] = new Medicine(*other.medicines[i]);
        if (!medicines[i])
        {
            cout << "Memory allocation failed for medicine " << i << "\n";
            // Clean up previously allocated medicines
            for (int j = 0; j < i; j++)
            {
                delete medicines[j];
            }
            delete[] medicines;
            medicines = nullptr;
            capacity = 0;
            medicineCount = 0;
            return;
        }
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
        if (!medicines)
        {
            cout << "Memory allocation failed for medicines array\n";
            capacity = 0;
            medicineCount = 0;
            return *this;
        }

        // Copy all medicines (deep copy)
        for (int i = 0; i < medicineCount; i++)
        {
            medicines[i] = new Medicine(*other.medicines[i]);
            if (!medicines[i])
            {
                cout << "Memory allocation failed for medicine " << i << "\n";
                // Clean up previously allocated medicines
                for (int j = 0; j < i; j++)
                {
                    delete medicines[j];
                }
                delete[] medicines;
                medicines = nullptr;
                capacity = 0;
                medicineCount = 0;
                return *this;
            }
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
        if (!newMedicines)
        {
            cout << "Memory allocation failed for expanding medicines array\n";
            return;
        }

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

    if (!medicines[medicineCount])
    {
        cout << "Memory allocation failed for medicine: " << medName << "\n";
        return;
    }

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

bool Shop::buyMedicine(string medName, int quantity)
{
    int index = findMedicine(medName);
    if (index != -1 && medicines[index]->canBuy(quantity))
    {
        double cost = medicines[index]->getPrice() * quantity;
        std::cout << "Cost: $" << fixed << setprecision(2) << cost << "\n";
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
    shops[0].addMedicine("Paracetamol", 5.50, 50, "Tablet");
    shops[0].addMedicine("Ibuprofen", 8.50, 30, "Tablet");

    shops[1] = Shop("HealthPlus");
    shops[1].addMedicine("Amoxicillin", 16.00, 25, "Tablet");
    shops[1].addMedicine("Cetirizine", 12.50, 20, "Tablet");

    shops[2] = Shop("QuickMeds");
    shops[2].addMedicine("Metformin", 19.00, 35, "Tablet");
    shops[2].addMedicine("Insulin", 45.00, 10, "General");
}

Marketplace::~Marketplace() {}

// MARK: Friend Function
void addShop(Marketplace &market, const string &shopName)
{
    if (market.shopCount >= 5)
    {
        cout << "Cannot add more shops. Limit reached." << "\n";
        return;
    }
    market.shops[market.shopCount] = Shop(shopName);
    market.shopCount++;
    cout << "Shop added: " << shopName << "\n";
}

// MARK: Behavior
void Marketplace::orderMedicine()
{
    string medName;
    int quantity;
    std::cout << "\nEnter medicine name: ";
    std::cin.ignore();
    getline(std::cin, medName);
    std::cout << "Enter quantity: ";
    std::cin >> quantity;

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
            if (shops[i].buyMedicine(medName, quantity))
                return;
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
        // MARK: err handling
        if (!in && !in.eof())
        {
            cout << "Error reading from " << filePath << "\n";
            break;
        }
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