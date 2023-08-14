#include "main.h"
#include "gpio.h"
#include "flash.h"
#include "rcc.h"
#include "timer.h"

volatile uint8_t toggle = 0;

volatile void dummy_wait()
{
    int dummy = 0;
    for (int i=0; i<100000; ++i)
    {
        dummy++;
    }
}

void nvic_config()
{
    // ‘Level 0’ is the highest priority, and ‘Level 3’ is the lowest.
    NVIC_SetPriority(TIM3_IRQn, 0x03);
    NVIC_EnableIRQ(TIM3_IRQn);
}

int main()
{
    gpio_clock_on();
    gpio_configue();

    flash_latency_configure();
    rcc_clock_init();
    rcc_PLL_clock_on();

    timer_clock_on();
    timer_configure(10000);
    timer_enable();

    nvic_config();

    for (int i=0; i<10; ++i) {
        gpio_toggle();
        dummy_wait();
    }

    gpio_write(0);
    while (1)
    {
        __WFI();
        if (toggle)
        {
            gpio_toggle();
            toggle = 0;
        }
    }

    return 0;
}