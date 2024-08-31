# Default target
all: bin/lib_simpleloader.so test/fib

# Rule to build the shared library in the bin directory
bin/lib_simpleloader.so: src/Makefile
	$(MAKE) -C src

# Rule to build the fib executable in the test directory
test/fib: test/Makefile
	$(MAKE) -C test

# Rule to create the bin directory if it does not exist
bin:
	@mkdir -p bin

# Clean all targets
clean:
	$(MAKE) -C src clean
	$(MAKE) -C test clean
	@rm -f bin/lib_simpleloader.so

.PHONY: all clean

