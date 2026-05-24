#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP

#include <string>

struct Transaction {
    std::string transactionID;
    std::string type;
    std::string peerPhone;
    double amount;
    double balanceAfter;
    std::string timestamp;

    Transaction();
    Transaction(const std::string& id,
                const std::string& txType,
                const std::string& peer,
                double amt,
                double bal,
                const std::string& time);
};

#endif // TRANSACTION_HPP
