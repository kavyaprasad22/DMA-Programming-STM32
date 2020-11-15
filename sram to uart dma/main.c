#include <stdint.h>
#include <stdio.h>
#include "stm32f446xx.h"
void button(void);
void uart2(void);
void dma_inititialization(void);
void DMA1_interrupt(void);


void HalfTransfer_callback(void);
void FullTransfer_callback(void);
void TransferError_callback(void);
void DME_callback(void);
void FIFO_error_callback(void);

static char data_stream[] ="hello World\r\n";

#define DEST_ADDR   (SRAM1_BASE+0x800)

int main()
{
  button();
	uart2();
  dma_inititialization();
	DMA1_interrupt();
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

void uart2(void)
{
	RCC_TypeDef *pRCC;
	pRCC=RCC;
	//Enable clock for UART and GPIOA//
	pRCC->APB1ENR|=(1<<17);
	pRCC->AHB1ENR|=(1<<0);
	GPIO_TypeDef *pGPIOA;
	pGPIOA=GPIOA;
	
	//Configuring PA3 and PA2 as UART Transmit and recieve pins//
	pGPIOA->MODER&=~(uint32_t)(0x3<<4);
	pGPIOA->MODER|=(0x2<<4);
	pGPIOA->AFR[0]&=~(uint32_t)(0xF<<8);
	pGPIOA->AFR[0]|=(0x7<<8);
	pGPIOA->PUPDR|=(0x1<<4);
	
	pGPIOA->MODER&=~(uint32_t)(0x3<<6);
	pGPIOA->MODER|=(0x2<<6);
	pGPIOA->AFR[0]&=~(uint32_t)(0xF<<12);
	pGPIOA->AFR[0]|=(0x7<<12);
	pGPIOA->PUPDR|=(0x1<<6);
	
	// UART Initializations- Enable transmit engine//
	USART_TypeDef *pUART2;
	pUART2=USART2;
	pUART2->BRR=0x8B;
	pUART2->CR1|=(1<<3);
	pUART2->CR1|=(1<<13);
	
	
	
	
}
void dma_inititialization(void)
{
	RCC_TypeDef *pRCC;
	pRCC = RCC;
	
	DMA_TypeDef *pDMA;
	pDMA=DMA1;
	//Selecting stream 6//
	DMA_Stream_TypeDef *pSTREAM6;
	pSTREAM6 = DMA1_Stream6;
	
	USART_TypeDef *pUART2;
	pUART2 = USART2;
	
	
	//Enable Clock for DMA1
	pRCC->AHB1ENR |= ( 1 << 21);

	//Selecting channel 4//
	pSTREAM6->CR &= ~(uint32_t)( 0x7 << 25);
	pSTREAM6->CR |=  ( 0x4 << 25);
	
	//Memory Address//
	pSTREAM6->M0AR = (uint32_t) data_stream;
	
	//Peripheral Address//
	pSTREAM6->PAR = (uint32_t) &pUART2->DR;
	
	//Data length//
	uint32_t len = sizeof(data_stream);
	pSTREAM6->NDTR = len;
	
	//Direction of data transfer//
	pSTREAM6->CR |= (0x1 << 6);
	
	//Source and destination data width-Byte//  
	pSTREAM6->CR &= ~(uint32_t)(0x3 << 13); 
	pSTREAM6->CR &= ~(uint32_t)(0x3 << 11);
	
	//Auto increment for memory//
	pSTREAM6->CR |= ( 1 << 10);
	
	//FIFO mode enabled//
	pSTREAM6->FCR |= ( 1 << 2);
	
	//Select FIFO threshold//
	pSTREAM6->FCR &= ~(uint32_t)(0x3 << 0); //clearing 
	pSTREAM6->FCR |=  (0x3 << 0); //setting
	
  //All other configuartions left at default value//
	
	//Enable DMA//
	pSTREAM6->CR|=(1<<0);

	
}
void DMA1_interrupt(void)
{
	DMA_Stream_TypeDef *pSTREAM6;
	pSTREAM6 = DMA1_Stream6;
//Half transfer complete interrupt//		
	pSTREAM6->CR|=(1<<3);
	//Full tansfer complete interrupt//
	pSTREAM6->CR|=(1<<4);
		//FIFO Error interrupt//
		pSTREAM6->FCR |= (1 << 7);
  //DME Error//
	pSTREAM6->CR |= (1 << 1);
	
	//Enable the IRQ for DMA1 stream6 interrupt in NVIC//
	NVIC_EnableIRQ(DMA1_Stream6_IRQn);
}
void HalfTransfer_callback(void)
{

}

void FullTransfer_callback(void)
{
	USART_TypeDef *pUART2;
	pUART2=USART2;
	DMA_Stream_TypeDef *pSTREAM6;
	pSTREAM6 = DMA1_Stream6;
	//Disable DMA request from peripheral//
	pUART2->CR3 &= ~(uint32_t)( 1 << 7);
	//Reinitialize data length//
	uint32_t len = sizeof(data_stream);
	pSTREAM6->NDTR = len;
	//Enable DMA for next transfer//
	pSTREAM6->CR|=(1<<0);
		
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
