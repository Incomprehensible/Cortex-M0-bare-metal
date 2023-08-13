#include "main.h"

extern volatile uint8_t toggle;

// To run a C function when the interrupt triggers, we just need to define one with the same name as the vector table entry. 
// That will override the ‘default interrupt handler’ link, because we used the weak keyword when defining those defaults in the vector table.
void EXTI0_1_IRQ_handler()
{
    if (EXTI->PR & (0b1 << BUTTON))
    {
        EXTI->PR |= (0b1 << BUTTON);
        toggle = 1;
    }
    // else
    //     toggle = 0;
    // if (EXTI->PR & EXTI_PR_PR0_Msk)
    // {
    //     EXTI->PR |= EXTI_PR_PR0_Msk;
    //     toggle = 1;
    // }
}