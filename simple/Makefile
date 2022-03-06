OBJS = $(patsubst %.S, %.o, $(patsubst %.c, %.o, $(wildcard *.[cS])))

PREFIX = aarch64-none-elf
BIN = ../$(PREFIX)/bin

CC = $(BIN)/$(PREFIX)-gcc
CFLAGS = -ffreestanding -mcpu=cortex-a72 -Og -Wall -Werror
LD = $(BIN)/$(PREFIX)-ld
LDFLAGS = -T link.ld
OCP = $(BIN)/$(PREFIX)-objcopy

all: kernel.img
clean:
	rm -rf *.o *.img *.bin *.elf

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@

kernel.img: $(OBJS)
	$(LD) $(LDFLAGS) $^ -o kernel.elf
	$(OCP) -Obinary kernel.elf kernel.img

.PHONY: clean
