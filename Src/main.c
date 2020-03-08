#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "../Inc/stm32f4xx.h"
#include "../Inc/memoryMapping.h"


/** 
PF_0 	CLK
PF_1	CS
PF_2	SI/O
**/

void RCC_Init();

int intTemperature = 0;
int decimalTemperature = 0;
char buff[20];



 void main(){
	RCC_Init();
	SPI_Init();
	USART_Init();
	while(1){
		spiData(&intTemperature, &decimalTemperature);
		sprintf(buff,"temp: %d.%04d C\r", intTemperature, decimalTemperature);
		sendToUART(buff);
                //printf("temp: %d.%04d C\n", intTemperature, decimalTemperature);
	}
}

void RCC_Init(){
	RCC_TypeDef* rcc = RCC;

	SET_BIT(rcc->AHB1ENR, RCC_AHB1ENR_GPIOFEN);	// ura SPI
    SET_BIT(rcc->AHB1ENR, RCC_AHB1ENR_GPIOAEN);		// ura UART_GPIO
    SET_BIT(rcc->APB2ENR, RCC_APB2ENR_USART1EN);	// ura UART
}


