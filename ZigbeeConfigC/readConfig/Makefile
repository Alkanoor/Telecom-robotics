OBJ_DIR = ../objs
RELEASE_DIR = ../bin
RELEASE_DEBUG_DIR = ../binDebug

EXEC=$(RELEASE_DIR)/readConfig
EXEC_DEBUG=$(RELEASE_DEBUG_DIR)/readConfig
SRC= $(wildcard *.c)
OBJ= $(OBJ_DIR)/$(SRC:.c=.o)


all: $(EXEC)

makeDebug: $(EXEC_DEBUG)

$(EXEC): $(OBJ)
	@$(CC) -o $@ $^ $(LDFLAGS)

$(EXEC_DEBUG): $(OBJ)
	@$(CC) -o $@ $^ $(LDFLAGS_DEBUG)

$(OBJ_DIR)/%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS)

clean :
