

#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<string.h>
#include <ctype.h>
#include"card.h"


   
    EN_cardError_t getCardHolderName(ST_cardData_t * cardData)

    {
     
        
            printf("Please, enter your name:  \n ");
            gets_s(cardData->cardHolderName, 25);


            if (strlen(cardData->cardHolderName) < 20 || strlen(cardData->cardHolderName) > 24 || strlen(cardData->cardHolderName) == NULL)
            {
                return  WRONG_NAME ;

            }
            else {
                return CARD_OK;
            }
    }
   

    EN_cardError_t getCardPAN(ST_cardData_t * cardData)
    {
        printf("enter your CardPAN ??\n");     
        scanf("%s", cardData->primaryAccountNumber);



        if (strlen(cardData->primaryAccountNumber) < 16 || strlen(cardData->primaryAccountNumber) > 19)
        {
            return  WRONG_PAN;

        }
        else {
            return  CARD_OK;
        }
    }

    EN_cardError_t getCardExpiryDate(ST_cardData_t * cardData)
    {
        printf("enter your CardExpiryDate as MM/YY ?\n");  
        scanf("%s" , cardData->cardExpirationDate); 
        if (strlen(cardData->cardExpirationDate) != 5) {
            return  WRONG_EXP_DATE;
            
        }

        else { return  CARD_OK; }
    }




   