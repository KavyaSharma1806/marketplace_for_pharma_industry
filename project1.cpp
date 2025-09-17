#include <iostream>
#include <string>
#include <iomanip>

using namespace std;


class Medicine
{
<<<<<<< HEAD
    // MARK: ENCAPSULATION 
=======
>>>>>>> 41268375bec9c14ce0789d52915e821e863119e0
private:
    string name;
    double price;
    int quantity;

public:
<<<<<<< HEAD
    // MARK: CONSTRUCTOR
=======
>>>>>>> 41268375bec9c14ce0789d52915e821e863119e0
    Medicine(string n = "", double p = 0, int q = 0)
    {
        name = n;
        price = p;
        quantity = q;
    }

<<<<<<< HEAD
    // MARK: DESTRUCTOR 
=======
>>>>>>> 41268375bec9c14ce0789d52915e821e863119e0
    ~Medicine() {}

    string getName() const { return name; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }

<<<<<<< HEAD
    // MARK: FRIEND FUNCTION
=======
>>>>>>> 41268375bec9c14ce0789d52915e821e863119e0
    friend void display(const Medicine &m);

    bool canBuy(int amount)
    {
        return quantity >= amount;
    }

    void reduceStock(int amount)
    {
        quantity -= amount;
    }
};

void display(const Medicine &m)
{
    cout << m.name << " - $" << fixed << setprecision(2) << m.price
         << " - Stock: " << m.quantity << "\n";
}

// #medicine class
class person // abstracted
{
private:
    Medicine medicine[50];
};

// MARK: INHERITANCE

class records : public person // inheritance
{
public:
    Medicine *logs;

private:
    records() : person()
    {
        // MARK: DYNAMIC MEMORY ALLOCATION - OOP Concept : Allocating memory at runtime
        logs = new Medicine[50];
        for (int i = 0; i < 50; i++)
        {
            logs[i] = Medicine();
        }
    }
};
class Shop
{
public:
    string name;
    Medicine medicines[50];
    int medicineCount;

    Shop()
    {
        name = "";
        medicineCount = 0;
    }

    Shop(string shopName)
    {
        name = shopName;
        medicineCount = 0;
    }

    ~Shop() {}

    void addMedicine(string medName, double price, int quantity = 1)
    {
        medicines[medicineCount] = Medicine(medName, price, quantity);
        medicineCount++;
    }

    int findMedicine(string medName)
    {
        for (int i = 0; i < medicineCount; i++)
        {
            if (medicines[i].getName() == medName)
            {
                return i;
            }
        }
        return -1;
    }

    void showInventory()
    {
        cout << "\n=== " << name << " ===" << "\n";
        for (int i = 0; i < medicineCount; i++)
        {
            cout << "  ";
            display(medicines[i]);
        }
    }

    bool buyMedicine(string medName, int quantity = 1)
    {
        int index = findMedicine(medName);
        if (index != -1 && medicines[index].canBuy(quantity))
        {
            double cost = medicines[index].getPrice() * quantity;
            cout << "Cost: $" << cost << "\n";
            cout << "Confirm purchase? (y/n): ";
            char choice;
            cin >> choice;

            if (choice == 'y' || choice == 'Y')
            {
                medicines[index].reduceStock(quantity);
                cout << "Purchase successful!\n";
                return true;
            }
        }
        return false;
    }
};

class Wholesaler
{
public:
    string name;

    Wholesaler()
    {
        name = "Main Wholesaler";
    }
    ~Wholesaler() {}
    double getPrice(string medName = "Generic")
    {
        // Simple pricing - just return a default price
        return 10.0;
    }

    void buyFromWholesaler(string medName, int quantity)
    {
        double price = getPrice(medName) * 1.2; // 20% markup
        double cost = price * quantity;

        cout << "\nWholesaler can provide " << medName << "\n";
        cout << "Price (with markup): $" << fixed << setprecision(2) << cost << "\n";
        cout << "Buy from wholesaler? (y/n): ";

        char choice;
        cin >> choice;

        if (choice == 'y' || choice == 'Y')
        {
            cout << "Order placed with wholesaler!\n";
        }
        else
        {
            cout << "Order cancelled.\n";
        }
    }
};

class Marketplace
{
public:
    Shop shops[5];
    Wholesaler wholesaler;
    int shopCount;

    Marketplace()
    {
        shopCount = 5;

        shops[0] = Shop("MediCare");
        shops[0].addMedicine("paracetamol", 5.50, 50);
        shops[0].addMedicine("Ibuprofen", 8.50, 30);
        shops[0].addMedicine("Aspirin", 6.50, 40);

        shops[1] = Shop("HealthPlus");
        shops[1].addMedicine("Amoxicillin", 16.00, 25);
        shops[1].addMedicine("Cetirizine", 12.50, 20);
        shops[1].addMedicine("Paracetamol", 5.25, 60);

        shops[2] = Shop("QuickMeds");
        shops[2].addMedicine("Metformin", 19.00, 35);
        shops[2].addMedicine("Insulin", 45.00, 10);
        shops[2].addMedicine("Aspirin", 7.00, 50);

        shops[3] = Shop("CityPharmacy");
        shops[3].addMedicine("Vitamin-C", 6.00, 100);
        shops[3].addMedicine("Cough-Syrup", 12.00, 30);
        shops[3].addMedicine("Ibuprofen", 8.25, 45);

        shops[4] = Shop("1212");
        shops[4].addMedicine("Vitamin-C", 6.00, 100);
        shops[4].addMedicine("Cough-Syrup", 12.00, 30);
        shops[4].addMedicine("rahul", 800, 45);
    }
    void orderMedicine()
    {
        string medName;
        int quantity;

        cout << "\nEnter medicine name: ";
        cin >> medName;
        cout << "Enter quantity: ";
        cin >> quantity;

        cout << "\nSearching in shops...\n";

        bool found = false;

        // Check each shop
        for (int i = 0; i < shopCount; i++)
        {
            int medIndex = shops[i].findMedicine(medName);
            if (medIndex != -1)
            {
                found = true;
                Medicine &med = shops[i].medicines[medIndex];

                cout << "\nFound at " << shops[i].name << "\n";
                cout << "Price: $" << fixed << setprecision(2) << med.getPrice() << " per unit\n";
                cout << "Available: " << med.getQuantity() << " units\n";

                if (med.canBuy(quantity))
                {
                    if (shops[i].buyMedicine(medName, quantity))
                    {
                        return; // Purchase complete
                    }
                }
                else
                {
                    cout << "Not enough stock in this shop.\n";
                }
            }
        }

        if (!found)
        {
            cout << "\nMedicine not found in any shop.\n";
        }

        // Try wholesaler as last option
        cout << "\nChecking wholesaler...\n";
        wholesaler.buyFromWholesaler(medName, quantity);
    }

    void searchMedicine()
    {
        string medName;
        cout << "\nEnter medicine name to search: ";
        cin >> medName;

        cout << "\nSearch Results:\n";
        bool found = false;

        for (int i = 0; i < shopCount; i++)
        {
            int index = shops[i].findMedicine(medName);
            if (index != -1)
            {
                found = true;
                cout << shops[i].name << " - $"
                     << fixed << setprecision(2) << shops[i].medicines[index].getPrice()
                     << " - Stock: " << shops[i].medicines[index].getQuantity() << "\n";
            }
        }

        if (!found)
        {
            cout << "Medicine not found in shops.\n";
            cout << "Wholesaler can arrange it at $"
                 << fixed << setprecision(2) << (wholesaler.getPrice(medName) * 1.2)
                 << " per unit.\n";
        }
    }

    void showAllShops()
    {
        for (int i = 0; i < shopCount; i++)
        {
            shops[i].showInventory();
        }
    }
};

// Simple menu function
void showMenu()
{
    cout << "\n=== Medicine Shop ===\n";
    cout << "1. Order Medicine\n";
    cout << "2. Search Medicine\n";
    cout << "3. Show All Shops\n";
    cout << "4. Exit\n";
    cout << "Choose: ";
}

int main()
{
<<<<<<< HEAD
    // - OOP Concept : Creating objects using new operator
    // MARK: INSTANTIATION
=======
>>>>>>> 41268375bec9c14ce0789d52915e821e863119e0
    Marketplace *market = new Marketplace();
    int choice;

    cout << "Welcome to Medicine Marketplace!\n";

    while (true)
    {
        showMenu();
        cin >> choice;

        switch (choice)
        {
        case 1:
            market->orderMedicine();
            break;
        case 2:
            market->searchMedicine();
            break;
        case 3:
            market->showAllShops();
            break;
        case 4:
            cout << "Thank you! Goodbye!\n";
            break;
        default:
            cout << "Invalid choice!\n";
            break;
        }
        if (choice == 4)
            break;
    }

    delete market;
    return 0;
}