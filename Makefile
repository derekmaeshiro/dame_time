# 1. Variables: Define tools and files
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size
OPENOCD = openocd

# Project Paths
SRC_DIR = src
BUILD_DIR = build
STM_PATH = /Users/derekmaeshiro/projects/STM32CubeF4

# Project Files
TARGET = dame_time
# Automatically find all .c and .s files in the src directory
SOURCES = $(wildcard $(SRC_DIR)/*.c)
STARTUP = $(wildcard $(SRC_DIR)/*.s)

# Create a list of object files in the build directory
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))
OBJECTS += $(patsubst $(SRC_DIR)/%.s, $(BUILD_DIR)/%.o, $(STARTUP))

# Include Directories
INCLUDE = -I$(SRC_DIR) \
          -I$(STM_PATH)/Drivers/CMSIS/Include \
          -I$(STM_PATH)/Drivers/CMSIS/Device/ST/STM32F4xx/Include

# 3. Build Flags
MCU_FLAGS = -mcpu=cortex-m4 -mthumb
CFLAGS = $(MCU_FLAGS) -O0 -g -Wall -Wextra $(INCLUDE)
LDFLAGS = -T linker_script.ld -nostdlib -nostartfiles -nodefaultlibs

# 4. Build Rules
all: $(BUILD_DIR)/$(TARGET).bin

# Rule to generate the binary file
$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@
	$(SIZE) $<

# Rule to link the ELF file
$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

# Rule for C files: Compiles src/%.c to build/%.o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule for Assembly files: Compiles src/%.s to build/%.o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.s | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create the build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $@

# Flash Rule (Updated to point to the binary in build folder)
flash: $(BUILD_DIR)/$(TARGET).bin
	$(OPENOCD) -f interface/stlink.cfg -f target/stm32f4x.cfg -c "program $(BUILD_DIR)/$(TARGET).elf verify reset exit"

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean flash