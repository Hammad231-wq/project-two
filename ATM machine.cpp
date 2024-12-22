#include <iostream>  // For input and output operations
#include <string>    // For using strings
#include <map>       // For storing currency rates
#include <vector>    // For storing transaction history
#include <ctime>     // For generating timestamps

using namespace std;

// Utility Function for Timestamp
string getCurrentTimestamp() {
    time_t now = time(0);              // Get the current time
    char* dt = ctime(&now);            // Convert time to string format
    return string(dt);                 // Return as a C++ string
}

// Class: Transaction
class Transaction {
private:
    int transactionID;                // Unique ID for the transaction
    string type;                      // Type of transaction (e.g., Deposit, Withdrawal)
    float amount;                     // Transaction amount
    string currency;                  // Currency used in the transaction
    string timestamp;                 // Timestamp of the transaction

public:
    // Constructor to initialize transaction attributes
    Transaction(int id, string t, float a, string c)
        : transactionID(id), type(t), amount(a), currency(c), timestamp(getCurrentTimestamp()) {}

    // Display transaction details
    void displayTransaction() const {
        cout << "Transaction ID: " << transactionID
             << " | Type: " << type
             << " | Amount: " << amount
             << " " << currency
             << " | Timestamp: " << timestamp;
        cout << endl;
    }
};

// Class: CurrencyConverter
class CurrencyConverter {
private:
    map<string, float> rates;         // Map to store currency exchange rates

public:
    // Constructor to initialize exchange rates
    CurrencyConverter() {
        rates["USD"] = 1.0;           // Base currency
        rates["EUR"] = 0.93;          // Conversion rate for EUR
        rates["GBP"] = 0.82;          // Conversion rate for GBP
    }

    // Convert an amount from one currency to another
    float convert(float amount, string from, string to) {
        if (rates.find(from) == rates.end() || rates.find(to) == rates.end()) {
            cout << "Invalid currency." << endl;  // Display error for invalid currency
            return 0;
        }
        return (amount / rates[from]) * rates[to];  // Perform currency conversion
    }

    // Display available exchange rates
    void displayRates() {
        for (auto& rate : rates) {
            cout << rate.first << ": " << rate.second << endl;
        }
    }
};

// Class: Account
class Account {
private:
    int pin;                           // PIN for account authentication
    float balance;                     // Account balance
    vector<Transaction> transactionHistory;  // History of all transactions

public:
    // Constructor to initialize account with PIN and balance
    Account(int p, float b) : pin(p), balance(b) {}

    // Validate entered PIN
    bool validatePIN(int enteredPIN) {
        return enteredPIN == pin;
    }

    // Update the account balance
    void updateBalance(float amount) {
        balance += amount;
    }

    // Get the current account balance
    float getBalance() {
        return balance;
    }

    // Add a transaction to the history
    void addTransaction(int id, string type, float amount, string currency) {
        transactionHistory.emplace_back(id, type, amount, currency);
    }

    // Display all transactions
    void viewTransactions() {
        cout << "Transaction History:" << endl;
        for (const auto& txn : transactionHistory) {
            txn.displayTransaction();
        }
    }
};

// Class: ATM
class ATM {
private:
    CurrencyConverter converter;       // CurrencyConverter instance

public:
    // Handle withdrawal operation
    void withdraw(Account& account, float amount) {
        if (account.getBalance() < amount) {  // Check if funds are sufficient
            cout << "Insufficient funds." << endl;
        } else {
            account.updateBalance(-amount);  // Deduct amount from balance
            account.addTransaction(account.getBalance(), "Withdrawal", amount, "USD");
            cout << "Withdrawal successful." << endl;
        }
    }

    // Handle deposit operation
    void deposit(Account& account, float amount) {
        account.updateBalance(amount);  // Add amount to balance
        account.addTransaction(account.getBalance(), "Deposit", amount, "USD");
        cout << "Deposit successful." << endl;
    }

    // Display the ATM menu
    void showMenu() {
        cout << "1. Withdraw\n2. Deposit\n3. Check Balance\n4. View Transactions\n5. Exchange Rates\n6. Exit" << endl;
    }

    // Handle user interactions with the ATM menu
    void handleMenu(Account& account) {
        int choice;
        do {
            showMenu();                 // Show the menu
            cout << "Choose an option: ";
            cin >> choice;              // Get user input
            switch (choice) {
                case 1: {
                    float amount;
                    cout << "Enter amount: ";
                    cin >> amount;
                    withdraw(account, amount);  // Perform withdrawal
                    break;
                }
                case 2: {
                    float amount;
                    cout << "Enter amount: ";
                    cin >> amount;
                    deposit(account, amount);  // Perform deposit
                    break;
                }
                case 3:
                    cout << "Balance: " << account.getBalance() << endl;  // Show balance
                    break;
                case 4:
                    account.viewTransactions();  // View transaction history
                    break;
                case 5:
                    converter.displayRates();  // Show exchange rates
                    break;
                case 6:
                    cout << "Goodbye!" << endl;  // Exit the ATM
                    break;
                default:
                    cout << "Invalid choice. Try again." << endl;  // Handle invalid input
            }
        } while (choice != 6);          // Loop until the user chooses to exit
    }
};

int main() {
    Account user(1234, 1000.0);         // Create an account with a PIN and initial balance
    ATM atm;                            // Create an ATM instance

    int pin;
    cout << "Enter PIN: ";
    cin >> pin;                         // Prompt user to enter PIN

    if (user.validatePIN(pin)) {        // Check if PIN is correct
        atm.handleMenu(user);           // Allow access to ATM menu
    } else {
        cout << "Invalid PIN." << endl; // Deny access if PIN is incorrect
    }

    return 0;                           // End of program
}
