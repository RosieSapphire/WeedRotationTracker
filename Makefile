APP := weedtracker
SRC := main.c
OBJ := $(patsubst %.c,%.o,$(SRC))

CFLAGS := -std=c2x -Wall -Wextra -Werror -ggdb3 -O0

default: $(APP)

$(APP): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(APP) $(OBJ)
