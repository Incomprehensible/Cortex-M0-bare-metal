#include "irq_handler.h"
#include <stdint.h>

extern volatile uint32_t s_ticks;

void SysTick_handler()
{
    ++s_ticks;
}