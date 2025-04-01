# 🚀 Voxel Engine Roadmap (ADHD-Friendly)

## 🔥 Immediate Wins
- [ ] Add chunk bounding box visualization
- [ ] Implement 5-line frustum culling
- [ ] Create debug chunk counter

## 🎯 Today's Goals
1. **Memory**
   - [ ] Chunk pool allocator
   - [ ] LRU cache tracker

2. **Rendering**
   - [ ] Instanced batches
   - [ ] Material atlas

3. **Chunks**
   - [ ] Neighbor awareness
   - [ ] Dirty flag system

## 🛠️ Tomorrow's Tasks
1. **Procedural Gen**
   - [ ] Add FastNoiseLite
   - [ ] Basic heightmap

2. **Interaction**
   - [ ] Voxel raycasting
   - [ ] Place/break system

3. **Performance**
   - [ ] Frame timers
   - [ ] 2-level LOD

## 📈 Day 3: Polish
1. **Optimization**
   - [ ] Compute shader meshes
   - [ ] Buffer recycling

2. **Saving**
   - [ ] Chunk diffing
   - [ ] .chunkdiff format

3. **UI**
   - [ ] Debug panel
   - [ ] Heatmap vis

## ⚡ Quick Boosters
- [ ] Tweak camera
- [ ] Rainbow texture
- [ ] Wireframe toggle
- [ ] Unload particles

## 🧠 ADHD Tips
1. Use 25min timers
2. Commit often
3. Physical notepad
4. Compile frequently

```

██████████████████████████████████████████
█           Core Architecture           █
██████████████████████████████████████████
                ▲           ▲
                │           │
        ┌───────┴───────┐ ┌─┴──────────┐
        ▼               ▼ ▼            ▼
┌─────────────┐   ┌─────────────┐   ┌─────────────┐
│  World      │   │ Chunk       │   │ Rendering   │
│ - Chunk Grid│◄─►│ Manager     │◄─►│ Pipeline    │
│ - Entities  │   │ - Loading   │   │ - Instancing│
└──────┬──────┘   │ - Unloading │   │ - Culling   │
       │          └──────▲──────┘   └──────▲──────┘
       │                 │                 │
       ▼                 │                 │
┌─────────────┐          │                 │
│ Event       │          │                 │
│ System      │──────────┘                 │
│ - Tile      │                            │
│   Triggers  │────────────────────────────┘
└─────────────┘

text
Copy

███ Chunk Lifecycle ███████████████████████
 Unloaded → Loading → Active ↔ Modified
    ▲          │          ▲
    └──────────┴─────◄────┘
              (Unloading)

Components:
- Loading: Mesh gen, neighbor detection
- Active: In memory, rendered
- Modified: Queued for saving
- Unloading: Serialize & release



████ Rendering Pipeline ███████████████████
Chunk Data → Frustum Culling → Batch Prep
    │                           │
    ▼                           ▼
LOD Selection             Material Sorting
    │                           │
    ▼                           ▼
GPU Upload ◄─────────────── Instancing
    │
    ▼
Draw Calls → Post Processing → Screen


████ Data Flow ████████████████████████████
Player → Event System → Chunk Manager
                           │
World State ←──────────────┤
                           ▼
                 Rendering Pipeline → GPU
                           ▼
                   Frame Presentation


███ Event System Flow █████████████████████
Input → Event Queue → Spatial Partitioning
                        │
                        ▼
                Chunk Processing Queue
                        │
                        ▼
               Game Logic & World Mods
                        │
                        ▼
                 Render Updates ◄─┐
                        │         │
                        └─────────┘


████ Voxel Memory Layout (16³) ███████████
┌───────────────────────────────┐
│ Chunk Header                  │
│ - Position                    │
│ - Neighbor ptrs [6]           │
│ - LOD level                   │
├───────────────────────────────┤
│ Voxel Data (4096)             │
│ - 4 bytes per voxel           │
│   (RGBA + matID)              │
├───────────────────────────────┤
│ Metadata                      │
│ - Light levels                │
│ - Modified timestamp          │
└───────────────────────────────┘


████ Debug HUD ████████████████████████████
┌─────────────────────────────────────────┐
│ Chunks: 142/1024  │ FPS: 72            │
│ VRAM: 1.2GB/2.0GB │ Draws: 86          │
│ CPU: 14ms         │ GPU: 9ms           │
│ LODs: 3.2k L0     │ 1.1k L1            │
└─────────────────────────────────────────┘

```
