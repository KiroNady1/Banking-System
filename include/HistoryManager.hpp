#ifndef HISTORY_MANAGER_HPP
#define HISTORY_MANAGER_HPP

#include <stack>
#include <vector>
#include <string>
#include "Transaction.hpp"

class HistoryManager {
private:
    std::stack<Transaction> historyStack;

public:
    void recordTransaction(const Transaction& tx);
    bool undoLastTransaction(Transaction& undoneTx);
    int getCount() const;

    std::vector<Transaction> getHistoryNewestFirst() const;
    std::vector<Transaction> getFilteredByType(const std::string& filterType) const;
};

#endif // HISTORY_MANAGER_HPP
