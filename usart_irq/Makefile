TARGET = main

# Define the linker script location and chip architecture.
LD_SCRIPT = link.ld
MCU_SPEC  = cortex-m0

# Toolchain definitions (ARM bare metal defaults)
TOOLCHAIN = /usr
CC = $(TOOLCHAIN)/bin/arm-none-eabi-gcc
AS = $(TOOLCHAIN)/bin/arm-none-eabi-as
LD = $(TOOLCHAIN)/bin/arm-none-eabi-ld
OC = $(TOOLCHAIN)/bin/arm-none-eabi-objcopy
OD = $(TOOLCHAIN)/bin/arm-none-eabi-objdump
OS = $(TOOLCHAIN)/bin/arm-none-eabi-size

# Assembly directives.
ASFLAGS += -c
ASFLAGS += -O0
ASFLAGS += -mcpu=$(MCU_SPEC)
ASFLAGS += -mthumb
ASFLAGS += -Wall
# (Set error messages to appear on a single line.)
ASFLAGS += -fmessage-length=0

# C compilation directives
CFLAGS += -mcpu=$(MCU_SPEC)
CFLAGS += -mthumb
CFLAGS += -Wall
CFLAGS += -g
# (Set error messages to appear on a single line.)
CFLAGS += -fmessage-length=0
# (Set system to ignore semihosted junk)
# CFLAGS += --specs=nosys.specs
CFLAGS += -mfloat-abi=soft

# Linker directives.
LSCRIPT = ./$(LD_SCRIPT)
LFLAGS += -mcpu=$(MCU_SPEC)
LFLAGS += -mthumb
LFLAGS += -Wall
LFLAGS += -nostartfiles
# LFLAGS += --specs=nosys.specs
# LFLAGS += -nostdlib
LFLAGS += -lgcc
LFLAGS += -T$(LSCRIPT)
LFLAGS += --specs=nano.specs 
LFLAGS += -lc

SRC_DIR = ./src
AS_DIR = ./startup
INC_DIR = ./includes

VECT_TBL = $(AS_DIR)/vt.s
AS_SRC   = $(AS_DIR)/start.s
C_SRC    = $(SRC_DIR)/main.c
C_SRC	+= $(SRC_DIR)/gpio.c
C_SRC 	+= $(SRC_DIR)/irq_handler.c
C_SRC 	+= $(SRC_DIR)/systick.c
C_SRC 	+= $(SRC_DIR)/usart.c
C_SRC 	+= $(SRC_DIR)/syscalls.c

OBJS =  $(VECT_TBL:.s=.o)
OBJS += $(AS_SRC:.s=.o)
OBJS += $(C_SRC:.c=.o)

INCLUDE = ./cmsis
INCLUDE += -I$(INC_DIR)

FLASH_ADDR = 0x08000000

.PHONY: all
all: $(TARGET).bin

%.o: %.s
	$(CC) -x assembler-with-cpp $(ASFLAGS) $< -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) -I$(INCLUDE) $< -o $@

$(TARGET).elf: $(OBJS)
	$(CC) $^ $(LFLAGS) -o $@

$(TARGET).bin: $(TARGET).elf
	$(OC) -S -O binary $< $@
	$(OS) $<

.PHONY: clean
clean:
	rm -f $(OBJS)
	rm -f $(TARGET).elf
	rm -f $(TARGET).bin

.PHONY: flash
flash: all
	st-flash write $(TARGET).bin $(FLASH_ADDR)