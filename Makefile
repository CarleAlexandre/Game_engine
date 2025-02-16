LIB = flib.a

GAME = game

EDITOR = editor

SERVER = server

SRC += $(wildcard source/*.c)

GAME_SRC = main/game.c

EDITOR_SRC = main/editor.c

SERVER_SRC = main/server.c

OBJ = $(SRC:%.c=%.o)

GAME_OBJ = $(GAME_SRC:%.c=%.o)

EDITOR_OBJ = $(EDITOR_SRC:%.c=%.o)

SERVER_OBJ = $(SERVER_SRC:%.c=%.o)

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

$(OBJ): %.o : %.c
	$(CC) $(CFLAGS) $(LIBS) ${INCLUDE} -c $< -o $@

$(GAME_OBJ): %.o : %.c
	$(CC) $(CFLAGS) ${INCLUDE} -c $< -o $@

$(EDITOR_OBJ): %.o : %.c
	$(CC) $(CFLAGS) ${INCLUDE} -c $< -o $@

$(SERVER_OBJ): %.o : %.c
	$(CC) $(CFLAGS) ${INCLUDE} -c $< -o $@

all: lib game editor server

lib: $(BUILDDIR)${LIB}

game: $(BUILDDIR)$(GAME) $(BUILDDIR)${LIB}

editor: $(BUILDDIR)$(EDITOR) $(BUILDDIR)${LIB}

server: $(BUILDDIR)$(SERVER) $(BUILDDIR)${LIB}

clean:
	rm -rf $(OBJ)
	rm -rf $(GAME_OBJ)
	rm -rf $(EDITOR_OBJ)
	rm -rf $(SERVER_OBJ)

fclean: clean
	rm -rf $(BUILDDIR)

re: fclean all
