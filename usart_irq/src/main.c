#include "main.h"
#include "gpio.h"
#include "systick.h"
#include "usart.h"
#include <stdio.h>

// volatile uint8_t toggle = 0;
volatile int irq = 0;

extern volatile uint32_t s_ticks;

volatile uint8_t *data_ptr = NULL;
uint8_t data[10] = "";

volatile void dummy_wait()
{
    int dummy = 0;
    for (int i=0; i<100000; ++i)
    {
        dummy++;
    }
}

void delay_ms(uint32_t ms)
{
    uint32_t ms_to_wait = s_ticks + ms; 
    while (s_ticks < ms_to_wait) ;
}

void nvic_config()
{
    // Enable the NVIC interrupt for UART1 at high priority.
    // ‘Level 0’ is the highest priority, and ‘Level 3’ is the lowest.
    NVIC_SetPriority( USART1_IRQn, 0x02 );
    NVIC_EnableIRQ( USART1_IRQn );
}

int main()
{
    const uint32_t sysclock = 8000000;
    gpio_clock_on();
    gpio_configue();

    usart_clock_on();
    usart_init(sysclock, 9600);
    usart_rx_irq_en();
    usart_enable();

// To generate timer every 500 ms:
// 8 Million cycles per sec -> 4 million cycles per half a second
// 8*10**6/1000 -> cycles per ms
    systick_init(sysclock/1000);
    //systick_enable();

    nvic_config();

    for (int i=0; i<10; ++i) {
        led_toggle();
        dummy_wait();
    }

    led_write(0);

    uint8_t data[10] = "";
    data_ptr = &data[0];
    while (1)
    {
        led_write(1);
        // halt until an interrupt triggers
        __WFI();
        led_write(0);
        printf("LOG: received: %s\r\n", data);
        
        if (data_ptr+1 == (uint32_t)&data[0] + sizeof(data))
            data_ptr = &data[0];
        else
            ++data_ptr;
    }

    return 0;
}