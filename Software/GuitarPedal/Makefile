# Project Name
TARGET =  guitarpedal

CPP_STANDARD = -std=gnu++20

# Sources
CPP_SOURCES = guitar_pedal.cpp \
              guitar_pedal_storage.cpp\
              Effect-Modules/Chopper/chopper.cpp\
              Effect-Modules/ImpulseResponse/ImpulseResponse.cpp\
              Effect-Modules/ImpulseResponse/dsp.cpp\
              $(wildcard */*.cpp)

# Compiler options
OPT = -Os

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
C_INCLUDES += -I./RTNeural -I./RTNeural/modules/Eigen -I./q/q/q_lib/include -I./q/infra/include
CPPFLAGS += -DRTNEURAL_DEFAULT_ALIGNMENT=8 -DRTNEURAL_NO_DEBUG=1

LDFLAGS += -u _printf_float