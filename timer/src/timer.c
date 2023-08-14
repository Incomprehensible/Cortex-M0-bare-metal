#include "timer.h"
#include "main.h"

extern volatile uint32_t sysclk_hz;

// General-purpose 16-bit Timer 3 will be used

// The main block of the programmable timer is a 16-bit/32-bit counter with its related auto-reload register. 
// The counter can count up but also down or both up and down. The counter clock can be divided by a prescaler.
// The counter, the auto-reload register and the prescaler register can be written or read by
// software. This is true even when the counter is running.

// In upcounting mode, the counter counts from 0 to the auto-reload value (content of the TIMx_ARR register), 
// then restarts from 0 and generates a counter overflow event.
// An Update event can be generated at each counter overflow or by setting the UG bit in the TIMx_EGR register

// When an update event occurs, all the registers are updated and the update flag 
// (UIF bit in TIMx_SR register) is set (depending on the URS bit):
// •The buffer of the prescaler is reloaded with the preload value (content of the TIMx_PSC
// register)
// •The auto-reload shadow register is updated with the preload value (TIMx_ARR)

void timer_clock_on()
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
}

void timer_clock_off()
{
    RCC->APB1ENR &= ~RCC_APB1ENR_TIM3EN;
}

// Timer is clocked from inner clock, which has a system clock frequency
// CK_CNT = fCK_PSC / (PSC[15:0] + 1)
// To increment the counter every 1 ms:
// for internal clock running at 48 MHz we need to devide 48MHz / 48000
void timer_configure(uint16_t ms)
{
    // Start by making sure the timer's 'counter' is off.
    TIM3->CR1 &= ~TIM_CR1_CEN;
    // Reset the timer peripheral
    RCC->APB1RSTR |= RCC_APB1RSTR_TIM3RST;
    RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM3RST;

    TIM3->PSC = sysclk_hz / 1000;
    TIM3->ARR = ms;

    // Send an update event to reset the timer and apply settings
    // This also happens when timer is already running and overflows
    TIM3->EGR |= TIM_EGR_UG;
    // Enable irq
    TIM3->DIER |= TIM_DIER_UIE;
}

void timer_enable()
{
    TIM3->CR1 |= TIM_CR1_CEN;
}

void timer_stop()
{
    // Disable IRQ
    TIM3->DIER &= ~TIM_DIER_UIE;
    TIM3->CR1 &= ~TIM_CR1_CEN;
    TIM3->SR &= ~(TIM_SR_UIF);
}