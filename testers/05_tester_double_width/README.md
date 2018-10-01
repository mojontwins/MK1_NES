# Double screen games

## Setup

Be sure to use vertical mirroring. Edit crt0.s and edit line 15 (`NES_MIRRORING` should equal 1).

## Map rendering

Map renderer is always "complex". Shall include `my/map_renderer_customization.h` *always*. note that the portion of `map_buff` being filled is pointed by `buff_ptr`.

You can't use breakables which are not `BREAKABLES_SOFT` in double screen mode.

In Lala the Magical, the map buffers were 512 pixels wide (32 tiles wide). That may pose a problem.

Having double-width buffers may add tons of complexity as I would have to touch most of the engine. Sure I usually use COORDS to calculate the index. Maybe I can do it differently: have the while left screen, then the whole right screen. The idea would be having the engine act as 1-screen for everything. If prx > 255 then an offset would be applied everywhere.

## Scroller

Scroller simply follows player. No fancy stuff. Maybe in the future.

## Enems rendering / collisions

Kind of sorted out with (relative) ease. Collisions seems to be handled as well, bar collisions with bullets. In fact, I need to decide what I will do with bullets and cocos...

## Bullets and cocos

The easiest route is having the X coordinates turned to integers.

