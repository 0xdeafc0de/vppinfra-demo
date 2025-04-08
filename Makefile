# Makefile for compiling test_vec with VPP's vppinfra

# Set BUILD=release to switch to release build
BUILD ?= debug

# Set paths based on the build type
VPP_BASE = /ws/vpp
VPP_INC = $(VPP_BASE)/src
BUILD_ROOT = $(VPP_BASE)/build-root

ifeq ($(BUILD),release)
	VPP_BUILD_DIR := $(BUILD_ROOT)/build-vpp-native
else
	VPP_BUILD_DIR := $(BUILD_ROOT)/build-vpp_debug-native
endif

VPP_LIB_DIR := $(VPP_BUILD_DIR)/vpp/lib
VPP_LIB_FILE := $(VPP_LIB_DIR)/libvppinfra.so

CC = gcc
CFLAGS = -O2 -Wall -I$(VPP_INC) -I./src
LDFLAGS = -L$(VPP_LIB_DIR) -lvppinfra

# Build target
TARGET = test_vec
SRC = src/test_vec.c

all: check_lib $(TARGET)

$(TARGET): $(SRC)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

check_lib:
	@if [ ! -f "$(VPP_LIB_FILE)" ]; then \
		echo "Error: libvppinfra.so not found at $(VPP_LIB_FILE)"; \
		echo "→ Make sure you've built VPP and the correct path is set in the Makefile"; \
		exit 1; \
	fi

clean:
	rm -f $(TARGET)

