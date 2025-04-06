# GAME ENGINE

## World of Haven : Chaos dungeons

	Je suis en train de développer un moteur de jeu écrit en c99, ayant pour but de créer un jeu rogue-like dans un univers Voxel.

## networking

## input

## thread

in c99 how does a struct is handled when accessing it's member from multiple thread, do i need a mutex for all of the struct or only for it's member ?

like i have this struct :

```c
static struct {
	queue_t		async_tasks;
	pthread_mutex_t	async_mutex;

	queue_t		sync_tasks;
	pthread_mutex_t	sync_mutex;

	queue_t		completed_tasks;

	bool		running;
	pthread_mutex_t	running;

	int		next_task_id;
	pthread_mutex_t	status_mtx;

	pthread_t	workers[32];
}    thread_mgr;
```
but should i use a mutex for the struct instead ? is it defined behavior or not ?

answer:

each member is a piece of memory.   a 'struct'  is not a 'real' thing..   it's just a conveneint way for us as programmers to group these related things.
mutex is required for modifying any piece of memory ..   so accessing each item would require the use of a mutex.    but it's not very practical to have 10 mutexes.  so you have 1 and that serves the purpose of accessing any of them
then there are exceptions for this on platforms like x86.. where WORD sized items can to read/wrote atomically. 
think about changing or reading things as  a single transaction..    if the transaction requires more than 1 member..  you need a mutex.

yes okay thanks i had a misconception about how struct work (i thinked of them as memory address with different data type contiguous in memory), but okay thanks, so  i can access different member at the same time, thanks for your great answer

yes.. they do appear in memory next to each other..  but there is nothing in code or memory  that tells the cpu something is a struct
NOUVEAU
[4:47 pm]
it stops being a struct once it's in binary/asm.

it's up to you to decide which things you want to associate to a mutex.
it's just very typical programmer/human that decides the things will be 'this struct'  or 'this namespace' or  these 3 int's.  or a complete subsystem.

on x86?     you'll find anything on the same 64byte span of memory is going to have the most need for mutex. 

this discussion is thanks to AJ the goat.

## Graphic



## Sound

## AI

## game loop


## physic

## animation

## voxel items

## terrain gen ::

/*
	first generate terrain
	then change block type
	then generate caves
	then add minerals
	then generate hydrometrie
	then add erosion and water material displacement
	then add structures
*/

### Idea

	discret non euclidian voxel element;

	make the player discover a little bit of the world by himself

	Procedural Dungeon with mob spawn

	Wave mod in certain handcrafted dungeon

	sql >> nosql;
	mongodb; atlas;
	
	elastic search

	collection pour reduire base de donnee;

	Optimisation des index;g

	Commerce de dofus, et le temps passer sur un craft item /fm augmente ca valeur focus et valeur des auugmentes en fonction des build
	
	i could add a system with different faction and aggro level

## CREDIT

	raylib
	raygui
	librg
	fastNoiseLight
	enet.h

## TODO 

	setup and update of shader

	engine setup


	create voxel mesh

	render voxel
	render one voxel
	render one chunk
	render all chunk

## Naration Idea

```
	Day 0 is when you start the game.

	Tutorial phase

	Day 12, the force of the shadow advance and start to build a bridge

	First game loop (gather ressources and combat)

	Day 20 The bridge is finished

	mid game, dungeon and exploring the mainland

	Day 50 the force advance through the middle island 

	Start introducing the full picture of the story
	player need to reenforce is base

	Day 75 The second bridge start building

	same gameloop as before

	Day 100 the scond bridge is finished and the shadow advance through the mainland

	Start to defend Your City (free build) (biger and biger wave).

	if player start to explore, he enter the third gameloop
	where

	when the player advance to the evil domain (south island), go to the late game where you start your journey to conquer the forgotten land

	you can go back but explored and conquered land will be lost

	the point of the late game is to use what you aquired trhough the early and mid game to go as far as you can, and when you finish you unlock the good ending,
	bad ending when you lose your base,
	neutral / Unending when cannot go to terme on the 999 days (hidden success).


	Pendant plus de 10000 ans, les manikins on prospère sous la montagne. Leurs royaumes s'étend à perte de vue sous terre, les gemmes et les minéraux brillent telle des étoiles donnant le sentiment de ne pas etre enterre, sur le plafond, soutenue par d'imposantes statues.
	Les murs sont recouverts d'habitations avec des mécanismes de toutes sortes.
```
