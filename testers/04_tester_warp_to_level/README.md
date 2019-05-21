# Tester "Warp to level"

This tester was created to test / illustrate the *warp to level* feature. It contains two implementations.

The example contains two levels. Both should connect side to side, so when you attempt to exit the rightmost room (room 2) in level 0 to the right, you must be warped automaticly to the leftmost room (room 0) in level 1. Besides, there's a couple 'warp points' (a tile with a star), one in level 0 and one in level 1, which can be activated pressing B.

## Scripting based 

To enable this implementation just define `ACTIVATE_SCRIPTING` in `config.h`.

Warping is done completely via scripting, detecting several cases where the player should be warped to a different level. Check `script/script.spt` while you read this document.

### The automatic connection.

We have created *fire zones* (so `ENABLE_FIRE_ZONE` should be on) to automaticly translate the player. The first one is at level 0, room 2:

```spt
	ENTERING SCREEN 2
		IF TRUE
		THEN
			SET FIRE_ZONE_TILES 15, 0, 15, 11
		END
	END
```

So when the player enters tile column 15 on screen, the `PRESS_FIRE` script for room 2 is launched:

```spt
	PRESS_FIRE ON SCREEN 2
		IF PLAYER_IN_X 240, 255
		IF VX > 0
		THEN
			SET X = 0
			WARP_TO_LEVEL 1, 0
		END
		[...]
	END
```

First check if the player is actually touching the rightmost column (pixel coordinates between 240 and 255), then check if it is moving right (`VX > 0`). 

Calling `WARP_TO_LEVEL` with just two parameters just sets the destination level and room, but leaves the player coordinates untouched, so we must set the X coordinate right before to move the player to the left side of the screen (but keeping the Y coordinate): `SET X = 0`.

Something simmiliar is done on level 1, screen 0, but with the leftmost column on the screen.

### Non-automatic connections

The 'warp points' have to be manually triggered by the player by walking on them and pressing B. These points should transfer the player to the new level and coordinates:

```spt
	PRESS_FIRE ON SCREEN 2
		[...]

		IF PLAYER_TOUCHES (7, 10)
		THEN
			WARP_TO_LEVEL 1, 2, 14, 8
		END
	END
```

Note that we are calling `WARP_TO_LEVEL` with *four* parameters. This version also sets the player coordinates in the destination (in this case, to `14, 8` (in tile coordinates)).

## Code injection based

Just comment `ACTIVATE_SCRIPTING`.

### Automatic connections

Those are performed via the `my/custom_flickscreen.h` injection point:

```c
	#ifndef ACTIVATE_SCRIPTING
		switch (level) {
			case 0:
				switch (n_pant) {
					case 2:
						if (prx == 244 && pvx > 0) {
							px = 4<<FIXBITS;
							level = 1;
							n_pant = 0;
							warp_to_level = 1;
							flick_override = 1;
						}
				}
				break;
			case 1:
				switch (n_pant) {
					case 0:
						if (prx == 4 && pvx < 0) {
							px = 244<<FIXBITS;
							level = 0;
							n_pant = 2;
							warp_to_level = 1;
							flick_override = 1;
						}
						break;
				}
				break;
		}	
	#endif
```

In **AGNES**, the player can change rooms laterally when `prx` is either 4 (exit to the left) or 244 (either to the right) *and* it is moving towards the correct direction (moving left to exit to the left, that is, `pvx < 0`, or moving right to exit to the right, `pvx > 0`). That's why, if we are on level 0, room 2, we have to check if `prx == 244 && pvx > 0`, and on level 1, room 0, we have to check if `prx == 4 && pvx < 0`.

If the conditions are met, the standard sequence is (usually): change the player coordiantes (in this case, just `px`), set the new `level`, set the new `n_pant` (room number), set `warp_to_level` and, in this case, `flick_override`, so the standard connection is not performed.

### Non-automatic connections

Just check if `b_button` is pressed, then use the macro `IF_PLAYER_TOUCHES (x,y)` to check if the player is touching the tile cell at (x, y) will do the trick. We can place this code in several places. We have decided to place it alongside the automatic connections in `my/custom_flickscreen.h` to save some checks. 
