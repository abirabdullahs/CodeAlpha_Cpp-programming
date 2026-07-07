#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>
#include <algorithm>

using namespace std;


class Transaction {
public:
    string type;      
    double amount;
    string timestamp;

    Transaction(string t, double a) {
        type = t;
        amount = a;
        timestamp = getCurrentTime();
    }

    static string getCurrentTime() {
        time_t now = time(0);
        char buf[80];
        struct tm* ltm = localtime(&now);
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", ltm);
        return string(buf);
    }

    void display() const {
        cout << left << setw(15) << type
             << setw(15) << fixed << setprecision(2) << amount
             << timestamp << endl;
    }
};


class Account {
public:
    int accountNumber;
    double balance;
    vector<Transaction> history;

    Account(int accNo, double initialBalance = 0.0) {
        accountNumber = accNo;
        balance = initialBalance;
    }

    void deposit(double amount) {
        if (amount <= 0) {
            cout << "Invalid deposit amount!\n";
            return;
        }
        balance += amount;
        history.push_back(Transaction("Deposit", amount));
        cout << "Deposit successful. New Balance: " << fixed << setprecision(2) << balance << endl;
    }

    bool withdraw(double amount) {
        if (amount <= 0) {
            cout << "Invalid withdrawal amount!\n";
            return false;
        }
        if (amount > balance) {
            cout << "Insufficient balance!\n";
            return false;
        }
        balance -= amount;
        history.push_back(Transaction("Withdraw", amount));
        cout << "Withdrawal successful. New Balance: " << fixed << setprecision(2) << balance << endl;
        return true;
    }

    void receiveTransfer(double amount) {
        balance += amount;
        history.push_back(Transaction("Transfer-In", amount));
    }

    bool sendTransfer(double amount) {
        if (amount > balance) {
            cout << "Insufficient balance for transfer!\n";
            return false;
        }
        balance -= amount;
        history.push_back(Transaction("Transfer-Out", amount));
        return true;
    }

    void showTransactionHistory() const {
        if (history.empty()) {
            cout << "No transactions yet.\n";
            return;
        }
        cout << "\n--- Transaction History (Account: " << accountNumber << ") ---\n";
        cout << left << setw(15) << "Type" << setw(15) << "Amount" << "Timestamp" << endl;
        cout << "--------------------------------------------------\n";
        for (const auto& t : history) {
            t.display();
        }
    }

    void showAccountInfo() const {
        cout << "\nAccount Number : " << accountNumber << endl;
        cout << "Balance        : " << fixed << setprecision(2) << balance << endl;
    }
};


class Customer {
public:
    int customerId;
    string name;
    string phone;
    vector<Account> accounts;

    Customer(int id, string n, string ph) {
        customerId = id;
        name = n;
        phone = ph;
    }

    Account* findAccount(int accNo) {
        for (auto& acc : accounts) {
            if (acc.accountNumber == accNo)
                return &acc;
        }
        return nullptr;
    }

    void addAccount(const Account& acc) {
        accounts.push_back(acc);
    }

    void showCustomerInfo() const {
        cout << "\nCustomer ID : " << customerId << endl;
        cout << "Name        : " << name << endl;
        cout << "Phone       : " << phone << endl;
        cout << "Accounts    : ";
        for (const auto& acc : accounts) cout << acc.accountNumber << " ";
        cout << endl;
    }
};


class Bank {
private:
    vector<Customer> customers;
    int nextCustomerId = 1001;
    int nextAccountNumber = 5001;

public:
    Customer* findCustomer(int customerId) {
        for (auto& c : customers) {
            if (c.customerId == customerId)
                return &c;
        }
        return nullptr;
    }

    Account* findAccountGlobally(int accNo, Customer** ownerOut = nullptr) {
        for (auto& c : customers) {
            Account* acc = c.findAccount(accNo);
            if (acc) {
                if (ownerOut) *ownerOut = &c;
                return acc;
            }
        }
        return nullptr;
    }

    int createCustomer(const string& name, const string& phone) {
        Customer c(nextCustomerId, name, phone);
        customers.push_back(c);
        cout << "Customer created successfully! Customer ID: " << nextCustomerId << endl;
        return nextCustomerId++;
    }

    int createAccount(int customerId, double initialDeposit) {
        Customer* c = findCustomer(customerId);
        if (!c) {
            cout << "Customer not found!\n";
            return -1;
        }
        Account acc(nextAccountNumber, initialDeposit);
        c->addAccount(acc);
        cout << "Account created successfully! Account Number: " << nextAccountNumber << endl;
        return nextAccountNumber++;
    }

    void deposit(int accNo, double amount) {
        Account* acc = findAccountGlobally(accNo);
        if (!acc) {
            cout << "Account not found!\n";
            return;
        }
        acc->deposit(amount);
    }

    void withdraw(int accNo, double amount) {
        Account* acc = findAccountGlobally(accNo);
        if (!acc) {
            cout << "Account not found!\n";
            return;
        }
        acc->withdraw(amount);
    }

    void transfer(int fromAccNo, int toAccNo, double amount) {
        Account* fromAcc = findAccountGlobally(fromAccNo);
        Account* toAcc = findAccountGlobally(toAccNo);

        if (!fromAcc || !toAcc) {
            cout << "One or both accounts not found!\n";
            return;
        }
        if (fromAcc->sendTransfer(amount)) {
            toAcc->receiveTransfer(amount);
            cout << "Transfer of " << fixed << setprecision(2) << amount
                 << " from " << fromAccNo << " to " << toAccNo << " successful!\n";
        }
    }

    void showAccountInfo(int accNo) {
        Account* acc = findAccountGlobally(accNo);
        if (!acc) {
            cout << "Account not found!\n";
            return;
        }
        acc->showAccountInfo();
    }

    void showTransactionHistory(int accNo) {
        Account* acc = findAccountGlobally(accNo);
        if (!acc) {
            cout << "Account not found!\n";
            return;
        }
        acc->showTransactionHistory();
    }

    void showCustomerInfo(int customerId) {
        Customer* c = findCustomer(customerId);
        if (!c) {
            cout << "Customer not found!\n";
            return;
        }
        c->showCustomerInfo();
    }

    bool hasCustomers() const {
        return !customers.empty();
    }
};


int getIntInput(const string& prompt) {
    int value;
    cout << prompt;
    while (!(cin >> value)) {
        cout << "Invalid input. Enter a number: ";
        cin.clear();
        cin.ignore(10000, '\n');
    }
    return value;
}

double getDoubleInput(const string& prompt) {
    double value;
    cout << prompt;
    while (!(cin >> value)) {
        cout << "Invalid input. Enter a valid amount: ";
        cin.clear();
        cin.ignore(10000, '\n');
    }
    return value;
}

string getStringInput(const string& prompt) {
    string value;
    cout << prompt;
    cin.ignore();
    getline(cin, value);
    return value;
}


void showMenu() {
    cout << "\n========== BANKING SYSTEM MENU ==========\n";
    cout << "1. Create New Customer\n";
    cout << "2. Create New Account\n";
    cout << "3. Deposit\n";
    cout << "4. Withdraw\n";
    cout << "5. Transfer Funds\n";
    cout << "6. View Account Info\n";
    cout << "7. View Transaction History\n";
    cout << "8. View Customer Info\n";
    cout << "0. Exit\n";
    cout << "==========================================\n";
}

int main() {
    Bank bank;
    int choice;

    cout << "=============================================\n";
    cout << "     WELCOME TO CODEALPHA BANKING SYSTEM      \n";
    cout << "=============================================\n";

    do {
        showMenu();
        choice = getIntInput("Enter your choice: ");

        switch (choice) {
            case 1: {
                string name = getStringInput("Enter customer name: ");
                string phone = getStringInput("Enter phone number: ");
                bank.createCustomer(name, phone);
                break;
            }
            case 2: {
                int custId = getIntInput("Enter Customer ID: ");
                double initial = getDoubleInput("Enter initial deposit amount: ");
                bank.createAccount(custId, initial);
                break;
            }
            case 3: {
                int accNo = getIntInput("Enter Account Number: ");
                double amount = getDoubleInput("Enter deposit amount: ");
                bank.deposit(accNo, amount);
                break;
            }
            case 4: {
                int accNo = getIntInput("Enter Account Number: ");
                double amount = getDoubleInput("Enter withdrawal amount: ");
                bank.withdraw(accNo, amount);
                break;
            }
            case 5: {
                int fromAcc = getIntInput("Enter Sender Account Number: ");
                int toAcc = getIntInput("Enter Receiver Account Number: ");
                double amount = getDoubleInput("Enter transfer amount: ");
                bank.transfer(fromAcc, toAcc, amount);
                break;
            }
            case 6: {
                int accNo = getIntInput("Enter Account Number: ");
                bank.showAccountInfo(accNo);
                break;
            }
            case 7: {
                int accNo = getIntInput("Enter Account Number: ");
                bank.showTransactionHistory(accNo);
                break;
            }
            case 8: {
                int custId = getIntInput("Enter Customer ID: ");
                bank.showCustomerInfo(custId);
                break;
            }
            case 0:
                cout << "Thank you for using CodeAlpha Banking System. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice! Please try again.\n";
        }

    } while (choice != 0);

    return 0;
}
