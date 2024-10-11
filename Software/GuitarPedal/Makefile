# Project Name
TARGET =  guitarpedal

# Sources
CPP_SOURCES = guitar_pedal.cpp guitar_pedal_storage.cpp $(wildcard */*.cpp) Effect-Modules/Chopper/chopper.cpp Effect-Modules/ImpulseResponse/ImpulseResponse.cpp Effect-Modules/ImpulseResponse/dsp.cpp

# Compiler options
OPT = -O1

# Boot options
APP_TYPE = BOOT_SRAM

# Library Locations
DAISYSP_DIR ?= DaisySP
LIBDAISY_DIR ?= libDaisy

USE_DAISYSP_LGPL=1

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile

# RT Neural lib
C_INCLUDES += -I./RTNeural -I./RTNeural/modules/Eigen 
CPPFLAGS += -DRTNEURAL_DEFAULT_ALIGNMENT=8 -DRTNEURAL_NO_DEBUG=1



# CMSIS START --------------------------------------------------

# To get CMSIS working:
# 1. I copied all lines in this section to the libDaisy Makefile
# 2. I built libDaisy again
# 3. I then copied the .o files from libDaisy/build/Drivers/CMSIS-DSP/Source folders to the ./build folder of this project
CMSIS_DSP = $(LIBDAISY_DIR)/Drivers/CMSIS-DSP

C_INCLUDES += -I$(CMSIS_DSP)/PrivateInclude -I$(CMSIS_DSP)/Include

ASM_SOURCES += \
$(CMSIS_DSP)/Source/TransformFunctions/arm_bitreversal2.s

C_SOURCES += \
$(CMSIS_DSP)/Source/TransformFunctions/arm_rfft_fast_f32.c \
$(CMSIS_DSP)/Source/TransformFunctions/arm_rfft_fast_init_f32.c \
$(CMSIS_DSP)/Source/CommonTables/arm_common_tables.c \
$(CMSIS_DSP)/Source/CommonTables/arm_const_structs.c \
$(CMSIS_DSP)/Source/TransformFunctions/arm_cfft_f32.c \
$(CMSIS_DSP)/Source/TransformFunctions/arm_bitreversal.c \
$(CMSIS_DSP)/Source/StatisticsFunctions/arm_max_f32.c \
$(CMSIS_DSP)/Source/ComplexMathFunctions/arm_cmplx_mag_f32.c \
$(CMSIS_DSP)/Source/TransformFunctions/arm_cfft_radix8_f32.c

# CMSIS END --------------------------------------------------