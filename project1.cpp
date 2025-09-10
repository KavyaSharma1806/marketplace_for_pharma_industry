#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

class Medicine
{
public:
    string name;
    double price;
    int quantity;

    Medicine()
    {
        name = "";
        price = 0;
        quantity = 0;
    }

    Medicine(string n, double p, int q)
    {
        name = n;
        price = p;
        quantity = q;
    }

    void display()
    {
        cout << name << " - $" << fixed << setprecision(2) << price
             << " - Stock: " << quantity << "\n";
    }

    bool canBuy(int amount)
    {
        return quantity >= amount;
    }

    void reduceStock(int amount)
    {
        quantity -= amount;
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

    void addMedicine(string medName, double price, int quantity)
    {
        medicines[medicineCount] = Medicine(medName, price, quantity);
        medicineCount++;
    }

    int findMedicine(string medName)
    {
        for (int i = 0; i < medicineCount; i++)
        {
            if (medicines[i].name == medName)
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
            medicines[i].display();
        }
    }

    bool buyMedicine(string medName, int quantity)
    {
        int index = findMedicine(medName);
        if (index != -1 && medicines[index].canBuy(quantity))
        {
            double cost = medicines[index].price * quantity;
            cout << "Cost: $" << fixed << setprecision(2) << cost << "\n";
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

// Main function
int main()
{
    Marketplace market;
    int choice;

    cout << "Welcome to Medicine Marketplace!\n";

    while (true)
    {
        showMenu();
        cin >> choice;

        switch (choice)
        {
        case 1:
            market.orderMedicine();
            break;
        case 2:
            market.searchMedicine();
            break;
        case 3:
            market.showAllShops();
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

        return 0;
    }
}

