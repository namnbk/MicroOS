#-*-makefile-*-

# This file is used by the make command to build the shell.  Simply
# type make at the shell prompt and press enter to compile the shell.
# Typically, there is no need to modify this file.

CXX               = gcc
SOURCE_c          = main.c
SOURCE_c         += $(filter-out main.c,$(wildcard *.c))
SOURCE_c         += $(wildcard ../common/*.c)
SOURCE_o          = $(patsubst %.c,%.o, $(SOURCE_c))
CPPFLAGS          = -nostdinc -march=i386 -g -n -nostdlib -nostartfiles \
		    -nodefaultlibs -Wall -I../common
COMPILE_c         = $(CXX) $(INCLUDE_DIRS) $(CPPFLAGS) -c $< -o $@

all: shell SHELL.BIN image

shell: $(SOURCE_o)
	$(CXX) $(CPPFLAGS) -e_start -Ttext=0x0 $(SOURCE_o) -o shell

SHELL.BIN: shell
	objcopy -S -j.text -j.rodata -j.data -O binary shell SHELL.BIN

image: SHELL.BIN
	mcopy -i../muos.img -o SHELL.BIN ::

.c.o:
	$(COMPILE_c)

clean:
	rm -f shell SHELL.BIN
	rm -f $(SOURCE_o)

dump: SHELL.BIN
	objdump -D -mi8086 -bbinary SHELL.BIN

floppy: SHELL.BIN
	mcopy -o SHELL.BIN a:

.PHONY: clean all floppy image
