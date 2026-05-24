#ifndef BANKING_SYSTEM_HPP
#define BANKING_SYSTEM_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include "Account.hpp"

class BankingSystem {
private:
    std::unordered_map<std::string, Account> accounts;

public:
    bool accountExists(const std::string& phone) const;
    bool addAccount(const Account& account);
    Account* findAccount(const std::string& phone);
    const Account* findAccount(const std::string& phone) const;

    bool validateSignup(const std::string& fname,
                        const std::string& lname,
                        const std::string& phone,
                        const std::string& password) const;

    Account* signup(const std::string& fname,
                    const std::string& lname,
                    const std::string& phone,
                    const std::string& password,
                    double initialBalance = 0.0);

    Account* login(const std::string& phone, const std::string& password);
    bool updateProfile(const std::string& phone,
                       const std::string& newFname,
                       const std::string& newLname);

    std::vector<Account*> getAllAccounts();

    void loadFromFile();
    void saveToFile() const;
};

#endif // BANKING_SYSTEM_HPP
