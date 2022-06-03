BIN = bin
SRC = src
INC = include

LDSCRIPT=$(SRC)/link.ld

PREFIX = aarch64-none-elf
CCBIN = ../$(PREFIX)/bin

OBJS := $(wildcard $(SRC)/*.[cS])
OBJS := $(patsubst $(SRC)/%.c, $(BIN)/%.o, $(OBJS))
OBJS := $(patsubst $(SRC)/%.S, $(BIN)/%.o, $(OBJS))

SPEED=-Og

CC = $(CCBIN)/$(PREFIX)-gcc
CFLAGS = -ffreestanding -mcpu=cortex-a72 $(SPEED) -Wall -Wextra -Werror -I$(INC) -MD
ASFLAGS = -mcpu=cortex-a72 -Wall -Wextra -Werror -I$(INC)
LD = $(CCBIN)/$(PREFIX)-ld
LDFLAGS =
OCP = $(CCBIN)/$(PREFIX)-objcopy
ODMP = $(CCBIN)/$(PREFIX)-objdump
AR = $(CCBIN)/$(PREFIX)-ar

all: libpi.a libpi.list
clean:
	rm -rf $(BIN) libpi.a

$(BIN):
	mkdir $(BIN)
$(OUT):
	mkdir $(OUT)

$(BIN)/%.o: $(SRC)/%.c | $(BIN)
	$(CC) $(CFLAGS) -c $< -o $@
$(BIN)/%.o: $(SRC)/%.S | $(BIN)
	$(CC) $(ASFLAGS) -c $< -o $@

libpi.a: $(OBJS)
	$(AR) rcsf libpi.a $(OBJS)

libpi.list: libpi.a
	$(ODMP) -D $< > $@

-include $(OBJS:.o=.d)

.PHONY: clean all
.SILENT: $(BIN) $(OUT)
