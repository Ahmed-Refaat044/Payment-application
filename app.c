


#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>


#include"card.h"
#include"terminal.h"
#include"server.h"

#include "app.h"
extern ST_accountsDB_t accountsDataBase[255];
extern uint8_t Member;

void main() {
	appStart();
}

void appStart(void)
{
	double balance;
	uint8_t choose;
	uint32_t number;
	EN_cardError_t carderror;
	EN_terminalError_t terminalerror;
	EN_transState_t transstateerror;
	EN_serverError_t servererror;

	float	deposit;

	ST_transaction_t userdata = { {"" ,"", ""} ,{0,0,""} , APPROVED , 0 };
	printf("welcome to FWD BANK \n\n\n");


	while (true) {
		carderror = getCardHolderName(&userdata.cardHolderData);
		if (carderror == WRONG_NAME)
		{
			printf("The name is less than 20 characters or greater than 24. \n");
			printf(" Enter acceptable name \n");

			continue;
		}

		carderror = getCardExpiryDate(&userdata.cardHolderData);
		if (carderror == WRONG_EXP_DATE)
		{
			printf("Wrong Expiration DATE");
			break;
		}


		carderror = getCardPAN(&userdata.cardHolderData);
		if (carderror == WRONG_PAN) {
			printf("Wrong Primary Account Number ");
			break;
		}

		terminalerror = isValidCardPAN(&userdata.cardHolderData);
		if (terminalerror == INVALID_CARD) {
			printf("Invalid Card PAN . \n");
			break;
		} 

		if (&userdata.transState == DECLINED_STOLEN_CARD) {
			printf("Declined Invalid Account or Blocked Account!\n");
			break;
		}
		printf("Welcom ,%s \n\n", userdata.cardHolderData.cardHolderName);
		while (true)
		{
			Member = SearchMember(&userdata.cardHolderData);

				printf("What Do you  Want To Do ?\n ");
			
				printf("1: Make new Transaction.\n");
				printf("2: Check Transaction.\n");
				printf("3: Check Balance.\n");
				printf("4: Deposit money.\n");
				printf("5: Report Stolen Card.\n"); 
				printf("6: Exit.\n"); fflush(stdout);
		scanf("%d",&choose);
				if (choose == 1)
				{
					if (userdata.transState == DECLINED_STOLEN_CARD)
					{
						printf("Declined Invalid Account or Blocked Account!\n\n");
					}
					else {
						terminalerror = getTransactionDate(&userdata.terminalData);
						if (terminalerror == WRONG_DATE)
						{
							printf("Transaction Declined.\n");
							break;
						}

						if (isCardExpired(userdata.cardHolderData, userdata.terminalData))
						{
							printf("Declined Expired Card!\n");
							printf("Exitiiiing\n");
							break;
						}
						else {
							printf("Accepted Card.\nwelcome!\n\n");
						}

						setMaxAmount(&userdata.terminalData);
						if (terminalerror == INVALID_MAX_AMOUNT)
						{
							printf("INVALID MAX AMOUNT.\n");
							break;
						}
						terminalerror = getTransactionAmount(&userdata.terminalData);
						if (terminalerror == INVALID_AMOUNT)
						{
							printf("INVALID AMOUNT.\n");
							break;
						}

						if (isBelowMaxAmount(&userdata.terminalData))
						{
							printf("Declined Amount Exceeding Limit!\n");
							printf("Exitiiiing\n");
							break;
						}

						transstateerror = recieveTransactionData(&userdata);

						if (transstateerror == DECLINED_STOLEN_CARD)
						{
							printf("Declined Invalid Account or Blocked Account!\n");
							break;
						}
						else if (transstateerror == DECLINED_INSUFFECIENT_FUND)
						{
							printf("Declined Insufficient Funds!\n");
							break;
						}
						else if (transstateerror == INTERNAL_SERVER_ERROR)
						{
							printf("Server is down, please try again later!\n");
							break;
						}


		balance = accountsDataBase[Member].balance + userdata.terminalData.transAmount;
						printf("Your curent balance was %0.4f.\n", balance);
						printf("Amount to be withdrawn = %0.4f.\n", userdata.terminalData.transAmount);
						printf("Current Balance = %0.4f.\n", accountsDataBase[Member].balance);
						printf("Transaction Number: %d\n", userdata.transactionSequenceNumber);
						printf("Transaction Completed.\n");
					}
				}
				else if (choose == 2)
				{
					transstateerror = isValidAccount(&userdata.cardHolderData);
					if (transstateerror == DECLINED_STOLEN_CARD)
					{
						printf("Declined Invalid Account or Blocked Account!\n");
						break;
					}
					else {
						if (userdata.transState == DECLINED_STOLEN_CARD)
						{
							printf("Declined Invalid Account or Blocked Account!\n\n");
						}
						else {
							printf("Enetr Transaction sequence number to be searched:  "); fflush(stdout);
							scanf_s("%d", &number);
							servererror = getTransaction(number, &userdata);
							if (servererror == ACCOUNT_NOT_FOUND)
							{
								printf("Declined Invalid Account!\n");
								break;
							}
							else if (servererror == TRANSACTION_NOT_FOUND)
							{
								printf("Declined TRANSACTION NOT FOUND!\n");
								break;
							}
							printf("Transaction sequence number: %d\n", userdata.transactionSequenceNumber);
							printf("Details of the Transaction owner.\n");
							printf("Name: %s.\n", userdata.cardHolderData.cardHolderName);
							printf("PAN: %s.\n", userdata.cardHolderData.primaryAccountNumber);
							printf("Transaction Date: %s.\n", userdata.terminalData.transactionDate);
							printf("Transaction money = %0.4f.\n", userdata.terminalData.transAmount);
							printf("Current Balance = %0.4f.\n", accountsDataBase[Member].balance);
						}
					}
				}
				else if (choose == 3)
				{
					transstateerror = isValidAccount(&userdata.cardHolderData);
					if (transstateerror == DECLINED_STOLEN_CARD)
					{
						printf("Declined Invalid Account or Blocked Account!\n");
						break;
					}
					else {
						if (userdata.transState == DECLINED_STOLEN_CARD)
						{
							printf("Declined Invalid Account or Blocked Account!\n\n");
						}
						else {
							balance = accountsDataBase[Member].balance;
							printf("Your curent balance is %0.4f.\n", &balance);
						}
					}
				}
				else if (choose == 4)
				{
					transstateerror = isValidAccount(&userdata.cardHolderData);
					if (transstateerror == DECLINED_STOLEN_CARD)
					{
						printf("Declined Invalid Account or Blocked Account!\n");
						break;
					}
					else {
						if (userdata.transState == DECLINED_STOLEN_CARD)
						{
							printf("Declined Invalid Account or Blocked Account!\n\n");
						}
						else {
							printf("Enter the amount of money: "); fflush(stdout);
							scanf_s("%f", &deposit);
							accountsDataBase[Member].balance += deposit;
							printf("Current Balance = %0.4f.\n", accountsDataBase[Member].balance);
							printf("Amount added successfully.\n");
						}
					}
				}
				else if (choose == 5)
				{
					transstateerror = isValidAccount(&userdata.cardHolderData);
					if (transstateerror == DECLINED_STOLEN_CARD)
					{
						printf("Declined Invalid Account or Blocked Account!\n");
						break;
					}
					else {
						if (userdata.transState == DECLINED_STOLEN_CARD)
						{
							printf("Declined Invalid Account or Blocked Account!\n\n");
						}
						else {
							userdata.transState = DECLINED_STOLEN_CARD;
							printf("Reported successfully.\n\n");
						}
					}

				}
				else if (choose == 6)
				{
					printf("Shutting Down.\n");
					break; 
				}
				else {
					printf("Wrong answer.\n");
					break;
				}

		}
		break;

	}

}
				





















	







