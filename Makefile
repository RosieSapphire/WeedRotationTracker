APP := weedtracker
SRC := main.c
OBJ := $(patsubst %.c,%.o,$(SRC))

CFLAGS := -Wall -Wextra -Werror -ansi -pedantic -ggdb3 -O0 -s

default: $(APP)

$(APP): $(OBJ)
	$(CC) $(CFLAGS) -s $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(APP) $(OBJ)
