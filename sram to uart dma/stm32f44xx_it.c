#include "stm32f446xx.h"

void HalfTransfer_callback(void);
void FullTransfer_callback(void);
void TransferError_callback(void);
void DME_callback(void);
void FIFO_error_callback(void);
#define HalfTransfer() 	DMA1->HISR & ( 1 << 20)
#define FullTransfer() 	DMA1->HISR & ( 1 << 21)
#define TransferError() 	DMA1->HISR & ( 1 << 19)
#define DME() 	DMA1->HISR & ( 1 << 16)
#define FIFO_error() DMA1->HISR & ( 1 << 18)

void EXTI15_10_IRQHandler(void);
void clear_pending_bit(void);
void DMA1_Stream6_IRQHandler(void);
void clear_pending_bit(void)
{
	EXTI_TypeDef *pEXTI;
	pEXTI = EXTI;
	
	if( (EXTI->PR & (1 << 13) )  )
	{
		//Clear the Pending bit //
		EXTI->PR |= (1 << 13);
	}

	
}


void EXTI15_10_IRQHandler(void)
{
	USART_TypeDef *pUART2;
	pUART2=USART2;

	pUART2->CR3|=(1<<7);
	clear_pending_bit();
}

void DMA1_Stream6_IRQHandler(void)
{
		//Half-transfer
	if( HalfTransfer() )
	{
		DMA1->HIFCR |= ( 1 << 20);
		HalfTransfer_callback();
	}else if(FullTransfer() )
	{
		DMA1->HIFCR |= ( 1 << 21);
		FullTransfer_callback();
	}else if ( TransferError() )
	{
		DMA1->HIFCR |= ( 1 << 19);
		TransferError_callback();
		
	}else if (FIFO_error())
	{
		DMA1->HIFCR |= ( 1 << 16);
		FIFO_error_callback();
	}else if( DME() )
	{
		DMA1->HIFCR |= ( 1 << 18);
		DME_callback();
	}
}
