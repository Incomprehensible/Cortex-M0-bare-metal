#include "main.h"

extern volatile uint8_t toggle;
extern volatile int8_t dir;

// To run a C function when the interrupt triggers, we just need to define one with the same name as the vector table entry. 
// That will override the ‘default interrupt handler’ link, because we used the weak keyword when defining those defaults in the vector table.
void EXTI0_1_IRQ_handler()
{
    if (EXTI->PR & (0b1 << ROTARY_A))
    {
        EXTI->PR |= 0b1 << ROTARY_A;
        dir = 1;
    }
    else if (EXTI->PR & (0b1 << ROTARY_B))
    {
        EXTI->PR |= 0b1 << ROTARY_B;
        dir = -1;
    }
}

void EXTI4_15_IRQ_handler()
{
    if (EXTI->PR & (0b1 << BUTTON))
    {
        EXTI->PR |= (0b1 << BUTTON);
        toggle = 1;
    }
}