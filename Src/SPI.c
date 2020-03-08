#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "../Inc/stm32f4xx.h"
#include "../Inc/memoryMapping.h"

int16_t readBuffer = 0;

void readStart();
void dataRead();
void dataConversion(int*, int*);
void clockCycle();

GPIO_TypeDef* gpiof = GPIOF;

/** 

PF_0 	CLK
PF_1	CS
PF_2	SI/O

**/



void SPI_Init(){
	SET_BIT (gpiof->MODER, 0x00000005);
}

void spiData(int* intPart, int* decimalPart){
	readStart();
	dataRead();
	dataConversion(intPart, decimalPart);
}

void readStart(){
	// PF1 = 1
	// PF0 = 0
	SET_BIT(gpiof->ODR, 0x0002);
	for (int i=0; i<100; i++); 

	// PF1 = 0
 	CLEAR_BIT (gpiof->ODR, 0x0002); 
}

void dataRead(){
	for (int i = 0; i < 16; i++){
		clockCycle();
		readBuffer = (readBuffer << 1) | (READ_BIT(gpiof->IDR, 0x0004) >> 2);
                //printf("stanje: %x\n", readBuffer);
                
  	}
  	//printf("HEX: %x\n", readBuffer);
  	SET_BIT(gpiof->ODR, 0x0002);
  	CLEAR_BIT(gpiof->ODR, 0x0001);
}

void dataConversion(int* intPart, int* decimalPart){
	if((readBuffer & 0x8000) != 0){
                 readBuffer = (readBuffer >> 3);
		readBuffer =  ~(readBuffer) + 1;
		*intPart = 	((readBuffer >> 4) & 0xFFF) * (-1);	// cijeli dio iz readBuffera
		*decimalPart = 	(readBuffer & 0x000F) * 625;	
                    // decimalni dio iz readBuffera
	}else{
		*intPart = 	((readBuffer >> 7) & 0xFFF);		// cijeli dio iz readBuffera
		*decimalPart = 	((readBuffer >> 3) & 0x000F) * 625;		// decimalni dio iz readBuffera
	}
}

void clockCycle(){
	// PF0 = 0/1    (CLK)
	CLEAR_BIT (gpiof->ODR, 0x0001); 	
	for (int i = 0; i < 100; i++);
	SET_BIT (gpiof->ODR, 0x0001); 		
	for (int i = 0; i < 100; i++);
}