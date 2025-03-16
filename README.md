# HAVEN GAME ENGINE

___

this game engine is currently in very early stage.

it's a game engine written in c99 using opengl, raylib, FastNoiseLite, glad.

this engine is inteded to build voxel games with realistic terrain generation and procedural generation.

___

## World of Haven : Chaos dungeons

___

this will be the first game made with this engine.
___

## How to build

prerequisite:

windows:

- need raylib, mingw with gcc and pthread, and gnumake.

```sh
	make

	make all
	
	make unit
	
	make lib
	
	make game
	
	make editor
	
	make server 
```

linux:

- need raylib
- not finished yet

mac os:

- not supported for now

___

## DESIGN

every function should be like this:

	[haven]_[module]_[submodule]_[use]();

	ex:

	haven_alloc_pool_init();
	or
	haven_thread_mgr_init();

___

## CREDIT

	raylib
	raygui
	librg
	fastNoiseLight
	enet.h

___

## TODO:

 - setup d'un ide avec d3ebvugger (gdb)

 - allocateur pour data_type (bump allocator pour dyn _ array ? )

 - dynamic array (gestion de l'alignement memoire)

___
