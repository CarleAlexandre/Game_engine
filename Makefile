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
SRC = $(wildcard $(SRC_DIR)entity/*.c) \
      $(wildcard $(SRC_DIR)audio/*.c) \
      $(wildcard $(SRC_DIR)core/*.c) \
      $(wildcard $(SRC_DIR)debug/*.c) \
      $(wildcard $(SRC_DIR)physics/*.c) \
      $(wildcard $(SRC_DIR)item/*.c) \
      $(wildcard $(SRC_DIR)render/*.c) \
      $(wildcard $(SRC_DIR)scene/*.c)
OBJ = $(SRC:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)

# Executables
EXEC = Game
EDITOR = HavenEditor  # Removed space to avoid issues

EXEC_SRC = $(SRC_DIR)main.c
EDITOR_SRC = $(SRC_DIR)editor.c

EXEC_OBJ = $(EXEC_SRC:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)
EDITOR_OBJ = $(EDITOR_SRC:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)

# Includes and libraries
INCLUDE = -Iinclude

# Platform-specific configurations
ifeq ($(OS), Windows_NT)
    INCLUDE += -IC:/mingw64/include
    LIBS = -lglfw -lvulkan -lraylib -lgl -lgdi32 -lwinmm
else ifeq ($(shell uname -s), Linux)
    CFLAGS += -fsanitize=address
    LDFLAGS += -fsanitize=address
    LIBS = -lglfw -lvulkan -lm -lpthread -ldl -lrt -lX11
endif

# Default target
all: lib $(EXEC) $(EDITOR)
.PHONY: all lib $(EXEC) $(EDITOR) clean fclean re

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

# Cleanup
clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(BUILDDIR)

re: fclean all
