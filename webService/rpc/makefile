.PHONY: all clean setup-client test

# FLAGS AND COMPILER
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
PROXY_OBJ = $(OBJ_DIR)rpc_proxy.o $(OBJ_DIR)rpc_clnt.o $(OBJ_DIR)rpc_xdr.o $(OBJ_DIR)limits.o $(OBJ_DIR)utils.o
SRV_OBJ   = $(OBJ_DIR)timestamp.o $(OBJ_DIR)rpc_server.o $(OBJ_DIR)rpc_svc.o $(OBJ_DIR)rpc_xdr.o $(OBJ_DIR)limits.o  $(OBJ_DIR)utils.o
SRV_TEST_OBJ = $(OBJ_DIR)timestamp.o $(OBJ_DIR)limits.o $(OBJ_DIR)utils.o $(OBJ_DIR)test_rpc_server.o


# Client/Server
CLIENTES = $(BIN_DIR)app-cliente
SERVIDOR = $(BIN_DIR)rpc_server
TEST_SRV = $(BIN_DIR)test_rpc_server

# ALL
all: directories rpcgen_files $(BIN_DIR)$(LIBNAME) $(CLIENTES) $(SERVIDOR) $(TEST_SRV)
	@echo "Build complete!"
	@echo "All tests built into $(BIN_DIR)"
	@echo "Run 'source ../scripts/setup.sh' in your BASH TERMINAL to set environment variables."

# DIRS
directories:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)

# ENV
setup-client:
	@echo "Run 'source ../scripts/setup.sh' in your BASH TERMINAL to set environment variables."

# CLEAN
clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)
	#find $(INCLUDES_DIR) -type f ! -name "*.x" -delete

# RPCGEN
rpcgen_files:
	#cd $(INCLUDES_DIR) && rm -f *.c *.h *.o
	#cd $(INCLUDES_DIR) && rpcgen -MNa rpc.x



# LIBTIMESTAMP: generar la .so en bin/ con rpath propio
$(BIN_DIR)$(LIBNAME): $(PROXY_OBJ)
	$(CC) -shared -o $@ $^ $(LDLIBS) -Wl,-rpath,"$(CURRENT_DIR)/$(BIN_DIR)"

# CLIENTES:
$(BIN_DIR)%: %.c $(BIN_DIR)$(LIBNAME)
	$(CC) $(CFLAGS) -o $@ $< \
	-L$(BIN_DIR) -ltimestamp \
	$(LDLIBS) \
	-Wl,-rpath,"$(CURRENT_DIR)/$(BIN_DIR)"

# SERVIDOR:
$(SERVIDOR): $(SRV_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

# TEST SERVIDOR:
$(TEST_SRV): $(SRV_TEST_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)


# OBJECTS:
$(OBJ_DIR)timestamp.o: $(INCLUDES_DIR)timestamp.c $(INCLUDES_DIR)timestamp.h 
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)limits.o: $(INCLUDES_DIR)limits.c $(INCLUDES_DIR)limits.h 
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)utils.o: $(INCLUDES_DIR)utils.c $(INCLUDES_DIR)utils.h 
	$(CC) $(CFLAGS) -c $< -o $@

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

$(OBJ_DIR)test_rpc_server.o: $(TESTS_DIR)test_rpc_server.c 
	$(CC) $(CFLAGS) -I$(INCLUDES_DIR) -c $< -o $@









