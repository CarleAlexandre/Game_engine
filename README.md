# GAME ENGINE

## World of Haven : Chaos dungeons

	Je suis en train de développer un moteur de jeu écrit en c99, ayant pour but de créer un jeu rogue-like dans un univers Voxel.

## networking

## input

## thread

## Graphic



## Sound

## AI

## game loop

###1. Define Your Narrative Goals

	Establish Core Themes and Structures

    Core Themes: Decide on the overarching themes or motifs (e.g., mystery, survival, redemption) that will be echoed in the generated stories.
    Narrative Arcs: Even in procedurally generated content, a basic narrative arc (introduction, conflict, climax, resolution) can help maintain coherence.
    Player Agency: Consider how much control the player will have in influencing the story. In roguelikes, emergent narratives often arise from the interplay between random events and player choices.

###2. Techniques for Procedural Story Generation

	Modular Narrative Building:

    Narrative Fragments: Break the story into smaller, reusable segments (such as events, quests, or dialogue snippets) that can be assembled in various ways.
    Grammar-Based Systems: Use formal grammar rules or templates to construct sentences or narrative segments dynamically. For instance, a context-free grammar can generate different story outcomes by recombining predefined elements.

	Graph-Based and State Machine Approaches:

    Story Graphs: Create nodes representing narrative events and edges as transitions between them. This graph can be traversed differently each playthrough, ensuring varied storylines while adhering to logical narrative progression.
    Finite State Machines: Use state machines to model story states and transitions triggered by player actions or random events. This helps manage narrative flow and ensure that the story progresses logically.

	Event-Driven and Emergent Narratives:

    Random Events and Triggers: Implement systems where random events (or player-triggered events) introduce narrative changes. These events can be linked to environmental factors or character interactions.
    Dynamic Quests: Generate quests on the fly by combining objectives, characters, and locations. Ensure that each quest fits within the broader narrative context, even if loosely.

	Balancing Randomness and Coherence:

    Constraints and Rules: Introduce constraints that prevent illogical or contradictory narrative outcomes. For example, if a character is established as an ally, subsequent events should not contradict that role unless there’s a narrative justification.
    Narrative “Anchors”: Design certain fixed or semi-fixed events that serve as anchors. These can provide a backbone to the story, ensuring that, despite procedural variability, the narrative maintains a coherent structure.

	###3. Algorithmic Approaches and Tools

	Algorithms and Techniques:

    Markov Chains: Use Markov chains to generate sequences of events or dialogue. This can work well for text-based narrative generation but may require careful tuning to maintain context over longer passages.
    Planning Algorithms: Techniques like Hierarchical Task Networks (HTNs) can be adapted to plan narrative sequences, ensuring that events follow a logical order.
    Machine Learning Approaches: Although more advanced, models trained on narrative data (or even transformers in a constrained setting) can suggest story elements or dialogue. However, integrating these effectively into game logic can be challenging.

	Integration with Narrative Scripting Tools:

    Narrative Scripting Languages: Tools like Ink or Yarn Spinner allow for branching narratives and can be combined with procedural systems. They let you write narrative nodes manually while incorporating dynamic elements via variables or external triggers.
    Hybrid Systems: Many games benefit from a mix of handcrafted and procedurally generated content. For example, you might use fixed story arcs with procedurally generated side quests that enrich the main narrative.

###4. Design Considerations for Roguelike Games

	Replayability and Variation:

    Randomization with Purpose: In roguelikes, players expect different experiences on each run. Ensure that your procedural system doesn’t just randomize elements but creates meaningful variations that affect gameplay and narrative.
    Emergent Storytelling: Allow the game world to react to player actions in unexpected ways. Emergent narratives arise when players combine procedural events with creative problem-solving.

	Balancing Complexity and Accessibility:

    Simplicity vs. Depth: While a highly complex narrative generator can be impressive, it might overwhelm both you and the player. Strive for a system that is deep enough to be engaging but simple enough to manage and debug.
    Player Feedback: Test the narrative generator with real players. Their feedback will help you balance narrative coherence with the variability that makes roguelikes fun.

	5. Resources and Further Reading

    Academic Papers and Articles: Research on procedural content generation (PCG) for narratives can provide insights into various algorithms and design philosophies. Look up studies on narrative generation and PCG in games.
    Game Postmortems: Many indie developers share their experiences with procedural narrative systems (for instance, insights from games like Dwarf Fortress or Rogue Legacy can be valuable).
    Online Communities and Forums: Communities like the Procedural Generation subreddit or game development forums (e.g., TIGSource, GameDev.net) can be great places to ask for advice and see how others have tackled similar challenges

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

	printf("UwU\n");
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
