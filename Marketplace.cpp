#include "marketplace.h"

using namespace std;

// MARK: Constructor/Destructor
Medicine::Medicine(string n, double p, int q) : name(n), price(p), quantity(q) {}
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
       << " - Stock: " << m.quantity;
    return os;
}

void display(const Medicine &m)
{
    std::cout << m << "\n";
}

// MARK: Constructor/Destructor
Shop::Shop()
{
    name = "";
    medicineCount = 0;
}

Shop::Shop(string shopName)
{
    name = shopName;
    medicineCount = 0;
}

Shop::~Shop() {}

void Shop::addMedicine(string medName, double price, int quantity)
{
    medicines[medicineCount] = Medicine(medName, price, quantity);
    medicineCount++;
}

int Shop::findMedicine(string medName) const
{
    for (int i = 0; i < medicineCount; i++)
    {
        if (medicines[i].getName() == medName)
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
        display(medicines[i]);
    }
}

bool Shop::buyMedicine(string medName, int quantity)
{
    int index = findMedicine(medName);
    if (index != -1 && medicines[index].canBuy(quantity))
    {
        double cost = medicines[index].getPrice() * quantity;
        std::cout << "Cost: $" << fixed << setprecision(2) << cost << "\n";
        std::cout << "Confirm purchase? (y/n): ";
        char choice;
        std::cin >> choice;
        if (choice == 'y' || choice == 'Y')
        {
            medicines[index].reduceStock(quantity);
            std::cout << "Purchase successful!\n";
            return true;
        }
    }
    return false;
}

// MARK: Constructor/Destructor
Marketplace::Marketplace()
{
    shopCount = 3;

    shops[0] = Shop("MediCare");
    shops[0].addMedicine("Paracetamol", 5.50, 50);
    shops[0].addMedicine("Ibuprofen", 8.50, 30);

    shops[1] = Shop("HealthPlus");
    shops[1].addMedicine("Amoxicillin", 16.00, 25);
    shops[1].addMedicine("Cetirizine", 12.50, 20);

    shops[2] = Shop("QuickMeds");
    shops[2].addMedicine("Metformin", 19.00, 35);
    shops[2].addMedicine("Insulin", 45.00, 10);
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
    std::cin >> medName;
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
            std::cout << "Price: $" << fixed << setprecision(2) << shops[i].medicines[idx].getPrice() << " per unit\n";
            std::cout << "Available: " << shops[i].medicines[idx].getQuantity() << " units\n";
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
    std::cin >> medName;

    std::cout << "\nSearch Results:\n";
    bool found = false;
    for (int i = 0; i < shopCount; i++)
    {
        int index = shops[i].findMedicine(medName);
        if (index != -1)
        {
            found = true;
            std::cout << shops[i].name << " - $" << fixed << setprecision(2)
                      << shops[i].medicines[index].getPrice()
                      << " - Stock: " << shops[i].medicines[index].getQuantity() << "\n";
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
void Marketplace::addMedicineAndPersist(int shopIndex, string medName, double price, int quantity, const string &filePath)
{
    if (shopIndex < 0 || shopIndex >= shopCount)
    {
        cout << "Invalid shop index." << "\n";
        return;
    }

    shops[shopIndex].addMedicine(medName, price, quantity);

    ofstream out(filePath, ios::app);
    if (!out)
    {
        cout << "Failed to open " << filePath << " for writing." << "\n";
        return;
    }
    out << shops[shopIndex].name << "," << medName << "," << fixed << setprecision(2) << price << "," << quantity << "\n";
    out.close();
    cout << "Item persisted to " << filePath << "\n";
}

// MARK: Abstraction + Polymorphism
void DummyFeature::run(Marketplace &market)
{
    cout << "[DummyFeature] Marketplace currently has " << market.getShopCount() << " shops." << "\n";
}