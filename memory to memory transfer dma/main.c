#include <stdint.h>
#include <stdio.h>
#include "stm32f446xx.h"
void button(void);
void uart2(void);
void dma_inititialization(void);
void DMA2_interrupt(void);


void HalfTransfer_callback(void);
void FullTransfer_callback(void);
void TransferError_callback(void);
void DME_callback(void);
void FIFO_error_callback(void);
static uint8_t GLOBAL_MESSAGE[]="hello world";
#define OFFSET 0x800
#define DEST_ADDR (SRAM1_BASE+OFFSET)

int main()
{
	
  button();
  dma_inititialization();
	DMA2_interrupt();
	while(1);
	
}

void button(void)
{
	//Button interrupt//
	GPIO_TypeDef *pGPIOC;
	pGPIOC=GPIOC;
	RCC_TypeDef *pRCC;
	pRCC=RCC;
	pRCC->AHB1ENR|=(1<<2);
  pGPIOC->MODER &=(uint32_t)~(0x3 << 26);
	EXTI_TypeDef *pEXTI;
	pEXTI=EXTI;

	pEXTI->IMR|=(1<<13);
	
	SYSCFG_TypeDef *pSYSCFG;
	pSYSCFG=SYSCFG;
	pRCC->APB2ENR|=(1<<14);
	pSYSCFG->EXTICR[3] &=(uint32_t)~(0xF<<4);
	pSYSCFG->EXTICR[3] |=(0x2<<4);
	pEXTI->FTSR|=(1<<13);
	NVIC_EnableIRQ(EXTI15_10_IRQn);
	
	
}


void dma_inititialization(void)
{
	RCC_TypeDef *pRCC;
	pRCC = RCC;
	
	DMA_TypeDef *pDMA;
	pDMA=DMA2;
	//Selecting stream 5//	
	DMA_Stream_TypeDef *pSTREAM0;
	pSTREAM0 = DMA2_Stream0;

	//Enable Clock for DMA2
	pRCC->AHB1ENR |= ( 1 << 22);


	//selecting channel 0//
	pSTREAM0->CR &= ~(uint32_t)( 0x7 << 25);
	
	
	//Memory Address-peripheral memory address//
	pSTREAM0->M0AR = (uint32_t) DEST_ADDR;
	
	//Peripheral Address- source memory address//
	pSTREAM0->PAR = (uint32_t) GLOBAL_MESSAGE;
	
	//Data length//
	uint32_t len = sizeof(GLOBAL_MESSAGE);
	pSTREAM0->NDTR = len;
	
	//Direction of data transfer//
	pSTREAM0->CR |=(2 << 6);
	
	//Source and destination data width-Byte// 
	pSTREAM0->CR &= ~(uint32_t)(0x3 << 13); 
	pSTREAM0->CR &= ~(uint32_t)(0x3 << 11);
	
	//Auto increment for memory//
	pSTREAM0->CR |= ( 1 << 10);
	//peripheral increment//
	pSTREAM0->CR |= ( 1 << 9);
	//fifo mode //
	pSTREAM0->FCR |= ( 1 << 2);
	
	//Select FIFO threshold//
	pSTREAM0->FCR &= ~(uint32_t)(0x3 << 0); //clearing 
	pSTREAM0->FCR |=  (0x3 << 0); //setting
	
	//All other configuartions left at default value//

}
void DMA2_interrupt(void)
{
	DMA_Stream_TypeDef *pSTREAM0;
	pSTREAM0 = DMA2_Stream0;	
	pSTREAM0->CR|=(1<<3);
	pSTREAM0->CR|=(1<<4);
		pSTREAM0->FCR |= (1 << 7);
	
	//5. Direct mode error (DMEIE)
	pSTREAM0->CR |= (1 << 1);
	
	//6. Enable the IRQ for DMA1 stream6 global interrupt in NVIC
	NVIC_EnableIRQ(DMA2_Stream0_IRQn);
}

void HalfTransfer_callback(void)
{

}

void FullTransfer_callback(void)
{

// No reinitializations//
	
}


void TransferError_callback(void)
{
	while(1);
}

void FIFO_error_callback(void)
{
	
	while(1);
}

void DME_callback(void)
{
	while(1);
}
