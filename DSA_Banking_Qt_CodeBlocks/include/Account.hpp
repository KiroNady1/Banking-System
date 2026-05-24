#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP

#include <string>
#include <vector>
#include "HistoryManager.hpp"

class Account {
private:
    std::string phone;
    std::string fname;
    std::string lname;
    double balance;
    std::string password;
    HistoryManager history;

    std::string getCurrentTime() const;

public:
    Account();
    Account(const std::string& ph,
            const std::string& first,
            const std::string& last,
            double bal,
            const std::string& pass);

    std::string getPhone() const;
    std::string getFname() const;
    std::string getLname() const;
    std::string getFullName() const;
    std::string getPassword() const;
    double getBalance() const;
    int getHistoryCount() const;
    bool verifyPassword(const std::string& pass) const;

    void setFname(const std::string& f);
    void setLname(const std::string& l);
    void setPassword(const std::string& p);

    bool deposit(double amount);
    bool withdraw(double amount);

    // Used by transfer logic so a transfer does not appear as a normal deposit/withdrawal.
    bool debitForTransfer(double amount);
    void creditForTransfer(double amount);

    void addTransaction(const Transaction& tx);
    bool undoLast();

    std::vector<Transaction> getHistoryNewestFirst() const;
};

#endif // ACCOUNT_HPP
