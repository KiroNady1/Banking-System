#include "Account.hpp"
#include <ctime>

Account::Account()
    : phone(""), fname(""), lname(""), balance(0.0), password("") {}

Account::Account(const std::string& ph,
                 const std::string& first,
                 const std::string& last,
                 double bal,
                 const std::string& pass)
    : phone(ph), fname(first), lname(last), balance(bal), password(pass) {}

std::string Account::getCurrentTime() const {
    std::time_t now = std::time(nullptr);
    char buffer[64];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    return std::string(buffer);
}

std::string Account::getPhone() const {
    return phone;
}

std::string Account::getFname() const {
    return fname;
}

std::string Account::getLname() const {
    return lname;
}

std::string Account::getFullName() const {
    if (lname.empty()) {
        return fname;
    }
    return fname + " " + lname;
}

std::string Account::getPassword() const {
    return password;
}

double Account::getBalance() const {
    return balance;
}

int Account::getHistoryCount() const {
    return history.getCount();
}

bool Account::verifyPassword(const std::string& pass) const {
    return password == pass;
}

void Account::setFname(const std::string& f) {
    fname = f;
}

void Account::setLname(const std::string& l) {
    lname = l;
}

void Account::setPassword(const std::string& p) {
    password = p;
}

bool Account::deposit(double amount) {
    if (amount <= 0) {
        return false;
    }

    balance += amount;
    history.recordTransaction(Transaction("", "Deposit", "N/A", amount, balance, getCurrentTime()));
    return true;
}

bool Account::withdraw(double amount) {
    if (amount <= 0 || amount > balance) {
        return false;
    }

    balance -= amount;
    history.recordTransaction(Transaction("", "Withdrawal", "N/A", amount, balance, getCurrentTime()));
    return true;
}

bool Account::debitForTransfer(double amount) {
    if (amount <= 0 || amount > balance) {
        return false;
    }
    balance -= amount;
    return true;
}

void Account::creditForTransfer(double amount) {
    if (amount > 0) {
        balance += amount;
    }
}

void Account::addTransaction(const Transaction& tx) {
    history.recordTransaction(tx);
}

bool Account::undoLast() {
    Transaction undoneTx;
    if (!history.undoLastTransaction(undoneTx)) {
        return false;
    }

    if (undoneTx.type == "Deposit") {
        balance -= undoneTx.amount;
        return true;
    }

    if (undoneTx.type == "Withdrawal") {
        balance += undoneTx.amount;
        return true;
    }

    // For transfer records, undo is not automatic because it involves two accounts.
    return false;
}

std::vector<Transaction> Account::getHistoryNewestFirst() const {
    return history.getHistoryNewestFirst();
}
