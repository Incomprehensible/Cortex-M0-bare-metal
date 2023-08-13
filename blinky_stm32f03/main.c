#include "main.h"


volatile uint32_t button_status = 0;
// GPIO reside at 0x48000000 address and are accessible via AHB2 bus
void gpio_clock_on()
{
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN ; // enable GPIO (Ports A,C) clock & power
}

// Each of the GPIO ports has four 32-bit memory-mapped control registers (GPIOx_MODER,
// GPIOx_OTYPER, GPIOx_OSPEEDR, GPIOx_PUPDR) to configure up to 16 I/Os. The
// GPIOx_MODER register is used to select the I/O mode (input, output, AF, analog). The
// GPIOx_OTYPER and GPIOx_OSPEEDR registers are used to select the output type (push-
// pull or open-drain) and speed. The GPIOx_PUPDR register is used to select the pull-
// up/pull-down whatever the I/O direction
void gpio_configue()
{
    // Strictly speaking, the register &= ~(value) statements are not necessary here; the chip will automatically set each register to 0 after a system reset. 
    // But when an option has more than one bit, it is good practice to reset the whole option before setting the bits you want. 
    // For example, if you had previously set a value to 01 and you tried to set its first two bits to 10 with the single command, 
    // register |= 0b10, then the value would actually get set to (01 | 10) = 11, which is not what you wanted. 
    // Clearing those two bits first with a command like register &= ~(0b11) prevents that problem without overwriting any other settings in the register.
    
    // in order to set the pin value in the register we first need
    // to clear the residing contents and then write the new value
    GPIOA->MODER &= ~(0b11 << (2*BUTTON)); // input mode

    GPIOA->MODER &= ~(0b11 << (2*LED));
    GPIOA->MODER |= 0b01 << (2*LED); // output mode

    // in reset mode register is all 0s
    // so no need to set manually
    GPIOA->OTYPER &= ~(0b1 << LED); // push-pull mode

    GPIOA->OSPEEDR |= 0b11 << (2*LED); // high-speed mode

    GPIOA->PUPDR &= ~(0b11 << (2*BUTTON));
    // GPIOA->PUPDR |= 0b01 << (2*BUTTON); // pull-up enabled

    GPIOA->PUPDR &= ~(0b11 << (2*LED)); // no pull-up or pull-down
}

// PIOx_ODR stores the data to be output, it is read/write
// accessible.
void gpio_write(uint32_t level)
{
    if (level == 0)
        GPIOA->ODR &= ~(0b1 << LED);
    else if (level == 1) {
        // GPIOA->ODR &= ~(0b1 << LED);
        GPIOA->ODR |= 0b1 << LED;
    }
}

void gpio_toggle()
{
    GPIOA->ODR ^= 0b1 << LED;
}

// The data input through the I/O are stored into the input data register
// (GPIOx_IDR), a read-only register.
uint32_t gpio_read()
{
    // bit will be 0 if button is pressed
    return ~(GPIOA->IDR & (0b1 << BUTTON));
}

uint32_t get_gpioa_status()
{
    return GPIOA->IDR;
}

// uint32_t get_gpioc_status()
// {
//     return GPIOC->IDR;
// }

volatile void dummy_wait()
{
    int dummy = 0;
    for (int i=0; i<100000; ++i)
    {
        dummy++;
    }
}

int main()
{
    gpio_clock_on();
    gpio_configue();
    // uint32_t LED_status = get_gpioc_status() & (0b1 << LED_G);
    // uint32_t button_status = get_gpioa_status() & (0b1 << BUTTON);

    // for (int i=0; i<10; ++i) {
    //     gpio_toggle();
    //     dummy_wait();
    // }

    int led_on = 1;

    while (1)
    {
        button_status = get_gpioa_status() & (0b1 << BUTTON);

        if (gpio_read() == 0)
        {
            // gpio_toggle();
            gpio_write(0);
            led_on = 0;
        }
        else if (gpio_read() == 1)
        {
            gpio_write(1);
            led_on = 1;
        }
        // if (gpio_read() == 1 && !led_on)
        // {
        //     gpio_toggle();
        //     //gpio_write(1);
        //     led_on = 1;
        // }
        // else if (led_on)
        // {
        //     gpio_toggle();
        //     //gpio_write(0);
        //     led_on = 0;
        // }
        dummy_wait();
    }

    return 0;
}