#include "ili9163.h"
#include "stm32f030x6.h"

// TEST
extern volatile void __attribute__((optimize("O0"))) dummy_wait();

// This TFT LCD display has 128 x 128 resolution and 262 colors, it uses SPI interface
// Driver IC is ILI9163
// Display model is DL144128TF

// As there isn't enough RAM to store full 16 bit pixels of the LCD display in RAM, buffer stores a paletted version of the data. 
// For instance if you only need 4 colours, these can be stored as 2 bits per pixel rather than 16. 
// The buffer then takes 128 * 128 / 4 = 4096 bytes which will fit into the available RAM.
// This means that all drawing functions can be done much more quickly offscreen, and then the LCD can be updated in one go (removing flicker).

void tft_gpio_configure()
{
    // PA8 and PA11 pins for D/C and RST
    GPIOA->MODER &= ~(0b11 << (2*TFT_DC) | 0b11 << (2*TFT_RST));
    GPIOA->MODER |= (0b01 << (2*TFT_DC) | 0b01 << (2*TFT_RST)); // output mode
    GPIOA->OTYPER &= ~(0b1 << TFT_DC | 0b1 << TFT_RST); // push-pull mode
    GPIOA->OSPEEDR |= (0b11 << (2*TFT_DC) | 0b11 << (2*TFT_RST)); // high-speed mode
    GPIOA->PUPDR &= ~(0b11 << (2*TFT_DC) | 0b11 << (2*TFT_RST)); // no pull-up or pull-down

    // USART1 peripheral
    // PA5, PA7 and PB12 pins for SCK, MOSI and CS
    GPIOA->OTYPER &= ~(0b1 << SPI1_SCK | 0b1 << SPI1_MOSI); // push-pull mode
    GPIOA->OSPEEDR |= (0b11 << (2*SPI1_SCK) | 0b11 << (2*SPI1_MOSI)); // high-speed mode
    GPIOA->PUPDR &= ~(0b11 << (2*SPI1_SCK) | 0b11 << (2*SPI1_MOSI));
    GPIOA->PUPDR |= (0b01 << (2*SPI1_SCK) | 0b01 << (2*SPI1_MOSI)); // pull-up

    // Set gpio mode as alternative (10)
    GPIOA->MODER &= ~(0b11 << (SPI1_SCK * 2) | 0b11 << (SPI1_MOSI * 2));
    GPIOA->MODER |= (0b10 << (SPI1_SCK * 2) | 0b10 << (SPI1_MOSI * 2));

    // SPI CS as output general purpose io
    GPIOB->MODER &= ~(0b11 << (SPI1_CS * 2));
    GPIOB->MODER |= 0b01 << (SPI1_CS * 2); // output
    GPIOB->OTYPER &= ~(0b1 << SPI1_CS); // push-pull mode
    GPIOB->OSPEEDR |= 0b11 << (2*SPI1_CS); // high-speed mode
    GPIOB->PUPDR &= ~(0b11 << (2*SPI1_CS));
    GPIOB->PUPDR |= 0b01 << (2*SPI1_CS);  // pull up
    // no pull-up or pull-down
    // Alternative function AF0: 0000
    // GPIOA alternate function low/high register (GPIOx_AFRH)
    GPIOA->AFR[0] &= ~(0b1111 << ((SPI1_SCK & 0b111)*4) | 0b1111 << ((SPI1_MOSI & 0b111)*4));
    //GPIOA->AFR[0] |= (0b0 << ((SPI1_SCK & 0b111)*4) | 0b0 << ((SPI1_MOSI & 0b111)*4));
    //GPIOB->AFR[1] &= ~(0b1111 << ((SPI1_CS & 0b111)*4));
}


void test_tft_gpio_configure()
{
// Set gpio mode as alternative (10)
    GPIOA->MODER &= ~(0b11 << (SPI1_SCK * 2) | 0b11 << (SPI1_MOSI * 2));
    GPIOA->MODER &= ~(0b11 << (TFT_DC*2));

    // AF0 fo PA5, PA7
    // GPIOA->AFR[0] &= ~(0b1111 << ((SPI1_SCK & 0b111)*4) | 0b1111 << ((SPI1_MOSI & 0b111)*4));
    // GPIOA->AFR[0] &= ~(GPIO_AFRL_AFRL5 | GPIO_AFRL_AFRL7);
    GPIOA->AFR[0] &= ~(GPIO_AFRL_AFSEL5 | GPIO_AFRL_AFSEL7); //TEST
    
    GPIOA->MODER |= (0b10 << (SPI1_SCK * 2) | 0b10 << (SPI1_MOSI * 2) | 0b01 << (TFT_DC * 2));

    GPIOA->PUPDR &= ~(0b11 << (2*SPI1_SCK) | 0b11 << (2*SPI1_MOSI) | 0b11 << (2*TFT_DC));
    GPIOA->PUPDR |= (0b01 << (2*SPI1_SCK) | 0b01 << (2*SPI1_MOSI)); // pull-up

    // PA5, PA7 and PB1 pins for SCK, MOSI and CS
    GPIOA->OTYPER &= ~(0b01 << SPI1_SCK | 0b01 << SPI1_MOSI | 0b01 << TFT_DC); // push-pull mode
    GPIOA->OSPEEDR |= (0b11 << (SPI1_SCK*2) | 0b11 << (SPI1_MOSI*2) | 0b11 << (TFT_DC*2)); // high-speed mode

    // SPI CS as output general purpose io
    // PA8 and PA11 pins for D/C and RST
    GPIOA->MODER &= ~(0b11 << (TFT_RST * 2));
    GPIOB->MODER &= ~(0b11 << (SPI1_CS * 2));
    GPIOA->MODER |= 0b01 << (TFT_RST * 2); // output
    GPIOB->MODER |= 0b01 << (SPI1_CS * 2);

    GPIOA->OTYPER &= ~(0b01 << TFT_RST); // push-pull mode
    GPIOB->OTYPER &= ~(0b01 << SPI1_CS);
    GPIOA->PUPDR &= ~(0b11 << (TFT_RST*2)); // no pull-up or pull-down
    GPIOB->PUPDR &= ~(0b11 << (SPI1_CS*2));
}

// inits gpio with starting logic level
void tft_gpio_init()
{
    GPIOA->ODR |= (1 << TFT_DC | 1 << SPI1_SCK); // we send data by default
    GPIOB->ODR |= 1 << SPI1_CS;
}

void tft_reset()
{
    GPIOA->ODR &= ~(1 << TFT_RST);
    //delay_ms(100);
    dummy_wait(2000000);
    GPIOA->ODR |= 1 << TFT_RST;
    // delay_ms(100);
    dummy_wait(2000000);
}

void tft_start()
{
    GPIOB->ODR &= ~(1 << SPI1_CS);
    delay_ms(10);

    // Set addressing mode
    spi_write_byte(0x36, TFT_CMD);
    spi_write_byte(0x48, TFT_DATA);
    // Set vertical scrolling starting point
    spi_write_byte(0x37, TFT_CMD);
    spi_write_2bytes(0x00, TFT_DATA);
    // Set pixel format
    spi_write_byte(0x3A, TFT_CMD);
    spi_write_byte(0x05, TFT_DATA); // 8-bit
    // Set gamma curve
    spi_write_byte(0x26, TFT_CMD);
    spi_write_byte(0x01, TFT_DATA);
    // Exit sleep mode
    spi_write_byte(0x11, TFT_CMD);
    dummy_wait(2000000);
    // Display on
    spi_write_byte(0x29, TFT_CMD);
    dummy_wait(2000000);
    // 'Normal' display mode (whole screen)
    spi_write_byte(0x13, TFT_CMD);
}

#define hspi_cmd(c) spi_write_byte(c, TFT_CMD)
#define hspi_w8(c) spi_write_byte(c, TFT_DATA)

void ili9341_hspi_init() {
  // (Display off)
  //hspi_cmd(SPIx, 0x28);

  // Issue a series of initialization commands from the
  // Adafruit library for a simple 'known good' test.
//   hspi_cmd(0xEF);
//   hspi_w8(0x03);
//   hspi_w8(0x80);
//   hspi_w8(0x02);
//   hspi_cmd(0xCF);
//   hspi_w8(0x00);
//   hspi_w8(0xC1);
//   hspi_w8(0x30);
//   hspi_cmd(0xED);
//   hspi_w8(0x64);
//   hspi_w8(0x03);
//   hspi_w8(0x12);
//   hspi_w8(0x81);
//   hspi_cmd(0xE8);
//   hspi_w8(0x85);
//   hspi_w8(0x00);
//   hspi_w8(0x78);
//   hspi_cmd(0xCB);
//   hspi_w8(0x39);
//   hspi_w8(0x2C);
//   hspi_w8(0x00);
//   hspi_w8(0x34);
//   hspi_w8(0x02);
//   hspi_cmd(0xF7);
//   hspi_w8(0x20);
//   hspi_cmd(0xEA);
//   hspi_w8(0x00);
//   hspi_w8(0x00);
  // PWCTR1
//   hspi_cmd(0xC0);
//   hspi_w8(0x23);
  // PWCTR2
//   hspi_cmd(0xC1);
//   hspi_w8(0x10);
  // VMCTR1
//   hspi_cmd(0xC5);
//   hspi_w8(0x3E);
//   hspi_w8(0x28);
  // VMCTR2
//   hspi_cmd(0xC7);
//   hspi_w8(0x86);
  // MADCTL
  hspi_cmd(0x36);
  hspi_w8(0x48);
  // VSCRSADD
  hspi_cmd(0x37);
//   hspi_w8(0x00); // 2 parameters!
    hspi_w8(0x00);
    hspi_w8(0x00);
  // PIXFMT
  hspi_cmd(0x3A);
  hspi_w8(0x05);

  // FRMCTR1
//   hspi_cmd(0xB1);
//   hspi_w8(0x00);
//   hspi_w8(0x18);
  // DFUNCTR
//   hspi_cmd(0xB6);
//   hspi_w8(0x08);
//   hspi_w8(0x82);
//   hspi_w8(0x27);
//   hspi_cmd(0xF2);
//   hspi_w8(0x00);
  // GAMMASET
  hspi_cmd(0x26);
  hspi_w8(0x01);
  // (Actual gamma settings)
//   hspi_cmd(0xE0);
//   hspi_w8(0x0F);
//   hspi_w8(0x31);
//   hspi_w8(0x2B);
//   hspi_w8(0x0C);
//   hspi_w8(0x0E);
//   hspi_w8(0x08);
//   hspi_w8(0x4E);
//   hspi_w8(0xF1);
//   hspi_w8(0x37);
//   hspi_w8(0x07);
//   hspi_w8(0x10);
//   hspi_w8(0x03);
//   hspi_w8(0x0E);
//   hspi_w8(0x09);
//   hspi_w8(0x00);
//   hspi_cmd(0xE1);
//   hspi_w8(0x00);
//   hspi_w8(0x0E);
//   hspi_w8(0x14);
//   hspi_w8(0x03);
//   hspi_w8(0x11);
//   hspi_w8(0x07);
//   hspi_w8(0x31);
//   hspi_w8(0xC1);
//   hspi_w8(0x48);
//   hspi_w8(0x08);
//   hspi_w8(0x0F);
//   hspi_w8(0x0C);
//   hspi_w8(0x31);
//   hspi_w8(0x36);
//   hspi_w8(0x0F);

  // Exit sleep mode.
  hspi_cmd(0x11);
  dummy_wait(2000000);
  // Display on.
  hspi_cmd(0x29);
  dummy_wait(2000000);
  // 'Normal' display mode.
  hspi_cmd(0x13);
}

// 128 x 128 display
void tft_set_dims(uint16_t h, uint16_t w)
{
    spi_write_byte(0x2A, TFT_CMD);
    spi_write_2bytes(0x0000, TFT_DATA);
    spi_write_2bytes(h, TFT_DATA);
    spi_write_byte(0x2B, TFT_CMD);
    spi_write_2bytes(0x0000, TFT_DATA);
    spi_write_2bytes(w, TFT_DATA);
}

void tft_en_mem_transfer()
{
    spi_write_byte(0x2C, TFT_CMD);
}


// Any instruction can be sent in any order to the Driver. The MSB is transmitted first. The serial interface is
// initialized when CSX is high status. In this state, SCL clock pulse or SDA data have no effect. A falling edge on
// CSX enables the serial interface and indicated the start of data transmission.
// If D/CX is “low”, the transmission byte is interpreted as a command byte. 
// If D/CX is “high”, the transmission byte is stored in the display data RAM (Memory write command), 
// or command register as parameter.
// SDA is sampled at the rising edge of SCL
// If CSX stays low after the last bit of command/data byte, 
// the serial interface expects the D/CX bit (3-pin serial interface) or D7(4-pins serial interface)
// of the next byte at the next rising edge of SCL.

void spi_clock_on()
{
    RCC->AHBENR |= (RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN);
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
}

// does not enable SPI afterwards
void spi_reset(bool en)
{
    SPI1->CR1 &= ~(SPI_CR1_SPE);

    RCC->APB2RSTR |= RCC_APB2RSTR_SPI1RST;
    RCC->APB2RSTR &= ~(RCC_APB2RSTR_SPI1RST);

    if (en)
        SPI1->CR1 |= SPI_CR1_SPE;
}

void spi_configure()
{
    spi_reset(false); // reset to defaults and enable the peripheral
    SPI1->CR1 &= ~(SPI_CR1_LSBFIRST); // MSB first
    SPI1->CR1 |= SPI_CR1_MSTR; // master mode
    SPI1->CR1 |= SPI_CR1_CPOL; // clk polarity: SCK to 1 when idle
    SPI1->CR1 |= SPI_CR1_CPHA; // clk phase: the 2nd clock transition is the 1st data capture edge
    SPI1->CR1 |= SPI_CR1_SSM; // no CS pulse, only software chip select
    // Careful! may be wrong value
    // The value of this bit is forced onto the
    // NSS pin and the I/O value of the NSS pin is ignored.
    SPI1->CR1 |= SPI_CR1_SSI; // internal slave select
}

void test_spi_configure()
{
    spi_reset(false); // reset to defaults and enable the peripheral
    
    SPI1->CR1 &= ~(SPI_CR1_CPOL | SPI_CR1_CPHA);
    SPI1->CR1 |= (SPI_CR1_CPOL | SPI_CR1_CPHA); // clk polarity: SCK to 1 when idle

    SPI1->CR1 |= SPI_CR1_MSTR; // master mode
    SPI1->CR1 |= SPI_CR1_SSM; // no CS pulse, only software chip select
    // Careful! may be wrong value
    // The value of this bit is forced onto the
    // NSS pin and the I/O value of the NSS pin is ignored.
    SPI1->CR1 |= SPI_CR1_SSI; // internal slave select

    SPI1->CR1 &= ~(SPI_CR1_LSBFIRST); // MSB first
    
    SPI1->CR2 &= ~(SPI_CR2_DS); // data size
    SPI1->CR2 |= (0b0111 << SPI_CR2_DS_Pos); // 8bit

    SPI1->CR1 &= ~(SPI_CR1_BR); // fclk/2
}

void spi_enable()
{
    SPI1->CR1 |= SPI_CR1_SPE;
}

void spi_disable()
{
    SPI1->CR1 = ~(SPI_CR1_SPE);
}

// the peripheral’s clock speed is divided by fclk/=2^(value+1). 
// Divides the clock by 256
void spi_slow_down()
{
    SPI1->CR1 |= SPI_CR1_BR;
}

void spi_write_byte(uint8_t byte, TFT_Packet type)
{
    while (!(SPI1->SR & SPI_SR_TXE)) ;
    if (type == TFT_CMD)
    {
        // Since we don’t want to change the D/C pin while the peripheral is still sending data in its transmit queue, 
        // we should wait for the peripheral’s BSY (Busy) flag to be cleared before changing the state of the D/C GPIO pin
        while (SPI1->SR & SPI_SR_BSY) ;
        GPIOA->ODR &= ~(1 << TFT_DC); // clear "data" flag
    }

    *((uint8_t*)&(SPI1->DR)) = byte;

    if (type == TFT_CMD) {
        while (SPI1->SR & SPI_SR_BSY) ;
        GPIOA->ODR |= 1 << TFT_DC;
    }
}

void spi_write_2bytes(uint16_t byte, TFT_Packet type)
{
    while (!(SPI1->SR & SPI_SR_TXE)) ;
    if (type == TFT_CMD)
    {
        while (SPI1->SR & SPI_SR_BSY) ;
        GPIOA->ODR &= ~(1 << TFT_DC);
    }

    // Careful!!
    // *(uint16_t*)&(SPI1->DR) = ( ((byte & 0x00FF) << 8) | (byte >> 8) ) ;
      byte = (((byte & 0x00FF) << 8) | ((byte & 0xFF00) >> 8));
  *(uint16_t*)&(SPI1->DR) = byte;

    if (type == TFT_CMD) {
        while (SPI1->SR & SPI_SR_BSY) ;
        GPIOA->ODR |= 1 << TFT_DC;
    }
}