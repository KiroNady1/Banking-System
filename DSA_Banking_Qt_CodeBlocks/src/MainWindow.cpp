#include "MainWindow.hpp"

#include <QStackedWidget>
#include <QLineEdit>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QInputDialog>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QStringList>
#include <QDateTime>
#include <QFrame>

MainWindow::MainWindow(QWidget* parent)
    : QWidget(parent), currentUser(nullptr), pages(nullptr),
      loginPage(nullptr), signupPage(nullptr), dashboardPage(nullptr),
      transferPage(nullptr), historyPage(nullptr), profilePage(nullptr),
      loginPhoneEdit(nullptr), loginPasswordEdit(nullptr),
      signupFnameEdit(nullptr), signupLnameEdit(nullptr), signupPhoneEdit(nullptr),
      signupPasswordEdit(nullptr), signupInitialBalanceSpin(nullptr),
      dashboardNameLabel(nullptr), dashboardPhoneLabel(nullptr), dashboardBalanceLabel(nullptr),
      dashboardHistoryCountLabel(nullptr), receiverPhoneEdit(nullptr),
      recipientResultLabel(nullptr), transferAmountSpin(nullptr), historyTable(nullptr),
      profileFnameEdit(nullptr), profileLnameEdit(nullptr) {
    setWindowTitle("DSA Banking System - Qt Widgets");
    resize(950, 650);
    
    system.loadFromFile();
    if (system.getAllAccounts().empty()) {
        seedDemoData();
    }
    
    buildUi();
    applyStyle();
}

MainWindow::~MainWindow() {
    system.saveToFile();
}


void MainWindow::seedDemoData() {
    Account* mark = system.signup("Mark", "Sameh", "01234567890", "pass123", 6000.0);
    Account* alice = system.signup("Alice", "Smith", "01111111111", "pass123", 3500.0);
    Account* bob = system.signup("Bob", "Johnson", "01000000000", "pass123", 2500.0);
    Account* sara = system.signup("Sara", "Nabil", "01555555555", "pass123", 4200.0);

    if (mark) {
        mark->deposit(500.0);
    }
    if (alice) {
        alice->withdraw(200.0);
    }
    if (bob && sara) {
        transactionManager.executeTransfer("01000000000", "01555555555", 300.0, system, currentTimestamp());
    }
}

void MainWindow::buildUi() {
    pages = new QStackedWidget(this);

    loginPage = createLoginPage();
    signupPage = createSignupPage();
    dashboardPage = createDashboardPage();
    transferPage = createTransferPage();
    historyPage = createHistoryPage();
    profilePage = createProfilePage();

    pages->addWidget(loginPage);
    pages->addWidget(signupPage);
    pages->addWidget(dashboardPage);
    pages->addWidget(transferPage);
    pages->addWidget(historyPage);
    pages->addWidget(profilePage);

    QVBoxLayout* rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->addWidget(pages);

    pages->setCurrentWidget(loginPage);
}

QWidget* MainWindow::createLoginPage() {
    QWidget* page = new QWidget(this);
    QVBoxLayout* outer = new QVBoxLayout(page);
    outer->setAlignment(Qt::AlignCenter);

    QGroupBox* card = new QGroupBox("Welcome to InstaBank DSA", page);
    card->setObjectName("card");
    card->setMinimumWidth(430);
    QVBoxLayout* cardLayout = new QVBoxLayout(card);

    QLabel* subtitle = new QLabel("Login using your phone number and password.", card);
    subtitle->setObjectName("subtitle");

    loginPhoneEdit = new QLineEdit(card);
    loginPhoneEdit->setPlaceholderText("Example: 01234567890");

    loginPasswordEdit = new QLineEdit(card);
    loginPasswordEdit->setPlaceholderText("Example: pass123");
    loginPasswordEdit->setEchoMode(QLineEdit::Password);

    QFormLayout* form = new QFormLayout();
    form->addRow("Phone", loginPhoneEdit);
    form->addRow("Password", loginPasswordEdit);

    QPushButton* loginButton = new QPushButton("Login", card);
    QPushButton* signupButton = new QPushButton("Create New Account", card);
    QPushButton* demoButton = new QPushButton("Fill Demo Login", card);

    cardLayout->addWidget(subtitle);
    cardLayout->addSpacing(8);
    cardLayout->addLayout(form);
    cardLayout->addSpacing(10);
    cardLayout->addWidget(loginButton);
    cardLayout->addWidget(signupButton);
    cardLayout->addWidget(demoButton);

    QLabel* demoInfo = new QLabel("Demo account: 01234567890 / pass123", card);
    demoInfo->setObjectName("hint");
    cardLayout->addWidget(demoInfo);

    outer->addWidget(card);

    connect(loginButton, &QPushButton::clicked, this, [this]() { handleLogin(); });
    connect(signupButton, &QPushButton::clicked, this, [this]() { pages->setCurrentWidget(signupPage); });
    connect(demoButton, &QPushButton::clicked, this, [this]() {
        loginPhoneEdit->setText("01234567890");
        loginPasswordEdit->setText("pass123");
    });

    return page;
}

QWidget* MainWindow::createSignupPage() {
    QWidget* page = new QWidget(this);
    QVBoxLayout* outer = new QVBoxLayout(page);
    outer->setAlignment(Qt::AlignCenter);

    QGroupBox* card = new QGroupBox("Create Account", page);
    card->setObjectName("card");
    card->setMinimumWidth(460);
    QVBoxLayout* cardLayout = new QVBoxLayout(card);

    signupFnameEdit = new QLineEdit(card);
    signupFnameEdit->setPlaceholderText("First name");

    signupLnameEdit = new QLineEdit(card);
    signupLnameEdit->setPlaceholderText("Last name");

    signupPhoneEdit = new QLineEdit(card);
    signupPhoneEdit->setPlaceholderText("11 digits phone number");

    signupPasswordEdit = new QLineEdit(card);
    signupPasswordEdit->setPlaceholderText("At least 6 characters");
    signupPasswordEdit->setEchoMode(QLineEdit::Password);

    signupInitialBalanceSpin = new QDoubleSpinBox(card);
    signupInitialBalanceSpin->setRange(0.0, 100000000.0);
    signupInitialBalanceSpin->setDecimals(2);
    signupInitialBalanceSpin->setPrefix("EGP ");
    signupInitialBalanceSpin->setValue(1000.0);

    QFormLayout* form = new QFormLayout();
    form->addRow("First Name", signupFnameEdit);
    form->addRow("Last Name", signupLnameEdit);
    form->addRow("Phone", signupPhoneEdit);
    form->addRow("Password", signupPasswordEdit);
    form->addRow("Initial Balance", signupInitialBalanceSpin);

    QPushButton* createButton = new QPushButton("Sign Up", card);
    QPushButton* backButton = new QPushButton("Back to Login", card);

    cardLayout->addLayout(form);
    cardLayout->addSpacing(10);
    cardLayout->addWidget(createButton);
    cardLayout->addWidget(backButton);

    outer->addWidget(card);

    connect(createButton, &QPushButton::clicked, this, [this]() { handleSignup(); });
    connect(backButton, &QPushButton::clicked, this, [this]() { pages->setCurrentWidget(loginPage); });

    return page;
}

QWidget* MainWindow::createDashboardPage() {
    QWidget* page = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(30, 30, 30, 30);

    QLabel* title = new QLabel("Dashboard", page);
    title->setObjectName("title");

    QGroupBox* infoCard = new QGroupBox("Account Info", page);
    infoCard->setObjectName("card");
    QGridLayout* infoGrid = new QGridLayout(infoCard);

    dashboardNameLabel = new QLabel("Name: -", infoCard);
    dashboardPhoneLabel = new QLabel("Phone: -", infoCard);
    dashboardBalanceLabel = new QLabel("Balance: -", infoCard);
    dashboardHistoryCountLabel = new QLabel("Transactions: -", infoCard);

    dashboardBalanceLabel->setObjectName("balance");

    infoGrid->addWidget(dashboardNameLabel, 0, 0);
    infoGrid->addWidget(dashboardPhoneLabel, 1, 0);
    infoGrid->addWidget(dashboardBalanceLabel, 0, 1);
    infoGrid->addWidget(dashboardHistoryCountLabel, 1, 1);

    QGroupBox* actionsCard = new QGroupBox("Actions", page);
    actionsCard->setObjectName("card");
    QGridLayout* actionsGrid = new QGridLayout(actionsCard);

    QPushButton* transferButton = new QPushButton("Send Money", actionsCard);
    QPushButton* depositButton = new QPushButton("Deposit", actionsCard);
    QPushButton* withdrawButton = new QPushButton("Withdraw", actionsCard);
    QPushButton* historyButton = new QPushButton("View History", actionsCard);
    QPushButton* profileButton = new QPushButton("Edit Profile", actionsCard);
    QPushButton* logoutButton = new QPushButton("Logout", actionsCard);

    actionsGrid->addWidget(transferButton, 0, 0);
    actionsGrid->addWidget(depositButton, 0, 1);
    actionsGrid->addWidget(withdrawButton, 0, 2);
    actionsGrid->addWidget(historyButton, 1, 0);
    actionsGrid->addWidget(profileButton, 1, 1);
    actionsGrid->addWidget(logoutButton, 1, 2);

    layout->addWidget(title);
    layout->addWidget(infoCard);
    layout->addWidget(actionsCard);
    layout->addStretch();

    connect(transferButton, &QPushButton::clicked, this, [this]() {
        resetTransferPage();
        pages->setCurrentWidget(transferPage);
    });
    connect(depositButton, &QPushButton::clicked, this, [this]() { handleDeposit(); });
    connect(withdrawButton, &QPushButton::clicked, this, [this]() { handleWithdraw(); });
    connect(historyButton, &QPushButton::clicked, this, [this]() {
        refreshHistoryTable();
        pages->setCurrentWidget(historyPage);
    });
    connect(profileButton, &QPushButton::clicked, this, [this]() {
        if (currentUser) {
            profileFnameEdit->setText(QString::fromStdString(currentUser->getFname()));
            profileLnameEdit->setText(QString::fromStdString(currentUser->getLname()));
        }
        pages->setCurrentWidget(profilePage);
    });
    connect(logoutButton, &QPushButton::clicked, this, [this]() { handleLogout(); });

    return page;
}

QWidget* MainWindow::createTransferPage() {
    QWidget* page = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(30, 30, 30, 30);

    QLabel* title = new QLabel("Send Money", page);
    title->setObjectName("title");

    QGroupBox* card = new QGroupBox("Recipient Verification", page);
    card->setObjectName("card");
    QVBoxLayout* cardLayout = new QVBoxLayout(card);

    receiverPhoneEdit = new QLineEdit(card);
    receiverPhoneEdit->setPlaceholderText("Receiver phone number");

    QPushButton* verifyButton = new QPushButton("Verify Recipient", card);
    recipientResultLabel = new QLabel("Enter a receiver phone number, then click Verify.", card);
    recipientResultLabel->setObjectName("hint");

    transferAmountSpin = new QDoubleSpinBox(card);
    transferAmountSpin->setRange(0.01, 100000000.0);
    transferAmountSpin->setDecimals(2);
    transferAmountSpin->setPrefix("EGP ");
    transferAmountSpin->setValue(100.0);

    QPushButton* confirmButton = new QPushButton("Confirm Transfer", card);
    QPushButton* backButton = new QPushButton("Back", card);

    QFormLayout* form = new QFormLayout();
    form->addRow("Receiver Phone", receiverPhoneEdit);
    form->addRow("Amount", transferAmountSpin);

    cardLayout->addLayout(form);
    cardLayout->addWidget(verifyButton);
    cardLayout->addWidget(recipientResultLabel);
    cardLayout->addSpacing(10);
    cardLayout->addWidget(confirmButton);
    cardLayout->addWidget(backButton);

    layout->addWidget(title);
    layout->addWidget(card);
    layout->addStretch();

    connect(verifyButton, &QPushButton::clicked, this, [this]() { handleVerifyRecipient(); });
    connect(confirmButton, &QPushButton::clicked, this, [this]() { handleTransfer(); });
    connect(backButton, &QPushButton::clicked, this, [this]() { showDashboard(); });

    return page;
}

QWidget* MainWindow::createHistoryPage() {
    QWidget* page = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setContentsMargins(30, 30, 30, 30);

    QLabel* title = new QLabel("Transaction History", page);
    title->setObjectName("title");

    historyTable = new QTableWidget(page);
    historyTable->setColumnCount(6);
    historyTable->setHorizontalHeaderLabels(QStringList() << "Type" << "Peer Phone" << "Amount" << "Balance After" << "Timestamp" << "Transaction ID");
    historyTable->horizontalHeader()->setStretchLastSection(true);
    historyTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    historyTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    historyTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    QPushButton* refreshButton = new QPushButton("Refresh", page);
    QPushButton* backButton = new QPushButton("Back", page);

    QHBoxLayout* buttons = new QHBoxLayout();
    buttons->addWidget(refreshButton);
    buttons->addWidget(backButton);
    buttons->addStretch();

    layout->addWidget(title);
    layout->addWidget(historyTable);
    layout->addLayout(buttons);

    connect(refreshButton, &QPushButton::clicked, this, [this]() { refreshHistoryTable(); });
    connect(backButton, &QPushButton::clicked, this, [this]() { showDashboard(); });

    return page;
}

QWidget* MainWindow::createProfilePage() {
    QWidget* page = new QWidget(this);
    QVBoxLayout* outer = new QVBoxLayout(page);
    outer->setAlignment(Qt::AlignCenter);

    QGroupBox* card = new QGroupBox("Edit Profile", page);
    card->setObjectName("card");
    card->setMinimumWidth(430);
    QVBoxLayout* cardLayout = new QVBoxLayout(card);

    profileFnameEdit = new QLineEdit(card);
    profileLnameEdit = new QLineEdit(card);

    QFormLayout* form = new QFormLayout();
    form->addRow("First Name", profileFnameEdit);
    form->addRow("Last Name", profileLnameEdit);

    QPushButton* saveButton = new QPushButton("Save Changes", card);
    QPushButton* backButton = new QPushButton("Back", card);

    cardLayout->addLayout(form);
    cardLayout->addWidget(saveButton);
    cardLayout->addWidget(backButton);

    outer->addWidget(card);

    connect(saveButton, &QPushButton::clicked, this, [this]() { handleUpdateProfile(); });
    connect(backButton, &QPushButton::clicked, this, [this]() { showDashboard(); });

    return page;
}

void MainWindow::handleLogin() {
    std::string phone = loginPhoneEdit->text().trimmed().toStdString();
    std::string password = loginPasswordEdit->text().toStdString();

    Account* account = system.login(phone, password);
    if (account == nullptr) {
        QMessageBox::warning(this, "Login Failed", "Invalid phone number or password.");
        return;
    }

    currentUser = account;
    showDashboard();
}

void MainWindow::handleSignup() {
    std::string fname = signupFnameEdit->text().trimmed().toStdString();
    std::string lname = signupLnameEdit->text().trimmed().toStdString();
    std::string phone = signupPhoneEdit->text().trimmed().toStdString();
    std::string password = signupPasswordEdit->text().toStdString();
    double initialBalance = signupInitialBalanceSpin->value();

    Account* account = system.signup(fname, lname, phone, password, initialBalance);
    if (account == nullptr) {
        QMessageBox::warning(this, "Signup Failed", "Please check all fields. Phone must be unique, 11 digits, and password at least 6 characters.");
        return;
    }

    currentUser = account;
    QMessageBox::information(this, "Signup Successful", "Account created successfully.");
    showDashboard();
}

void MainWindow::handleLogout() {
    currentUser = nullptr;
    loginPasswordEdit->clear();
    pages->setCurrentWidget(loginPage);
}

void MainWindow::handleDeposit() {
    if (currentUser == nullptr) {
        return;
    }

    bool ok = false;
    double amount = QInputDialog::getDouble(this, "Deposit", "Amount:", 100.0, 0.01, 100000000.0, 2, &ok);
    if (!ok) {
        return;
    }

    if (!currentUser->deposit(amount)) {
        QMessageBox::warning(this, "Deposit Failed", "Invalid deposit amount.");
        return;
    }

    QMessageBox::information(this, "Deposit Successful", "Deposit completed successfully.");
    refreshDashboard();
}

void MainWindow::handleWithdraw() {
    if (currentUser == nullptr) {
        return;
    }

    bool ok = false;
    double amount = QInputDialog::getDouble(this, "Withdraw", "Amount:", 100.0, 0.01, 100000000.0, 2, &ok);
    if (!ok) {
        return;
    }

    if (!currentUser->withdraw(amount)) {
        QMessageBox::warning(this, "Withdraw Failed", "Invalid amount or insufficient balance.");
        return;
    }

    QMessageBox::information(this, "Withdraw Successful", "Withdrawal completed successfully.");
    refreshDashboard();
}

void MainWindow::handleVerifyRecipient() {
    std::string receiverPhone = receiverPhoneEdit->text().trimmed().toStdString();
    std::string name = transactionManager.verifyRecipient(receiverPhone, system);

    if (name == "NOT_FOUND") {
        recipientResultLabel->setText("Recipient not found.");
        recipientResultLabel->setStyleSheet("color: #b00020; font-weight: bold;");
        return;
    }

    if (currentUser != nullptr && receiverPhone == currentUser->getPhone()) {
        recipientResultLabel->setText("You cannot transfer to your own account.");
        recipientResultLabel->setStyleSheet("color: #b00020; font-weight: bold;");
        return;
    }

    recipientResultLabel->setText(QString("Sending to: ") + QString::fromStdString(name));
    recipientResultLabel->setStyleSheet("color: #0b7a3b; font-weight: bold;");
}

void MainWindow::handleTransfer() {
    if (currentUser == nullptr) {
        return;
    }

    std::string receiverPhone = receiverPhoneEdit->text().trimmed().toStdString();
    double amount = transferAmountSpin->value();

    int code = transactionManager.executeTransfer(currentUser->getPhone(), receiverPhone, amount, system, currentTimestamp());
    QString message = QString::fromStdString(transactionManager.getTransferMessage(code));

    if (code == 0) {
        QMessageBox::information(this, "Transfer", message);
        resetTransferPage();
        refreshDashboard();
        pages->setCurrentWidget(dashboardPage);
    } else {
        QMessageBox::warning(this, "Transfer Failed", message);
    }
}

void MainWindow::handleUpdateProfile() {
    if (currentUser == nullptr) {
        return;
    }

    std::string newFname = profileFnameEdit->text().trimmed().toStdString();
    std::string newLname = profileLnameEdit->text().trimmed().toStdString();

    if (newFname.empty() || newLname.empty()) {
        QMessageBox::warning(this, "Profile", "First name and last name cannot be empty.");
        return;
    }

    bool updated = system.updateProfile(currentUser->getPhone(), newFname, newLname);
    if (!updated) {
        QMessageBox::warning(this, "Profile", "Could not update profile.");
        return;
    }

    currentUser = system.findAccount(currentUser->getPhone());
    QMessageBox::information(this, "Profile", "Profile updated successfully.");
    showDashboard();
}

void MainWindow::refreshDashboard() {
    if (currentUser == nullptr) {
        return;
    }

    dashboardNameLabel->setText(QString("Name: ") + QString::fromStdString(currentUser->getFullName()));
    dashboardPhoneLabel->setText(QString("Phone: ") + QString::fromStdString(currentUser->getPhone()));
    dashboardBalanceLabel->setText(QString("Balance: ") + moneyText(currentUser->getBalance()));
    dashboardHistoryCountLabel->setText(QString("Transactions: ") + QString::number(currentUser->getHistoryCount()));
}

void MainWindow::refreshHistoryTable() {
    if (currentUser == nullptr || historyTable == nullptr) {
        return;
    }

    std::vector<Transaction> history = currentUser->getHistoryNewestFirst();
    historyTable->setRowCount(static_cast<int>(history.size()));

    for (int row = 0; row < static_cast<int>(history.size()); ++row) {
        const Transaction& tx = history[static_cast<std::size_t>(row)];
        historyTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(tx.type)));
        historyTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(tx.peerPhone)));
        historyTable->setItem(row, 2, new QTableWidgetItem(moneyText(tx.amount)));
        historyTable->setItem(row, 3, new QTableWidgetItem(moneyText(tx.balanceAfter)));
        historyTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(tx.timestamp)));
        historyTable->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(tx.transactionID)));
    }
}

void MainWindow::resetTransferPage() {
    receiverPhoneEdit->clear();
    recipientResultLabel->setText("Enter a receiver phone number, then click Verify.");
    recipientResultLabel->setStyleSheet("color: #666666;");
    transferAmountSpin->setValue(100.0);
}

void MainWindow::showDashboard() {
    refreshDashboard();
    pages->setCurrentWidget(dashboardPage);
}

QString MainWindow::moneyText(double value) const {
    return QString("EGP ") + QString::number(value, 'f', 2);
}

std::string MainWindow::currentTimestamp() const {
    return QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss").toStdString();
}

void MainWindow::applyStyle() {
    setStyleSheet(
        "QWidget { background: #f4f6fb; font-family: Segoe UI, Arial; font-size: 14px; }"
        "QGroupBox#card { background: white; border: 1px solid #dfe3ec; border-radius: 14px; margin-top: 12px; padding: 18px; }"
        "QGroupBox#card::title { subcontrol-origin: margin; left: 18px; padding: 0 8px; color: #1f2a44; font-weight: bold; }"
        "QLineEdit, QDoubleSpinBox { background: white; border: 1px solid #c9d1e2; border-radius: 8px; padding: 8px; }"
        "QLineEdit:focus, QDoubleSpinBox:focus { border: 2px solid #4f6df5; }"
        "QPushButton { background: #4f6df5; color: white; border: none; border-radius: 9px; padding: 10px 14px; font-weight: bold; }"
        "QPushButton:hover { background: #3f5be0; }"
        "QPushButton:pressed { background: #3349b5; }"
        "QLabel#title { color: #1f2a44; font-size: 28px; font-weight: bold; margin-bottom: 10px; }"
        "QLabel#subtitle { color: #4b587c; font-size: 15px; }"
        "QLabel#hint { color: #666666; }"
        "QLabel#balance { color: #0b7a3b; font-size: 20px; font-weight: bold; }"
        "QTableWidget { background: white; border: 1px solid #dfe3ec; border-radius: 10px; gridline-color: #e7eaf2; }"
        "QHeaderView::section { background: #e8ecfb; color: #1f2a44; padding: 8px; border: none; font-weight: bold; }"
    );
}
