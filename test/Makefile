# test/Makefile

# Compiler and flags
CC = gcc
CFLAGS = -m32 -no-pie -nostdlib

# Output files
OUTPUT = ../bin/fib

# Default target
all: $(OUTPUT)

# Rule to build the executable
$(OUTPUT): fib.c
	$(CC) $(CFLAGS) -o $(OUTPUT) fib.c

# Clean up
clean:
	rm -f $(OUTPUT)

.PHONY: all clean


