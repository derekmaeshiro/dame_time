# 1. Variables: Define tools and files
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size
OPENOCD = openocd

# Cloned STM32CubeF4 repo
STM_PATH = /Users/derekmaeshiro/projects/STM32CubeF4

# Include Directories
INCLUDE = -I. \
		  -I$(STM_PATH)/Drivers/CMSIS/Include \
		  -I$(STM_PATH)/Drivers/CMSIS/Device/ST/STM32F4xx/Include

# 2. Project Files
TARGET = firmware
SOURCES = main.c
STARTUP = startup_stm32f401xe.s
# Fixed the extension mapping for the startup file
OBJECTS = $(SOURCES:.c=.o) $(STARTUP:.s=.o)

# 3. Build Flags
MCU_FLAGS = -mcpu=cortex-m4 -mthumb
CFLAGS = $(MCU_FLAGS) -O0 -g -Wall -Wextra $(INCLUDE)
LDFLAGS = -T linker_script.ld -nostdlib -nostartfiles -nodefaultlibs

# 4. Build Rules
all: $(TARGET).bin

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@
	$(SIZE) $<

# Changed '@' to ':'
$(TARGET).elf: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

# Rule for C files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule for Assembly files (.s)
%.o: %.s
	$(CC) $(CFLAGS) -c $< -o $@

# Flash Rule
flash: $(TARGET).bin
	$(OPENOCD) -f interface/stlink.cfg -f target/stm32f4x.cfg -c "program $(TARGET).elf verify reset exit"

clean:
	rm -f $(OBJECTS) $(TARGET).elf $(TARGET).bin

.PHONY: all clean