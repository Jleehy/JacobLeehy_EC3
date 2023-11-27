#include <iostream>
#include <string>

class Account {
protected:
    std::string accountNumber;
    std::string accountHolder;
    std::string accountType;
    
public:
    double balance;
    Account(const std::string& number, const std::string& holder, double initialBalance)
        : accountNumber(number), accountHolder(holder), balance(initialBalance) {
            accountType = "Account";
        }

    void displayDetails() const {
        std::cout << "Account details for " << accountType << " (ID: " << accountNumber << "):" << "\n\tAccount Holder: " << accountHolder
                  << "\n\tBalance: $" << balance << "\n";
    }

    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            std::cout << "Deposited $" << amount << " successfully.\n";
        } else {
            std::cout << "Invalid deposit amount.\n";
        }
    }

    virtual void withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            std::cout << "Withdrawn $" << amount << " successfully.\n";
        } else {
            std::cout << "Insufficient balance or invalid withdrawal amount.\n";
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const Account& account);
};
///////////////////
class SavingsAccount : public Account {
private:
    double interestRate;

public:
    SavingsAccount(const std::string& number, const std::string& holder, double initialBalance, double rate)
        : Account(number, holder, initialBalance), interestRate(rate) {
            accountType = "Savings Account";
        }
    
    void displayDetails() const {
        Account::displayDetails();
        std::cout << "\tInterest Rate: " << interestRate * 100 << "%\n";
    }

    void withdraw(double amount) override {
        const double minimumBalance = 100.0;
        if (amount > 0 && (balance - amount) >= minimumBalance) {
            balance -= amount;
            std::cout << "Withdrawn $" << amount << " successfully.\n";
        } else {
            std::cout << "Cannot withdraw due to insufficient balance or minimum balance limit.\n";
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const SavingsAccount& savings);
};
//////////////////////////
class CurrentAccount : public Account {
private:
    double overdraftLimit;

public:
    CurrentAccount(const std::string& number, const std::string& holder, double initialBalance, double limit)
        : Account(number, holder, initialBalance), overdraftLimit(limit) {
            accountType = "Current Account";
        }
    
    void displayDetails() const {
        Account::displayDetails();
        std::cout << "\tOverdraft Limit: " << overdraftLimit << "\n";
    }

    void withdraw(double amount) override {
        if (amount > 0 && (balance - amount) >= -overdraftLimit) {
            balance -= amount;
            std::cout << "Withdrew $" << amount << " successfully.\n";
        } else {
            std::cout << "Cannot withdraw due to insufficient balance or overdraft limit.\n";
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const CurrentAccount& current);
};

//overloading
std::ostream& operator<<(std::ostream& os, const Account& account) {
    account.displayDetails();
    return os;
}

std::ostream& operator<<(std::ostream& os, const SavingsAccount& savings) {
    savings.displayDetails();
    return os;
}

std::ostream& operator<<(std::ostream& os, const CurrentAccount& current) {
    current.displayDetails();
    return os;
}

CurrentAccount operator+(CurrentAccount& current, SavingsAccount& savings) {
    double transferAmount = 300.0;
    if (transferAmount > 0 && transferAmount <= savings.balance) {
        current.balance += transferAmount;
        savings.balance -= transferAmount;
        std::cout << "Transferred $" << transferAmount << " from Savings to Current successfully.\n";
    } else {
        std::cout << "Invalid transfer amount or insufficient balance in Savings.\n";
    }
    return current;
}

int main() {
    SavingsAccount savings("S123", "John Doe", 1000, 0.02);
    CurrentAccount current("C456", "Jane Doe", 2000, 500);

    std::cout << "Initial Account Details:\n";
    savings.displayDetails();
    std::cout << "\n";
    current.displayDetails();
    std::cout << "\n";
    
    savings.deposit(500);
    current.withdraw(1000);

    std::cout << "\nUpdated Account Details:\n";
    std::cout << savings << "\n";
    std::cout << current << "\n";
    
    current.deposit(500);
    savings.withdraw(1000);

    std::cout << "\nUpdated Account Details:\n";
    std::cout << savings << "\n";
    std::cout << current << "\n";

    // Transfer 300 from savings to current
    current = current + savings;

    std::cout << "\nFinal Account Details:\n";
    std::cout << savings << "\n";
    std::cout << current << "\n";

    return 0;
}

