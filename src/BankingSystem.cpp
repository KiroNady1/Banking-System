#include "BankingSystem.hpp"
#include <cctype>
#include <fstream>
#include <sstream>

bool BankingSystem::accountExists(const std::string& phone) const {
    return accounts.find(phone) != accounts.end();
}

bool BankingSystem::addAccount(const Account& account) {
    if (account.getPhone().empty() || accountExists(account.getPhone())) {
        return false;
    }

    accounts.emplace(account.getPhone(), account);
    return true;
}

Account* BankingSystem::findAccount(const std::string& phone) {
    auto it = accounts.find(phone);
    if (it == accounts.end()) {
        return nullptr;
    }
    return &(it->second);
}

const Account* BankingSystem::findAccount(const std::string& phone) const {
    auto it = accounts.find(phone);
    if (it == accounts.end()) {
        return nullptr;
    }
    return &(it->second);
}

bool BankingSystem::validateSignup(const std::string& fname,
                                   const std::string& lname,
                                   const std::string& phone,
                                   const std::string& password) const {
    if (fname.empty() || lname.empty() || phone.empty() || password.empty()) {
        return false;
    }

    if (phone.length() != 11) {
        return false;
    }

    for (char c : phone) {
        if (!std::isdigit(static_cast<unsigned char>(c))) {
            return false;
        }
    }

    if (password.length() < 6) {
        return false;
    }

    if (accountExists(phone)) {
        return false;
    }

    return true;
}

Account* BankingSystem::signup(const std::string& fname,
                               const std::string& lname,
                               const std::string& phone,
                               const std::string& password,
                               double initialBalance) {
    if (!validateSignup(fname, lname, phone, password)) {
        return nullptr;
    }

    if (initialBalance < 0) {
        initialBalance = 0.0;
    }

    Account newAccount(phone, fname, lname, initialBalance, password);
    bool inserted = addAccount(newAccount);
    if (!inserted) {
        return nullptr;
    }

    return findAccount(phone);
}

Account* BankingSystem::login(const std::string& phone, const std::string& password) {
    Account* account = findAccount(phone);
    if (account != nullptr && account->verifyPassword(password)) {
        return account;
    }
    return nullptr;
}

bool BankingSystem::updateProfile(const std::string& phone,
                                  const std::string& newFname,
                                  const std::string& newLname) {
    Account* account = findAccount(phone);
    if (account == nullptr) {
        return false;
    }

    if (!newFname.empty()) {
        account->setFname(newFname);
    }

    if (!newLname.empty()) {
        account->setLname(newLname);
    }

    return true;
}

std::vector<Account*> BankingSystem::getAllAccounts() {
    std::vector<Account*> result;
    for (auto& pair : accounts) {
        result.push_back(&(pair.second));
    }
    return result;
}

void BankingSystem::loadFromFile() {
    std::ifstream accFile("accounts.csv");
    if (accFile.is_open()) {
        std::string line;
        while (std::getline(accFile, line)) {
            if (line.empty()) continue;
            std::stringstream ss(line);
            std::string phone, fname, lname, balanceStr, password;
            std::getline(ss, phone, ',');
            std::getline(ss, fname, ',');
            std::getline(ss, lname, ',');
            std::getline(ss, balanceStr, ',');
            std::getline(ss, password, ',');
            
            if (!phone.empty()) {
                double balance = 0.0;
                try { balance = std::stod(balanceStr); } catch(...) {}
                Account acc(phone, fname, lname, balance, password);
                addAccount(acc);
            }
        }
        accFile.close();
    }

    std::ifstream txFile("transactions.csv");
    if (txFile.is_open()) {
        std::string line;
        while (std::getline(txFile, line)) {
            if (line.empty()) continue;
            std::stringstream ss(line);
            std::string accPhone, txID, type, peerPhone, amtStr, balStr, timestamp;
            std::getline(ss, accPhone, ',');
            std::getline(ss, txID, ',');
            std::getline(ss, type, ',');
            std::getline(ss, peerPhone, ',');
            std::getline(ss, amtStr, ',');
            std::getline(ss, balStr, ',');
            std::getline(ss, timestamp); // Timestamp might be the rest of the string
            
            Account* acc = findAccount(accPhone);
            if (acc != nullptr) {
                double amt = 0.0, bal = 0.0;
                try { amt = std::stod(amtStr); } catch(...) {}
                try { bal = std::stod(balStr); } catch(...) {}
                Transaction tx(txID, type, peerPhone, amt, bal, timestamp);
                acc->addTransaction(tx);
            }
        }
        txFile.close();
    }
}

void BankingSystem::saveToFile() const {
    std::ofstream accFile("accounts.csv");
    if (accFile.is_open()) {
        for (const auto& pair : accounts) {
            const Account& acc = pair.second;
            accFile << acc.getPhone() << ","
                    << acc.getFname() << ","
                    << acc.getLname() << ","
                    << acc.getBalance() << ","
                    << acc.getPassword() << "\n";
        }
        accFile.close();
    }

    std::ofstream txFile("transactions.csv");
    if (txFile.is_open()) {
        for (const auto& pair : accounts) {
            const Account& acc = pair.second;
            std::string accPhone = acc.getPhone();
            std::vector<Transaction> history = acc.getHistoryNewestFirst();
            for (auto it = history.rbegin(); it != history.rend(); ++it) {
                const Transaction& tx = *it;
                txFile << accPhone << ","
                       << tx.transactionID << ","
                       << tx.type << ","
                       << tx.peerPhone << ","
                       << tx.amount << ","
                       << tx.balanceAfter << ","
                       << tx.timestamp << "\n";
            }
        }
        txFile.close();
    }
}

