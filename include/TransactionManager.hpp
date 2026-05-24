#ifndef TRANSACTION_MANAGER_HPP
#define TRANSACTION_MANAGER_HPP

#include <string>
#include "BankingSystem.hpp"

class TransactionManager {
public:
    std::string verifyRecipient(const std::string& receiverPhone, BankingSystem& system);
    bool validateBalance(const std::string& senderPhone, double amount, BankingSystem& system);

    // Return codes:
    // 0 = Success
    // 1 = Sender not found
    // 2 = Receiver not found
    // 3 = Insufficient balance
    // 4 = Same account
    // 5 = Invalid amount
    int executeTransfer(const std::string& senderPhone,
                        const std::string& receiverPhone,
                        double amount,
                        BankingSystem& system,
                        const std::string& timestamp);

    std::string getTransferMessage(int code) const;
};

#endif // TRANSACTION_MANAGER_HPP
