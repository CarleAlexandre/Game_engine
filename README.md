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

## Design

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

	[module]_[submodule]_[use]();

	ex:

	alloc_pool_init();
	thread_mgr_init();


```
██████████████████████████████████████████
█       Hybrid Client-Server Flow       █
██████████████████████████████████████████

           ┌───────────────────┐            
           │     Game Client   │            
           │ ┌───────────────┐ │            
           │ │  Input        │ │   ┌───────┐
           │ │  Processing   ├─┼──►│ Local │
           │ └───────┬───────┘ │   │ Server│
           │         │         │   └───┬───┘
           │ ┌───────▼───────┐ │       │    
           │ │  Prediction  │ │       │    
           │ │  & Rendering │ │       │    
           │ └───────┬───────┘ │       │    
           └─────┬───┼───┬─────┘       │    
                 │   │   │           │    
                 ▼   ▼   ▼           ▼    
┌─────────────────────────────────────────┐
│           Shared Simulation Core        │
│ ┌──────────────────┐  ┌───────────────┐ │
│ │  Command Queue   │  │ World State   │ │
│ │ - Input buffer   │  │ - Chunk data  │ │
│ │ - Priority sorted│  │ - Entities    │ │
│ └────────┬─────────┘  └───────┬───────┘ │
│          │                    │         │
│ ┌────────▼────────────────────▼───────┐ │
│ │          Simulation Loop           │ │
│ │ 1. Process commands                │ │
│ │ 2. Update physics                  │ │
│ │ 3. Generate snapshots              │ │
│ └────────────────────────────────────┘ │
└───────────────────┬─────────────────────┘
                    │                    
          ┌─────────▼──────────┐         
          │  Snapshot Delivery │         
          │ - Local: Memcpy    │         
          │ - Remote: Network  │         
          └─────────┬──────────┘         
                    │                    
          ┌─────────▼──────────┐         
          │   Client State     │         
          │ Reconciliation     │         
          └────────────────────┘         
```

### Communication Flow

```	
 Client Side              Server Side               Shared
─────────────────────────────────────────────────────────────
[Input Polling]       [Command Queue]
      │                     │
      ├──User Input───────►│
      │                    │
[Prediction]          [Simulation Tick]
      │                    │
      │◄───World Snapshot──┤
      │                    │
[Render Interp.]      [State Broadcast]
      │                     │
      ├──Correction Req.──►│
      │                    │
[State Apply]         [Delta Update]
```

### Sequence Diagram

```
Local Mode                          Remote Mode
┌────────┐    ┌────────┐            ┌────────┐    ┌────────┐
│ Client │    │ Server │            │ Client │    │ Server │
└───┬────┘    └───┬────┘            └───┬────┘    └───┬────┘
    │ Input       │                     │ Input       │     
    │────────────►│                     │───via Net──►│     
    │             │ Simulate            │             │     
    │ Prediction  │───┐                 │ Prediction  │     
    │◄────────────│◄──┘                 │◄───via Net──│     
    │ Render      │                     │ Render      │     
    │             │                     │             │ 
```

### Data Model Relationships

```
WorldSnapshot ◄───┐         ┌───► InputCommand
       ▲          │         │          ▲
       │          ├─has a───┤          │
       │          │         │          │
       │     ┌────┴─────┐   │     ┌────┴─────┐
       └─────┤  World   │   └─────┤  Player  │
             │  State   │         │  State   │
             └──────────┘         └──────────┘
```

### Memory Layout Visualization

```
┌───────────────────────────┐
│ Client Memory             │
├─────────────┬─────────────┤
│ Predicted   │ Command     │
│ World State │ Buffer      │
│ (Write)     │ (Ring)      │
└──────┬──────┴──────┬──────┘
       │             │
       ▼             ▼
┌───────────────────────────┐
│ Shared Memory             │
├─────────────┬─────────────┤
│ Server      │ Snapshot    │
│ World State │ Buffer      │
│ (Read-Only) │ (Double)    │
└─────────────┴─────────────┘
```

### Performance Metrics

```
┌──────────────────────┬──────────────────────┐
│ Client               │ Server               │
├──────────────────────┼──────────────────────┤
│ FPS: 60              │ TPS: 20              │
│ Pred. Error: 0.2m    │ Cmd Lag: 12ms        │
│ Chunks Rendered: 142 │ Chunks Loaded: 256   │
│ VRAM: 1.4GB          │ RAM: 2.1GB           │
└──────────────────────┴──────────────────────┘
```

### Debug Visualization

```
┌─────────────────────────────────────────┐
│ Debug Overlay (F1)                      │
├─────────────────────────────────────────┤
│ Local Server: ACTIVE              [✔]   │
│ Snapshot Delay: 42ms              [❗]   │
│ Unacknowledged Commands: 3        [⚠]   │
│ Predicted vs Actual: 0.4m         [🔴]  │
└─────────────────────────────────────────┘
```

### Testing Strategy

```
┌───────────────────┬───────────────────────┐
│ Test Case         │ Visual Indicator      │
├───────────────────┼───────────────────────┤
│ Normal Operation  │ Green network icon    │
│ High Latency      │ Yellow ping counter   │
│ Prediction Error  │ Red position marker   │
│ Server Desync     │ Purple warning flash  │
└───────────────────┴───────────────────────┘
```


___

## CREDIT

	raylib
	raygui
	librg
	fastNoiseLight
	enet.h

___
