Tester Punchy
=============

Simple project to test/add/debug MK1 engine stuff. Punchy games.

Things, so far:

- Tall player
- Shooties

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
- Type 12, 13, 14, 15, place exactly like normal type 1-4 enemies/platforms. The number means you can select different sprite faces (up to 4), from `SHOOTIES_BASE_SPRID` onwards.
- Imply linear cocos.
- `SHOOTIES_SHOOT_OFFS_X` and `SHOOTIES_SHOOT_OFFS_Y`, offset from top-left of sprite bounding box to shoot. Notice bullets are 8 pixels wide (top-left centered), and enemies are always 16 pixels wide (top-left centered).
- Shoot when player is "in range" using `PUNCH_FREQ` (en expresion which will produce a coco when evaluates to true).

Punchies
--------

- `#define ENABLE_PUNCHIES`.
- 6 frames, 3 right, 3 left; two walk frames, 1 shooting frame.
- Type 16, 17, 18, 19, place exactly like normal type 1-4 enemies/platforms. The number means you can select different sprite faces (up to 4), from `SHOOTIES_BASE_SPRID` onwards.
- ` PUNCHIES_PUNCH_OFFS_X` and `PUNCHIES_PUNCH_OFFS_Y`, offset from top-left of sprite bounding box to punch. Notice punching hitbox is 8x8, top-left centered, and enemies are always 16 pixels wide (top-left centered).
- Punch when player is "in range" using `PUNCH_FREQ` (an expresion which will produce a punch when evaluates to true).
