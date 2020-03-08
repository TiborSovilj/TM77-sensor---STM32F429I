#pragma once

#ifndef MEMORY_MAPPING
#define MEMORY_MAPPING

void SPI_Init();
void spiData(int*, int*);

void USART_Init();
void sendToUART(char*);

#endif