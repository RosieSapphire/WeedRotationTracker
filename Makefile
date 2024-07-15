VERBOSE ?= 0
ifeq ($(VERBOSE), 1)
	V :=
else
	V := @
endif

BUILD_DIR := build
TARGET := weedtracker-gui

CC := gcc
CFLAGS := -O3 -Wall -Wextra -Werror -pedantic -std=gnu89

C_FILES := main.c
O_FILES := $(C_FILES:%.c=$(BUILD_DIR)/%.o)

final: $(TARGET)

$(TARGET): $(O_FILES)
	@echo "    [LD] $@"
	$(V)$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: %.c
	@mkdir -p "$(dir $@)"
	@echo "    [CC] $@"
	$(V)$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo Cleaning Previous Build...
	$(V)rm -rf $(TARGET) $(BUILD_DIR) .cache log.txt compile_commands.json

FORMAT_SCAN := $(C_FILES)

format:
	clang-format -i --style=file $(FORMAT_SCAN)
