// Specify what syntax compiler should expect,
// Attributes of our chip and assembly language
.syntax unified
.cpu cortex-m0
.fpu softvfp
.thumb

// Global memory locations
.global vtable
.global reset_handler

/*
 * The actual vector table.
 * Only the size of RAM and 'reset' handler are
 * included, for simplicity.
 */
 .type vtable, %object
 vtable:
    .word _estack
    .word reset_handler
.size vtable, .-vtable

/*
 * Reset handler, called on reset/power-up
*/
.type reset_handler, %function
reset_handler:
    ldr r0, =_estack
    mov sp, r0

// Dummy values for debugging purposes
    ldr r7, =0xDEADBEEF
    movs r0, #0
    main_loop:
        adds r0, r0, #1
        b main_loop
.size reset_handler, .-reset_handler

