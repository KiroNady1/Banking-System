#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <QWidget>
#include <QString>
#include "BankingSystem.hpp"
#include "TransactionManager.hpp"

class QStackedWidget;
class QLineEdit;
class QLabel;
class QDoubleSpinBox;
class QTableWidget;
class QPushButton;
class QListWidget;

class MainWindow : public QWidget {
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    BankingSystem system;
    TransactionManager transactionManager;
    Account* currentUser;

    QStackedWidget* pages;

    QWidget* loginPage;
    QWidget* signupPage;
    QWidget* dashboardPage;
    QWidget* transferPage;
    QWidget* historyPage;
    QWidget* profilePage;

    QLineEdit* loginPhoneEdit;
    QLineEdit* loginPasswordEdit;

    QLineEdit* signupFnameEdit;
    QLineEdit* signupLnameEdit;
    QLineEdit* signupPhoneEdit;
    QLineEdit* signupPasswordEdit;
    QDoubleSpinBox* signupInitialBalanceSpin;

    QLabel* dashboardNameLabel;
    QLabel* dashboardPhoneLabel;
    QLabel* dashboardBalanceLabel;
    QLabel* dashboardHistoryCountLabel;

    QLineEdit* receiverPhoneEdit;
    QLabel* recipientResultLabel;
    QDoubleSpinBox* transferAmountSpin;

    QTableWidget* historyTable;

    QLineEdit* profileFnameEdit;
    QLineEdit* profileLnameEdit;

    void buildUi();
    QWidget* createLoginPage();
    QWidget* createSignupPage();
    QWidget* createDashboardPage();
    QWidget* createTransferPage();
    QWidget* createHistoryPage();
    QWidget* createProfilePage();

    void seedDemoData();
    void applyStyle();

    void handleLogin();
    void handleSignup();
    void handleLogout();
    void handleDeposit();
    void handleWithdraw();
    void handleVerifyRecipient();
    void handleTransfer();
    void handleUpdateProfile();

    void refreshDashboard();
    void refreshHistoryTable();
    void resetTransferPage();
    void showDashboard();

    QString moneyText(double value) const;
    std::string currentTimestamp() const;
};

#endif // MAIN_WINDOW_HPP
