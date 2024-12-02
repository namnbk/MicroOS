#-*-makefile-*-

# This file is used by the make command to build the kernel.  Simply
# type make at the shell prompt and press enter to compile the kernel.
# Typically, there is no need to modify this file.

CXX               = gcc
SOURCE_c          = main.c
SOURCE_c         += $(filter-out main.c,$(wildcard *.c))
SOURCE_c         += $(wildcard ../common/*.c)
SOURCE_o          = $(patsubst %.c,%.o, $(SOURCE_c))
CPPFLAGS          = -nostdinc -march=i386 -g -n -nostdlib -nostartfiles \
		    -nodefaultlibs -Wall -I../common
COMPILE_c         = $(CXX) $(INCLUDE_DIRS) $(CPPFLAGS) -c $< -o $@

all: kernel KERNEL.BIN image

kernel: $(SOURCE_o)
	$(CXX) $(CPPFLAGS) -e_start -Ttext=0x7e00 $(SOURCE_o) -o kernel

KERNEL.BIN: kernel
	objcopy -S -j.text -j.rodata -j.data -O binary kernel KERNEL.BIN

image: KERNEL.BIN
	mcopy -i../muos.img -o KERNEL.BIN ::/

.c.o:
	$(COMPILE_c)

clean:
	rm -f kernel KERNEL.BIN
	rm -f $(SOURCE_o)

dump: KERNEL.BIN
	objdump -D -mi8086 -bbinary KERNEL.BIN

floppy: KERNEL.BIN
	mcopy -o KERNEL.BIN a:

.PHONY: clean all floppy image
