# Double Width

New with v2.0, **MK1_NES / AGNES** games can use both nametables arranged horizontally for 512 pixels wide rooms. The camera will follow the player horizontally as he or she moves.

Enabling `DOUBLE_WIDTH` puts a strain on the engine, which was created for simple flick-screen games. The engine has to generally update twice the objects and the RAM usage is much bigger. 

## Mirroring

Be sure to use vertical mirroring. Edit crt0.s and edit line 15 (`NES_MIRRORING` should equal 1).

## RAM problems

With some configurations, you will probably run out of RAM if you enable `DOUBLE_WIDTH`.

By default, the engine keeps a buffer with all the tiles on screen (normally 16x12 = 192; with wide screens this is 32x12 = 384 tiles!), plus an extra, precalculated buffer with the behaviour of all tiles on screen. This is done for speed, but also allows for some nice tricks (you can modify the behaviours buffer to make non walkable tiles walkable and hide paths, to name a trivial example). This extra buffer can be deactivated so 384 precious bytes of RAM are freed. All calculations will be performed in real time using your `behs` arrays and the standard buffer. To do so, enable `REAL_TIME_MAP_ATTR`.

## Single screens support

By default, `DOUBLE_WIDTH` games needs maps which width in rooms is an even number (2, 4, 6, 8 ... screens wide). All rooms are 512 pixels wide and composed by room N and room N+1, with N an even number.

If you enable `REAL_TIME_MAP_ATTR` some code is added to allow for maps which width in rooms is an odd number (1, 3, 5... screens wide). The rightmost rooms won't be double and won't scroll.

You can also use this to your advantage to have single non scrolling rooms randomly, but this can be tricky and hasn't been properly tested as of v2.0.

## Split screen

Probably you have a HUD and probably you want it to stay fixed.

As of v2.0, the engine only supports your hud to be located on top, and use the sprite 0 hit method. comment `NO_SPLIT` and set up your bg and sprite 0 at `/my/game_frame.h`.

You probably want to read about the sprite 0 based raster split [here](https://wiki.nesdev.com/w/index.php/PPU_registers#Sprite_0) and specially [here](https://retrocomputing.stackexchange.com/questions/1898/how-can-i-create-a-split-scroll-effect-in-an-nes-game) and check [tester #5](https://github.com/mojontwins/MK1_NES/tree/double-width/testers/05_tester_double_width).
