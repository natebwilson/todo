CC = cc
CFLAGS = -Wall -Wextra -pedantic -std=c99
LDFLAGS = -lncurses

SRC = main.c ui.c todo.c file.c utils.c
OBJ = $(SRC:.c=.o)

TARGET = todo

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

.PHONY: all clean

