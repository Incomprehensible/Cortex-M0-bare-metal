#include "rcc.h"
#include "main.h"

volatile uint32_t sysclk_hz = 8*10e6;

// The STM32F0 line has four options for the PLL clock source:
//     HSI / 2: The PLL’s core clock source is set to the HSI oscillator divided by 2, which is 4MHz. 
//     With this option selected, you don’t need to select a specific division factor.
//     HSI / PREDIV: Similar to the HSI / 2 option, but the HSI signal is divided by the value in the PREDIV bits of RCC_CFGR2.
//     HSE / PREDIV: The “HSE” (High-Speed External) oscillator is used as the PLL’s base signal, 
//     after being divided by the PREDIV bits of RCC_CFGR2.
//     HSI48 / PREDIV: Some of the more advanced STM32F0 chips have a 48MHz HSI oscillator built in, 
//     and that can be selected as a core source for the PLL.

// In the STM32F0 line, the PREDIV values in the RCC_CFGR2 register are used to 
// automatically divide the selected clock signal by a certain factor. 
// Using the HSI / 2 option for the PLL’s clock source lets us ignore these bits 
// by using a constant division factor of 2.

// Once the division settings are chosen, 
// the final PLL signal is created by multiplying the post-division frequency 
// by the PLLMUL bit settings in the RCC_CFGR register.

// PLL_frequency = (input_frequency / PLL_division) * PLL_multiplication
//Setting 8MHz HSI to maximum 48MHz: 
// using HSI / 2 signal -> 4MHz signal * 12 -> 48MHz
void rcc_clock_init()
{
    // Set PLL clock source to HSI/2
    // AHTUNG : we can't write like this:
    // RCC->CFGR &= ~(0b11 << 15);
    // because bit PLLSRC[0] is available only on STM32F070xx and STM32F030xC devices, 
    // otherwise it is reserved (with value zero).
    RCC->CFGR &= ~(0b1 << 16);
    // or
    // RCC->CFGR &= ~RCC_CFGR_PLLSRC;
    RCC->CFGR |= RCC_CFGR_PLLSRC_HSI_DIV2;

    // Set multiplicator
    RCC->CFGR &= ~(0b1111 << 18);
    RCC->CFGR |= (0b1010 << 18);
    // or
    // RCC->CFGR &= ~RCC_CFGR_PLLMUL;
    // RCC->CFGR |= RCC_CFGR_PLLMUL12;

    // Clear the divisor
    RCC->CFGR2 &= ~(0b1111 << 0);
    // or
    //RCC->CFGR2 &= ~(RCC_CFGR2_PREDIV);
}

void rcc_PLL_clock_on()
{
    // Turn the PLL on and wait for it to be ready
    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLRDY)) ;

    // Set and cleared by software to select SYSCLK source
    RCC->CFGR &= ~(0b11 << 0);
    // PLL selected as system clock
    RCC->CFGR |= (0b10 << 0);
    // or
    // RCC->CFGR &= ~RCC_CFGR_SW;
    // RCC->CFGR |= RCC_CFGR_SW_PLL;

    // Wait until PLL is selected as SYSCLK by hardware
    while (!(RCC->CFGR & RCC_CFGR_SWS_PLL)) ;
    sysclk_hz = 48000000;
}

void reference()
{
    RCC->CFGR  &= ~(RCC_CFGR_PLLMUL |
                  RCC_CFGR_PLLSRC);
    RCC->CFGR  |=  (RCC_CFGR_PLLSRC_HSI_DIV2 |
                  RCC_CFGR_PLLMUL12);
    // Turn the PLL on and wait for it to be ready.
    RCC->CR    |=  (RCC_CR_PLLON);
    while (!(RCC->CR & RCC_CR_PLLRDY)) {};
    // Select the PLL as the system clock source.
    RCC->CFGR  &= ~(RCC_CFGR_SW);
    RCC->CFGR  |=  (RCC_CFGR_SW_PLL);
    while (!(RCC->CFGR & RCC_CFGR_SWS_PLL)) {};
}
