#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "../Inc/stm32f4xx.h"
#include "../Inc/memoryMapping.h"

GPIO_TypeDef* gpioa = GPIOA;
USART_TypeDef* usart = USART1;

void USART_Init(){
	// alternate function = 10 (pins: PA9, PA10)
	SET_BIT(gpioa->MODER, 0x00280000);
	// alternate function 7 (AF7 = USART_1..3) for PA9 and PA10
	SET_BIT(gpioa->AFR[1], 0x00000770);		

/**
	CR1 - control register

	OVE8 = 0 		(OVERSAMPLING = 16)
	UE = 1 			(UART ENABLE = 1)
	TE = 1			(TRANSMITTER ENABLE = 1)
	PCE = 0 		(PARITY CONTROL ENABLE = 0)
	M = 0			(8 DATA BITS)
  	
  	0010 0000 0000 1000	= 0x2008	
**/
	SET_BIT(usart->CR1, 0x2008);
        
        // baud rate
        WRITE_REG(usart->BRR, 0x8B);	
  	//WRITE_REG(usart->BRR, 0x683); 		
}


void sendToUART(char *buffer){
	while(*buffer!='\r'){
		while(!(READ_BIT(usart->SR,USART_SR_TXE))); 		// wait until TXE != 0
   		WRITE_REG(usart->DR, *buffer & 0xFF); 		        // send char
		buffer++; 						// for each iteration *(ch+1)
		while (!(READ_BIT(usart->SR, USART_SR_TC))); 	        // wait TC to be 1
	}
	while(!(READ_BIT(usart->SR,USART_SR_TXE)));
	WRITE_REG(usart->DR, '\n' & 0xFF);
	while (!(READ_BIT(usart->SR, USART_SR_TC)));
}