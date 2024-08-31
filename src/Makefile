# src/Makefile

# Compiler
CC = gcc

# Compilation flags
CFLAGS = -m32 -fPIC

# Output library
OUTPUT = ../bin/lib_simpleloader.so

# Default target
all: $(OUTPUT)

# Rule to compile loader.c into a shared library
$(OUTPUT): loader.c
	$(CC) $(CFLAGS) -shared -o $@ $<

# Clean rule to remove the compiled output
clean:
	@rm -f $(OUTPUT)

.PHONY: all clean

