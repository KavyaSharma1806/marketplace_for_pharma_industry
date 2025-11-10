#include "marketplace.h"
#include <iostream>

using namespace std;

// MARK: Customer Menu Functions
void showCustomerMenu()
{
    cout << "\n=== Medicine Marketplace Menu ===\n";
    cout << "1. Order Medicine\n";
    cout << "2. Search for a Medicine\n";
    cout << "3. Show All Shop Inventories\n";
    // cout << "4. Add Shop\n";
    // cout << "5. Add Item + Save\n";
    cout << "0. Exit\n";
    cout << "===============================\n";
    cout << "Choose an option: ";
}

void runCustomerMode(Marketplace &market)
{
    int choice = 0;

    while (choice != 6)
    {
        showCustomerMenu();
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
        case 0:
            cout << "Exiting the program. Goodbye!" << endl;
            return ;
        default:
            cout << "Invalid choice! Please try again." << endl;
            break;
        }
    }
}

// MARK: Retailer Menu Functions
void showRetailerMenu()
{
    cout << "\n=== Retailer Menu ===\n";
    cout << "1. List Shops\n";
    cout << "2. Select Shop\n";
    cout << "3. Add Medicine to Shop\n";
    cout << "4. Remove Medicine from Shop\n";
    cout << "5. Back to Main Menu\n";
    cout << "=====================\n";
    cout << "Choose an option: ";
}

void runRetailerMode(Marketplace &market)
{
    int choice = 0;
    int selectedShop = -1;

    while (choice != 5)
    {
        showRetailerMenu();
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
            market.listShopsForRetailer();
            break;

        case 2:
        {
            market.listShopsForRetailer();
            cout << "Enter shop number: ";
            int shopNum;
            if (!(cin >> shopNum) || shopNum < 0 || shopNum >= market.getShopCount())
            {
                cout << "Invalid shop number!" << endl;
                cin.clear();
                cin.ignore(10000, '\n');
                break;
            }
            selectedShop = shopNum;
            cout << "Selected shop: " << market.getShop(shopNum).name << endl;
            market.getShop(shopNum).showInventory();
            break;
        }

        case 3:
        {
            if (selectedShop == -1)
            {
                cout << "Please select a shop first (option 2)!" << endl;
                break;
            }
            string med;
            double price;
            int qty;
            string type;
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
            cout << "Type (Tablet/Syrup/General): ";
            cin >> type;
            market.addMedicineAndPersist(selectedShop, med, price, qty, type);
            cout << "Medicine added successfully!" << endl;
            break;
        }

        case 4:
        {
            if (selectedShop == -1)
            {
                cout << "Please select a shop first (option 2)!" << endl;
                break;
            }
            string med;
            cout << "Enter medicine name to remove: ";
            cin.ignore();
            getline(cin, med);
            if (med.empty())
            {
                cout << "Medicine name cannot be empty!" << endl;
                break;
            }
            if (market.removeMedicineFromShop(selectedShop, med))
            {
                cout << "Medicine removed successfully!" << endl;
            }
            else
            {
                cout << "Medicine not found in shop!" << endl;
            }
            break;
        }

        case 5:
            return;

        default:
            cout << "Invalid choice! Please try again." << endl;
            break;
        }
    }
}

int main()
{
    // MARK: Object Creation
    Marketplace market;
    market.loadInventoryFromFile();

    cout << "Welcome to the Medicine Marketplace!\n";
    cout << "\nEnter as Customer (1) or Retailer (2): ";

    int mode;
    cin >> mode;

    if (cin.fail())
    {
        cout << "Invalid input! Please enter 1 or 2." << endl;
        cin.clear();
        cin.ignore(10000, '\n');
        return 1;
    }

    switch (mode)
    {
    case 1:
        runCustomerMode(market);
        break;
    case 2:
        runRetailerMode(market);
        break;
    default:
        cout << "Invalid mode! Please enter 1 for Customer or 2 for Retailer." << endl;
        break;
    }

    return 0;
}