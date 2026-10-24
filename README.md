# CodeAlpha_BankingSystem

[![C Programming](https://img.shields.io/badge/Language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

A robust, menu-driven Bank Account Management System developed as **Task 4** for the **CodeAlpha C Programming Internship**. This application implements advanced systems programming concepts including structures, function pointers, and binary file handling.

---

## 🚀 Features

* **Create Account**: Registers a new bank account with a holder name, secure 4-digit PIN, and automatically assigns a sequential account number starting from `1001`.
* **Deposit**: Securely adds funds to an account after authenticating with the account number and PIN.
* **Withdraw**: Safely deducts funds from an account with built-in insufficient-fund checks and PIN protection[cite: 3].
* **Balance Enquiry**: Displays current account information, holder name, and balance securely[cite: 3].
* **Persistent Binary Storage**: Automatically saves all changes and account profiles to `accounts.dat` so data persists between runs[cite: 3].

---

## 🛠️ Concepts & Technologies

* **Language**: C (`-std=c11`)[cite: 3]
* **Core Concepts**: Structures, custom functions, function pointers, and binary file operations (`fopen`, `fread`, `fwrite`, `fseek`)[cite: 3].
* **Precision Financial Design**: Monetary amounts are stored internally as whole cents (`long long Cents`) to completely eliminate floating-point rounding errors[cite: 3].
* **Security**: Every transaction session is strictly protected by account validation and a 4-digit PIN[cite: 3].

---

## ⚙️ Getting Started

### Prerequisites
* A C compiler supporting the C11 standard (such as `gcc`, `clang`, or MinGW)[cite: 3].

### Compilation
Clone or download the repository, open your terminal in the project directory, and compile the source code using the recommended compiler flags[cite: 3]:

```bash
gcc -Wall -Wextra -std=c11 -o bank banking_system.c
./bank
bank.exe
========== BANK ACCOUNT MANAGEMENT SYSTEM ==========
 1. Create Account
 2. Deposit
 3. Withdraw
 4. Balance Enquiry
 5. Exit
Enter your choice:
Select an option from 1 to 5 to perform banking operations[cite: 3].

For financial transactions (Deposit, Withdraw, Balance Enquiry), you will be prompted to enter your Account Number and 4-digit PIN for secure session authentication[cite: 3].

All changes are written directly and safely to the binary database file (accounts.dat)[cite: 3].
📜 License
This project is open-source and available under the MIT License.
