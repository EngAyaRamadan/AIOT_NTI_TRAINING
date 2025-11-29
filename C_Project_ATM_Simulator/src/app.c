/*
 * Name : Aya Ramadan
 * Date : 28/11/2025
 * Project Name : ATM Simulator
 */
#include "../include/app.h"

void ATM(void)
{
    int mode;
    printf("----------------------Welcome to The ATM------------------ \n");
    printf(">>>>>>>>>>>>> Enter the mode <<<<<<<<<<<<<<<< \n");
    printf("if you want to      \"Admin Mode\"      Enter number <1> \n");
    printf("if you want to     \"Customer Mode\"    Enter number <2>\n ");
    printf("Enter the mode number : ");
    scanf("%d", &mode);
    switch (mode)
    {
    case 1:
    {
        Admain_minue();
    }
    break;
    case 2:
    {
        Customer_minue();
    }
    break;
    default:
    {
        printf("there is No Mode for this number\n");
        ATM();
    }
    break;
    }
    return;
}

void Admain_minue(void)
{
    int op = 0;
    printf("----------------------Welcome to The Admin mode------------------ \n");
    printf(">>>>> choose the Operation you want to do <<<<<<<<\n");
    printf("if you want to      \"Create Account\"      Enter number <1> \n");
    printf("if you want to      \"List Accounts\"       Enter number <2> \n");
    printf("if you want to      \"Search Account\"      Enter number <3> \n");
    printf("if you want to      \"Load ATM Cash\"       Enter number <4> \n");
    printf("if you want to      \"View ATM Cash\"       Enter number <5> \n");
    printf("if you want to     \"Freeze_Unfreeze\"      Enter number <6> \n");
    printf("if you want to        \"ResetPin\"          Enter number <7> \n");
    printf("if you want to      \"ViewAuditLog \"       Enter number <8> \n");
    printf("if you want to      \"SimpleReports\"       Enter number <9> \n");
    printf("if you want to   \"out from Admin mode\"    Enter number <10>\n");
    printf("           --------------------------------------- \n");
    printf("Enter the number of operation: ");
    scanf("%d", &op);
    Admain_Operation(op);
    Admain_minue();
    return;
}
void Customer_minue(void)
{
    int op = 0;
    printf("----------------------Welcome to The Customer mode------------------ \n");
    printf(">>>>> choose the Operation you want to do <<<<<<<<\n");
    printf("if you want to       \"LogIn\"             Enter number <1> \n");
    printf("if you want to    \"Balance Inquiry\"      Enter number <2> \n");
    printf("if you want to    \"Cash Withdrawal\"      Enter number <3> \n");
    printf("if you want to    \"Cash Deposit\"         Enter number <4> \n");
    printf("if you want to    \"Fund Transfer\"        Enter number <5> \n");
    printf("if you want to    \"miniStatement\"        Enter number <6> \n");
    printf("if you want to      \"ChangePin\"          Enter number <7> \n");
    printf("if you want to        \"LogOut\"           Enter number <8> \n");
    printf("if you want to \"out from Customer mode\"  Enter number <9> \n");
    printf("           --------------------------------------- \n");
    printf("Enter the number of operation: ");
    scanf("%d", &op);
    Customer_Operation(op);
    Customer_minue();
    return;
}

void Admain_Operation(int op)
{
    switch (op)
    {
    case 1:
    {
        printf("--------Welcome to The Create Account operation-------- \n");
        CreateAccount();
    }
    break;
    case 2:
    {
        printf("--------Welcome to The List Account operation-------- \n");
        ListAccounts();
    }
    break;
    case 3:
    {
        printf("--------Welcome to The Search Account operation-------- \n");
        char cardNumber[20];
        printf("Enter the card number : ");
        scanf("%s", cardNumber);
        SearchAccount(cardNumber, ADMIN, N_OUTBALANCE, N_TRANSFER);
    }
    break;
    case 4:
    {
        printf("--------Welcome to The Load ATM Cash  operation-------- \n");
        LoadATMCash();
    }
    break;
    case 5:
    {
        printf("--------Welcome to The  View ATM Cash  operation-------- \n");
        ViewATMCash(N_WITHDRAUAL);
    }
    break;
    case 6:
    {
        printf("--------Welcome to The Freeze_Unfreeze operation-------- \n");
        Freeze_Unfreeze_ResetPin(FREEZE_UNFEEZE);
    }
    break;
    case 7:
    {
        printf("--------Welcome to The Reset Pin operation-------- \n");
        Freeze_Unfreeze_ResetPin(RESET_PIN);
    }
    break;
    case 8:
    {
        printf("--------Welcome to The View Audit Log  operation-------- \n");
        ViewAuditLog();
    }
    break;
    case 9:
    {
        printf("--------Welcome to The Simple Reports operation-------- \n");
        SimpleReports();
    }
    break;
    case 10:
    {
        printf("--------Welcome to The Log Out operation-------- \n");
        ATM();
    }
    break;
    default:
    {
        printf("--------No Operation for this number-------- \n");
    }
    break;
    }
    return;
}
void Customer_Operation(int op)
{
    switch (op)
    {
    case 1:
    {
        printf("--------Welcome to The Log In operation-------- \n");
        LogIn();
    }
    break;
    case 2:
    {
        printf("--------Welcome to The Balance Inquiry operation-------- \n");
        BalanceInquiry();
    }
    break;
    case 3:
    {
        printf("--------Welcome to The Cash Withdrawal operation-------- \n");
        CashWithdrawal();
    }
    break;
    case 4:
    {
        printf("--------Welcome to The Cash Deposit operation-------- \n");
        CashDeposit();
    }
    break;
    case 5:
    {
        printf("--------Welcome to The Fund Transfer operation-------- \n");
        FundTransfer();
    }
    break;
    case 6:
    {
        printf("--------Welcome to The miniStatement operation-------- \n");
        char cardNumber[20];
        printf("Enter the card number : ");
        scanf("%s", cardNumber);
        miniStatement(cardNumber);
    }
    break;
    case 7:
    {
        printf("--------Welcome to The Change Pin operation-------- \n");
        ChangePin();
    }
    break;
    case 8:
    {
        printf("--------Welcome to The Log Out from the account operation-------- \n");
        ATM();
    }
    case 9:
    {
        printf("--------Welcome to The Out mode operation-------- \n");
        ATM();
    }
    break;
    default:
    {
        printf("--------No Operation for this number-------- \n");
    }
    break;
    }
    return;
}