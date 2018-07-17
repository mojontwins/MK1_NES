# Engine Configuration

The way your game will behave is configured in `config.h` via the definition / undefinition of a bunch of compiler directives and constants.

This is a short reference.

## Game area

```c
    //#define GAME_AREA_TOP
    #define GAME_AREA_MIDDLE
    //#define GAME_AREA_BOTTOM
    //#define GAME_AREA_CUSTOM
```

Leave three commented, one uncommented. Defines where the game area is located, vertically, on screen. There are three predefined locations:

### `GAME_AREA_TOP`

Places the game area at the top of the screen. There's room for a 3-4 lines tall hud at the bottom of the screen, from Y = 26 to Y = 29.

### `GAME_AREA_MIDDLE`

Places the game area vertically centered. There's room for a couple of lines above and a couple of lines below the game area for your hud (Y= 2, 3 and Y = 28, 29).

### `GAME_AREA_BOTTOM`

Places the game area at the bottom of the screen. There's room for a 3-4 lines tall hud at the top of the screen, from Y = 2 to Y = 5.

Besides, there's a custom location:

### `GAME_AREA_CUSTOM`

Just place manually at `autodefs.h`:

```c
    #ifdef GAME_AREA_CUSTOM
    #define TOP_ADJUST              6
    #define SCROLL_Y                12
    #endif
```

## Map size

Used to define the size of the map (or the biggest map in the set if your game has several levels):

```c
    #define MAP_W                   3
    #define MAP_H                   3
```

## Player life and refill

Define player's initial life gauge (and MAX!), plus the amount of life points recovered when getting a life refill (hotspot type 3):

```c
    #define PLAYER_LIFE             5
    #define PLAYER_REFILL           1
```

## Multi level & number of levels

For multilevel games, uncoment `MULTI_LEVEL` and set `MAX_LEVELS` with the amount of levels.

Single level example:

```c
    //#define MULTI_LEVEL
    #define MAX_LEVELS              1
```

Multiple level example:

```c
    #define MULTI_LEVEL
    #define MAX_LEVELS              5
```

## General initial values for single-level

If you are creating a single level game, you must give proper values to this set of constants:

```c
    #define SCR_INI                 0       // Initial screen
    #define PLAYER_INI_X            2       //
    #define PLAYER_INI_Y            2       // Initial position
    //#define SCR_END               99      // Final screen, undefine if N/A
    #define PLAYER_END_X            99      //
    #define PLAYER_END_Y            99      // Ending position
    #define PLAYER_MAX_OBJECTS      1       // Objects to finish game
```

Those constants are ignored if you defined MULTI_LEVEL (in such case, values are extracted from `assets/levelset.h`).

## Hotspot types

In the next section, you give actual numbers to every hotspot type you are using. Comment out the hotspot types you are *not* using so you save space.

### Basic types

Basic hotspot types are collectible items, keys and refills. As mentioned, if you are not using any of such kinds of hotspots, just comment out the #define:

```c
    #define HOTSPOT_TYPE_OBJECT     1
    #define HOTSPOT_TYPE_KEYS       2
    #define HOTSPOT_TYPE_REFILL     3
    //#define HOTSPOT_TYPE_AMMO     4
    //#define HOTSPOT_TYPE_TIME     5
```

Note that the numbers here are those you use in `ponedor` when placing the hotspots, so there's room for some customization. The numbers are also used to render the objects on screen: the number is also an index to the `spr_hs` metasprite array defined in `assets/metasprites.h`.

### Extended types

You may add code to support more kinds of hotspots. You can also use resonators, which are kind of custom but are kind of built in (!):

```c
    //#define HOTSPOT_TYPE_RESONATOR    4
    //#define HOTSPOT_TYPE_RESONATOR_ON 5
```

## Custom "win level" condition

A level (or the game, in single level games) is won if (in order of importance):

* If scripting is enabled (see `ACTIVATE_SCRIPTING` below), if `WIN GAME` was executed in the level script.
* If `PLAYER_MAX_OBJECTS` is set, when the player gets all hotspots of `HOTSPOTS_TYPE_OBJECT` type in the level.
* If `SCR_END` is defined, when the player hits `PLAYER_END_X`, `PLAYER_END_Y` in `SCR_END`.

To define a customized "win level" condition, just define `WIN_LEVEL_CUSTOM`. Once you have, the level will be won if and only if the variable `win_level` is set. 

It's up to you to set `win_level` upon meeting a custom condition. You can do it in `my/extra_checks.h` for example.

## Map format

There are several map formats. You may define one and only one format, leaving all others commented.

```c
    //#define MAP_FORMAT_PACKED
    //#define MAP_FORMAT_RLE16
    #define MAP_FORMAT_RLE53
    //#define MAP_FORMAT_RLE44
    //#define MAP_FORMAT_RLE53_CHRROM
    //#define MAP_FORMAT_RLE44_CHRROM
```

### `MAP_FORMAT_PACKED`

This is a legacy format. Each tile takes one nibble. Screens are 16x12 = 192 tiles so they take 96 bytes each. As nibbles are used to store tile indexes, the maximum number of different tiles is 16.

To convert .MAP files into packed maps use `mapcnvnes2`.

If you use decorations (i.e., tiles out of range which are stored separately) they use the old format (which needs a pointer index taking `2*MAP_W*MAP_H` bytes).

### `MAP_FORMAT_RLE16`

This is a legacy format. Map screens are stored using a not very efficient RLE algorithm.

To convert .MAP files into RLE16 maps use `legacy/rlemap2`.

If you use decorations (i.e., tiles out of range which are stored separately) they use the old format (which needs a pointer index taking `2*MAP_W*MAP_H` bytes).

### `MAP_FORMAT_RLE53` and `MAP_FORMAT_RLE44`

Maps are stored using RLE. The 53 variant uses 5 bits for tile number and 3 bits for run length, which allows for 32 different tiles. The 44 variant uses 4 bits for tile number and 4 bit for run length. If you have 16 different tiles or less, 44 compresses a tad better.

To convert .MAP files into RLE53 maps use `rle53mapMK1`. To convert .MAP files into RLE44 maps use `rle44mapMK1`.

If you use decorations (i.e., tiles out of range which are stored separately) they use the new format which doesn't require an extra bytes (they take less space!).

### `MAP_FORMAT_RLE53_CHROM` and `MAP_FORMAT_RLE44_CHROM`

Exactly like `MAP_FORMAT_RLE53` and `MAP_FORMAT_RLE44`, but read from `CHR-ROM` instead.

The map format is the same, but they require quite a setup. First of all, you should create binaries using `rle53mapchrrom` or `rle44mapchrrom` which generate CHR-ROM-ready images, plus an index and a bunch of constants. Please check [this](https://github.com/mojontwins/MK1_NES/blob/master/src/UTILS.md) for more info on the utilities, and [Cheril the Writer](https://github.com/mojontwins/MK1_NES/tree/master/examples/08_cheril_the_writer) to see CHR-ROM based maps in action.

### Decorations

A "Decoration" is a tile which index is out of bounds. Imagine that you are using RLE53 and have 32 tiles maps. You can have more than 32 tiles for casual decorations. When the converter finds a tile with index >= 32, it stores it separately, in a "decorations" array.

Decorations are good for exclusive stuff. Don't overuse them, as they can take up quite a bit of space.

If you use decorations in your maps, activate support for them:

```c
    #define MAP_WITH_DECORATIONS
```

Decoration arrays are exported alongside maps. You have to reference them in your level asset arrays (`assets/levelset.h`).

## Complex renderer

If you enable the complex renderer,

```c 
    #define MAP_RENDERER_COMPLEX
```

Each screen is decoded to a buffer, then custom code is injected, then the screen is painted using the contents of such buffer. That means that you can alter the contents of the buffer in the injected code. 

The decoder writes the screen into the `map_buff` array. Such array contains 192 bytes, one per tile, in row order, from top to bottom and left to right. That means that `map_buff [i]` contains the tile at `Y = (i >> 4)`, `X = (i & 0xf)`.

Usually, in `my/map_renderer_customization.h` you iterate the values of `map_buff` and modify them if they meet certain conditions.

This allows for much more complex visuals, and also to modify the screen contents if some conditions are met.

The possibilities are endless. Check the examples and testers to see this in action.

## Bounding boxes

The engine allows for some customization of player and enemies bounding boxes.

### Player collision boxes

Theres a bounding box for collisions with the background an a different one for collisions with other actors. Both bounding boxes are 8 pixels wide. The base height is 16 pixels but can be modified using a couple of constants.

```
            8 pixels wide
         O-----+
         |     |
         |     |
         |     | 16 pixels tall
         |     |
         |     |
         |     |
         +-----+
```

O is the origin of coordinates (`prx, pry`). The value of the constants moves the top edge of the box up (positive values) or down (negative values). Note that the origin O is not moved, just the collision box edge!

Example 1, a value of 4 moves the top edge UP:

```
            8 pixels wide

         +-----+                   \
         |     | 4 pixels stretch  |
         O·····+                   |
         |     |                   |
         |     |                   |
         |     | 16 pixels tall    | 20 pixels tall overall
         |     |                   |
         |     |                   |
         |     |                   |
         +-----+                   /
```

Example 2, a value of -4 moves the top edge DOWN:

```
            8 pixels wide
         O······
         ·     · 4 pixels shrink
         +-----+                   \
         |     | 16 pixels tall    |
         |     |                   |
         |     |                   | 12 pixels tall overall
         |     |                   |
         +-----+                   /
```

The constants are:

```c
    #define PLAYER_COLLISION_VSTRETCH_BG    -4
    #define PLAYER_COLLISION_VSTRETCH_FG    -4
```

The first extends the collision box for collisions with the background. The second extends the collision box for collisions with other actors. Usable values range from -8 to 8.

### Enemy collision box

It works like the player's, but is only used for collisions with other actors (player / bullets / hitter):

```c
    #define ENEMS_COLLISION_VSTRETCH_FG     0
```

## Engine configuration miscellaneous

### Push boxes

Push boxes are boxes which can be pushed around. This works better for top-down games are pushable tiles don't get affected by gravity in side-view games.

Define `PLAYER_PUSH_BOXES` to enable this feature:

```c
    #define PLAYER_PUSH_BOXES
```

If `PLAYER_PUSH_BOXES` is defined, the behaviour can be further configured:

```c
    #define FIRE_TO_PUSH
```

If defined, player has to push B besides the D-PAD to push a pushable block. If commented out, D-PAD is enough.

Pushable boxes can be used alongside the scripting system. To enable the required hooks in the code, just define:

```c
    #define ENABLE_PUSHED_SCRIPT
```

When `ENABLE_PUSHED_SCRIPT` is defined, further constants are used to configure the behaviour:

```c
    #define PUSHED_TILE_FLAG                1
    #define PUSHED_TO_X_FLAG                2
    #define PUSHED_TO_Y_FLAG                3
```

The values to these three constants are indexes in the `flags` array. If `ENABLE_PUSHED_SCRIPT` is defined, when a player pushes a pushable block, the overlapped tile is copied to `flags [PUSHED_TILE_FLAG]`, and the end coordinates of the pushable block are copied to `flags [PUSHED_TO_X_FLAG]` and `flags [PUSHED_TO_Y_FLAG]`. Then, the `PRESS_FIRE` scripts are run.

### Completely deactivate keys and objects

If you are not using keys/bolts and/or collectible items, besides leaving out the related `HOTSPOT_TYPE_*`, you should uncomment one or both of those:

```c
    //#define DEACTIVATE_KEYS
    //#define DEACTIVATE_OBJECTS
```

### What the player does when hit

When hit, the player can bounce off the offending peril:

```c 
    #define PLAYER_BOUNCES
    #define DOUBLE_BOUNCE
```

If `DOUBLE_BOUNCE` is on, the bounce is stronger.

It can also be relocated to a safe spot:

```c
    #define DIE_AND_RESPAWN 
    #define DIE_AND_REENTER
```

If `DIE_AND_REENTER` is defined alongside `DIE_AND_RESPAWN`, the screen is reloaded even if the respawn point is in the same screen.

`PLAYER_BOUNCES` are `DIE_AND_RESPAWN` are practically exclusive: if you enable both, you won't see the bounce as the player will be relocated.

You can also make the player flicker for a short while after being hit. This configuration is compatible with both `PLAYER_BOUNCES` and `DIE_AND_RESPAWN`.

```c
    #define PLAYER_FLICKERS 
```

### Patrollers collide with the background

Usually, patrollers walk from (X1, Y1) to (X2, Y2) and back from (X2, Y2) to (X1, Y1), or bounce in the rectangle described by (X1, Y1) and (X2, Y2) if both X1 != X2 and Y1 != Y2. If you define

```c
    #define WALLS_STOP_ENEMIES
```

Obstacles will make enemies bounce as well.

## Tile behaviours.

By default, the engine supports four basic tile behaviours:

* **Empty**, or behaviour 0. 
* **Evil**, or behaviour 1. They *kill* the player.
* **Obstacle**, or behaviour 8.
* **Platform** (only in side-view), or behaviour 4. Like obstacles, but only stop the player when falling.

Tile behaviours, or "behs", are defined in `assets/behs.h`. Basicly you define a behaviour for each tile in the tileset for each level in your game.

If you are creating a side-view game and all "evil" tiles are spikes or lava pits, you can save some space by defining

```c
    #define NO_HORIZONTAL_EVIL_TILE
```

As the engine will only check the collision against evil tiles while moving vertically.

Extra tile behaviours can be added by enabling several extra modules:

### Quicksands

```c
    #define ENABLE_QUICKSANDS
```

If defined, tiles with beh == 2 are considered "quicksands". They will make the player sink slowly.

### Breakable tiles

```c
    #define ENABLE_BREAKABLE
```

If defined, tiles with beh & 16 are considered "breakable" (note that & 16 means that it can be combined with other behaviours: for example 8|16=24 for a breakable obstacle, or 1|16=17 for a breakable killable, like the barb wire in [Sgt. Helmet's Training Day](https://github.com/mojontwins/MK1_NES/tree/master/testers/01_tester_top_down)).

If `ENABLE_BREAKABLE` is defined, the behaviour of this module can be further configured:

```c
    #define BREAKABLE_LIFE          2
```

The amount of hits for the breakable tile to break completely. If `BREAKABLE_LIFE` equals 1 the RAM requirements and the amount of code involved are MUCH lower, so think if you *really* need several hits to break breakables ;-) (basicly, you save 192 bytes of RAM and a couple 100s of bytes of ROM).

```c
    #define BREAKABLE_ANIM
```

Animate breaks. If undefined, broken tiles will simply disappear. If defined, an intermediate, broken representation of the tile will be shown for a while, then cleared. This needs further configuration:

```c
    #define BREAKABLE_MAX           4
```

Max. number of simultaneous break animations. 4 seems a good number but you may need more. The optimal value depends on how long the "animations" are and how you break tiles (by shooting, for example, with lots of simultaneous bullets). The only way to actually find the best value is by trial & error ;-)

```c
    #define BREAKABLE_MAX_FRAMES    8
```

Number of frames to show the "broken" representation of a tile which is about to disappear.

```c
    #define BREAKABLE_ERASE         0
```

"Disappear" means replacing the broken tile with tile number `BREAKABLE_ERASE`

```c
    #define BREAKABLE_BREAKING      8
```

This constant defines which tile in the tileset contains the "broken" representation of the breakable tile.

```c
    #define BREAKABLE_WALKABLE
```

If `BREAKABLE_WALKABLE` is defined, breakable tiles can be broken by walking on them. See [tester sideview](https://github.com/mojontwins/MK1_NES/tree/master/testers/00_tester_sideview) for a nice example.

### Conveyors

(only for side view)

```c
    #define ENABLE_CONVEYORS
```

Conveyor belts will displace the player left or right if he or she lands on a tile marked as "conveyor". Conveyor belts are 40 (pushes left) or 41 (pushes right).

### Slippery floor

(only for side view)

```c
    #define ENABLE_SLIPPERY
```

A tile with beh "obstacle" (8) or "platform" (4) can be made "slippery" just adding 64 (beh == 72 or 68). Slippery floors decrease acceleration and friction (instead of `PLAYER_AX` and `PLAYER_RX`, constants `PLAYER_AX_ICE` and `PLAYER_RX_ICE` are used).

### Ladders

(only for side view)

```c
    #define ENABLE_LADDERS
```

Tiles with beh == 32 are ladders. Check [tester punchy](https://github.com/mojontwins/MK1_NES/tree/master/testers/02_tester_punchy) or [Cheril the Writer's last level](https://github.com/mojontwins/MK1_NES/tree/master/examples/08_cheril_the_writer).

## Special stuff related to the background

### Propellers

(only for side view)

Propellers are animated fans (or whatever) which create a column of tiles which make the player "float". The height can be configured. Obstacles and platforms block propellers.

```c
    #define ENABLE_PROPELLERS
```

Configuration:

```c
    #define PROPELLERS_MAX                  4
```

Max number of simultaneous propellers on the same screen. Please adjust this number to the minimum possible value.

```c
    #define PROPELLERS_BASE_PATTERN         64
```

Propellers are animated using 4 patterns to create two 8x16 pixels animation "cells". Such patterns should be contiguous and start at index `PROPELLERS_BASE_PATTERN`. Check [Cheril the Writer](https://github.com/mojontwins/MK1_NES/tree/master/examples/08_cheril_the_writer).

```c 
    #define PROPELLERS_MAX_LENGTH           6
```

Maximum height of the area affected by a propeller. Note that obstacles and platforms actively "block" propellers.

```c
    #define PROPELLER_TILE                  14
```

The engime will place a propeller on screen everytime it founds the `PROPELLER_TILE` while rendering the current screen (room).

```c
    #define PROPELLERS_ON_BY_DEFAULT
```

Propellers only work if `propellers_on` is set. If you define `PROPELLERS_ON_BY_DEFAULT`, `propellers_on` is set by default. Otherwise it isn't and you have to. This is what happens in [Cheril the Writer](https://github.com/mojontwins/MK1_NES/tree/master/examples/08_cheril_the_writer).

### Shines

```c
    #define ENABLE_SHINES
```

Shines are just cosmetic. They make evil tiles shine randomly, from time to time, using two consecutive patterns in the sprite bank.

```c 
    #define SHINES_MAX                      8
```

Maximum amount of shines on the same room.

```c
    #define SHINES_BASE_PATTERN             10
```

Base pattern in the sprite bank (bank 1)

```c
    #define SHINES_PALETTE                  3
```

Use this palette to draw the sprites.

```c
    #define SHINING_TILE                    23
```

As with propellers, everytime the renderer finds a tile `SHINING_TILE` it will create a shine for current screen.

### Resonators

Not really interesting. They are used in [Cheril Perils Classic](https://github.com/mojontwins/MK1_NES/tree/master/examples/07_cheril_perils_classic) and [Cheril the Writter](https://github.com/mojontwins/MK1_NES/tree/master/examples/08_cheril_the_writer)

### Springs

They can make appear a spike or some sort of killing tile if the player steps on or jumps over a certain tile index, like a booby trap.

```c
//#define ENABLE_SPRINGS
#define SPRING_TILE                     10
#define SPRING_SPIKE_TILE               11
//#define SPRINGS_NEED_POSSEE               // You have to actually STEP on the tile for the spikes to show
//#define SPRINGS_ON_BY_DEFAULT
```

## Warpers

## No!

## Use animation

## Timer
