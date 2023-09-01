#ifndef MAIN_H
#define MAIN_H

#include <stdbool.h>
#include <stdint.h>

#define LED (4) // PA4
#define SPI1_SCK (5) // PA5
#define SPI1_MOSI (7) // PA7
#define SPI1_CS (1) // PB1, SPI1_NSS
#define TFT_DC (9) // PA9, general purpose
#define TFT_RST (10) // PA10, general purpose

void delay_ms(uint32_t);

#endif