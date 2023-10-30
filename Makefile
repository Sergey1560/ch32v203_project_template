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
Lib/Peripheral/src/ch32v20x_pwr.c \
Lib/Peripheral/src/ch32v20x_flash.c \
Lib/Peripheral/src/ch32v20x_bkp.c \
Lib/Peripheral/src/ch32v20x_usart.c \
Lib/Peripheral/src/ch32v20x_spi.c \
Lib/Peripheral/src/ch32v20x_tim.c \
Lib/Peripheral/src/ch32v20x_wwdg.c \
Lib/Peripheral/src/ch32v20x_rtc.c \
Lib/Peripheral/src/ch32v20x_exti.c \
Lib/Peripheral/src/ch32v20x_gpio.c \
Lib/Peripheral/src/ch32v20x_rcc.c \
Lib/Peripheral/src/ch32v20x_can.c \
Lib/Peripheral/src/ch32v20x_adc.c \
Lib/Peripheral/src/ch32v20x_i2c.c \
Lib/Peripheral/src/ch32v20x_misc.c \
Lib/Peripheral/src/ch32v20x_iwdg.c \
Lib/Peripheral/src/ch32v20x_opa.c \
Lib/Peripheral/src/ch32v20x_dma.c \
Lib/Peripheral/src/ch32v20x_crc.c \
Lib/Peripheral/src/ch32v20x_dbgmcu.c \
Lib/RVSIS/syscalls.c \
Lib/RVSIS/debug.c \
Lib/RVSIS/core_riscv.c \
Lib/RVSIS/system_ch32v20x.c \
Lib/RVSIS/ch32v20x_it.c \
src/main.c


# ASM sources
ASM_SOURCES =  \
startup/startup_ch32v20x_D6.S

#######################################
# binaries
#######################################

GCC_PATH = /home/sergey/soft/SDK/xpack-riscv-none-elf-gcc-13.2.0-2/bin
PREFIX = $(GCC_PATH)/riscv-none-elf-

CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size

HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

#######################################
# CFLAGS
#######################################
# cpu
#CPU = -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 

# For gcc v12 and above
CPU = -march=rv32imac_zicsr -mabi=ilp32 -msmall-data-limit=8

# mcu
MCU = $(CPU) $(FPU) $(FLOAT-ABI)

# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES =  \
-ILib/Peripheral/inc \
-ILib/RVSIS \
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
