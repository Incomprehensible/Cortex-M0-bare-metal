/* Define the end of RAM and limit of stack memory */
/* (8KB SRAM on the STM32F051x8 line) */
/* (RAM starts at address 0x20000000) */
_estack = 20002000; /*0x20001FA0;*/

/* Specify RAM size and FLASH size */
MEMORY
{
    FLASH (rx)  :   ORIGIN = 0x08000000, LENGTH = 64K /*0x10000*/
    RAM (rwx)   :   ORIGIN = 0x20000000, LENGTH = 8K
}