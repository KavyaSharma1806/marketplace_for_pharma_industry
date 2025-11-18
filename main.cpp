#include "marketplace.h"
#include <iostream>

using namespace std;

void showCustomerMenu()
{
    cout << "\n=== Medicine Marketplace ===\n";
    cout << "1. Order Medicine\n";
    cout << "2. Search for Medicine\n";
    cout << "3. View All Shop Inventories\n";
    cout << "0. Exit\n";
    cout << "============================\n";
    cout << "Choose an option: ";
}

void runCustomerMode(Marketplace &market)
{
    int choice = -1;

    while (choice != 0)
    {
        showCustomerMenu();
        cin >> choice;

        if (cin.fail())
        {
            cout << "Invalid input! Please enter a number.\n";
            cin.clear();
            cin.ignore(10000, '\n');
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
            cout << "Thank you for using our marketplace. Goodbye!\n";
            return;
        default:
            cout << "Invalid choice! Please try again.\n";
            break;
        }
    }
}

void showRetailerMenu()
{
    cout << "\n=== Retailer Dashboard ===\n";
    cout << "1. View All Shops\n";
    cout << "2. Select a Shop\n";
    cout << "3. Add New Medicine to Selected Shop\n";
    cout << "4. Refill Stock of Existing Medicine\n";
    cout << "5. Remove Medicine from Selected Shop\n";
    cout << "6. Return to Main Menu\n";
    cout << "==========================\n";
    cout << "Choose an option: ";
}

void runRetailerMode(Marketplace &market)
{
    int choice = -1;
    int selectedShop = -1;

    while (choice != 6)
    {
        showRetailerMenu();
        cin >> choice;

        if (cin.fail())
        {
            cout << "Invalid input! Please enter a number.\n";
            cin.clear();
            cin.ignore(10000, '\n');
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
                cout << "Invalid shop number! Please try again.\n";
                cin.clear();
                cin.ignore(10000, '\n');
                break;
            }
            selectedShop = shopNum;
            cout << "You have selected: " << market.getShop(shopNum).name << "\n";
            market.getShop(shopNum).showInventory();
            break;
        }

        case 3:
        {
            if (selectedShop == -1)
            {
                cout << "Please select a shop first using option 2!\n";
                break;
            }
            
            string medName;
            double price;
            int qty;
            string type;
            
            cout << "\n--- Add New Medicine ---\n";
            cout << "Enter medicine name: ";
            cin.ignore();
            getline(cin, medName);
            
            if (medName.empty())
            {
                cout << "Medicine name cannot be empty!\n";
                break;
            }
            
            cout << "Enter price: ";
            if (!(cin >> price) || price < 0)
            {
                cout << "Invalid price!\n";
                cin.clear();
                cin.ignore(10000, '\n');
                break;
            }
            
            cout << "Enter initial quantity: ";
            if (!(cin >> qty) || qty < 0)
            {
                cout << "Invalid quantity!\n";
                cin.clear();
                cin.ignore(10000, '\n');
                break;
            }
            
            cout << "Enter type (Tablet/Syrup/General): ";
            cin >> type;
            
            market.addMedicineAndPersist(selectedShop, medName, price, qty, type);
            break;
        }

        case 4:
        {
            if (selectedShop == -1)
            {
                cout << "Please select a shop first using option 2!\n";
                break;
            }
            
            string medName;
            int additionalQty;
            
            cout << "\n--- Refill Stock ---\n";
            cout << "Enter medicine name: ";
            cin.ignore();
            getline(cin, medName);
            
            if (medName.empty())
            {
                cout << "Medicine name cannot be empty!\n";
                break;
            }
            
            cout << "Enter quantity to add: ";
            if (!(cin >> additionalQty) || additionalQty <= 0)
            {
                cout << "Invalid quantity! Please enter a positive number.\n";
                cin.clear();
                cin.ignore(10000, '\n');
                break;
            }
            
            market.refillStockAndPersist(selectedShop, medName, additionalQty);
            break;
        }

        case 5:
        {
            if (selectedShop == -1)
            {
                cout << "Please select a shop first using option 2!\n";
                break;
            }
            
            string medName;
            cout << "\n--- Remove Medicine ---\n";
            cout << "Enter medicine name to remove: ";
            cin.ignore();
            getline(cin, medName);
            
            if (medName.empty())
            {
                cout << "Medicine name cannot be empty!\n";
                break;
            }
            
            if (market.removeMedicineFromShop(selectedShop, medName))
            {
                cout << "Medicine removed successfully!\n";
            }
            else
            {
                cout << "Medicine not found in the selected shop!\n";
            }
            break;
        }

        case 6:
            cout << "Returning to main menu...\n";
            return;

        default:
            cout << "Invalid choice! Please try again.\n";
            break;
        }
    }
}

int main()
{
    Marketplace market;
    market.loadInventoryFromFile();

    cout << "========================================\n";
    cout << "   Welcome to Medicine Marketplace!    \n";
    cout << "========================================\n\n";
    
    cout << "Select your role:\n";
    cout << "1. Customer\n";
    cout << "2. Retailer\n";
    cout << "Enter choice: ";

    int mode;
    cin >> mode;

    if (cin.fail())
    {
        cout << "Invalid input! Please restart and enter 1 or 2.\n";
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
        cout << "Invalid mode! Please enter 1 for Customer or 2 for Retailer.\n";
        break;
    }

    return 0;
}