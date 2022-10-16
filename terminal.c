

#define _CRT_SECURE_NO_WARNINGS


#include<stdio.h>
#include<string.h>
#include"card.h"
#include"terminal.h"
#include<time.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>






EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
	uint32_t day, month, year;
	uint8_t tempYear[4], tempMonth[2], tempDay[2];
	uint32_t temp;
	time_t t;
	time(&t);
	struct tm* local = localtime(&t);
	day = local->tm_mday;
	month = local->tm_mon + 1;
	year = local->tm_year + 1900;
	printf("Enter the transation Date like DD/MM/YYYY \n");
	scanf("%s", &termData->transactionDate);	
	if ((strlen(termData->transactionDate) != 10) )
	{
		return  WRONG_DATE;
	}
	else
	{
		return TERMINAL_OK;
	}

}

EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData)
{
	uint8_t tempTerminalYear[2], tempTerminalMonth[2];
	uint8_t tempCardYear[2], tempCardMonth[2];    


	tempTerminalYear[0] = termData.transactionDate[8];
	tempTerminalYear[1] = termData.transactionDate[9];
	tempCardYear[0] = cardData.cardExpirationDate[3];
	tempCardYear[1] = cardData.cardExpirationDate[4];

	if (atoi(tempTerminalYear) > atoi(tempCardYear)) {
		return EXPIRED_CARD;
	}
		
	else if (atoi(tempTerminalYear) == atoi(tempCardYear))
	{
		tempTerminalMonth[0] = termData.transactionDate[3];
		tempTerminalMonth[1] = termData.transactionDate[4];
		tempCardMonth[0] = cardData.cardExpirationDate[0];
		tempCardMonth[1] = cardData.cardExpirationDate[1];

		if (atoi(tempTerminalMonth) > atoi(tempCardMonth)) {
			return   EXPIRED_CARD;
		}
		else {
			return TERMINAL_OK;
		}
	}

	else {
		return TERMINAL_OK;
	}
            
}


EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData)
{
	uint8_t value = 0, result = 0;
	bool flag = false;

	for (char i = strlen(cardData->primaryAccountNumber) - 1; i >= 0; i--)

	{

		value = cardData->primaryAccountNumber[i] - '0';

		if (flag == true)
			value = value * 2;
		result += value / 10;
		result += value % 10;
		flag = !flag;
	}
	if (result % 10 != 0)
		return INVALID_CARD;
	return TERMINAL_OK;

}


		EN_terminalError_t getTransactionAmount(ST_terminalData_t * termData)
		{

			printf("Enter Your Transaction Amount\n");
			scanf("%f", &termData->transAmount);
			if ((termData->transAmount <= 0))
				return INVALID_AMOUNT;

			return TERMINAL_OK;
		}


		EN_terminalError_t isBelowMaxAmount(ST_terminalData_t * termData)
		{

			if ((termData->transAmount) > (termData->maxTransAmount))
			{
				return EXCEED_MAX_AMOUNT;
			}
			else {
				return TERMINAL_OK;
			}


		}
		EN_terminalError_t setMaxAmount(ST_terminalData_t * termData)
		{

			termData->maxTransAmount = 15000.00;

			if ((termData->maxTransAmount <= 0))

			{
				return  INVALID_MAX_AMOUNT;
			}
			else {
				return TERMINAL_OK;
			}


		}
	