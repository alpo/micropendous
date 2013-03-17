#
#             LUFA Library
#     Copyright (C) Dean Camera, 2013.
#
#  dean [at] fourwalledcubicle [dot] com
#           www.lufa-lib.org
#
# --------------------------------------
#         LUFA Project Makefile.
# --------------------------------------

# Run "make help" for target help.

MCU			= at90usb1287
ARCH			= AVR8
BOARD			= MICROPENDOUS_REV2
F_CPU			= 16000000
F_USB			= $(F_CPU)
OPTIMIZATION		= s
TARGET			= VirtualSerial
LUFA_PATH		= ../../libs/LUFA/LUFA
FREERTOS_PATH		= ../../libs/FreeRTOS
FREERTOS_DEMO_DIR	= $(FREERTOS_PATH)/Demo/Common/Minimal
FREERTOS_SOURCE_DIR	= $(FREERTOS_PATH)/Source
FREERTOS_PORT_DIR	= $(FREERTOS_PATH)/Source/portable/GCC/AT90USB_ATmegaXXUY
FREERTOS_SOURCE		= $(FREERTOS_SOURCE_DIR)/tasks.c \
				$(FREERTOS_SOURCE_DIR)/queue.c \
				$(FREERTOS_SOURCE_DIR)/list.c \
				$(FREERTOS_SOURCE_DIR)/portable/MemMang/heap_1.c \
				$(FREERTOS_PORT_DIR)/port.c
SRC			= $(TARGET).c Descriptors.c $(LUFA_SRC_USB) $(LUFA_SRC_USBCLASS) $(FREERTOS_SOURCE)
CC_FLAGS		= -DUSE_LUFA_CONFIG_HEADER -IConfig/ -I$(FREERTOS_SOURCE_DIR)/include -I$(FREERTOS_SOURCE_DIR) -I$(FREERTOS_PORT_DIR) -I$(FREERTOS_DEMO_DIR)
LD_FLAGS		=
CDC_BOOTLOADER_PORT	= /dev/ttyACM0
#CDC_BOOTLOADER_PORT	= COM5

# Default target
all:

# Include LUFA build script makefiles
include $(LUFA_PATH)/Build/lufa_core.mk
include $(LUFA_PATH)/Build/lufa_sources.mk
include $(LUFA_PATH)/Build/lufa_build.mk
include $(LUFA_PATH)/Build/lufa_cppcheck.mk
include $(LUFA_PATH)/Build/lufa_doxygen.mk
include $(LUFA_PATH)/Build/lufa_dfu.mk
include $(LUFA_PATH)/Build/lufa_hid.mk
include $(LUFA_PATH)/Build/lufa_avrdude.mk
include $(LUFA_PATH)/Build/lufa_atprogram.mk
