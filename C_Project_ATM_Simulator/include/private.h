/*
 * Name : Aya Ramadan
 * Date : 28/11/2025
 * Project Name : ATM Simulator
 */
#ifndef PRIVATE_H_
#define PRIVATE_H_

#include "admin.h"
#include <stdio.h>

// Paths of Files (my lab nead absolute path )
#define ACCOUNT_FILE_PATH "E:/NTI_AIOT/C_Programming/ATM_project/Saved_Files/accounts.text"
#define ATMCASH_FILE_PATH "E:/NTI_AIOT/C_Programming/ATM_project/Saved_Files/atm_cash.text"
#define TEMP_FILE_PATH "E:/NTI_AIOT/C_Programming/ATM_project/Saved_Files/temp_file.text"
#define TEMP_ATM_FILE_PATH "E:/NTI_AIOT/C_Programming/ATM_project/Saved_Files/temp_atm_file.text"
#define MINISTATEMENT_FILE_PATH "E:/NTI_AIOT/C_Programming/ATM_project/Saved_Files/mini_statement_file.text"
#define AUDITLOG_FILE_PATH "E:/NTI_AIOT/C_Programming/ATM_project/Saved_Files/aduit_Log_file.text"

// input type
#define ADMIN 0
#define CUSTOMER 1
// account status search_function
#define ACCOUNT_NOT_FOUND 0
#define ACCOUNT_FOUND 1
#define ACCOUNT_FREEZE 2
#define ACCOUNT_NOT_FREEZE 3
#define CANNOT_OPEN_FILE 4
// reset function
#define FREEZE_UNFEEZE 0
#define RESET_PIN 1
// flags
#define OUTBALANCE 0
#define N_OUTBALANCE 1

#define WITHDRAUAL 0
#define N_WITHDRAUAL 1

#define TRANSFER_OWNER 0
#define TRANSFER_CUETOMER 1
#define N_TRANSFER 2
// mini statment
#define MAX_TX 1000
#define LINE_SIZE 300

// Private functions
void print(S_account *account);
void TakeAccountData(S_account *data);
void TakeLoadCash(S_denom_count *data);
void Write_Account_File(FILE *accounts, S_account newAccount);
int Read_Account_File(FILE *accounts, S_account *account);

int withdrawFromATM(int requested);
void getDayMonthHourMin(char *buffer, int size);

#endif