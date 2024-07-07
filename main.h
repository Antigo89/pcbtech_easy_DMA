#ifndef H_MAIN
#define H_MAIN 1

#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx.h"

#define USART_BOUDRATE 115200

void RCC_Init(void);
void DMA2_Stream0_Init(void);
void DMA2_Stream7_Init(void);
void USART1_Init(void);

#endif