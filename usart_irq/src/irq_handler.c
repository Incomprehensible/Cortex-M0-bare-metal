#include "irq_handler.h"
#include "main.h"

extern volatile uint32_t s_ticks;
extern volatile uint8_t *data_ptr;

void SysTick_handler()
{
    ++s_ticks;
}

void USART1_IRQ_handler()
{
    // irq = 1;
    // char c;
    //c = (uint8_t) USART1->RDR;

    if (data_ptr != NULL)
    {
        if (USART1->ISR & USART_ISR_RXNE)
            *data_ptr = (uint8_t) USART1->RDR;
    }
}