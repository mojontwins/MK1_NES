# Double screen games

Just a tester. Tested lots of stuff, but also wrote down some shit:

## Split screen setup

The split will only work if the hud (non movable part of the screen) is on top. The thicker the hud the less frame time you have, so beware! 

First we draw a simple hud (`gfx/hud.png`) which will ensure a sprite 0 hit at the bottom. I know it's a bit lame, but note how the last "S" in "KEYS" is a bit taller. We'll use the bottom right white pixel in that "S" to do the raster split, so we'll place sprite 0 (which will contain a single dot at the top left corner) so they overlap. this pixel is at (213, 39).

Don't forget to define the pattern for the sprite 0. Check `import_patterns.spt` - it's the first pattern captured (at 15, 0).

Configure `GAME_AREA_BOTTOM` (will do). This will scroll the screen 8 pixels up so we must take that in account to calculate the Y coordinate in sprite 0: the pixel we want to overlap is at Y = 39, substract 1 as the PPU renders sprites one scanline later, then substract 8. So we have to place sprite 0 at (213, 30).

So comment `#define NO_SPLIT` in `config.h`, don't forget to include `assets/hud_rle.h`, and set everything up at `my/game_frame.h`.

```c
	void draw_game_frame (void) {
		unrle_vram (hud_rle, 0x2000);
		oam_spr (213, 30, 0, 0, 0);
	}
```

This should do XD
