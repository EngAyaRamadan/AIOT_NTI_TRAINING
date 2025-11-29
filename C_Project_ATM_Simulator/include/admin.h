/*
 * Name : Aya Ramadan
 * Date : 28/11/2025
 * Project Name : ATM Simulator
 */

#ifndef ADMAIN_H
#define ADMAIN_H

#include <time.h>

// account struct
typedef struct
{
    double initialBalance;
    double dailyLimit;
    int freezeStatus;
    char name[50];
    char cardNumber[20];
    char pin[5];
} S_account;
// denomination struct
typedef struct
{
    int denomination;
    int count;
} S_denom_count;

// Admain APIs
void CreateAccount();
void ListAccounts();
int SearchAccount(char cardNumber[], int inputType, int out_balance, int trensfer); // transfer no pin
void LoadATMCash();
double ViewATMCash(int withdrawal);
void Freeze_Unfreeze_ResetPin(int type);
void ViewAuditLog();
void SimpleReports();

// customer APIs
int LogIn();
void BalanceInquiry();
void CashWithdrawal();
void CashDeposit();
void FundTransfer();
void miniStatement(const char *card);
void ChangePin();
void LogOut();

#endif
