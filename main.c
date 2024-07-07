/*
File    : main.c
Software "Kurs STM32 PCBtech"
Lesson 9: DMA Memory to USART.
Student: antigo1989@gmail.com
*/

#include "main.h"

char bufferOUT[15] __attribute__((section(".fast"))) = {"USART-DMA OK!\r\n"};
char bufferIN[15] __attribute__((section(".fast")));


/*********************************main************************************/
int main(void) {
  //Values initial

  //System Initial
  SystemInit();
  RCC_Init();
  __enable_irq();
  DMA2_Stream0_Init();
  USART1_Init();
  DMA2_Stream7_Init();
  
  while(1){
    __NOP();
  }
}

/****************************** function**********************************/
void DMA2_Stream0_Init(void){
  RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
  DMA2_Stream0->PAR = (uint32_t)bufferOUT;
  DMA2_Stream0->M0AR = (uint32_t)bufferIN;
  DMA2_Stream0->NDTR = 15; //length(bufferOUT)
  DMA2_Stream0->FCR |= DMA_SxFCR_DMDIS|(0b11<<DMA_SxFCR_FTH_Pos); // FIFO full 
  DMA2_Stream0->CR &= ~(DMA_SxCR_CHSEL); //clear channel  
  DMA2_Stream0->CR &= ~(DMA_SxCR_MBURST|DMA_SxCR_PBURST|DMA_SxCR_DBM);
  DMA2_Stream0->CR |= DMA_SxCR_PL|DMA_SxCR_MBURST_1|DMA_SxCR_PBURST_1; //priorety very hight, 2 burst
  DMA2_Stream0->CR &= ~(DMA_SxCR_MSIZE|DMA_SxCR_PSIZE); //8bit (char)
  DMA2_Stream0->CR &= ~(DMA_SxCR_CIRC|DMA_SxCR_DIR);
  DMA2_Stream0->CR |= DMA_SxCR_PINC|DMA_SxCR_MINC|DMA_SxCR_DIR_1; //per/mem(inc++) > mem(inc++) 
  //start transfer
  DMA2_Stream0->CR |= DMA_SxCR_EN;
}

void DMA2_Stream7_Init(void){
  RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
  DMA2_Stream7->PAR = (uint32_t)&(USART1->DR);
  DMA2_Stream7->M0AR = (uint32_t)bufferIN;
  DMA2_Stream7->NDTR = 15; //length(bufferOUT)
  DMA2_Stream7->FCR &= ~(DMA_SxFCR_DMDIS); //ohne FIFO
  DMA2_Stream7->CR &= ~(DMA_SxCR_CHSEL); //clear channel
  DMA2_Stream7->CR |= (4<<DMA_SxCR_CHSEL_Pos)|DMA_SxCR_PL_1; //channel 4 USART1_Tx, priorety hight
  DMA2_Stream7->CR &= ~(DMA_SxCR_MBURST|DMA_SxCR_PBURST|DMA_SxCR_DBM);
  DMA2_Stream7->CR &= ~(DMA_SxCR_MSIZE|DMA_SxCR_PSIZE|DMA_SxCR_PINC); //8bit (char), per(inc=const)
  DMA2_Stream7->CR &= ~(DMA_SxCR_CIRC|DMA_SxCR_DIR);
  DMA2_Stream7->CR |= DMA_SxCR_MINC|DMA_SxCR_DIR_0; //mem(inc++) > per
  //start transfer
  USART1->SR &= ~(USART_SR_TC);
  DMA2_Stream7->CR |= DMA_SxCR_EN;
}

void USART1_Init(void){
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
  //PA9 and PA10
  GPIOA->MODER |= GPIO_MODER_MODE9_1|GPIO_MODER_MODE10_1;
  GPIOA->AFR[1] |= (7<<GPIO_AFRH_AFSEL9_Pos)|(7<<GPIO_AFRH_AFSEL10_Pos);
  // 84MHz / 115200bod / 16 = 45,57  M=45 (0x2D) F=0,57*16=9 (0x09)
  USART1->BRR = 0x02D9;
  USART1->CR1 |= USART_CR1_TE;
  USART1->CR1 &= ~(USART_CR1_M|USART_CR1_PCE);
  USART1->CR2 &= ~(USART_CR2_STOP);
  USART1->CR3 |= USART_CR3_DMAT;
  //Enable USART1
  USART1->CR1 |= USART_CR1_UE;
  //USART1->DR = '\n';
}

/*************************** End of file ****************************/
