.syntax unified
.cpu cortex-m0
.fpu softvfp
.thumb

.global reset_handler

/*
 * The Reset handler. Called on reset.
 */
.type reset_handler, %function
reset_handler:
    ldr  r0, =_estack
    mov  sp, r0

    // Load initialized data from Flash into RAM .data section
    movs r0, #0     // bytes copied
    ldr r1, =_sdata
    ldr r2, =_edata
    ldr r3, =_sidata
    b copy_data_to_ram

copy_sidata:
    // Offset the data init section by our copy progress
    ldr r4, [r3, r0]
    str r4, [r1, r0]
    // Copy the current word into data, and increment.
    adds r0, r0, #4

copy_data_to_ram:
    // Unless we've copied the whole data section, copy the
    // next word from sidata->data
    adds r4, r0, r1
    cmp r4, r2
    bcc copy_sidata // blo, bne

movs r0, #0
ldr r1, =_sbss
ldr r2, =_ebss
b zero_out_bss_in_ram

zero_out:
    // Store a 0 and increment by a word.
    str r0, [r1]
    adds r1, r1, #4

// Once we are done copying the data section into RAM,
// move on to filling the BSS section with 0s.
zero_out_bss_in_ram:
    cmp r1, r2
    bcc zero_out

ldr r5, =0xAEAEAE
b main
.size reset_handler, .-reset_handler



