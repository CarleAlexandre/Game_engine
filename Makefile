LIB = flib.a

GAME = game

EDITOR = EDITOR

SRC += source/struct.cpp

OBJ = $(SRC:%.cpp = %.o)

ifeq ($(OS), Windows_NT)
CFLAGS		+=	-I C:/mingw64/include -I include
LIBS		+=	-lraylib -lopengl32 -lgdi32 -lwinmm -lstdc++ -latomic
endif
ifeq ($(shell uname -s), Linux)
CFLAGS		+=	-fsanitize=address -I include
LIBS		+=	-lasan -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -lstdc++
endif

all:

re:

clean:

fclean:
