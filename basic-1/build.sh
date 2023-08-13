arm-none-eabi-gcc -O0 -mcpu=cortex-m0 -mthumb -Wall -c vt.s -o vt.o
arm-none-eabi-gcc -O0 -mcpu=cortex-m0 -mthumb -Wall -c start.s -o start.o
arm-none-eabi-gcc -O0 -mcpu=cortex-m0 -Wall -c main.c -o main.o

arm-none-eabi-gcc start.o vt.o main.o -mcpu=cortex-m0 -mthumb -Wall --specs=nosys.specs -nostdlib -lgcc -T./link.ld -o main.elf