ENTRY(reset_handler)

/* Start of stack - it will go down */

/* Set minimum size for stack and dynamic memory. */
/* (The linker will generate an error if there is less than this much RAM leftover.) */
/* (1KB) */
_min_stack_left = 0x400;

MEMORY
{
    FLASH (rx)  :   ORIGIN = 0x08000000, LENGTH = 16K
    RAM (rwx)   :   ORIGIN = 0x20000000, LENGTH = 4K
}

_estack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{
    /* The vector table goes at the start of flash. */
    .vector_table :
    {
        . = ALIGN(4);
        KEEP (*(.vector_table))
        . = ALIGN(4);
    } >FLASH
    
    /* The 'text' section contains the main program code. */
    .text :
    {
        . = ALIGN(4);
        *(.text)
        *(.text*)
        . = ALIGN(4);
    } >FLASH

    /* The 'rodata' section contains read-only data,
    * constants, strings, information that won't change. */
    .rodata :
    {
        . = ALIGN(4);
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);
    } >FLASH

    /* The 'data' section is space set aside in RAM for
    * things like variables, which can change. */
    _sidata = .;
    .data : AT(_sidata)
    {
        . = ALIGN(4);
        /* Mark start/end locations for the 'data' section. */
        _sdata = .;
        *(.data)
        *(.data*)
        _edata = .;
        . = ALIGN(4);
    } >RAM

    /* The 'bss' section is similar to the 'data' section,
    * but its space is initialized to all 0s at the
    * start of the program. */
    .bss :
    {
        . = ALIGN(4);
        /* Also mark the start/end of the BSS section. */
        _sbss = .;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
    } >RAM

    /* Space set aside for the application's heap/stack. */
    .dynamic_allocations :
    {
        . = ALIGN(4);
        _ssystem_ram = .;
        . = . + _min_stack_left;
        . = ALIGN(4);
        _esystem_ram = .;
    } >RAM
}