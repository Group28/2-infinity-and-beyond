##########################################################################################################################
# File automatically-generated by tool: [projectgenerator] version: [2.27.0] date: [Thu Apr 19 17:58:53 BST 2018]
##########################################################################################################################

# ------------------------------------------------
# Generic Makefile (based on gcc)
#
# ChangeLog :
#	2017-02-10 - Several enhancements + project update mode
#   2015-07-22 - first version
# ------------------------------------------------

######################################
# target
######################################
TARGET = buggy


######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -O1


#######################################
# paths
#######################################
# source path
SOURCES_DIR =  \
Drivers/CMSIS \
Drivers/LL \
Drivers \
System_Files

# firmware library path
PERIFLIB_PATH = 

# Build path
BUILD_DIR = build

######################################
# source
######################################
# C sources
C_SOURCES =  \
src/main.c \
src/Analog.c \
src/Arbiter.c \
src/DMA.c \
src/ds2781.c \
src/Encoders.c \
src/IO.c \
src/LF.c \
src/RA.c \
src/LS.c \
src/Magnet.c \
src/Memory.c \
src/Motors.c \
src/OneWire.c \
src/PID.c \
src/SR.c \
src/stm32f4xx_it.c \
src/System_Functions.c \
src/Timers.c \
src/USART.c \
src/utils.c \
Drivers/LL/Src/stm32f4xx_ll_rcc.c \
Drivers/LL/Src/stm32f4xx_ll_utils.c \
Drivers/LL/Src/stm32f4xx_ll_pwr.c \
Drivers/LL/Src/stm32f4xx_ll_exti.c \
Drivers/LL/Src/stm32f4xx_ll_gpio.c \
Drivers/LL/Src/stm32f4xx_ll_adc.c \
Drivers/LL/Src/stm32f4xx_ll_crc.c \
Drivers/LL/Src/stm32f4xx_ll_dma.c \
Drivers/LL/Src/stm32f4xx_ll_i2c.c \
Drivers/LL/Src/stm32f4xx_ll_rtc.c \
Drivers/LL/Src/stm32f4xx_ll_spi.c \
Drivers/LL/Src/stm32f4xx_ll_tim.c \
Drivers/LL/Src/stm32f4xx_ll_usart.c \
System_Files/system_stm32f4xx.c
#Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.c


# ASM sources
ASM_SOURCES =  \
startup_stm32f401xe.s


######################################
# firmware library
######################################
PERIFLIB_SOURCES = 

#######################################
# binaries
#######################################
BINPATH = /usr/bin
PREFIX = arm-none-eabi-
CC = $(BINPATH)/$(PREFIX)gcc
AS = $(BINPATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(BINPATH)/$(PREFIX)objcopy
AR = $(BINPATH)/$(PREFIX)ar
SZ = $(BINPATH)/$(PREFIX)size
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m4

# fpu
FPU = -mfpu=fpv4-sp-d16 -mlittle-endian 

# float-abi
FLOAT-ABI = -mfloat-abi=hard

# mcu
MCU = $(CPU) -mthumb -mthumb-interwork $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  \
-DUSE_FULL_LL_DRIVER \
-DSTM32F401xE \
-DARM_MATH_CM4


# AS includes
AS_INCLUDES = \


# C includes
C_INCLUDES =  \
-IInc \
-ISystem_Files\
-IDrivers/LL/Inc \
-IDrivers/CMSIS/Device/ST/STM32F4xx/Include \
-IDrivers/CMSIS/Include \
-IDrivers/CMSIS/


# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" 

#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = STM32F401RETx_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = Drivers/CMSIS/Lib/GCC/libarm_cortexM4lf_math.a
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections -u _printf_float -u _scanf_float

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@		
	
	
flash:
	st-flash write $(BUILD_DIR)/$(TARGET).bin 0x8000000

#######################################
# clean up
#######################################
clean:
	-rm -fR .dep $(BUILD_DIR)
  
#######################################
# dependencies
#######################################
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

# *** EOF ***
