arm-none-eabi-gcc -O0 -mcpu=cortex-m0 -mthumb -Wall -c start.s -o start.o
arm-none-eabi-gcc start.o -mcpu=cortex-m0 -mthumb -Wall --specs=nosys.specs -nostdlib -lgcc -T./link.ld -o start.elf