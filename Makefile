# Top-Level Makefile

# Subdirectory names
SRC_DIR = src
TEST_DIR = test
BIN_DIR = bin

# Default target
all: $(BIN_DIR)/lib_simpleloader.so $(TEST_DIR)/fib

# Rule to build the shared library in the bin directory
$(BIN_DIR)/lib_simpleloader.so: $(SRC_DIR)/Makefile
	$(MAKE) -C $(SRC_DIR)

# Rule to build the fib executable in the test directory
$(TEST_DIR)/fib: $(TEST_DIR)/Makefile
	$(MAKE) -C $(TEST_DIR)

# Rule to create the bin directory if it does not exist
$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

# Clean all targets
clean:
	$(MAKE) -C $(SRC_DIR) clean
	$(MAKE) -C $(TEST_DIR) clean
	@rm -f $(BIN_DIR)/lib_simpleloader.so

.PHONY: all clean

