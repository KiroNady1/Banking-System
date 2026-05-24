# DSA Banking System — InstaBank

A desktop banking application built with **C++ and Qt Widgets**, developed as a Data Structures and Algorithms course project. The application simulates core banking operations — account management, deposits, withdrawals, peer-to-peer transfers, and transaction history — using appropriate data structures for each responsibility.

---

## Team Members

| Name | ID |
|---|---|
| Kerolos Nady | 2300375 |
| Youssef Kamel | 2300651 |
| Mariam Emad | 2300450 |
| Mark Sameh Maher | 2301079 |

---

## Project Description

InstaBank is a multi-page GUI banking system that allows users to register accounts, authenticate securely, manage their balance through deposits and withdrawals, send money to other registered users, view their full transaction history, and update their profile. The application is entirely in-memory; no database or file persistence is used. A set of demo accounts is seeded automatically on startup so the system can be explored immediately.

---

## Data Structures Used

### 1. `std::unordered_map<std::string, Account>` — Account Registry
Used in `BankingSystem` to store all registered accounts, keyed by phone number.

**Why:** Phone numbers are unique identifiers, and a hash map provides **O(1) average-case** lookup, insertion, and existence checks. This makes login, transfer verification, and profile updates fast regardless of how many accounts exist.

**Rejected alternative:** We initially considered nested or chained hash tables (where one table points to another once full). We rejected this approach after recognising that it would degrade search complexity to **O(n)** in the worst case and add unnecessary implementation complexity. We instead opted for a single `std::unordered_map`, which handles dynamic rehashing internally and keeps average lookup at O(1).

---

### 2. `std::stack<Transaction>` — Transaction History
Used in `HistoryManager` to record each account's transaction log.

**Why:** A stack naturally models the **Last-In, First-Out (LIFO)** behaviour needed for transaction history: the most recent transaction is always on top, which directly supports the undo feature (pop the top entry and reverse its effect on the balance). Iterating from top to bottom gives the history in newest-first order, matching what users expect to see in the history table.

---

### 3. `std::vector<Transaction>` — History Display Buffer
Returned by `getHistoryNewestFirst()` when the GUI needs to render the history table.

**Why:** The stack cannot be iterated without destruction. A `vector` is built by copying the stack into a temporary and draining it, giving a non-destructive, index-accessible snapshot of the history. This separates display logic from the stack's undo responsibility cleanly.

---

## Project Structure

```
DSA_Banking_Qt/
├── DSA_Banking_Qt.pro       # qmake project file
├── include/
│   ├── Account.hpp
│   ├── BankingSystem.hpp
│   ├── HistoryManager.hpp
│   ├── MainWindow.hpp
│   ├── Transaction.hpp
│   └── TransactionManager.hpp
└── src/
    ├── main.cpp
    ├── Account.cpp
    ├── BankingSystem.cpp
    ├── HistoryManager.cpp
    ├── MainWindow.cpp
    ├── Transaction.cpp
    └── TransactionManager.cpp
```

---

## Implemented Features

- **Account Registration (Sign Up)** — Full name, unique 11-digit Egyptian phone number, password (minimum 6 characters), and an optional initial balance.
- **Login / Logout** — Password-verified session management.
- **Dashboard** — Displays the logged-in user's full name, phone number, current balance, and total transaction count.
- **Deposit** — Adds funds to the account and records the transaction.
- **Withdrawal** — Deducts funds with balance validation and records the transaction.
- **Undo Last Transaction** — Reverses the most recent deposit or withdrawal and restores the previous balance.
- **Peer-to-Peer Transfer** — Verifies the recipient by phone number before executing. Handles errors for invalid amounts, self-transfer, missing accounts, and insufficient balance.
- **Transaction History** — Displays all transactions (deposits, withdrawals, sent/received transfers) in a table, newest first, with type, peer phone, amount, balance after, timestamp, and transaction ID.
- **Profile Update** — Allows the logged-in user to change their first and last name.
- **Demo Accounts** — Four pre-seeded accounts are available on startup for immediate testing (see below).

---

## How to Compile and Run

### Prerequisites

- **Qt 5 or Qt 6** (Qt Widgets module required) — download from [https://www.qt.io/download](https://www.qt.io/download)
- **Qt Creator** (included with the Qt installer)
- A C++ compiler supported by Qt on Windows (MinGW or MSVC)

### Steps

1. Open **Qt Creator**.
2. Go to **File → Open File or Project…** and select `DSA_Banking_Qt.pro`.
3. Qt Creator will prompt you to configure a kit — select any available Qt kit (Qt 5.x or Qt 6.x with MinGW or MSVC) and click **Configure Project**.
4. Click the green **Run** button (▶) or press `Ctrl+R` to build and launch the application.

### Building from the Command Line (qmake)

If you prefer not to use Qt Creator, open the Qt command prompt (shipped with the Qt installer) and run:

```bat
cd path\to\DSA_Banking_Qt
qmake DSA_Banking_Qt.pro
mingw32-make        :: if using MinGW
:: or
nmake               :: if using MSVC
```

The compiled executable will be placed in the build output folder. Run it directly:

```bat
DSA_Banking_Qt.exe
```

### Demo Accounts (pre-seeded on startup)

| Name | Phone | Password | Starting Balance |
|---|---|---|---|
| Mark Sameh | 01234567890 | pass123 | EGP 6,500.00 |
| Alice Smith | 01111111111 | pass123 | EGP 3,300.00 |
| Bob Johnson | 01000000000 | pass123 | EGP 2,200.00 |
| Sara Nabil | 01555555555 | pass123 | EGP 4,500.00 |

A **"Fill Demo Login"** button on the login screen auto-fills Mark's credentials for quick access.

---

## AI Usage Declaration

**Tool used:** Claude (Anthropic)

**Purpose of use:**
- Assisting with Qt GUI implementation (widget layout, signal/slot connections, stylesheet syntax).
- Code cleaning and formatting for consistency across files.
- Defining function interfaces early in the project to allow team members to develop their modules simultaneously without merge conflicts.

**Example where AI output required modification:**
The AI initially suggested implementing transaction history using a `std::stack` for both storage and display. However, reading directly from a `std::stack` is destructive — iterating it pops every element, which would destroy the undo history. We modified this by keeping the `std::stack` inside `HistoryManager` for undo operations, and separately building a `std::vector<Transaction>` snapshot in `getHistoryNewestFirst()` by draining a temporary copy of the stack. This preserves the undo stack while giving the GUI a safe, non-destructive list to render.

**What the group implemented and understood themselves:**
- The balance validation logic: checking that the sender has sufficient funds before any debit is performed, and that the debit and credit are applied atomically within `executeTransfer`.
- The P2P transfer flow: separating recipient verification (`verifyRecipient`) from execution (`executeTransfer`) so the GUI can confirm the recipient's name before the user commits to the transfer.
- The class interface design: the team structured `BankingSystem`, `TransactionManager`, `Account`, and `HistoryManager` as independent modules that communicate through a shared `BankingSystem` registry (equivalent to a `UserRegistry`), so different team members (Security, Finance, Operations) could build their modules in parallel against agreed-upon interfaces without depending on each other's implementation being complete.
