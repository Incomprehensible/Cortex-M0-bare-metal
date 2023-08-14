#include "flash.h"
#include "main.h"

// The instruction fetch and the data access are both done through the same AHB bus. Read
// accesses can be performed with the following options managed through the flash access
// control register (FLASH_ACR):
// •Instruction fetch: Prefetch buffer enabled for a faster CPU execution
// •Latency: number of wait states for a correct read operation (from 0 to 1)

// The implementation of this prefetch buffer makes a faster CPU execution possible as the
// CPU fetches one word at a time with the next word readily available in the prefetch buffer.
// This implies that the acceleration ratio is of the order of 2, assuming that the code is aligned
// at a 32-bit boundary for the jumps.

// In order to maintain the control signals to read the flash memory, the ratio of the prefetch
// controller clock period to the access time of the flash memory has to be programmed in the
// flash access control register with the LATENCY[2:0] bits. This value gives the number of
// cycles needed to maintain the control signals of the flash memory and correctly read the
// required data. After reset, the value is zero and only one cycle without additional wait states
// is required to access the flash memory.

void flash_latency_configure()
{
    FLASH->ACR |= (1 << 4);
    // or
    //FLASH->ACR |= FLASH_ACR_PRFTBE;

    // One wait state for latency, if 24 MHz < SYSCLK ≤ 48 MHz
    FLASH->ACR &= ~(0b111 << 0);
    FLASH->ACR |= (0b1 << 0);
}

