#include "Transaction.hpp"

Transaction::Transaction()
    : transactionID(""), type(""), peerPhone("N/A"),
      amount(0.0), balanceAfter(0.0), timestamp("") {}

Transaction::Transaction(const std::string& id,
                         const std::string& txType,
                         const std::string& peer,
                         double amt,
                         double bal,
                         const std::string& time)
    : transactionID(id), type(txType), peerPhone(peer),
      amount(amt), balanceAfter(bal), timestamp(time) {}
