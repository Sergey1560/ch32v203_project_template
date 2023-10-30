######################################
# target
######################################
TARGET = firmware
TARGET_DEFS=

######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization for size
OPT = -Os

#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

######################################
# source
######################################
# C sources

C_SOURCES = \
Lib/RVSIS/syscalls.c \
Lib/RVSIS/debug.c \
Lib/RVSIS/core_riscv.c \
Lib/RVSIS/system_ch32v20x.c \
Lib/RVSIS/ch32v20x_it.c \
Lib/Mylib/uart.c \
src/main.c

# ASM sources
ASM_SOURCES =  \
startup/startup_ch32v20x_D6.S

#######################################
# binaries
#######################################

GCC_PATH = /home/sergey/soft/SDK/xpack-riscv-none-elf-gcc-13.2.0-2/bin
PREFIX = $(GCC_PATH)/riscv-none-elf-

# GCC 8 toolchain
#GCC_PATH = /home/sergey/soft/SDK/riscv-eabi-none/bin
#PREFIX = $(GCC_PATH)/riscv-none-embed-

CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size

HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

#######################################
# CFLAGS
#######################################
GCC_VERSION_GTEQ12 := $(shell expr `${CC} -dumpversion | cut -f1 -d.` \>= 12)
# cpu
ifeq "$(GCC_VERSION_GTEQ12)" "1"
# For gcc v12 and above
    CPU = -march=rv32imac_zicsr -mabi=ilp32 -msmall-data-limit=8
else
	CPU = -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 
endif

# mcu
MCU = $(CPU) $(FPU) $(FLOAT-ABI)

# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES =  \
-ILib/RVSIS \
-ILib/Mylib \
-Isrc

# compile gcc flags
ASFLAGS = $(MCU) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

CFLAGS += $(TARGET_DEFS)

#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = startup/Link.ld

# libraries
LIBS = -lc -lm -lnosys
LIBDIR = 
LDFLAGS = $(MCU) -mno-save-restore -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -T $(LDSCRIPT) -nostartfiles -Xlinker --gc-sections -Wl,-Map=$(BUILD_DIR)/$(TARGET).map --specs=nano.specs $(LIBS) -Wl,--print-memory-usage

include std_perif.mk

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.S=.o)))
vpath %.S $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.S Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@
#$(LUAOBJECTS) $(OBJECTS)
$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@		

#######################################
# Program
#######################################
program: $(BUILD_DIR)/$(TARGET).elf 
	sudo wch-openocd -f /usr/share/wch-openocd/openocd/scripts/interface/wch-riscv.cfg -c 'init; halt; program $(BUILD_DIR)/$(TARGET).elf; reset; wlink_reset_resume; exit;'

isp: $(BUILD_DIR)/$(TARGET).bin
	wchisp flash $(BUILD_DIR)/$(TARGET).bin

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)
  
#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***
