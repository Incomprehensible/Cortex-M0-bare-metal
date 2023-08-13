#include "systick.h"
#include "main.h"

volatile uint32_t s_ticks;

// The internal RC 8 MHz oscillator is
// selected as default CPU clock on reset. 

// The RELOAD value can be any value in the range 0x00000001-0x00FFFFFF.
// The SysTick exception request and COUNTFLAG are activated when counting from 1 to 0. 
// To generate a timer with a period of N processor clock cycles, use a RELOAD value of N-1. 
// For example, if the SysTick interrupt is required every 100 clock pulses, set RELOAD to 99.

void systick_init(uint32_t value)
{
    // 1 for processor clocking | 1 for systick irq
    SysTick->CTRL |= (1 << 2 | 1 << 1);
    SysTick->LOAD = (value-1) & 0x00FFFFFF;
    SysTick->VAL = 0;

    s_ticks = 0;
}

// The RCC feeds the Cortex System Timer (SysTick) external clock with the AHB clock
// (HCLK) divided by 8. The SysTick can work either with this clock or directly with the Cortex
// clock (HCLK), configurable in the SysTick Control and Status Register.
void systick_clock_on()
{
    //RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
}


void systick_enable()
{
    SysTick->CTRL |= (1 << 0);
}

void systick_disable()
{
    SysTick->CTRL &= ~0b1;
}

