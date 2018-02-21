Tester Punchy
=============

Simple project to test/add/debug MK1 engine stuff. Punchy games.

Things, so far:

- Tall player
- Shooties
- Punchies
- Player punches and kicks
- Ladders
- Propellers
- Warpers

To Fix
======

All fixed.

Things to note for possible documentation
=========================================

Tall player
-----------

- Makes bounding boxes from 17 to 32 pixels tall for players.
- Sprite handle NOT ALTERED, still 8x16 bottom-centered. This is a hack, remember?
- Horizontal collision points will be three: `(X, pry)`, `(X, pry + 15)` and `(X, pry - 16 + PLAYER_COLLISION_TOP)`.
- **When cutting metasprites, remember that the player box is still 8x16, bottom-centered**

Tall enemies
------------

- There are three kind of enemy collisions for now. For more granularity, you should customize the engine.
- SMALL_COLLISION (8x8) and undef (12x12) as always.
- New hacky TALL_COLLISION (8x24), from within the 16x16 enemy rectangle.

Shooties
--------

- `#define ENABLE_SHOOTIES`.
- 6 frames, 3 right, 3 left; two walk frames, 1 shooting frame.
- Type 12, 13, 14, 15, (0xC, 0xD, 0xE, 0xF) place exactly like normal type 1-4 enemies/platforms. The number means you can select different sprite faces (up to 4), from `SHOOTIES_BASE_SPRID` onwards.
- Imply linear cocos.
- `SHOOTIES_SHOOT_OFFS_X` and `SHOOTIES_SHOOT_OFFS_Y`, offset from top-left of sprite bounding box to shoot when looking RIGHT. Notice bullets are 8 pixels wide (top-left origin), and enemies are always 16 pixels wide (top-left origin).
- Shoot when player is "in range" using `PUNCH_FREQ` (en expresion which will produce a coco when evaluates to true).

Punchies
--------

- `#define ENABLE_PUNCHIES`.
- 6 frames, 3 right, 3 left; two walk frames, 1 shooting frame.
- Type 16, 17, 18, 19, (0x10, 0x11, 0x12, 0x13) place exactly like normal type 1-4 enemies/platforms. The number means you can select different sprite faces (up to 4), from `SHOOTIES_BASE_SPRID` onwards.
- ` PUNCHIES_PUNCH_OFFS_X` and `PUNCHIES_PUNCH_OFFS_Y`, offset from top-left of sprite bounding box to punch when looking RIGHT. Notice punching hitbox is 8x8, top-left origin, and enemies are always 16 pixels wide (top-left origin).
- Punch when player is "in range" using `PUNCH_FREQ` (an expresion which will produce a punch when evaluates to true).

Player punches, player kicks
----------------------------

```c
    // Silly Brawlers
    // --------------
    #define PLAYER_PUNCHES                  // When on floor
    #define PLAYER_PUNCH_OFFS_X             15
    #define PLAYER_PUNCH_OFFS_Y             -7

    #define PLAYER_KICKS                    // While airborne
    #define PLAYER_KICK_OFFS_X              12
    #define PLAYER_KICK_OFFS_Y              -3
```

- Offsets define 8x8 hit boxes from the top-left of the sprite bounding box when looking RIGHT. Remember that the player's bounding box is a 8x16 rectangle bottom-centered.

Ladders
-------

- Full ladders. must start and end on obstacle.
- beh == 32, strict.

Propellers
----------

Enable and configure.

```c
    #define ENABLE_PROPELLERS
    #define PROPELLERS_MAX                  4
    #define PROPELLERS_BASE_PATTERN         64
    #define PROPELLERS_MAX_LENGTH           6
    #define PROPELLER_TILE                  14
```

Place in your map using tile `PROPELLER_TILE`. Propeller animation will alternate two 2x1 patterns cells defined in the bg tileset from pattern `PROPELLERS_BASE_PATTERN`. Place as much as `PROPELLERS_MAX` per screen.

If you define `PROPELLERS_MAX_LENGTH` the propeller will affect an area max N tiles high, otherwise it will reach the top of the game area.

Vertical movement is then controlled by these:

```c
    #define PLAYER_AY_FLOAT         16  
    #define PLAYER_VY_FLOAT_MAX     256
```

Simple Warpers
--------------

Simple warpers are portal which take you from where they are placed to an arbitrary position (screen, Y, X).

```c
    #define ENABLE_SIMPLE_WARPERS
    #define SIMPLE_WARPERS_BASE_SPRID   56
    #define SIMPLE_WARPERS_FIRE_BUTTON
```

They are placed *as enemies*, type = 0xff. Use `attr` for the destination screen number and `s1` for the YX coordinate (high nibble Y, low nibble X).

`SIMPLE_WARPERS_BASE_SPRID` must be defined as the base sprite id (in the enemy metasprites array) for warpers to display. Of course, you can use an expression depending on `half_life` or `frame_counter` if you want animation.

If `SIMPLE_WARPERS_FIRE_BUTTON` is defined, you have to press B to activate the portal.

