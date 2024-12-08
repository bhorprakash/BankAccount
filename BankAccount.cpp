#include <iostream>
#include <string>
#include <memory>
#include <vector>

using namespace std;

// Base Class - BankAccount
class BankAccount {
protected:
    string accountHolderName;
    double balance;

public:
    // Constructor
    BankAccount(const string &name, double initialBalance)
        : accountHolderName(name), balance(initialBalance) {}

    // Method to deposit money
    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            cout << "Deposited ₹" << amount << ". New balance: ₹" << balance << endl;
        } else {
            cout << "Invalid deposit amount." << endl;
        }
    }

    // Abstract Method for withdrawing money
    virtual void withdraw(double amount) = 0;

    // Getter for balance
    double getBalance() const {
        return balance;
    }

    // Virtual Destructor
    virtual ~BankAccount() {
        cout << "BankAccount Destructor called for " << accountHolderName << endl;
    }

    // Method to display account details
    virtual void displayAccountInfo() const {
        cout << "Account Holder: " << accountHolderName << "\nBalance: ₹" << balance << endl;
    }
};

// Derived Class - SavingsAccount
class SavingsAccount : public BankAccount {
private:
    double interestRate;

public:
    SavingsAccount(const string &name, double initialBalance, double rate)
        : BankAccount(name, initialBalance), interestRate(rate) {}

    // Override withdraw method
    void withdraw(double amount) override {
        if (amount <= balance) {
            balance -= amount;
            cout << "Withdrew ₹" << amount << " from Savings Account. New balance: ₹" << balance << endl;
        } else {
            cout << "Insufficient balance in Savings Account." << endl;
        }
    }

    // Method to add interest
    void addInterest() {
        balance += balance * (interestRate / 100);
        cout << "Interest added at " << interestRate << "% rate. New balance: ₹" << balance << endl;
    }

    // Override display method
    void displayAccountInfo() const override {
        cout << "Savings Account\n";
        BankAccount::displayAccountInfo();
        cout << "Interest Rate: " << interestRate << "%" << endl;
    }
};

// Derived Class - CheckingAccount
class CheckingAccount : public BankAccount {
private:
    double overdraftLimit;

public:
    CheckingAccount(const string &name, double initialBalance, double overdraft)
        : BankAccount(name, initialBalance), overdraftLimit(overdraft) {}

    // Override withdraw method
    void withdraw(double amount) override {
        if (amount <= balance + overdraftLimit) {
            balance -= amount;
            cout << "Withdrew ₹" << amount << " from Checking Account. New balance: ₹" << balance << endl;
        } else {
            cout << "Exceeded overdraft limit in Checking Account." << endl;
        }
    }

    // Override display method
    void displayAccountInfo() const override {
        cout << "Checking Account\n";
        BankAccount::displayAccountInfo();
        cout << "Overdraft Limit: ₹" << overdraftLimit << endl;
    }
};

// Function to create a Savings Account
shared_ptr<BankAccount> createSavingsAccount() {
    string name;
    double initialBalance, interestRate;

    cout << "Enter account holder's name: ";
    cin >> name;
    cout << "Enter initial balance: ";
    cin >> initialBalance;
    cout << "Enter interest rate (%): ";
    cin >> interestRate;

    return make_shared<SavingsAccount>(name, initialBalance, interestRate);
}

// Function to create a Checking Account
shared_ptr<BankAccount> createCheckingAccount() {
    string name;
    double initialBalance, overdraftLimit;

    cout << "Enter account holder's name: ";
    cin >> name;
    cout << "Enter initial balance: ";
    cin >> initialBalance;
    cout << "Enter overdraft limit: ";
    cin >> overdraftLimit;

    return make_shared<CheckingAccount>(name, initialBalance, overdraftLimit);
}

// Function to display menu and perform actions
void accountMenu(shared_ptr<BankAccount> account) {
    int choice;
    do {
        cout << "\n1. Deposit\n2. Withdraw\n3. Display Account Info\n4. Add Interest (only for Savings)\n5. Exit\n";
        cout << "Choose an option: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                double amount;
                cout << "Enter deposit amount: ";
                cin >> amount;
                account->deposit(amount);
                break;
            }
            case 2: {
                double amount;
                cout << "Enter withdrawal amount: ";
                cin >> amount;
                account->withdraw(amount);
                break;
            }
            case 3:
                account->displayAccountInfo();
                break;
            case 4: {
                // Check if account is of type SavingsAccount
                if (auto savings = dynamic_pointer_cast<SavingsAccount>(account)) {
                    savings->addInterest();
                } else {
                    cout << "Interest can only be added to Savings Accounts." << endl;
                }
                break;
            }
            case 5:
                cout << "Exiting account menu.\n";
                break;
            default:
                cout << "Invalid option. Try again." << endl;
        }
    } while (choice != 5);
}

int main() {
    vector<shared_ptr<BankAccount>> accounts;
    int choice;

    do {
        cout << "\n--- Bank Account Management System ---\n";
        cout << "1. Create Savings Account\n2. Create Checking Account\n3. Manage Existing Account\n4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                auto account = createSavingsAccount();
                accounts.push_back(account);
                cout << "Savings account created successfully!" << endl;
                break;
            }
            case 2: {
                auto account = createCheckingAccount();
                accounts.push_back(account);
                cout << "Checking account created successfully!" << endl;
                break;
            }
            case 3: {
                if (accounts.empty()) {
                    cout << "No accounts available. Please create an account first." << endl;
                    break;
                }
                
                int accountIndex;
                cout << "Available accounts:\n";
                for (size_t i = 0; i < accounts.size(); ++i) {
                    cout << i + 1 << ". ";
                    accounts[i]->displayAccountInfo();
                    cout << endl;
                }
                
                cout << "Enter account number to manage: ";
                cin >> accountIndex;
                
                if (accountIndex < 1 || accountIndex > accounts.size()) {
                    cout << "Invalid account number." << endl;
                } else {
                    accountMenu(accounts[accountIndex - 1]);
                }
                break;
            }
            case 4:
                cout << "Exiting program.\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 4);

    return 0;
}
