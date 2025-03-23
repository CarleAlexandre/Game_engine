# Compiler and flags
CC = gcc
CFLAGS = -std=c99 -g -Wall
LDFLAGS = 

# Directories
BUILDDIR = build/
OBJ_DIR = obj/
SRC_DIR = source/

# Library
LIB = HavenLib.a

# Sources and objects for the library
SRC	+= $(wildcard $(SRC_DIR)core/*.c)
$SRC	+= $(wildcard $(SRC_DIR)render/*.c)
SRC	+= $(wildcard $(SRC_DIR)audio/*.c)
SRC	+= $(wildcard $(SRC_DIR)entity/*.c)
$SRC	+= $(wildcard $(SRC_DIR)physics/*.c)
$SRC	+= $(wildcard $(SRC_DIR)item/*.c)


OBJ = $(SRC:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)

# Executables
EXEC = Game
EDITOR = HavenEditor

EXEC_SRC = $(SRC_DIR)main.c
EDITOR_SRC = $(SRC_DIR)editor.c

EXEC_OBJ = $(EXEC_SRC:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)
EDITOR_OBJ = $(EDITOR_SRC:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)

# Includes and libraries
INCLUDE = -Iinclude

# Platform-specific configurations
ifeq ($(OS), Windows_NT)
    INCLUDE += -IC:/mingw64/include
    LIBS = -lglfw -lraylib -lgl -lgdi32 -lwinmm -lc
else ifeq ($(shell uname -s), Linux)
    CFLAGS += -fsanitize=address
    LDFLAGS += -fsanitize=address
    LIBS = -lglfw -lraylib -lm -lpthread -ldl -lrt -lX11 -lc
endif

# Default target
all: lib $(EXEC) $(EDITOR)

# Pattern rule for object files
$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

# Static library
$(BUILDDIR)$(LIB): $(OBJ)
	@mkdir -p $(BUILDDIR)
	ar rcs $@ $^

# Game executable
$(EXEC): $(EXEC_OBJ) $(BUILDDIR)$(LIB)
	@mkdir -p $(BUILDDIR)
	$(CC) $(LDFLAGS) $^ $(LIBS) -o $(BUILDDIR)$@

# Editor executable
$(EDITOR): $(EDITOR_OBJ) $(BUILDDIR)$(LIB)
	@mkdir -p $(BUILDDIR)
	$(CC) $(LDFLAGS) $^ $(LIBS) -o $(BUILDDIR)$@

# Dynamic library target
dynamic: CFLAGS += -fPIC
dynamic: LDFLAGS += -shared
ifeq ($(OS), Windows_NT)
    dynamic: $(BUILDDIR)$(LIB:.a=.dll)
else
    dynamic: $(BUILDDIR)$(LIB:.a=.so)
endif

# Windows DLL
$(BUILDDIR)$(LIB:.a=.dll): $(OBJ)
	@mkdir -p $(BUILDDIR)
	$(CC) $(LDFLAGS) $^ $(LIBS) -o $@

# Unix shared object
$(BUILDDIR)$(LIB:.a=.so): $(OBJ)
	@mkdir -p $(BUILDDIR)
	$(CC) $(LDFLAGS) $^ $(LIBS) -o $@


# Cleanup
clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(BUILDDIR)

.PHONY: all lib $(EXEC) $(EDITOR) clean fclean re dynamic