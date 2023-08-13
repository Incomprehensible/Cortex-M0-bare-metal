#include "main.h"
#include "gpio.h"

volatile uint8_t toggle = 0;
volatile int8_t dir = 0;

volatile void dummy_wait()
{
    int dummy = 0;
    for (int i=0; i<100000; ++i)
    {
        dummy++;
    }
}

// Nested vectored interrupt controller (NVIC)
// 32 maskable interrupt channels
// The NVIC and the processor core interface are closely coupled, which enables low latency
// interrupt processing and efficient processing of late arriving interrupts.
// All interrupts including the core exceptions are managed by the NVIC.

// The extended interrupts and events controller (EXTI) manages the external and internal
// asynchronous events/interrupts and generates the event request to the CPU/Interrupt
// controller and a wake-up request to the Power manager.
// The EXTI allows the management of up to 32 external/internal event line (23 external event
// lines and 9 internal event lines).
// The GPIOs are connected to the 16 external interrupt/event lines in the following manner:
// [picture]

// We will use the ‘EXTI’ peripheral to trigger a hardware interrupt from a GPIO input pin. 
// ‘EXTI’ stands for ‘EXTended Interrupt controller’, and its purpose is to extend the number of hardware interrupts available to us programmers. 
// Basically, the chip’s “ARM Cortex-M0” core is where these interrupts originate from
// So, the STM32 EXTI peripherals take a small number of NVIC interrupts and assign several different ‘extended interrupt events’ to each one.
// There are 32 EXTI lines on the STM32F0 line. 
// The first 16 of them can be used to trigger hardware interrupts when changes are detected on any of the 16 GPIO pins associated with each GPIO bank. 
// EXTI0 refers to pin #0, EXTI1 refers to pin #1, and so on. But the STM32 also has several different banks of GPIO pins (GPIOA, GPIOB, GPIOC, etc.),  
// so there is a limitation that we can only use an interrupt for ‘Pin #X’ on one of those banks at a time. 
// For example, we can have an interrupt on PA0 and PB1 at the same time, but not PA1 and PB1 since they have the same pin number.
// You can see that in our STM32F031K6 vector table, there are only three NVIC interrupts dedicated to those 16 EXTI lines. 
// EXTI0_1 can listen to pins 0 and 1, EXTI2_3 can listen to pins 2 and 3, and EXTI4_15 can listen to pins 4 through 15, inclusive.

// To review, these chips have core “NVIC” interrupts which need to be enabled in addition to the actual STM32 peripheral interrupts that we want to use. 
// Our code will configure the STM32’s “EXTI” line to listen on a specific GPIO pin, and then we will enable the NVIC interrupt.

// How can we map EXTIn line to a GPIO pin?
// It’s a good idea to set up the EXTI line to point to the correct GPIO pin (or signal) before actually turning the interrupt on. 
// This is done with the ‘SYSCFG’ (System Configuration) peripheral, which we need to turn on using an RCC register, just like the GPIOB peripheral.
// [picture]
// SYSCFG resides on APB bus (APB2)
// [picture]

void syscfg_clock_on()
{
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
}

void syscfg_config()
{
    // SYSCFG_EXTICR1 register for GPIO[3-0] configuration
    SYSCFG->EXTICR[0] &= ~(0b1111 << ROTARY_A*4);
    SYSCFG->EXTICR[0] |= 0b0001 << ROTARY_A*4;
    SYSCFG->EXTICR[0] &= ~(0b1111 << ROTARY_B*4);
    SYSCFG->EXTICR[0] |= 0b0101 << ROTARY_B*4;

    // x0000 value for GPIOA
    SYSCFG->EXTICR[2] &= ~(0b1111 << (BUTTON % 4)*4);

    // or we can use CMSIS helper functions:
    // SYSCFG->EXTICR[0] &= ~(SYSCFG_EXTICR1_EXTI0_Msk);
    // SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PA;
}

// more general function
void syscfg_gpio_config(uint32_t gpio, uint32_t port)
{
    SYSCFG->EXTICR[gpio / 4] &= ~(0xF << ((gpio % 4) * 4));
    SYSCFG->EXTICR[gpio / 4] |= (port & 0xF) << ((gpio % 4) * 4);
}

void exti_config()
{
    // Set the bit for specific EXTI line which corresponds to pin number
    EXTI->IMR |= 0b1 << BUTTON;
    EXTI->FTSR |= 0b1 << BUTTON;
    // Even though reset value for Falling trigger selection register is all zero's
    // we can clear RTSR bit just in case
    EXTI->RTSR &= ~(0b1 << BUTTON);

    EXTI->IMR |= (0b1 << ROTARY_A | 0b1 << ROTARY_B);
    EXTI->FTSR |= (0b1 << ROTARY_A | 0b1 << ROTARY_B);
    EXTI->RTSR &= ~(0b1 << ROTARY_A | 0b1 << ROTARY_B);
}

void nvic_config()
{
    // Enable the NVIC interrupt for EXTI0 and EXTI1 at minimum priority.
    // ‘Level 0’ is the highest priority, and ‘Level 3’ is the lowest.
    NVIC_SetPriority(EXTI4_15_IRQn, 0x03);
    NVIC_EnableIRQ(EXTI4_15_IRQn);

    NVIC_SetPriority(EXTI0_1_IRQn, 0x02);
    NVIC_EnableIRQ(EXTI0_1_IRQn);
}

int main()
{
    gpio_clock_on();
    gpio_configue();

    syscfg_clock_on();
    syscfg_config();
    exti_config();
    nvic_config();

    for (int i=0; i<10; ++i) {
        led_toggle();
        dummy_wait();
    }

    led_write(0);
    uint8_t led_on = 0;
    while (1)
    {
        if (dir > 0 && !led_on)
        {
            led_write(1);
            // dir = 0;
            led_on = 1;
        }
        else if (dir < 0 && led_on)
        {
            led_write(0);
            // dir = 0;
            led_on = 0;
        }
        // else if (toggle)
        // {
        //     led_toggle();
        //     toggle = 0;
        // }
        // dummy_wait();
    }

    return 0;
}