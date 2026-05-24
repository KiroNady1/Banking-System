QT += widgets

CONFIG += c++17
CONFIG += warn_on

TEMPLATE = app
TARGET = DSA_Banking_Qt

INCLUDEPATH += include

SOURCES += \
    src/main.cpp \
    src/Transaction.cpp \
    src/HistoryManager.cpp \
    src/Account.cpp \
    src/BankingSystem.cpp \
    src/TransactionManager.cpp \
    src/MainWindow.cpp

HEADERS += \
    include/Transaction.hpp \
    include/HistoryManager.hpp \
    include/Account.hpp \
    include/BankingSystem.hpp \
    include/TransactionManager.hpp \
    include/MainWindow.hpp
