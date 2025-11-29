/*
 * Name : Aya Ramadan
 * Date : 28/11/2025
 * Project Name : ATM Simulator
 */
#include <stdio.h>
#include <string.h>
#include "../include/admin.h"
#include "../include/private.h"
/*------------------------------------------------------------------------------------------------- */
/*
 this function allow the customer to login
*/
int LogIn()
{
    char cardnumber[20];
    int status = 0, trial = 1;
    // open log file save this failed login in Log File
    FILE *temp_aduit_file = fopen(AUDITLOG_FILE_PATH, "a");
    if (temp_aduit_file == NULL)
    {
        printf("Error cannot open aduit log file\n");
        return CANNOT_OPEN_FILE;
    }
    // time
    char timestamp[20];
    getDayMonthHourMin(timestamp, sizeof(timestamp));
    // there are 3 trial to log in
    printf("!!! There is 3 trial to login\n");
    while (trial <= 3)
    {
        printf("--------------------------\n");
        printf(" Enter data To login\n ");
        printf("Enter Your card numer: ");
        scanf(" %[^\n]s", cardnumber);
        status = SearchAccount(cardnumber, CUSTOMER, N_OUTBALANCE, N_TRANSFER);
        // if this account not found continue to the next trial
        if (status == ACCOUNT_NOT_FOUND)
        {
            printf("The %d is false\n", trial);
            fprintf(temp_aduit_file, "%s,%s,%s\n", cardnumber, "LOG_failed", timestamp);
            fclose(temp_aduit_file);
            ++trial;
            continue;
        }
        else
        {
            break;
        }
    }
    if (trial != 4)
    {
        if (status == ACCOUNT_NOT_FREEZE)
        {
            fprintf(temp_aduit_file, "%s,%s,%s\n", cardnumber, "LOG_SUccess", timestamp);
            fclose(temp_aduit_file);

            printf("succesful login\n");
            return ACCOUNT_NOT_FREEZE;
        }
        else if (status == ACCOUNT_FREEZE)
        {
            fprintf(temp_aduit_file, "%s,%s,%s\n", cardnumber, "LOG_Freeze", timestamp);
            fclose(temp_aduit_file);
            printf(" the account is freeze\n");
            return ACCOUNT_FREEZE;
        }
    }
    fclose(temp_aduit_file);
    return -1;
}
/*------------------------------------------------------------------------------------------------- */
/*
 this function allow the customer to show the total balance
*/
void BalanceInquiry()
{
    char cardnumber[20];
    printf("Enter Your card numer to show Balance Inquiry: ");
    scanf(" %[^\n]s", cardnumber);
    SearchAccount(cardnumber, CUSTOMER, OUTBALANCE, N_TRANSFER);
}
/*------------------------------------------------------------------------------------------------- */
/*
 this function allow the customer to Cash Withdrawal
*/
void CashWithdrawal()
{
    char cardnumber[20];
    int ammount;
    printf("Enter Your card numer to Withdrawal: ");
    scanf(" %[^\n]s", cardnumber);
    // search about this cardnumber and check if it freeze or not
    int status = SearchAccount(cardnumber, CUSTOMER, N_OUTBALANCE, N_TRANSFER);
    if (status == ACCOUNT_NOT_FREEZE)
    {
        printf("Enter the number you want to Withdrawal: ");
        scanf("%d", &ammount);

        FILE *accounts = fopen(ACCOUNT_FILE_PATH, "r");
        if (accounts == NULL)
        {
            printf("Error : Could not open file! \n");
            return;
        }
        else
        {
            S_account account;
            // open file to update balance after Withdrawal
            FILE *temp_file = fopen(TEMP_FILE_PATH, "a");
            if (temp_file == NULL)
            {
                printf("Error : Could not open file! \n");
                return;
            }
            while (Read_Account_File(accounts, &account) == 6)
            {
                if (!strcmp(cardnumber, account.cardNumber))
                {
                    // check if this amount greater than his total balance
                    if (ammount > account.initialBalance)
                    {
                        printf("No Enough money\n");
                        printf("the total Balance: %.3lf\n", account.initialBalance);
                        fclose(accounts);
                        fclose(temp_file);
                        remove(TEMP_FILE_PATH);
                        return;
                    }
                    else if (ammount <= account.initialBalance)
                    {
                        // check if this amount greater than the dailylimit or not
                        if (ammount > account.dailyLimit)
                        {
                            printf("This ammount Greater than the Daily Limit \n");
                            printf("the dailay limit : %.3lf\n", account.dailyLimit);
                            fclose(accounts);
                            fclose(temp_file);
                            remove(TEMP_FILE_PATH);
                            return;
                        }
                        else
                        {
                            // time
                            char timestamp[20];
                            // check if the ATM has enough money or not
                            if (ViewATMCash(WITHDRAUAL) >= ammount)
                            {
                                // update balaance
                                account.initialBalance -= ammount;
                                // open files to save this operation
                                FILE *temp_mini_file = fopen(MINISTATEMENT_FILE_PATH, "a");
                                FILE *temp_aduit_file = fopen(AUDITLOG_FILE_PATH, "a");
                                if (temp_mini_file == NULL || temp_aduit_file == NULL)
                                {
                                    printf("Error cannot open  file\n");
                                    return;
                                }
                                // Update ATM cash
                                withdrawFromATM(ammount);

                                getDayMonthHourMin(timestamp, sizeof(timestamp));
                                fprintf(temp_mini_file, "%s,%s,%s,%d,%s\n", account.name, account.cardNumber, "CashWithdrawal", ammount, timestamp);
                                fprintf(temp_aduit_file, "%s,%s,%d,%s\n", cardnumber, "CashWithdrawal", ammount, timestamp);

                                fclose(temp_mini_file);
                                fclose(temp_aduit_file);
                            }

                            else
                            {
                                printf("There is no Enough money in ATM\n");
                                return;
                            }
                        }
                    }
                }
                Write_Account_File(temp_file, account);
            }
            fclose(accounts);
            fclose(temp_file);
            remove(ACCOUNT_FILE_PATH);
            rename(TEMP_FILE_PATH, ACCOUNT_FILE_PATH);
            return;
        }
    }
    else if (status == ACCOUNT_FREEZE)
    {
        printf("This account is freeze so you cannot CashWithdrawal\n");
        return;
    }
}
/*------------------------------------------------------------------------------------------------- */
/*
 this function allow the customer to Cash Deposit
*/
void CashDeposit()
{
    char cardnumber[20];
    int ammount;
    printf("Enter Your card numer to deposite: ");
    scanf(" %[^\n]s", cardnumber);
    // search about this cardnumber and check if it freeze or not
    int status = SearchAccount(cardnumber, CUSTOMER, N_OUTBALANCE, N_TRANSFER);
    if (status == ACCOUNT_NOT_FREEZE)
    {
        printf("Enter the number you want to Deposit: ");
        scanf("%d", &ammount);

        FILE *accounts = fopen(ACCOUNT_FILE_PATH, "r");
        if (accounts == NULL)
        {
            printf("Error : Could not open file! \n");
            return;
        }
        else
        {
            S_account account;
            char timestamp[20];
            // open file to save this operation
            FILE *temp_file = fopen(TEMP_FILE_PATH, "a");
            if (temp_file == NULL)
            {
                printf("Error : Could not open file! \n");
                return;
            }
            while (Read_Account_File(accounts, &account) == 6)
            {
                if (!strcmp(cardnumber, account.cardNumber))
                {
                    // update balance
                    account.initialBalance += ammount;
                    // open file to save operations
                    FILE *temp_mini_file = fopen(MINISTATEMENT_FILE_PATH, "a");
                    FILE *temp_aduit_file = fopen(AUDITLOG_FILE_PATH, "a");
                    if (temp_mini_file == NULL || temp_aduit_file == NULL)
                    {
                        printf("Error cannot open  file\n");
                        return;
                    }

                    getDayMonthHourMin(timestamp, sizeof(timestamp));

                    fprintf(temp_mini_file, "%s,%s,%s,%d,%s\n", account.name, account.cardNumber, "Deposit", ammount, timestamp);
                    fprintf(temp_aduit_file, "%s,%s,%d,%s\n", cardnumber, "Deposit", ammount, timestamp);
                    printf("the Deposit successfuly\n");
                    fclose(temp_aduit_file);
                    fclose(temp_mini_file);
                }
                Write_Account_File(temp_file, account);
            }

            fclose(accounts);
            fclose(temp_file);
            remove(ACCOUNT_FILE_PATH);
            rename(TEMP_FILE_PATH, ACCOUNT_FILE_PATH);
            return;
        }
    }
    else if (status == ACCOUNT_FREEZE)
    {
        printf("This account is freeze so you cannot Deposit\n");
        return;
    }
}
/*------------------------------------------------------------------------------------------------- */
/*
 this function allow the customer to Fund Transfer
*/
void FundTransfer()
{
    char cardnumber1[20], cardnumber2[20];
    int ammount;
    printf("Enter Your card numer to transfer: ");
    scanf(" %[^\n]s", cardnumber1);
    // search about this cardnumber and check if it freeze or not
    int status = SearchAccount(cardnumber1, CUSTOMER, N_OUTBALANCE, TRANSFER_OWNER);
    if (status == ACCOUNT_NOT_FREEZE)
    {
        printf("Enter the card numer you want to Transfer: ");
        scanf(" %[^\n]s", cardnumber2);
        // search about this cardnumber and check if it freeze or not
        int status = SearchAccount(cardnumber2, CUSTOMER, N_OUTBALANCE, TRANSFER_CUETOMER);
        if (status == ACCOUNT_NOT_FREEZE)
        {
            printf("Enter the number you want to Transfer: ");
            scanf("%d", &ammount);

            FILE *accounts = fopen(ACCOUNT_FILE_PATH, "r");
            if (accounts == NULL)
            {
                printf("Error : Could not open file! \n");
                return;
            }
            else
            {
                S_account account;
                // time
                char timestamp[20];
                // open file to update balance
                FILE *temp_file = fopen(TEMP_FILE_PATH, "a");
                if (temp_file == NULL)
                {
                    printf("Error : Could not open file! \n");
                    return;
                }
                while (Read_Account_File(accounts, &account) == 6)
                {
                    if (!strcmp(cardnumber1, account.cardNumber))
                    {
                        // check if ammount is less than the total balance and the delaiy
                        if (account.initialBalance >= ammount && account.dailyLimit >= ammount)
                        {
                            // update Total balance
                            account.initialBalance -= ammount;
                            // open file to save this operations
                            FILE *temp_mini_file = fopen(MINISTATEMENT_FILE_PATH, "a");
                            FILE *temp_aduit_file = fopen(AUDITLOG_FILE_PATH, "a");
                            if (temp_mini_file == NULL || temp_aduit_file == NULL)
                            {
                                printf("Error cannot open file\n");
                                return;
                            }

                            getDayMonthHourMin(timestamp, sizeof(timestamp));

                            fprintf(temp_mini_file, "%s,%s,%s,%d,%s\n", account.name, account.cardNumber, "Transfer Out", ammount, timestamp);
                            fprintf(temp_aduit_file, "%s,%s,%d,%s\n", cardnumber1, "transfer out", ammount, timestamp);

                            fclose(temp_aduit_file);
                            fclose(temp_mini_file);
                            printf("the Transfer successfuly\n");
                        }
                        else if (account.initialBalance < ammount)
                        {
                            printf("you cannot Transfer this ammount your initialBalance is less than it \n");
                            printf("Your Balance is %f\n", account.initialBalance);
                            fclose(accounts);
                            fclose(temp_file);
                            remove(TEMP_FILE_PATH);
                            return;
                        }
                        else if (account.dailyLimit < ammount)
                        {
                            printf("you cannot Transfer this ammount your dailaylimit  is less than it \n");
                            printf("Your Daily limit is %f\n", account.dailyLimit);
                            fclose(accounts);
                            fclose(temp_file);
                            remove(TEMP_FILE_PATH);
                            return;
                        }
                    }
                    else if (!strcmp(cardnumber2, account.cardNumber))
                    {
                        // update Total balance
                        account.initialBalance += ammount;

                        FILE *temp_mini_file = fopen(MINISTATEMENT_FILE_PATH, "a");
                        FILE *temp_aduit_file = fopen(AUDITLOG_FILE_PATH, "a");
                        if (temp_mini_file == NULL || temp_aduit_file == NULL)
                        {
                            printf("Error cannot open  file\n");
                            return;
                        }

                        getDayMonthHourMin(timestamp, sizeof(timestamp));

                        fprintf(temp_mini_file, "%s,%s,%s,%d,%s\n", account.name, account.cardNumber, "Transfer In", ammount, timestamp);
                        fprintf(temp_aduit_file, "%s,%s,%d,%s\n", cardnumber2, "transfer In", ammount, timestamp);

                        fclose(temp_mini_file);
                        fclose(temp_aduit_file);

                        printf("%s  transfer to you \n", cardnumber1);
                    }
                    Write_Account_File(temp_file, account);
                }

                fclose(accounts);
                fclose(temp_file);
                remove(ACCOUNT_FILE_PATH);
                rename(TEMP_FILE_PATH, ACCOUNT_FILE_PATH);
                return;
            }
        }
        else if (status == ACCOUNT_FREEZE)
        {
            printf("this account is freeze so you cannot Transfer to \n");
            return;
        }
    }
    else if (status == ACCOUNT_FREEZE)
    {
        printf("your account is freeze so you cannot Transfer\n");
        return;
    }
}
/*------------------------------------------------------------------------------------------------- */
/*
 this function allow the customer to change your Pin Number
*/
void ChangePin()
{
    if (LogIn() == ACCOUNT_NOT_FREEZE)
    {
        Freeze_Unfreeze_ResetPin(RESET_PIN);
        printf("Change Pin Success\n");
        printf("-----------------\n");
    }

    return;
}
/*------------------------------------------------------------------------------------------------- */
/*
 this function allow the customer to show the last 10 operations
*/
void miniStatement(const char *card)
{
    char line[LINE_SIZE];
    char filtered[MAX_TX][LINE_SIZE];
    int count = 0;

    FILE *temp_mini_file = fopen(MINISTATEMENT_FILE_PATH, "r");
    if (temp_mini_file == NULL)
    {
        printf("Error cannot open ministatement file\n");
        return;
    }
    while (fgets(line, sizeof(line), temp_mini_file))
    {
        char fileName[50], filecard[20];
        sscanf(line, "%49[^,],%19[^,]", fileName, filecard);
        if (strcmp(filecard, card) == 0)
        {
            strcpy(filtered[count], line);
            count++;
        }
    }
    fclose(temp_mini_file);

    if (count == 0)
    {
        printf("\nNo transactions found for this account.\n");
        return;
    }
    printf("\n------ Mini Statement (Last 10 Transactions) ------\n");
    int start = count - 10;
    if (start < 0)
    {
        start = 0;
    }
    for (int i = start; i < count; i++)
    {
        printf("%s", filtered[i]);
    }
    printf("--------------------------------------------------\n");
}
/*------------------------------------------------------------------------------------------------- */
/*
 this function allow the customer to log out from this account
*/
void LogOut()
{
    printf("LOg Out\n");
}
/*---------------------------------------------------------------------------------------------------------------*/
// private function used in side public
int withdrawFromATM(int requested)
{
    int amount = requested;
    int denom[10], count[10], used[10] = {0};
    int n = 0;

    FILE *ATM = fopen(ATMCASH_FILE_PATH, "r");
    if (ATM == NULL)
    {
        printf("Error opening atm_cash file!\n");
        return CANNOT_OPEN_FILE;
    }

    while (fscanf(ATM, "%d,%d\n", &denom[n], &count[n]) == 2)
    {
        n++;
    }

    // Greedy
    for (int i = 0; i < n; i++)
    {
        int need = amount / denom[i];
        if (need > 0)
        {
            if (count[i] >= need)
            {
                used[i] = need;
            }
            else
            {
                used[i] = count[i];
            }

            amount -= used[i] * denom[i];
        }
    }

    // Deduct used notes
    for (int i = 0; i < n; i++)
        count[i] -= used[i];

    // Write new file
    FILE *temp_atm_file = fopen(TEMP_ATM_FILE_PATH, "a");
    for (int i = 0; i < n; i++)
        fprintf(temp_atm_file, "%d,%d\n", denom[i], count[i]);

    fclose(ATM);
    fclose(temp_atm_file);
    remove(ATMCASH_FILE_PATH);
    rename(TEMP_ATM_FILE_PATH, ATMCASH_FILE_PATH);
    printf("the CashWithdrawal successfuly\n");
    return 1; // success
}
// calculate time
void getDayMonthHourMin(char *buffer, int size)
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    // صيغة الإخراج: يوم-شهر  ساعة:دقيقة
    strftime(buffer, size, "%d-%m  %H:%M", t);
}
