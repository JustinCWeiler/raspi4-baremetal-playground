BIN = bin
OUT = out
SRC = src
INC = include

OBJS := $(wildcard $(SRC)/*.[cS])
OBJS := $(patsubst $(SRC)/%.c, $(BIN)/%.o, $(OBJS))
OBJS := $(patsubst $(SRC)/%.S, $(BIN)/%.o, $(OBJS))

CC = gcc
CFLAGS = -Og -Wall -Wextra -Werror -I$(INC) -MD
ASFLAGS =
LD = gcc
LDFLAGS =

all: $(OUT)/pi4-install
clean:
	rm -rf $(BIN) $(OUT)

$(BIN):
	mkdir $(BIN)
$(OUT):
	mkdir $(OUT)

$(BIN)/%.o: $(SRC)/%.c | $(BIN)
	$(CC) $(CFLAGS) -c $< -o $@
$(BIN)/%.o: $(SRC)/%.S | $(BIN)
	$(CC) $(ASFLAGS) -c $< -o $@

$(OUT)/pi4-install: $(OBJS) | $(OUT)
	$(LD) $(LDFLAGS) $^ -o $@

-include $(OBJS:.o=.d)

.PHONY: clean all
.SILENT: $(BIN) $(OUT)
