/*
 * ============================================================================
 *  CodeAlpha C Programming Internship - Task 4 : Bank Account Management System
 * ============================================================================
 *  Features : Create Account | Deposit | Withdraw | Balance Enquiry | Exit
 *  Concepts : structures, functions, function pointers, binary file handling
 *  Storage  : accounts.dat (binary) - all accounts persist between runs
 *  Design   : money is stored as whole cents (integer) to avoid floating-point
 *             rounding errors; every transaction is protected by a 4-digit PIN.
 *
 *  Build    : gcc -Wall -Wextra -std=c11 -o bank banking_system.c
 *  Run      : ./bank            (Windows: bank.exe)
 * ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DB_FILE    "accounts.dat"
#define FIRST_ID   1001
#define MAX_ID     999999999
#define NAME_LEN   50
#define MAX_AMOUNT 1e9                          /* per-transaction limit       */
#define AMT(c)     ((double)(c) / 100)          /* cents -> displayable amount */

typedef long long Cents;

typedef struct {
    int   id;
    char  pin[5];
    char  name[NAME_LEN];
    Cents balance;
} Account;

typedef int (*Action)(Account *);               /* returns 1 if account changed */

/* ------------------------------ Input helpers ------------------------------ */

/* Prompts and reads one line safely: handles EOF and discards over-long input. */
static void readLine(const char *prompt, char *buf, int size) {
    printf("%s", prompt);
    fflush(stdout);
    if (!fgets(buf, size, stdin)) { puts(""); exit(EXIT_SUCCESS); }
    if (!strchr(buf, '\n')) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { }
    }
    buf[strcspn(buf, "\n")] = '\0';
}

/* Reads an integer in [min, max], re-prompting until the input is valid. */
static int readInt(const char *prompt, int min, int max) {
    char buf[32], *end;
    for (;;) {
        readLine(prompt, buf, sizeof buf);
        long v = strtol(buf, &end, 10);
        if (end != buf && *end == '\0' && v >= min && v <= max) return (int)v;
        printf("  Please enter a number between %d and %d.\n", min, max);
    }
}

/* Reads a positive money amount (e.g. 250.75) and returns it in cents. */
static Cents readAmount(const char *prompt) {
    char buf[32], junk;
    double v;
    for (;;) {
        readLine(prompt, buf, sizeof buf);
        if (sscanf(buf, "%lf %c", &v, &junk) == 1 && v >= 0.01 && v <= MAX_AMOUNT)
            return (Cents)(v * 100 + 0.5);
        printf("  Invalid amount. Enter a value between 0.01 and %.0f.\n", MAX_AMOUNT);
    }
}

/* Reads a non-blank line (used for names). */
static void readText(const char *prompt, char *buf, int size) {
    do {
        readLine(prompt, buf, size);
    } while (buf[strspn(buf, " \t")] == '\0');
}

/* ----------------------------- Account actions ----------------------------- */

static int deposit(Account *a) {
    Cents amt = readAmount("Deposit amount  : ");
    a->balance += amt;
    printf("  Deposited %.2f. New balance: %.2f\n", AMT(amt), AMT(a->balance));
    return 1;
}

static int withdraw(Account *a) {
    Cents amt = readAmount("Withdraw amount : ");
    if (amt > a->balance) {
        printf("  Insufficient funds. Available balance: %.2f\n", AMT(a->balance));
        return 0;
    }
    a->balance -= amt;
    printf("  Withdrawn %.2f. New balance: %.2f\n", AMT(amt), AMT(a->balance));
    return 1;
}

static int balanceEnquiry(Account *a) {
    printf("  Account %d | %s | Balance: %.2f\n", a->id, a->name, AMT(a->balance));
    return 0;
}

/* ------------------------------ File handling ------------------------------ */

/* Finds an account by id; returns its byte offset in the file, or -1. */
static long findAccount(FILE *fp, int id, Account *acc) {
    long pos = 0;
    rewind(fp);
    while (fread(acc, sizeof *acc, 1, fp) == 1) {
        if (acc->id == id) return pos;
        pos += (long)sizeof *acc;
    }
    return -1;
}

/* Opens the database, authenticates the customer, runs `action`, saves changes. */
static void runSession(Action action) {
    Account acc;
    char pin[16];
    FILE *fp = fopen(DB_FILE, "r+b");

    if (!fp) { puts("  No accounts found. Please create an account first."); return; }

    int id = readInt("Account number : ", 1, MAX_ID);
    readLine("PIN            : ", pin, sizeof pin);
    long pos = findAccount(fp, id, &acc);

    if (pos < 0 || strcmp(acc.pin, pin) != 0) {
        puts("  Invalid account number or PIN.");
    } else {
        printf("  Welcome, %s!\n", acc.name);
        if (action(&acc)) {
            fseek(fp, pos, SEEK_SET);
            if (fwrite(&acc, sizeof acc, 1, fp) != 1) perror("  Could not save changes");
        }
    }
    fclose(fp);
}

static void createAccount(void) {
    Account a = {0}, last;
    char pin[16];

    readText("Account holder name : ", a.name, sizeof a.name);
    for (;;) {
        readLine("Set a 4-digit PIN   : ", pin, sizeof pin);
        if (strlen(pin) == 4 && strspn(pin, "0123456789") == 4) break;
        puts("  PIN must be exactly 4 digits.");
    }
    strcpy(a.pin, pin);

    FILE *fp = fopen(DB_FILE, "a+b");
    if (!fp) { perror("  Cannot open " DB_FILE); return; }

    a.id = FIRST_ID;                                    /* next id = last id + 1 */
    if (fseek(fp, -(long)sizeof last, SEEK_END) == 0 && fread(&last, sizeof last, 1, fp) == 1)
        a.id = last.id + 1;

    fseek(fp, 0, SEEK_END);
    if (fwrite(&a, sizeof a, 1, fp) == 1)
        printf("  Account created! Your account number is %d\n", a.id);
    else
        perror("  Could not save account");
    fclose(fp);
}

/* ---------------------------------- Main ----------------------------------- */

int main(void) {
    for (;;) {
        puts("\n========== BANK ACCOUNT MANAGEMENT SYSTEM ==========");
        puts(" 1. Create Account\n 2. Deposit\n 3. Withdraw\n 4. Balance Enquiry\n 5. Exit");

        switch (readInt("Enter your choice: ", 1, 5)) {
            case 1: createAccount();              break;
            case 2: runSession(deposit);          break;
            case 3: runSession(withdraw);         break;
            case 4: runSession(balanceEnquiry);   break;
            case 5: puts("Thank you for banking with us. Goodbye!");
                    return EXIT_SUCCESS;
        }
    }
}
