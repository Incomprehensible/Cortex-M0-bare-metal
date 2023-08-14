#include "main.h"

extern volatile uint8_t toggle;

void TIM3_IRQ_handler()
{
    if (TIM3->SR & TIM_SR_UIF)
    {
        // Clear irq event bit
        TIM3->SR &= ~(TIM_SR_UIF);
        toggle = 1;
    }
}