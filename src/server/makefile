.PHONY: all clean test

CC = gcc
CFLAGS = -Wall -pthread
LDLIBS = -ltirpc 

# RUTAS
INCLUDES_DIR = includes/
TESTS_DIR   = tests/
OBJ_DIR     = obj/
BIN_DIR     = bin/
CURRENT_DIR := $(shell pwd)

# OBJ
SRV_OBJ   = $(OBJ_DIR)services.o $(OBJ_DIR)server.o  $(OBJ_DIR)server.o $(OBJ_DIR)limits.o  $(OBJ_DIR)utils.o $(OBJ_DIR)server_utils.o
SRV_TEST_OBJ = $(OBJ_DIR)services.o $(OBJ_DIR)limits.o $(OBJ_DIR)utils.o $(OBJ_DIR)test_server.o $(OBJ_DIR)server_utils.o


# Server/tests
SERVIDOR = $(BIN_DIR)server
TEST_SRV = $(BIN_DIR)test_server


all: directories $(SERVIDOR) $(TEST_SRV)
	@echo "Build complete!"
	@echo "All tests built into $(BIN_DIR)"

# DIRS
directories:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)


# CLEAN
clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)


# SERVIDOR:
$(SERVIDOR): $(SRV_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

# TEST SERVIDOR:
$(TEST_SRV): $(SRV_TEST_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)


# OBJECTS:
$(OBJ_DIR)limits.o: $(INCLUDES_DIR)limits.c $(INCLUDES_DIR)limits.h 
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)utils.o: $(INCLUDES_DIR)utils.c $(INCLUDES_DIR)utils.h 
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)server_utils.o: $(INCLUDES_DIR)server_utils.c $(INCLUDES_DIR)server_utils.h 
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)services.o: $(INCLUDES_DIR)services.c $(INCLUDES_DIR)services.h 
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)server.o: server.c $(INCLUDES_DIR)server_utils.h $(INCLUDES_DIR)services.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)test_server.o: $(TESTS_DIR)test_server.c 
	$(CC) $(CFLAGS) -I$(INCLUDES_DIR) -c $< -o $@
