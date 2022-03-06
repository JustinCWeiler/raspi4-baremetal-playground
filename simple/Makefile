BIN = bin
OUT = out
SRC = src
INC = include

PREFIX = aarch64-none-elf
CCBIN = ../$(PREFIX)/bin

OBJS = $(patsubst $(SRC)/%.S, $(BIN)/%.o, $(patsubst $(SRC)/%.c, $(BIN)/%.o, $(wildcard $(SRC)/*.[cS])))

CC = $(CCBIN)/$(PREFIX)-gcc
CFLAGS = -ffreestanding -mcpu=cortex-a72 -Og -Wall -Werror
LD = $(CCBIN)/$(PREFIX)-ld
LDFLAGS = -T $(SRC)/link.ld
OCP = $(CCBIN)/$(PREFIX)-objcopy

all: kernel.img
clean:
	rm -rf $(BIN) $(OUT)

$(BIN):
	mkdir $(BIN)
$(OUT):
	mkdir $(OUT)

$(BIN)/%.o: $(SRC)/%.c $(BIN)
	$(CC) $(CFLAGS) -c $< -o $@
$(BIN)/%.o: $(SRC)/%.S $(BIN)
	$(CC) $(CFLAGS) -c $< -o $@

kernel.img: $(OBJS) $(OUT)
	$(LD) $(LDFLAGS) $(filter-out $(OUT), $^) -o $(OUT)/kernel.elf
	$(OCP) -Obinary $(OUT)/kernel.elf $(OUT)/kernel.img

.PHONY: clean
