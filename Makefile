LOGGING_ENABLED := 1

BUILD_DIR := build
TARGET := weedtracker
INC_DIRS := include
SRC_DIRS := src
H_FILES := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.h))
C_FILES := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
O_FILES := $(C_FILES:%.c=$(BUILD_DIR)/%.o)

OPT := -O3
CFLAGS := $(OPT) $(INC_DIRS:%=-I%) -Wall -Wextra -Werror -std=c89 -pedantic
ifeq ($(LOGGING_ENABLED), 1)
	CFLAGS += -DLOGGING_ENABLED
endif

default: $(TARGET)

$(TARGET): $(O_FILES)
	@echo "    [LD] $@"
	@$(CC) $(CFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "    [CC] $@"
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo Cleaning Previous Build...
	@rm -rf $(TARGET) $(BUILD_DIR)

BETTY_IGNORE :=
BETTY_FLAGS := -strict -subjective --show-types \
	       --allow-global-variables $(BETTY_IGNORE)
BETTY_SCAN := $(H_FILES) $(C_FILES)

betty:
	betty-style $(BETTY_FLAGS) $(BETTY_SCAN)
