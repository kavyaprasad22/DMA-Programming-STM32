#include "stm32f446xx.h"
void HalfTransfer_callback(void);
void FullTransfer_callback(void);
void TransferError_callback(void);
void DME_callback(void);
void FIFO_error_callback(void);
#define HalfTransfer() 	DMA1->HISR & ( 1 << 10)
#define FullTransfer() 	DMA1->HISR & ( 1 << 11)
#define TransferError() 	DMA1->HISR & ( 1 << 9)
#define DME() 	DMA1->HISR & ( 1 << 6)
#define FIFO_error() DMA1->HISR & ( 1 << 8)

void EXTI15_10_IRQHandler(void);
void clear_pending_bit(void);
void DMA1_Stream5_IRQHandler(void);
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

	pUART2->CR3|=(1<<6);
	clear_pending_bit();
}

void DMA1_Stream5_IRQHandler(void)
{
	//clearing interrupt flags//
	if( HalfTransfer() )
	{
		DMA1->HIFCR |= ( 1 << 10);
		HalfTransfer_callback();
	}else if(FullTransfer() )
	{
		DMA1->HIFCR |= ( 1 << 11);
		FullTransfer_callback();
	}else if ( TransferError() )
	{
		DMA1->HIFCR |= ( 1 << 9);
		TransferError_callback();
		
	}else if (FIFO_error() )
	{
		DMA1->HIFCR |= ( 1 << 6);
		FIFO_error_callback();
	}else if( DME() )
	{
		DMA1->HIFCR |= ( 1 << 8);
		DME_callback();
	}
}
