#include "marketplace.h"
#include <iostream>

using namespace std;

void showMenu()
{
    cout << "\n=== Medicine Marketplace Menu ===\n";
    cout << "1. Order Medicine\n";
    cout << "2. Search for a Medicine\n";
    cout << "3. Show All Shop Inventories\n";
    cout << "4. Add Shop\n";
    cout << "5. Add Item + Save\n";
    cout << "6. Exit\n";
    cout << "===============================\n";
    cout << "Choose an option: ";
}

int main()
{
    // MARK: Object Creation
    Marketplace market;
    market.loadInventoryFromFile(); 
    int choice = 0;

    cout << "Welcome to the Medicine Marketplace!\n";

    while (choice != 6)
    {
        showMenu();
        cin >> choice;

        if (cin.fail())
        {
            cout << "Invalid input! Please enter a number." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            choice = 0;
            continue;
        }

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
        {
            string name;
            cout << "Enter new shop name: ";
            cin >> name;
            addShop(market, name);
            break;
        }
        case 5:
        {
            int index;
            string med;
            double price;
            int qty;
            cout << "Enter shop index (0-" << (market.getShopCount() - 1) << "): ";
            if (!(cin >> index) || index < 0 || index >= market.getShopCount())
            {
                cout << "Invalid shop index!" << endl;
                cin.clear();
                cin.ignore(10000, '\n');
                break;
            }
            cout << "Medicine name: ";
            cin.ignore();
            getline(cin, med);
            if (med.empty())
            {
                cout << "Medicine name cannot be empty!" << endl;
                break;
            }
            cout << "Price: ";
            if (!(cin >> price) || price < 0)
            {
                cout << "Invalid price!" << endl;
                cin.clear();
                cin.ignore(10000, '\n');
                break;
            }
            cout << "Quantity: ";
            if (!(cin >> qty) || qty < 0)
            {
                cout << "Invalid quantity!" << endl;
                cin.clear();
                cin.ignore(10000, '\n');
                break;
            }
            market.addMedicineAndPersist(index, med, price, qty);
            break;
        }

        case 6:
            break;
        default:
            cout << "Invalid choice! Please try again." << endl;
            break;
        }
    }
    return 0;
}