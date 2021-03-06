export CC=gcc
export CFLAGS=-W -Wall
export LDFLAGS=
export LDFLAGS_DEBUG=-g

OBJ_DIR = objs
RELEASE_DIR = bin
RELEASE_DEBUG_DIR = binDebug

SEND_FILE=sendFile
READ_CONFIG=readConfig
CHANGE_CONFIG=changeConfig
EXEC=$(RELEASE_DIR)/operationZigbee
EXEC_DEBUG=$(RELEASE_DEBUG_DIR)/operationZigbee
SRC= $(wildcard *.c)
OBJ= $(OBJ_DIR)/$(SRC:.c=.o)


all: createDirs $(EXEC)

makeDebug: createDirsDebug $(EXEC_DEBUG)

createDirs:
	@mkdir -p $(RELEASE_DIR)
	@mkdir -p $(RELEASE_DEBUG_DIR)
	@mkdir -p $(OBJ_DIR)
	@(cd $(SEND_FILE) && $(MAKE))
	@(cd $(READ_CONFIG) && $(MAKE))
	@(cd $(CHANGE_CONFIG) && $(MAKE))

createDirsDebug:
	@mkdir -p $(RELEASE_DIR)
	@mkdir -p $(RELEASE_DEBUG_DIR)
	@mkdir -p $(OBJ_DIR)
	@(cd $(SEND_FILE) && $(MAKE) makeDebug)
	@(cd $(READ_CONFIG) && $(MAKE) makeDebug)
	@(cd $(CHANGE_CONFIG) && $(MAKE) makeDebug)

$(EXEC): $(OBJ)
	@$(CC) -o $@ $^ $(LDFLAGS)

$(EXEC_DEBUG): $(OBJ)
	@$(CC) -o $@ $^ $(LDFLAGS_DEBUG)

$(OBJ_DIR)/%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS)

clean :
	@rm -rf $(OBJ_DIR)/*
	@rm -rf $(RELEASE_DIR)/*
	@rm -rf $(RELEASE_DEBUG_DIR)/*
	@rm -rf $(OBJ_DIR)
	@rm -rf $(RELEASE_DIR)
	@rm -rf $(RELEASE_DEBUG_DIR)
	@(cd $(SEND_FILE) && $(MAKE) $@)
	@(cd $(READ_CONFIG) && $(MAKE) $@)
	@(cd $(CHANGE_CONFIG) && $(MAKE) $@)
	@echo "Suppression of subdirs objs and bin"
