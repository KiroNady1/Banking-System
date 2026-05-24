#include "HistoryManager.hpp"

void HistoryManager::recordTransaction(const Transaction& tx) {
    historyStack.push(tx);
}

bool HistoryManager::undoLastTransaction(Transaction& undoneTx) {
    if (historyStack.empty()) {
        return false;
    }
    undoneTx = historyStack.top();
    historyStack.pop();
    return true;
}

int HistoryManager::getCount() const {
    return static_cast<int>(historyStack.size());
}

std::vector<Transaction> HistoryManager::getHistoryNewestFirst() const {
    std::vector<Transaction> result;
    std::stack<Transaction> temp = historyStack;

    while (!temp.empty()) {
        result.push_back(temp.top());
        temp.pop();
    }

    return result;
}

std::vector<Transaction> HistoryManager::getFilteredByType(const std::string& filterType) const {
    std::vector<Transaction> result;
    std::stack<Transaction> temp = historyStack;

    while (!temp.empty()) {
        if (temp.top().type == filterType) {
            result.push_back(temp.top());
        }
        temp.pop();
    }

    return result;
}
