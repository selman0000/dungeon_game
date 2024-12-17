# Compiler and flags
CC = gcc
CFLAGS = -Wall -g

# Target executable name
TARGET = dungeon_game

# Default rule: Compile the program
all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) -o $(TARGET) main.c

# Clean rule: Remove executable file
clean:
	rm -f $(TARGET)
