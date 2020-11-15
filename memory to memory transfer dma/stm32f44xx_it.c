#include "stm32f446xx.h"
void HalfTransfer_callback(void);
void FullTransfer_callback(void);
void TransferError_callback(void);
void DME_callback(void);
void FIFO_error_callback(void);

#define HalfTransfer() 	DMA2->LISR & ( 1 << 4)
#define FullTransfer() 	DMA2->LISR & ( 1 << 5)
#define TransferError() 	DMA2->LISR & ( 1 << 3)
#define DME() 	DMA2->LISR & ( 1 << 0)
#define FIFO_error() DMA2->LISR & ( 1 << 2)

void EXTI15_10_IRQHandler(void);
void clear_pending_bit(void);
void DMA2_Stream0_IRQHandler(void);
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
  DMA_TypeDef *pDMA;
	pDMA=DMA2;
	DMA_Stream_TypeDef *pSTREAM0;
	pSTREAM0 = DMA2_Stream0;
	pSTREAM0->CR|=(1<<0);
	clear_pending_bit();
}

void DMA2_Stream0_IRQHandler(void)
{
		//Half-transfer
	if( HalfTransfer() )
	{
		DMA2->LIFCR |= ( 1 << 4);
		HalfTransfer_callback();
	}else if(FullTransfer() )
	{
		DMA2->LIFCR |= ( 1 << 5);
		FullTransfer_callback();
	}else if ( TransferError() )
	{
		DMA2->LIFCR |= ( 1 << 3);
		TransferError_callback();
		
	}else if (FIFO_error() )
	{
		DMA2->LIFCR |= ( 1 << 0);
		FIFO_error_callback();
	}else if( DME() )
	{
		DMA2->LIFCR |= ( 1 << 2);
		DME_callback();
	}
}
