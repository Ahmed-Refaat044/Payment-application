







#include<stdio.h>
#include<string.h>
#include"card.h"
#include"terminal.h"
#include <stdlib.h>
#include"server.h"

ST_accountsDB_t accountsDataBase[255] = { {67050.00,"4485456770455996"},{86000.00, "4716069021758224"},{10000.00, "4539207880267326"}, {10000.00,"4916945627505481"} };//valid pan







ST_transaction_t transactionDataBase[255] = { 0 };


uint8_t Member ;

uint8_t SearchMember(ST_cardData_t* cardData)
{
    for(uint8_t i =0 ; i <255 ; i++)
	{
		if (strcmp(cardData->primaryAccountNumber, accountsDataBase[i].primaryAccountNumber) == 0)
			return i;
	}

	return -1;
}

EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
	EN_transState_t transLocalError = isValidAccount(&(transData->cardHolderData));

	if (transLocalError == DECLINED_STOLEN_CARD)
	{
		transData->transState = DECLINED_STOLEN_CARD;
		return DECLINED_STOLEN_CARD;

	}

	EN_serverError_t serverLocalError = isAmountAvailable(&(transData->terminalData));

	if (serverLocalError == LOW_BALANCE)
	{
		transData->transState = DECLINED_INSUFFECIENT_FUND;
		return DECLINED_INSUFFECIENT_FUND;
	}
	accountsDataBase[Member].balance -= transData->terminalData.transAmount;

	
	serverLocalError = saveTransaction(transData);
	if (serverLocalError == SAVING_FAILED)
	{
		transData->transState = INTERNAL_SERVER_ERROR;
		return INTERNAL_SERVER_ERROR;
	}


	return SERVER_OK;
}

EN_transState_t isValidAccount(ST_cardData_t* cardData)
{
	if (Member == 255)
		return DECLINED_STOLEN_CARD;
	else
		return SERVER_OK;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t* termData)
{
	if ((termData->transAmount) > (accountsDataBase[Member].balance))
		return LOW_BALANCE;

	return SERVER_OK;
}

EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
	static uint8_t iterator = 0;

	transactionDataBase[Member].cardHolderData = transData->cardHolderData;
	transactionDataBase[Member].terminalData = transData->terminalData;
	transactionDataBase[Member].transState = transData->transState;

	transData->transactionSequenceNumber = rand();
	transactionDataBase[Member].transactionSequenceNumber = transData->transactionSequenceNumber;

	if (transactionDataBase[Member].transactionSequenceNumber == 0)
	{
		transData->transState = SAVING_FAILED;
		return SAVING_FAILED;
	}
	else if (iterator > 254)
	{
		transData->transState = SAVING_FAILED;
		return SAVING_FAILED;
	}

	iterator++;

	return SERVER_OK;
}

EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t* transData)
{

	if (Member == 255)
		return ACCOUNT_NOT_FOUND;


	if (transactionDataBase[Member].transactionSequenceNumber != transactionSequenceNumber)
	{
		transData->transState = TRANSACTION_NOT_FOUND;
		return TRANSACTION_NOT_FOUND;
	}


	return SERVER_OK;
}



	








