# Map Creation Guide

## Prerequisites

1. **Tiled Map Editor**: [Download here](https://www.mapeditor.org)
2. **Asset Structure**: Ensure standard folders exist:
    * `assets/maps` (JSON, tmx and tsx exports)
    * `assets/sprites` (PNG images)

## Understanding the Chunk System

The game world is **not** one giant image. It is a grid of **Chunks**.
* **The Golden Rule**: Every chunk should be exactly **1024x1025 pixels**.
* **Coordinate Logic**: The `ZoneManager` calculates which file to load based on the player's pixel position divided by 1024.

> **Note**: Interiors are technically just Chunks placed far away from the main game (e.g., at Chunk(-1, -1), so at coordinates `-1024, -1024`) so the player doesn't see them while walking outside.

## Creation Workflow

### 1. **Create the artwork**: Draw your ground/terrain in whichever app you prefer.
1. **Size**: Must be exactly **1024x1024 pixels**.
2. **Save as**: PNG (e.g., `map_0_0.png`).

### 2. **Create Tiled Map**:
1. Open Tiled -> New Map
2. **Size**: 32x32 tiles (Tile size 32px)

### 3. **Add Background**:
1. In the **Layers Panel**, right-click -> **New Image Layer**.
2. Right-click the layer -> **Layer Properties**.
3. Set **Image** to your chunk PNG (e.g., `assets/sprites/gameMap/map_0_0.png`).

### 4. **Add Objects and Collision**:
1. Create a **New Object Layer** named `Entities`.
    
#### A. Adding barriers
* Select the **Insert Rectangle Tool*.
* Draw a box over the area the player cannot walk.
* With the box selected, go to the **Properties Panel**.
* Set **Class** to: `Barrier`
    
#### B. Adding buildings/entities
* Create a Tileset in Tiled containing your sprite if needed and place it in `assets/maps/tilesets`
* Select the **Insert Tile Tool**
* Place the object on the map.
* In Properties, set **Class** to the corresponding entity type which will be checked in `src/Rpg/map/entities/EntityFactory.cpp`

### 5. **Export**:
1. File -> Export as -> **Json map files (*.json)**.
2. **Naming Convention** `map_X_Y.json`.

