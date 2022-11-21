FOLDERS := $(patsubst %/, %, $(sort $(dir $(wildcard */*))))
FOLDERS := $(filter-out aarch64-none-elf, $(FOLDERS))
FOLDERS := $(patsubst %, clean/%, $(FOLDERS))

clean/%:
	$(MAKE) -C $* clean-simple

clean: $(FOLDERS)

.PHONY: clean
