TARGET = main

# Default target chip.
#MCU ?= STM32F030K6
MCU ?= STM32F031K6
#MCU ?= STM32L031K6
#MCU ?= STM32L052K8
#MCU ?= STM32L082KZ

ifeq ($(MCU), STM32F030K6)
	MCU_FILES = STM32F030K6T6
	ST_MCU_DEF = STM32F030x6
	MCU_CLASS = F0
else ifeq ($(MCU), STM32F031K6)
	MCU_FILES = STM32F031K6T6
	ST_MCU_DEF = STM32F031x6
	MCU_CLASS = F0
else ifeq ($(MCU), STM32L031K6)
	MCU_FILES = STM32L031K6T6
	ST_MCU_DEF = STM32L031xx
	MCU_CLASS = L0
else ifeq ($(MCU), STM32L052K8)
	MCU_FILES = STM32L052K8T6
	ST_MCU_DEF = STM32L052xx
	MCU_CLASS = L0
else ifeq ($(MCU), STM32L082KZ)
	MCU_FILES = STM32L082KZT6
	ST_MCU_DEF = STM32L082xx
	MCU_CLASS = L0
endif

# Define the linker script location and chip architecture.
LD_SCRIPT = $(MCU_FILES).ld
ifeq ($(MCU_CLASS), F0)
	MCU_SPEC = cortex-m0
else ifeq ($(MCU_CLASS), L0)
	MCU_SPEC = cortex-m0plus
endif

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
ASFLAGS += -DVVC_$(MCU_CLASS)

# C compilation directives
CFLAGS += -mcpu=$(MCU_SPEC)
CFLAGS += -mthumb
CFLAGS += -Wall
CFLAGS += -g
# (Set error messages to appear on a single line.)
CFLAGS += -fmessage-length=0
# (Set system to ignore semihosted junk)
CFLAGS += --specs=nosys.specs
CFLAGS += -D$(ST_MCU_DEF)
CFLAGS += -DVVC_$(MCU_CLASS)

# Linker directives.
LSCRIPT = ./ld/$(LD_SCRIPT)
LFLAGS += -mcpu=$(MCU_SPEC)
LFLAGS += -mthumb
LFLAGS += -Wall
LFLAGS += --specs=nosys.specs
LFLAGS += -nostdlib
LFLAGS += -lgcc
LFLAGS += -T$(LSCRIPT)

AS_SRC   =  ./boot_code/$(MCU_FILES)_core.S
AS_SRC   += ./vector_tables/$(MCU_FILES)_vt.S
C_SRC    =  ./src/main.c
C_SRC    += ./src/sspi.c
C_SRC    += ./src/util.c

INCLUDE  =  -I./
INCLUDE  += -I./device_headers

OBJS  = $(AS_SRC:.S=.o)
OBJS += $(C_SRC:.c=.o)

.PHONY: all
all: $(TARGET).bin

%.o: %.S
	$(CC) -x assembler-with-cpp $(ASFLAGS) $< -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $(INCLUDE) $< -o $@

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
