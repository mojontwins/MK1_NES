Cadàveriön
==========

Quick and dirty port from the 2013 ZX Spectrum original used mainly to test the scripting system. Stuff used to make this happen:

- Timer enabled.
- If timer enabled *and* script active, call ON_TIMER_OFF section when timer reaches zero.
- Use of EXTERN to show "TIMER OVER" screen.
- These:

```c
	#define ENABLE_PUSHED_SCRIPT
	#define PUSHED_TILE_FLAG		1
	#define PUSHED_TO_X_FLAG		2
	#define PUSHED_TO_Y_FLAG		3
	#define PUSHED_FIRE
```

`PUSHED_FIRE` means that the fire scripts will be ran when you push a tile. In the script, `JUST_PUSHED` will evaluate to true. The pushable tile coordinates will be copied to the pair of flags `PUSHED_TO_X_FLAG` and `PUSHED_TO_Y`, and the overwritten tile will be put to flag `PUSHED_TILE_FLAG`.

- Custom code when rendering the screen which records retry blocks (tile #25) coordinates to flags #4 and #5, and gates (tile #15) coordinates to flags #6 and #7.

- Custom code when rendering the screen which counts statues (tile #14, pushable) in flag #9.

- This is the map. 'X' means a screen with gate. The number is the # of the puzzle, in order, which is stored at flag #8.

When entering screen N, the value of 8 will be compared to that screen puzzle #, and if it is greater, the gate will be cleared.

```
  0    1    2    3    4
  9    8    7    6    5
 10    ·   11   12   13
 18   17   16   15   14
```

On screen 15, (gate puzzle 18), a fire zone is created where the gate is, so when the gate is clear the player can win the game.

Gate is here at `(1, 11)`. Fire zone is `(16, 192)-(31, 207)`. 

On screens 0, 9, 10 and 19 a new floor starts, so we record the screen number and coordinates manually to flags 12, 13 and 14 so  when time is out, the script will jump back there.

So most of the game logic is at the `PRESS_FIRE AT ANY` section.

Won't be used in this game but just added
-----------------------------------------

```c
	#define HOTSPOT_TYPE_RECORDER	4
	#define RECORDER_X_ON_FLAG		4
	#define RECORDER_Y_ON_FLAG		5
```

When a screen is entered with a hotspot of this type, its (X, Y) coordinates will be recorded in the flags specified (#4 and #5).
