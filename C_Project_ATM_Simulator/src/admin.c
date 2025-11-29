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
 this function allow the admain to create a new account
*/
void CreateAccount()
{
    S_account newAccount;
    /* to cheak if data is true or false to save the true only*/
    int true_flag = 0;
    // take the data
    TakeAccountData(&newAccount);
    // print data to check if it true or not
    print(&newAccount);
    // check if this user if found before or not
    int dublicate = SearchAccount(newAccount.cardNumber, ADMIN, N_OUTBALANCE, N_TRANSFER);
    if (dublicate == ACCOUNT_NOT_FOUND)
    { // take the order from admin to save this data or not
        printf("if this data is correct Enter 1 else Enter 0: \n");
        scanf("%d", &true_flag);
        // check to save
        if (true_flag == 1)
        {
            // open file to save on it
            FILE *accounts = fopen(ACCOUNT_FILE_PATH, "a");
            if (accounts == NULL)
            {
                printf("Error opening file! \n");
                printf("This account is  failed to save !!! \n");
                return;
            }
            else
            {
                Write_Account_File(accounts, newAccount);
                fclose(accounts);
                // save this operation in log file
                FILE *temp_aduit_file = fopen(AUDITLOG_FILE_PATH, "a");
                if (temp_aduit_file == NULL)
                {
                    printf("Error cannot open aduit log file");
                    return;
                }
                char timestamp[20];
                getDayMonthHourMin(timestamp, sizeof(timestamp));

                fprintf(temp_aduit_file, "%s,%s,%s\n", newAccount.cardNumber, "Accoun_create", timestamp);
                fclose(temp_aduit_file);

                printf("This account is saved succesfully \n");
                printf("--------------------------------\n");
                return;
            }
        }
        else
        {
            printf("This account is failed to create \n");
            printf("--------------------------------\n");
            return;
        }
    }
    else if (dublicate == ACCOUNT_FOUND)
    {
        printf("This account created befor");
        printf("--------------------------------\n");
        return;
    }
}
/*------------------------------------------------------------------------------------------------- */
/*
 this function list all accounts
*/
void ListAccounts()
{
    // open file to read it
    FILE *accounts = fopen(ACCOUNT_FILE_PATH, "r");
    if (accounts == NULL)
    {
        printf("Error : Could not open file! \n");
        return;
    }
    else
    {
        S_account account;
        printf("--------Account List----------: \n");
        while (Read_Account_File(accounts, &account) == 6)
        {
            print(&account);
        }
        fclose(accounts);
        printf("---------Finish--------------- \n");
        return;
    }
}
/*------------------------------------------------------------------------------------------------- */
/*
 this function allow the admain to searsh for account
 updated to use in multiple function
*/
int SearchAccount(char cardNumber[], int inputType, int out_balance, int trensfer)
{
    char pin[5];
    // if it use in customer mode and  not in transfer in
    if (inputType && trensfer != TRANSFER_CUETOMER)
    {
        printf("Enter Your PIN number: ");
        scanf(" %s", pin);
    }
    // open account file to search
    FILE *accounts = fopen(ACCOUNT_FILE_PATH, "r");
    if (accounts == NULL)
    {
        printf("Error : Could not open file! \n");
        return CANNOT_OPEN_FILE;
    }
    else
    {
        S_account account;
        while (Read_Account_File(accounts, &account) == 6)
        {
            // if this line is the same user I want
            if (!strcmp(cardNumber, account.cardNumber))
            {
                // if admin mode and not nead print balance
                if (!inputType && out_balance == N_OUTBALANCE)
                {
                    printf("the Search account: \n ");
                    print(&account);
                    fclose(accounts);
                    return ACCOUNT_FOUND;
                }
                // if customer mode and not nead print balance and want to return status for trensfer
                else if (inputType && out_balance == N_OUTBALANCE && trensfer != TRANSFER_CUETOMER)
                {
                    if (!strcmp(pin, account.pin))
                    {
                        if (!account.freezeStatus)
                        {
                            fclose(accounts);
                            return ACCOUNT_NOT_FREEZE;
                        }
                        else
                        {
                            fclose(accounts);
                            return ACCOUNT_FREEZE;
                        }
                    }
                }
                // if customer mode and not nead print balance and not transfer
                else if (inputType && out_balance == OUTBALANCE && trensfer == N_TRANSFER)
                {
                    if (!strcmp(pin, account.pin))
                    {
                        printf("the total Balance: %.3lf\n", account.initialBalance);
                        printf("--------------------------------\n");
                        fclose(accounts);
                        return ACCOUNT_FOUND;
                    }
                }
                // if customer mode and transfer In mode
                else if (inputType && trensfer == TRANSFER_CUETOMER)
                {
                    if (!account.freezeStatus)
                    {
                        fclose(accounts);
                        return ACCOUNT_NOT_FREEZE;
                    }
                    else
                    {
                        fclose(accounts);
                        return ACCOUNT_FREEZE;
                    }
                }
            }
        }
        printf("There is no account for this card number or pin \n");
        printf("--------------------------------\n");
        fclose(accounts);
        return ACCOUNT_NOT_FOUND;
    }
}
/*------------------------------------------------------------------------------------------------- */
/*
 this function allow the admain to load money to atm
*/
void LoadATMCash()
{
    FILE *accounts = fopen(ATMCASH_FILE_PATH, "r");
    if (accounts == NULL)
    {
        printf("Error : Could not open file! \n");
        return;
    }
    else
    {
        S_denom_count load, temp;
        int flag = 0;
        // take denomination and its count from user
        TakeLoadCash(&load);
        // open file to add or update denomination and count
        FILE *temp_file = fopen(TEMP_FILE_PATH, "a");
        if (temp_file == NULL)
        {
            printf("Error : Could not open file! \n");
            return;
        }
        while (fscanf(accounts, " %d,%d", &temp.denomination, &temp.count) == 2)
        {
            if (load.denomination == temp.denomination)
            {
                flag = 1;
                temp.count += load.count;
            }
            fprintf(temp_file, "%d,%d\n", temp.denomination, temp.count);
        }
        if (flag == 0)
        {
            fprintf(temp_file, "%d,%d\n", load.denomination, load.count);
        }
        printf("Lood succesfully \n");
        fclose(accounts);
        fclose(temp_file);
        remove(ATMCASH_FILE_PATH);
        rename(TEMP_FILE_PATH, ATMCASH_FILE_PATH);
    }
}
/*------------------------------------------------------------------------------------------------- */
/*
 this function to show total money in atm
*/
double ViewATMCash(int withdrawal)
{
    S_denom_count temp;
    double totalCash = 0;
    // open file to read the denomination and its count
    FILE *ATM = fopen(ATMCASH_FILE_PATH, "r");
    if (ATM == NULL)
    {
        printf("Error : Could not open file! \n");
        return CANNOT_OPEN_FILE;
    }
    // print all denomination and its count and the total money in ATM if in  not withdraual mode
    if (withdrawal == N_WITHDRAUAL)
    {

        printf("--- ATM Cash Inventory---\n");
        printf("Denomination\tCount\tSubtotal\n");
        while (fscanf(ATM, " %d,%d", &temp.denomination, &temp.count) == 2)
        {
            printf("   %d\t          %d\t  %d \n", temp.denomination, temp.count,
                   (temp.denomination * temp.count));
            totalCash += (temp.denomination * temp.count);
        }
        printf("-------------------------------\n");
        printf(" The total cash is : %.2lf\n", totalCash);
    }
    // if in withdraual mode I want to Knoghw the Total money in ATM only
    else if (withdrawal == WITHDRAUAL)
    {
        while (fscanf(ATM, " %d,%d", &temp.denomination, &temp.count) == 2)
        {
            totalCash += (temp.denomination * temp.count);
        }
    }
    fclose(ATM);
    return totalCash;
}
/*------------------------------------------------------------------------------------------------- */
/*
 this function to freeze or unfreeze account and Reset Pin
*/
void Freeze_Unfreeze_ResetPin(int type)
{
    int found = 0;
    FILE *accounts = fopen(ACCOUNT_FILE_PATH, "r");
    if (accounts == NULL)
    {
        printf("Error : Could not open file! \n");
        return;
    }
    else
    {
        char cardNumber[20], NewPin[5];
        int status;
        S_account account;
        FILE *temp_file = fopen(TEMP_FILE_PATH, "a");
        if (temp_file == NULL)
        {
            printf("Error : Could not open file! \n");
            return;
        }

        printf("Enter the card number to change status: ");
        scanf(" %[^\n]s", cardNumber);
        // If I in FREEZE_UNFEEZE mode I nead Freeze or Unfreeze
        if (type == FREEZE_UNFEEZE)
        {
            printf("Enter the 0 for unfreeze  1 for freeze : ");
            scanf(" %d", &status);
        }
        // If I in RESET_PIN mode I nead the new pin
        else if (type == RESET_PIN)
        {
            printf("Enter the New PIN : ");
            scanf(" %s", NewPin);
        }

        // search about this cardnumber and change status if found
        while (Read_Account_File(accounts, &account) == 6)
        {
            if (!strcmp(cardNumber, account.cardNumber))
            {
                FILE *temp_aduit_file = fopen(AUDITLOG_FILE_PATH, "a");
                if (temp_aduit_file == NULL)
                {
                    printf("Error : Could not open file! \n");
                    return;
                }

                found = 1;

                // time stamp to save this operation in Aduit log file
                char timestamp[20];
                getDayMonthHourMin(timestamp, sizeof(timestamp));

                // If I in FREEZE_UNFEEZE mode
                if (type == FREEZE_UNFEEZE)
                {
                    account.freezeStatus = status;
                    if (status == 0)
                    {
                        fprintf(temp_aduit_file, "%s,%s,%s\n", cardNumber, "Accoun_unfreeze", timestamp);
                        printf("this account unfreeze successfully \n");
                    }
                    else
                    {
                        fprintf(temp_aduit_file, "%s,%s,%s\n", cardNumber, "Accoun_freeze", timestamp);
                        printf("this account freeze successfully \n");
                    }
                }
                // If I in RESET_PIN mode
                else if (type == RESET_PIN)
                {
                    fprintf(temp_aduit_file, "%s,%s,%s\n", cardNumber, "Pin_Reset", timestamp);
                    strcpy(account.pin, NewPin);
                }

                fclose(temp_aduit_file);
            }
            Write_Account_File(temp_file, account);
        }

        if (found == 0)
        {
            printf("There is no account for this card number \n");
        }

        fclose(accounts);
        fclose(temp_file);
        remove(ACCOUNT_FILE_PATH);
        rename(TEMP_FILE_PATH, ACCOUNT_FILE_PATH);
        return;
    }
}
/*------------------------------------------------------------------------------------------------- */
/*
 this function to Show the Audit Log File
*/
void ViewAuditLog()
{
    FILE *temp_aduit_file = fopen(AUDITLOG_FILE_PATH, "r");
    if (temp_aduit_file == NULL)
    {
        printf("Error cannot open aduit log file");
        return;
    }

    char line[300];
    printf("\n------ System Audit Log ------\n");
    while (fgets(line, sizeof(line), temp_aduit_file))
    {
        printf("%s", line);
    }

    printf("--------------------------------\n");
    fclose(temp_aduit_file);
}
/*------------------------------------------------------------------------------------------------- */
/*
 this function to Show Simple report about the ATM
*/
void SimpleReports()
{
    FILE *accounts = fopen(ACCOUNT_FILE_PATH, "r");
    FILE *temp_aduit_file = fopen(AUDITLOG_FILE_PATH, "r");
    if (accounts == NULL || temp_aduit_file == NULL)
    {
        printf("Error : Could not open file! \n");
        return;
    }
    printf("\n===== START OF REPORT =====\n\n");

    //  ATM CASH REPORT
    ViewATMCash(N_WITHDRAUAL);

    //  TOTAL ACCOUNTS
    int accountCount = 0;
    char line[200];
    while (fgets(line, sizeof(line), accounts) != NULL)
    {
        accountCount++;
    }
    printf("----------------------------------------\n");
    printf("\nTotal Registered Accounts: %d\n", accountCount);
    printf("----------------------------------------\n");

    //  TOTAL AUDIT LOG ENTRIES
    int logCount = 0;
    while (fgets(line, sizeof(line), temp_aduit_file) != NULL)
    {
        logCount++;
    }
    printf("Total Audit Log Entries: %d\n", logCount);
    printf("----------------------------------------\n");

    // FINISH
    printf("\nATM STATUS: ACTIVE\n");
    printf("\n===== END OF REPORT =====\n\n");

    fclose(accounts);
    fclose(temp_aduit_file);
}
/*------------------------------------------------------------------------------------------------------------------------*/
/* Private function used in side public function */
void Write_Account_File(FILE *accounts, S_account newAccount)
{
    fprintf(accounts, "%s,%s,%s,%.3lf,%.3lf,%d\n", newAccount.name, newAccount.cardNumber,
            newAccount.pin, newAccount.initialBalance, newAccount.dailyLimit, newAccount.freezeStatus);
}

int Read_Account_File(FILE *accounts, S_account *account)
{
    if (fscanf(accounts, " %49[^,],%19[^,],%4[^,],%lf,%lf,%d", account->name, account->cardNumber,
               account->pin, &account->initialBalance, &account->dailyLimit, &account->freezeStatus) == 6)
    {
        return 6;
    }
    return 0;
}

/*
  function inside create account to take data from admin
 */
void TakeAccountData(S_account *data)
{
    printf("Enter the data of this account: \n");
    printf("Enter the Name : ");
    scanf(" %[^\n]s", data->name);
    printf("Enter the card number: ");
    scanf(" %[^\n]s", data->cardNumber);
    printf("Enter the PIN number: ");
    scanf(" %[^\n]s", data->pin);
    printf("Enter the initial balance: ");
    scanf("%lf", &data->initialBalance);
    printf("Enter the daily limit: ");
    scanf("%lf", &data->dailyLimit);
    printf("Enter the Account status: ");
    scanf("%d", &data->freezeStatus);
}

void TakeLoadCash(S_denom_count *data)
{
    printf("Enter the denomination: ");
    scanf("%d", &data->denomination);
    printf("Enter the count: ");
    scanf(" %d", &data->count);
}

void print(S_account *account)
{
    printf("---------------------------\n");
    printf(" the data of account are: \n");
    printf("the Name is : %s \n", account->name);
    printf("the card number  is : %s \n", account->cardNumber);
    printf("the PIN number  is : %s \n", account->pin);
    printf("the initial balance is : %.3lf \n", account->initialBalance);
    printf("the daily limit is : %.3lf \n", account->dailyLimit);
    printf("the Account Status is : %d \n", account->freezeStatus);
}
