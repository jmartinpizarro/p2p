.PHONY: all clean test

CC = gcc
CFLAGS = -Wall -Wextra -g -fPIC -I/usr/include/tirpc -I$(INCLUDES_DIR)
LDLIBS = -ltirpc 
LIBNAME = libtimestamp.so
# RUTAS
INCLUDES_DIR = includes/
TESTS_DIR   = tests/
OBJ_DIR     = obj/
BIN_DIR     = bin/
CURRENT_DIR := $(shell pwd)

# OBJ
RCP_PROXY_OBJ = $(OBJ_DIR)rpc_proxy.o $(OBJ_DIR)rpc_clnt.o $(OBJ_DIR)rpc_xdr.o $(OBJ_DIR)limits.o $(OBJ_DIR)utils.o
RPC_SRV_OBJ = $(OBJ_DIR)timestamp.o $(OBJ_DIR)rpc_server.o $(OBJ_DIR)rpc_svc.o $(OBJ_DIR)rpc_xdr.o $(OBJ_DIR)limits.o  $(OBJ_DIR)utils.o
SRV_OBJ   = $(OBJ_DIR)services.o $(OBJ_DIR)server.o  $(OBJ_DIR)server.o $(OBJ_DIR)limits.o  $(OBJ_DIR)utils.o $(OBJ_DIR)server_utils.o
SRV_TEST_OBJ = $(OBJ_DIR)services.o $(OBJ_DIR)limits.o $(OBJ_DIR)utils.o $(OBJ_DIR)test_server.o $(OBJ_DIR)server_utils.o
RPC_SRV_TEST_OBJ = $(OBJ_DIR)timestamp.o $(OBJ_DIR)limits.o $(OBJ_DIR)utils.o $(OBJ_DIR)test_rpc_server.o

# Server/tests
SERVIDOR = $(BIN_DIR)server
TEST_SRV = $(BIN_DIR)test_server
SERVIDOR_RPC = $(BIN_DIR)rpc_server
TEST_RPC_SRV = $(BIN_DIR)test_rpc_server

all: directories $(BIN_DIR)$(LIBNAME) $(SERVIDOR) $(TEST_SRV) $(SERVIDOR_RPC) $(TEST_RPC_SRV)
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

# LIBTIMESTAMP: generar la .so en bin/ con rpath propio
$(BIN_DIR)$(LIBNAME): $(RCP_PROXY_OBJ)
	$(CC) -shared -o $@ $^ $(LDLIBS) -Wl,-rpath,"$(CURRENT_DIR)/$(BIN_DIR)"

#RPC_SERVIDOR: 
$(SERVIDOR_RPC): $(RPC_SRV_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

# SERVIDOR:
$(SERVIDOR): $(SRV_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ \
	-L$(BIN_DIR) -ltimestamp \
	$(LDLIBS) \
	-Wl,-rpath,"$(CURRENT_DIR)/$(BIN_DIR)"

# TEST SERVIDOR:
$(TEST_RPC_SRV): $(RPC_SRV_TEST_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

# TEST SERVIDOR RPC:
$(TEST_SRV): $(SRV_TEST_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

# OBJECTS:
$(OBJ_DIR)timestamp.o: $(INCLUDES_DIR)timestamp.c $(INCLUDES_DIR)timestamp.h 
	$(CC) $(CFLAGS) -c $< -o $@

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

$(OBJ_DIR)test_rpc_server.o: $(TESTS_DIR)test_rpc_server.c 
	$(CC) $(CFLAGS) -I$(INCLUDES_DIR) -c $< -o $@

$(OBJ_DIR)rpc_proxy.o: rpc_proxy.c $(INCLUDES_DIR)rpc.h $(INCLUDES_DIR)timestamp.h  $(INCLUDES_DIR)utils.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)rpc_clnt.o: $(INCLUDES_DIR)rpc_clnt.c $(INCLUDES_DIR)rpc.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)rpc_svc.o: $(INCLUDES_DIR)rpc_svc.c $(INCLUDES_DIR)rpc.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)rpc_server.o: rpc_server.c $(INCLUDES_DIR)rpc.h $(INCLUDES_DIR)timestamp.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)rpc_xdr.o: $(INCLUDES_DIR)rpc_xdr.c $(INCLUDES_DIR)rpc.h
	$(CC) $(CFLAGS) -c $< -o $@
