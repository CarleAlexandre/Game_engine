LIB = flib.a

GAME = game

EDITOR = editor

SERVER = server

UNIT = unitest

SRC += $(wildcard source/*.c)

GAME_SRC = main/game.c

EDITOR_SRC = main/editor.c

SERVER_SRC = main/server.c

UNIT_SRC = main/unitest.c

OBJ = $(SRC:%.c=%.o)

GAME_OBJ = $(GAME_SRC:%.c=%.o)

EDITOR_OBJ = $(EDITOR_SRC:%.c=%.o)

SERVER_OBJ = $(SERVER_SRC:%.c=%.o)

UNIT_OBJ = $(UNIT_SRC:%.c=%.o)

INCLUDE += -I include -I source

BUILDDIR = build/

CC = gcc

CFLAGS = -std=c99

ifeq ($(OS), Windows_NT)
CFLAGS += -I C:/mingw64/include
LIBS += -lraylib -lopengl32 -lgdi32 -lwinmm -latomic
endif
ifeq ($(shell uname -s), Linux)
CFLAGS += -fsanitize=address
LIBS += -lasan -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
endif

all: unit lib game editor server 

$(BUILDDIR)$(LIB) : $(OBJ)
	mkdir -p $(BUILDDIR)
	ar rcs -o $@ $(OBJ)

$(BUILDDIR)$(GAME) : $(GAME_OBJ)
	mkdir -p $(BUILDDIR)
	$(CC) $(GAME_OBJ) $(BUILDDIR)$(LIB) $(LIBS) -o $@

$(BUILDDIR)$(EDITOR) : $(EDITOR_OBJ)
	mkdir -p $(BUILDDIR)
	$(CC) $(EDITOR_OBJ) $(LIBS) -o $@ $(BUILDDIR)$(LIB)

$(BUILDDIR)$(SERVER) : $(SERVER_OBJ)
	mkdir -p $(BUILDDIR)
	$(CC) $(SERVER_OBJ) $(LIBS) -o $@ $(BUILDDIR)$(LIB)

$(BUILDDIR)$(UNIT) : $(UNIT_OBJ)
	mkdir -p $(BUILDDIR)
	$(CC) $(UNIT_OBJ) $(LIBS) -o $@

$(OBJ): %.o : %.c
	$(CC) $(CFLAGS) $(LIBS) ${INCLUDE} -c $< -o $@

$(GAME_OBJ): %.o : %.c
	$(CC) $(CFLAGS) ${INCLUDE} -c $< -o $@

$(EDITOR_OBJ): %.o : %.c
	$(CC) $(CFLAGS) ${INCLUDE} -c $< -o $@

$(SERVER_OBJ): %.o : %.c
	$(CC) $(CFLAGS) ${INCLUDE} -c $< -o $@

$(UNIT_OBJ): %.o : %.c
	$(CC) $(CFLAGS) ${INCLUDE} -c $< -o $@

lib: $(BUILDDIR)${LIB}

game: $(BUILDDIR)$(GAME) $(BUILDDIR)${LIB}

editor: $(BUILDDIR)$(EDITOR) $(BUILDDIR)${LIB}

server: $(BUILDDIR)$(SERVER) $(BUILDDIR)${LIB}

unit: $(BUILDDIR)$(UNIT)

clean:
	rm -rf $(OBJ)
	rm -rf $(GAME_OBJ)
	rm -rf $(EDITOR_OBJ)
	rm -rf $(SERVER_OBJ)
	rm -rf $(UNIT_OBJ)

fclean: clean
	rm -rf $(BUILDDIR)

re: fclean all
