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
- Type 12, place exactly like normal type 1-4 enemies/platforms.
- Imply linear cocos.
- `SHOOTIES_SHOOT_OFFS_X` and `SHOOTIES_SHOOT_OFFS_Y`, offset from top-left of sprite bounding box to shoot. Notice bullets are 8 pixels wide, and enemies are always 16 pixels wide.
- Shoot when player is "in range" using `SHOOT_FREQ` (en expresion which will produce a coco when evaluates to true).

