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

Note that the numbers here are those you use in **ponedor** when placing the hotspots, so there's room for some customization. The numbers are also used to render the objects on screen: the number is also an index to the `spr_hs` metasprite array defined in `assets/metasprites.h`.

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
    #define ENABLE_SPRINGS
```

Configuration:

```c
    #define SPRING_TILE                     10
```

`SPRING_TILE` is the number of the tile the engine should detect as "spring".

```c
    #define SPRING_SPIKE_TILE               11
```

`SPRING_SPIKE_TILE` is the number of the tile which appears once the spring trap is activated.

```c
    #define SPRINGS_NEED_POSSEE
```

If defined, `SPRINGS_NEED_POSSEE` means that the player has to actually land on the tile to activate the trap. Otherwise the trap appears once the player overlaps the tile cell above the trap.

```c
    #define SPRINGS_ON_BY_DEFAULT
```

Springs only work if the variable `springs_on` is set. If `SPRINGS_ON_BY_DEFAULT` is defined, `springs_on` is set automaticly.

This kind of booby traps are used in [Cheril Perils Classic](https://github.com/mojontwins/MK1_NES/tree/master/examples/07_cheril_perils_classic). In this game, `SPRINGS_ON_BY_DEFAULT` is undefined. Via custom initialziations in `my/extra_inits.h`, `springs_on` is 1 only on level 1.

## Warpers

Warpers are entities which, when interacted with, take the player elsewhere. They are placed as enemies with **ponedor**. `attr` is the destination screen (room) number, and `s1` should contain the destination coordinates (packed 0xYX).

```c
    #define ENABLE_SIMPLE_WARPERS
```

Configuration:

```c
    #define SIMPLE_WARPERS_BASE_SPRID       (32+((frame_counter>>2)&3))
```

Warpers are rendered using metasprites in the active `spr_enems?` array. `SIMPLE_WARPERS_BASE_SPRID` is used to determine which metasprite. It can be an expression, like the example above, which cycles four different cells from index 32 using `frame_counter` divided by four (one cell change every four frames).

```c
    #define SIMPLE_WARPERS_FIRE_BUTTON
```

Define this if you require the player to press B to activate the warper. If it is undefined, the warpers will be activated just by touching them.

## No!

```c
    #define ENABLE_NO 
```

This feature was designed to show a small text balloon with a "NO!" icon above the player for a while. It is triggered when 

* The player attempts to open a lock without a key.
* Using easy objects, the player attempts to use an item in the wrong hotspot.

It can be triggered anywhere just by setting `no_ct` with a frame count (50 = 1 second).

```c
    #define NO_METASPRITE                   ssit_06
    #define NO_OFFS_X                       0
    #define NO_OFFS_Y                       -24
```

The metasprite used is `NO_METASPRITE`, at `(prx + NO_OFFS_X, pry + NO_OFFS_Y)`.

## Use animation

```c
    #define ENABLE_USE_ANIM
```

The "use animation" is a subset of cells in the `spr_player` array which will be started once the player:

- Interacts with a hotspot.
- Presses B to run a fire script (when using scripting).
- Interacts with an interactive.

The actual "action" will be performed when the sequence reaches a determined value:

```c
    #define USE_ANIM_MAX_FRAMES             13
    #define USE_ANIM_INTERACT_ON            7
    #define USE_ANIM_FRAMES_PER_STEP        4
```

The animation counter, or `use_ct`, will iterate from 1 to `USE_ANIM_MAX_FRAMES` if the interaction was successful, or from 1 to `USE_ANIM_MAX_FRAMES + 1` (skipping `USE_ANIM_MAX_FRAMES`) if it wasn't.

Each cell will be displayed for `USE_ANIM_FRAMES_PER_STEP` frames but the last, which will be displayed for one second.

You are in charge of preparing your `my/player_frame_selector.h` to select the correct cell from `spr_player` if `use_ct` is != 0. If `CELL_USE` is the first frame in the sequence, then:

```c
    if (use_ct) {
        psprid = CELL_USE + use_ct - 1;
    } else // ...
```

`use_ct` should have priority over other player states in `my/player_frame_selector.h`. Check those games for inspiration:

* [Cheril the Writter](https://github.com/mojontwins/MK1_NES/tree/master/examples/08_cheril_the_writer)
* [Cadàveriön](https://github.com/mojontwins/MK1_NES/tree/master/examples/09_cad%C3%A0veri%C3%B6n)
* [Tester interactives](https://github.com/mojontwins/MK1_NES/tree/master/testers/03_tester_interactives)

## Timer

```c
    #define ENABLE_TIMER
```

The timer starts from the initial value and is decremented each second. When it reaches zero, a flag variable is risen (but you are in charge to zero it back!) and, if scripting is active, the `ON_TIMER_OFF` section is executed.

```c
    #define TIMER_INITIAL                   99
    #define TIMER_START_ON
    #define TIMER_REFILL                    30
```

`TIMER_INITIAL` is the timer's initial value. When it reaches zero, it will cycle back to `TIMER_INITIAL`.

The variable `timer_on` determines if the timer is running or not. `TIMER_START_ON` makes `timer_on = 1` when the game starts. If it is left out commented, you are in charge of starting the timer.

`TIMER_REFILL` is the amount of seconds added to the timer if the player gets a `HOTSPOT_TYPE_TIME`.

```c 
#define TIMER_RESET_ON_ENTER
#define TIMER_SOUND                     10
```

If `TIMER_RESET_ON_ENTER` is defined, the timer will be reset every time the player enters a new screen. `TIMER_SOUND`, if defined, will play a special sound if timer < the defined value.

Deprecated (because they are pretty useless):

```c
    #define TIMER_TIME_FLAG               0   // Useful with scripting. Copies time to flag
    #define TIMER_ZERO_FLAG               1   // Useful with scripting. raises flag when time zero
```

## Enemies

### General & global definitions

```c
    #define ENEMS_IN_CHRROM
```

Enemies are stored somewhere in CHR-ROM. You must tell the engine *where* in `assets/levelset.h`. The arrays `l_enems_chr_rombank`, `l_enems` and `l_hotspots` should contain valid data for every level in the game:

* `l_enems_chr_rombank` contains which CHR-ROM bank contains the enemies + hotspots data for each level.
* `l_enems` is the address of the enemy data for each level, in PPU addresses. 
* `l_hotspots` is the address of the hotspots data for each level, in PPU addresses.

If you use `eneexp3` in bin mode and `binpaster` to glue all binaries together, you should have offsets to the beginning of the enemy block (`ENEMS?_H_BIN_OFFS`) and offsets to the beginning of the hotspots block (`ENEMS0_?_BIN_OFFS + HOTSPOTS_OFFSET_?`).

A more detailed explanation is comming in form of tutorial in the near future. You can check [Cheril the Writter](https://github.com/mojontwins/MK1_NES/tree/master/examples/08_cheril_the_writer) meanwhile.

```c
    #define ENEMS_LIFE_GAUGE                1
```

Amount of shots/punches/kicks needed to kill enemies.

```c
    #define ENEMS_FLICKER
```

If `ENEMS_FLICKER` is defined, enemies will flicker for a while when hit. If it is left commented out, a explosion will show instead.

```c
    #define ENEMS_FLICKER_ONLY_ON_DYING
```

If defined, the flicker will only happen on the last hit (if `ENEMS_LIFE_GAUGE` > 1, that is).

```c
    //#define ENEMS_CAN_RESPAWN
```

Not yet complete. Needs documentation + further tests.

```c
    #define PERSISTENT_ENEMIES
    #define PERSISTENT_DEATHS
```

Those two are important. 

* `PERSISTENT_ENEMIES` makes the engine remember the position and direction of enemies when you exit the screen and come back later. It may be needed for some gameplays, but be warned that it eats RAM as it needs 4 bytes per enemy in the level, which means `MAP_W*MAP_H*4*3` bytes.
* `PERSISTENT_DEATH` remembers which enemies are killed. If not defined, enemies will respawn every time you reenter a screen. It takes `MAP_W*MAP_H` bytes.

```c
    #define ENEMS_TOUCHED_FRAMES            8
```

Stay "touched" (blinking or with the explosion sprite) for `ENEMS_TOUCHED_FRAMES` frames.

```c
    #define ENEMS_RECOIL_ON_HIT           2
    #define ENEMS_RECOIL_OVER_BOUNDARIES 
```

If `ENEMS_RECOIL_ON_HIT` is defined, enems will recoil a bit when hit. For every frame they are in the "touched" state, they will recoil `ENEMS_RECOIL_ON_HIT` pixels.

For linear enemies, the (X1, Y1) and (X2, Y2) boundaries will stop the recoiling. define `ENEMS_RECOIL_OVER_BOUNDARIES` if you don't want this to happen. 

```c
    #define ENEMS_ENABLE_DYING_FRAME
```

If defined, when enemies are hit, the "dying frame" cell is displayed. Remember that, generally, enemies' metaspritesets are:

```
    RIGHT_WALK_1, RIGHT_WALK_2, RIGHT_HITTING, RIGHT_DYING,
    LEFT_WALK_1, LEFT_WALK_2, LEFT_HITTING, LEFT_DYING
```

If you are using explosions (`ENEMS_FLICKER` is undefined):

```c
#define ENEMS_EXPLODING_CELL            32
#define ENEMS_EXPLODING_CELLS_HIDES
```

The value of `ENEMS_EXPLODING_CELL` refers to the index in the `spr_enems?` arrays of a explision metasprite. If `ENEMS_EXPLODING_CELLS_HIDES` is left commented out, the explision overlaps the enemy sprite. If it is defined, it substitutes it.

The enemies types "Saw" and "Pezons" eventually emerge from behind the background. To achieve the effect, the "occluding sprite" trick is used. The occluding metasprite should be in the `spr_enems?` arrays. Its index should be programmed into this constant:

```c
    #define ENEMS_OCCLUDING_CELL            33
```

[Cheril the Writter](https://github.com/mojontwins/MK1_NES/tree/master/examples/08_cheril_the_writer) implements saws, for example. You can see the occluding frame in the spriteset next to the saw.

![Cheril the Writer spriteset](https://raw.githubusercontent.com/mojontwins/MK1_NES/master/examples/08_cheril_the_writer/gfx/ss.png)

`mkts` will make metasprites *low priority* if the colour #FF00FF is found in the graphic. #FF00FF always encodes as colour 0 (background), by the way. 

```c
    #define ENEMIES_SUFFER_ON_PLAYER_COLLISION
```

Substracts life from enemies when they collide the player. Warning! This will only work if the engine is prepared for enemies to die, this is, if the player shoots, can step on enemies, or hits.

### Enemy type: Fanty & Homing Fanty

Fanties (type 6 enemies) hover around chasing the player. Homing Fanties only do it if the player is close enough, tho, and return "home" (the starting position) otherwise:

```c
    #define ENABLE_FANTY
    #define ENABLE_HOMING_FANTY
```

Define only one. **You can't have both**.

Fanties are further configured by:

```c
    #define FANTY_BASE_SPRID                32
```

The base index in `spr_enems?`.

```c
    #define FANTY_WITH_FACING
```

If defined, Fanty will look left / right. You will need four cells for fanties in `spr_enems?`:

```
    RIGHT_FANTY_A, RIGHT_FANTY_B, LEFT_FANTY_A, LEFT_FANTY_B
```

The engine will alternate between A & B for each direction while Fanty moves around. If `FANTY_WITH_FACING` is undefined you just need two cells.

```c
    #define FANTY_COLLIDES
```

Leave this undefined if you want Fanties to fly through walls.

```c
    #define FANTY_KILLED_BY_TILE
```

If defined, Fanties can be killed by killing tiles (beh & 1).

```c
    #define FANTY_LIFE_GAUGE              5 
```

If defined, Fanties will have their own life gauge. If not defined, fanties will share the same life gauge as the rest of the enemies.

```c
    #define FANTY_A                         4
    #define FANTY_MAXV                      48
```

Movement values for acceleration and maximum velocity. The lower the acceleration, the slower the fanty will react to the player movement.

Only for Homing Fanties:

```c
    #define FANTY_DISTANCE                  80
    #define FANTY_V_RETREAT                 16
```

Fanties will only notice the player if it is closer than `FANTY_DISTANCE` in pixels. `FANTY_V_RETREAT` is the speed at which Fanties return "home".

### Enemy type: Pursuers

```c
    #define ENABLE_PURSUERS
```

Type 7 enemies. Only really usable in top-down games. Pursuers are spawned from the initial position and chase the player. If the player kills a pursuer, a new one will be spawned after a while. Further configuration:

```c
    #define DEATH_COUNT_EXPRESSION          50+(rand8()&63)
```

When a pursuer is killed, the next one will be spawned after `DEATH_COUNT_EXPRESSION` frames. Of course (as pictured) you can use a complex expresion or a constant number.

```c
    #define TYPE_7_FIXED_SPRITE             4
```

If defined, all spawned enemies are of type `TYPE_7_FIXED_SPRITE`. Otherwise, they can be 1, 2, 3 or 4 at random.

### Enemy type: Saw

```c
    #define ENABLE_SAW
```

Saws (type 8 enemies) have 4 states: emerging, advancing, hiding, retreating. They can kill the player only while emerging. You define the travelling direction with start/end points in ponedor. They will emerge left or up if the attr is 0, or right or down if it is 2. 

```c
    #define SAW_BASE_SPRID                  48
```

The base index in `spr_enems?`. Saws need two cells which alternate very fast.

```c
    #define SAW_V_DISPL                     4
```

Speed when advancing or retreating, in pixels per frame.

```c
    #define SAW_EMERGING_STEPS              10
```

How many pixels saws pop out.

### Enemy type: Pezons

```c
    #define ENABLE_PEZONS
```

Pezons (Type 9) are usually fish which jump out of water or fire balls which jump out of lava. The frequency is defined in the attr field in **ponedor**. 

```c
    #define PEZONS_BASE_SPRID               40
```

The base index in `spr_enems?`. Pezons need two cells: "up" and "down". For fish or jumping animals, "mouth open" and "mouth closed" works quite nicely. For fire balls, "trail down" then "trail up" also looks great.

```c
    #define PEZON_WAIT                      50
```

Base idle time. The value of the attr field in **ponedor** multiplied by 8 will be added to `PEZON_WAIT`.

```c
    #define PEZON_THRUST                    384
    #define PEZON_VY_FALLING_MAX            256
    #define PEZON_G                         16
```

Movement values (Initial VY when jumping, maximum VY when falling and gravity, respectively).

### Enemy type: Chac-chacs (deprecated)

The support for "chac chacs as enemies" is deprecated. It's still there 'cause this was the original implementation.

### Enemy type: Monococo

```c
    #define ENABLE_MONOCOCOS
```

Monococos (Type 0xB enemies) are hiden, appear from time to time, shoot a coco to the player, then disappears again. Monococos cycle through 4 states:

* Idle, for A frames.
* Appearing, for B frames.
* On, it shoots a coco, then waits for C frames.
* Disappearing for B frames.

The values of A, B, C are defined using these constants:

```c
    #define MONOCOCO_BASE_TIME_HIDDEN       150 // A
    #define MONOCOCO_BASE_TIME_APPEARING    50  // B
    #define MONOCOCO_BASE_TIME_ONBOARD      50  // C
```

Once in the state "On", the coco will be shoot after `MONOCOCO_FIRE_COCO_AT` frames:

```c
    #define MONOCOCO_FIRE_COCO_AT           MONOCOCO_BASE_TIME_ONBOARD/2
```

There are two kinds of monococos: **type A** (define `MONOCOCO_TYPE_A`) and **type B** (comment `MONOCOCO_TYPE_A` out).

* **Type A**: they use two cells which alternate continuously. Hidden during the *idle* state, flickering during *appearing / disappearing*.
* **Type B**: they use four cells: A "hidden" cell during *idle*, an "appear / disappear" cell during *appearing / disappearing*, and the alternative two animation cells during the *on* state.

So you need to provide 2 or 4 cells of animation in the `spr_enems?` arrays depending on the type configured:

* **Type A**, two cells: `FRAME_A, FRAME_B`.
* **Type B**, four cells: `FRAME_A, FRAME_B, APPEARING/DISAPPEARING, HIDDEN`.

### Enemy type: Shooties & Punchies

(only for side view)

Shooties and Punchies are additions to patrollers. Patroller enemies are types 1-3. You can turn a patroller into a shootie by adding 0x80 and into a punchie adding 0x40 (so a type 2 punchie would be 0x42, for example).

Shooties will shoot when the player enters its line of sight. Punchies will hit the player when it comes close.

Remember that the first three groups of cells in the `spr_enems?` arrays are reserved for type 1, 2 and 3 patrollers. They will be used for related punchies and shooties. The cell groups are:

```
    RIGHT_WALK_1, RIGHT_WALK_2, RIGHT_HITTING, RIGHT_DYING,
    LEFT_WALK_1, LEFT_WALK_2, LEFT_HITTING, LEFT_DYING
```

When shooties shoot or punchies punch, the `RIGHT_HITTING` or `LEFT_HITTING` frame is displayed.

#### Shooties configuration:

```c 
    #define SHOOTIES_BASE_SPRID             32
```

If you want your shooties to look different from normal patrollers, add an offset here. For example, you can have three different shooties this way:

```c
    unsigned char spr_enems0 [] = {
        e1rw1, e1rw2, e1rh, e1rd, e1lw1, e1lw2, e1lh, e1ld, // Patroller 1
        e2rw1, e2rw2, e2rh, e2rd, e2lw1, e2lw2, e2lh, e2ld, // Patroller 2
        e3rw1, e3rw2, e3rh, e3rd, e3lw1, e3lw2, e3lh, e3ld, // Patroller 3
        pl1, pl2, 0, 0, pl1, pl2, 0, 0,                     // Platform
        e1rw1, e1rw2, e1rh, e1rd, e1lw1, e1lw2, e1lh, e1ld, // Shooty 1
        e2rw1, e2rw2, e2rh, e2rd, e2lw1, e2lw2, e2lh, e2ld, // Shooty 2
        e3rw1, e3rw2, e3rh, e3rd, e3lw1, e3lw2, e3lh, e3ld, // Shooty 3
    }
```

Using `SHOOTIES_BASE_SPRID` with value 32 will make this work. A type 0x42 enemy is being loaded, the base index would be that of enemy type 2 plus 32: (2-1)*8 + 32 = 40, which points to `e2rw1`.

```c
    #define SHOOTIES_SHOOT_OFFS_X           16
    #define SHOOTIES_SHOOT_OFFS_Y           -2
```

The coco will be shot from these offsets added to the enemy's origin of coordinates when facing right. The horizontal offset when facing left is autocalculated to match.

```c
    #define SHOOT_FREQ                      (pry+23>=en_y[gpit]&&pry<=en_y[gpit]+23&&((en_facing[gpit]&&en_x[gpit]>prx)||(en_facing[gpit]==0&&en_x[gpit]<prx))&&(rand8()&0x1f)==0)
```

The shootie will shoot when the expresion contained in `SHOOT_FREQ` is true, so there's a lot of room for customization here. The above example makes the shootie shoot if the player is in front of him, roughly, with a random factor added to the mix.

#### Punchies configuration:

```c
    #define PUNCHIES_BASE_SPRID             32
```

Works the same way `SHOOTIES_BASE_SPRID` does.

```c
    #define PUNCHIES_PUNCH_OFFS_X           16
    #define PUNCHIES_PUNCH_OFFS_Y           -7
```

These define where the hitbox is located. The values are offsets from the enemy's origin of coordinates when facing right. The horizontal offset when facing left is autocalculated to match. The hitbox is 8x8.

```c
    #define PUNCH_FREQ                      (pry+23>=en_y[gpit]&&pry<=en_y[gpit]+23&&((en_facing[gpit]&&en_x[gpit]>prx)||(en_facing[gpit]==0&&en_x[gpit]<prx))&&DELTA(prx,en_x [gpit]+4)<16)
```

As with shooties' `SHOOT_FREQ`, punchies will punch when the expression in `PUNCH_FREQ` evaluates to true. The above example makes use of the `DELTA` macro to determine if the player is close enough.

### Enemy type: Steady shooters

```c
    #define ENABLE_STEADY_SHOOTERS
```

Steady shooters (type 5) are just cannons. They shoot a coco in the direction they are facing. The direction is determined by the relation between (X1, Y1) and (X2, Y2). The `attr` field in **ponedor** is used to define the number of seconds between shoots.

```c
    #define STEADY_SHOOTERS_BASE_SPRID      44
```

Steady shooters need 4 cells in your `spr_enems?` arrays, one for each direction left, up, right, down, in that order. `STEADY_SHOOTERS_BASE_SPRID` points to the sequence.

```c
    #define STEADY_SHOOTER_KILLABLE
```

This makes steady shooters killable. But beware: they are not counted as killable by `eneexp3`'s option `gencounter`.  If you want to take them in account you should compare `pkilled` to `KILLABLE_ENEMS_prefix + MAX_ENEMS_TYPE_5_prefix`.

### Enemy type: Compiled

```c
    #define ENABLE_COMPILED_ENEMS
```

Compiled enems (Type 0x14) follow a programmed pattern. A future tutorial or article will describe them in more depth.

```c
    #define COMPILED_ENEMS_SHOOT
```

If defined, compiled enems can shoot. Leave it out if they don't to save space.

```c
    #define COMPILED_ENEMS_BASE_SPRID       48
```

Compiled enems are rendered facing left or right, and need four cells per direction:

```
    WALK_RIGHT_0
    WALK_RIGHT_1
    IDLE_RIGHT_0
    IDLE_RIGHT_1
    WALK_LEFT_0
    WALK_LEFT_1
    IDLE_LEFT_0
    IDLE_LEFT_1
```

The constant `COMPILED_ENEMS_BASE_SPRID` points to the first cell in the list.

## Cocos

Cocos are activated when needed (which means that you don't have to define `ENABLE_COCOS` yourself). But you should configure these values:

```c
    #define COCOS_MAX                       4
```

The maximum number of cocos on screen. 4 is usually good, but if you notice that sometimes your enemies don't fire when they should, increase this value.

```c
    #define COCO_V                          128
```

Velocity in 1/64ths of pixel per frame. That 128 means 2 pixels per frame.

```c
    #define COCO_COLLIDES
```

If defined, cocos collide with the background. If not, cocos will go through everything until they exit the game area.

```c
    #define COCO_PATTERN                    0
    #define COCO_PALETTE                    0
```

Used to draw cocos.

```c
    #define COCO_FAIR_D                     32
```

*Fair distance* - Cocos won't be shoot if the player's distance to the shooter is less than `COCO_FAIR_D`. If you want this feature off leave it at 0.

## Only one object

## Easy objects

## Player capabilities: brawlers

## Player capabilities: shooting

## Scripting

## Interactives

## Top view specific

## Player capabilities: miscellaneous

## Screen layout

## Player movement

## Player cells

## Sound effects
