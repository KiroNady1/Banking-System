#include "TransactionManager.hpp"

std::string TransactionManager::verifyRecipient(const std::string& receiverPhone, BankingSystem& system) {
    Account* receiver = system.findAccount(receiverPhone);
    if (receiver == nullptr) {
        return "NOT_FOUND";
    }
    return receiver->getFullName();
}

bool TransactionManager::validateBalance(const std::string& senderPhone, double amount, BankingSystem& system) {
    Account* sender = system.findAccount(senderPhone);
    if (sender == nullptr || amount <= 0) {
        return false;
    }
    return sender->getBalance() >= amount;
}

int TransactionManager::executeTransfer(const std::string& senderPhone,
                                        const std::string& receiverPhone,
                                        double amount,
                                        BankingSystem& system,
                                        const std::string& timestamp) {
    if (amount <= 0) {
        return 5;
    }

    if (senderPhone == receiverPhone) {
        return 4;
    }

    Account* sender = system.findAccount(senderPhone);
    Account* receiver = system.findAccount(receiverPhone);

    if (sender == nullptr) {
        return 1;
    }

    if (receiver == nullptr) {
        return 2;
    }

    if (sender->getBalance() < amount) {
        return 3;
    }

    bool debited = sender->debitForTransfer(amount);
    if (!debited) {
        return 3;
    }

    receiver->creditForTransfer(amount);

    std::string txID = "TXN_" + senderPhone + "_TO_" + receiverPhone + "_" + timestamp;

    Transaction senderTx(txID, "Sent", receiverPhone, amount, sender->getBalance(), timestamp);
    Transaction receiverTx(txID, "Received", senderPhone, amount, receiver->getBalance(), timestamp);

    sender->addTransaction(senderTx);
    receiver->addTransaction(receiverTx);

    return 0;
}

std::string TransactionManager::getTransferMessage(int code) const {
    switch (code) {
        case 0: return "Transfer completed successfully.";
        case 1: return "Sender account was not found.";
        case 2: return "Receiver account was not found.";
        case 3: return "Insufficient balance.";
        case 4: return "You cannot transfer money to the same account.";
        case 5: return "Invalid amount.";
        default: return "Unknown transfer error.";
    }
}
