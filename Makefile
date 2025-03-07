LIB		= HavenLib.a

SRC		+= $(wildcard $(SRC_DIR)entity/*.c)
SRC		+= $(wildcard $(SRC_DIR)audio/*.c)
SRC		+= $(wildcard $(SRC_DIR)core/*.c)
SRC		+= $(wildcard $(SRC_DIR)debug/*.c)
# SRC		+= $(wildcard $(SRC_DIR)networking/*.c)
SRC		+= $(wildcard $(SRC_DIR)phyic/*.c)
SRC		+= $(wildcard $(SRC_DIR)render/*.c)
SRC		+= $(wildcard $(SRC_DIR)scene/*.c)

OBJ		= $(SRC:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)

EXEC		= Haven Engine

EXEC_SRC	= $(SRC_DIR)main.c

EXEC_OBJ	= $(EXEC_SRC:%.c=%.o)


INCLUDE		= -I include

BUILDDIR	= build/
OBJ_DIR		= obj/
SRC_DIR		= source/

CC = gcc
CFLAGS = -std=c99

ifeq ($(OS), Windows_NT)
	INCLUDE += -I C:/mingw64/include
	LIBS = -lraylib -lopengl32 -lgdi32 -lwinmm
else ifeq ($(shell uname -s), Linux)
	CFLAGS += -fsanitize=address
	LIBS = -lasan -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
endif

all:		lib editor

$(BUILDDIR)$(LIB) : $(OBJ)
	mkdir -p $(BUILDDIR)
	ar rcs -o $@ $(OBJ)

$(OBJ): $(OBJ_DIR)%.o : $(SRC_DIR)%.c
	mkdir -p $(OBJ_DIR)
	mkdir -p $(OBJ_DIR)entity
	mkdir -p $(OBJ_DIR)audio
	mkdir -p $(OBJ_DIR)core
	mkdir -p $(OBJ_DIR)debug
	mkdir -p $(OBJ_DIR)networking
	mkdir -p $(OBJ_DIR)phyic
	mkdir -p $(OBJ_DIR)render
	mkdir -p $(OBJ_DIR)scene
	$(CC) $(CFLAGS) $(LIBS) ${INCLUDE} -c $< -o $@


$(BUILDDIR)$(EXEC) : $(OBJ_DIR)$(EXEC_OBJ)
	mkdir -p $(BUILDDIR)
	$(CC) $(OBJ_DIR)$(EXEC_OBJ) $(BUILDDIR)$(LIB) $(LIBS) -o $@

$(EXEC_OBJ): $(OBJ_DIR)%.o : $(SRC_DIR)%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) ${INCLUDE} -c $< -o $@


lib:		$(BUILDDIR)${LIB}

editor:		$(BUILDDIR)$(EXEC)

clean:
	rm -rf $(OBJ)

fclean:		clean
	rm -rf $(BUILDDIR)

re:		fclean all
