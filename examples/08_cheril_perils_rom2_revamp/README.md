Cheril Perils ROM2 Revamp
=========================

Back in 2015 we were making the ill-fated big Cheril game and Anjuel created a gorgeous tileset for the factory level which was sitting in my hard drive since then. 

I've decided it will be great to use it in a short game to test out some stuff and re-add some of the custom stuff as selectable components, such as:

CNROM
-----

To make CNROM games you have to:

- Generate four tileset?.chr files, with ? = 0..3, instead of just one tileset.chr file for NROM.
- Add `-D CNROM` in the `cc65` call which compiles the game in `compile.bat`
- Add `-D CNROM=1` in the `ca65` call which assembles `crt0.s` in `compile.bat`.
- Use `-C nes-CNROM.cfg` instead of `-C nes.cfg` in the `la65` call which links the cart in `compile.bat`
- Provide correct values in the `l_chr_rom_bank` array in `levelset.h` so the engine knows which CHR-ROM bank it should page in for each level.
- Note that you might need to include sets of `tileset?.h` and `spriteset?.h` files in `game.c` to fit your needs.

Take a peek at `08_cheril_perils_rom2_revamp` to see this in action!

Compiled enemies
----------------

Compiled enemies follow a programmed path from a set. Such set is generated compiling the `script/enembehs.spt` script. The language is pretty straightforward, just check the simple examples provided. 

Behaviours included in the script are compiled in order, the first one is #0. When adding enenmies in `ponedor.exe`, just add your enemy in its starting position as type 0x14 and especify the behaviour number in the attr field. The ending position is discarded.

```c
	#define ENABLE_COMPILED_ENEMS
	#define COMPILED_ENEMS_SHOOT
	#define COMPILED_ENEMS_BASE_SPRID		48
```

If your enemies shoot, define `COMPILED_ENEMS_SHOOT`. Leave it commented out if none of them do, as you will save space (`COCOS` won't get added). 

Enemmies are rendered facing left or right. There are two animation cells for when they are moving around and two extra animation cells for when they are idling (not moving). Cells are in the active `spr_enems` array starting at index `COMPILED_ENEMS_BASE_SPRID`:

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

Shines
------

Create small shines on killing objects, up to a maximum. Shines are rendered using two different patterns in bank 1:

```c
	#define ENABLE_SHINES
	#define SHINES_MAX 						8
	#define SHINES_BASE_PATTERN				10	// In the sprite bank, two patterns needed
	#define SHINES_PALETTE					3
	#define SHINING_TILE					23	// Tile # in map to add shines
```

Shines are added automaticly to the scene when rendering a new screen and `SHINING_TILE` is detected.

Resonators
----------

These make up the gameplay in Cheril Perils. If you press them, enemies freeze and are killable by means of jumping on them. Resonators are placed as hotspots, and need two metasprites in the items array.

```c
	#define ENABLE_RESONATORS
	#define RESONATOR_BASE_PATTERN			0
	#define RESONATOR_PALETTE				3
	#define RESONATOR_COUNTER_OFFS_X		4
	#define RESONATOR_COUNTER_OFFS_Y		7
```

They also need 10 patterns with all the digits 0-9 in bank 1, from `RESONATOR_BASE_PATTERN` onwards. Those are used to draw the counter, which is rendered at offset `(RESONATOR_COUNTER_OFFS_X, RESONATOR_COUNTER_OFFS_Y)` from the hotspot's top-left corner.

No!
---

Displays a balloon with "NO!" written on it over the sprite.

```c
	#define ENABLE_NO 
	#define NO_METASPRITE					ssit_6
	#define NO_OFFS_X						0
	#define NO_OFFS_Y						-24
```

Initially this is mapped to just one action: failing to open a lock 'cause you have no keys left, but you can trigger whenever you want just making `no_ct = 100` or any number of frames.

Use animation
-------------

Hotspots must be interacted with the B button, and an animation is displayed. When a certain step in the animation is displayed, the interaction with the hotspot is spawn.

```c
	#define ENABLE_USE_ANIM
	#define USE_ANIM_MAX_FRAMES				13
	#define USE_ANIM_INTERACT_ON			7
	#define USE_ANIM_FRAMES_PER_STEP		4
```

You are in charge of preparing your `my/player_frame_selector.h` to support this animation. Note that the first cell in your sequence will show when the counter is "1", and the last when it reaches `USE_ANIM_MAX_FRAMES`. Each frame will show for `USE_ANIM_FRAMES_PER_STEP` game frames; the last one will last for 1 whole second. The current animation step (1 to `USE_ANIM_MAX_FRAMES`) is `use_ct`, so you can do this on `my/player_frame_selector.h`:

```c
	if (use_ct) {
		psprid = CELL_USE + use_ct - 1;
	} else // ...
```

Where `CELL_USE` is the index of the first "use animation" metasprite in the `spr_player` array. Check the Cheril Perils ROM2 revamp example to se this in action!

Timer
-----

(Not used on this game?)

Runs a timer which you can display. It detects when it reaches zero. You can integrate this with the scripting engine, or react to it adding C code to `my/extra_checks.h`.

```c
	#define ENABLE_TIMER
	#define TIMER_INITIAL					5
	#define TIMER_START_ON
	#define TIMER_REFILL					10
	#define TIMER_TIME_FLAG					0
	#define TIMER_ZERO_FLAG 				1
	#define TIMER_RESET_ON_ENTER
	#define HOTSPOT_TYPE_TIME				5
```

- `timer` equals `TIMER_INITIAL` at the beginning.
- `timer` decrements each seconds if `timer_on`.
- `timer_on` is set at the beginning if `TIMER_START_ON` is defined.
- When `timer == 0`, `timer_zero` is set. It's up to you to reset it.
- if you define a `HOTSPOT_TYPE_TIME` you can use refills. Those will add `TIMER_REFILL` seconds to `timer`; if `TIMER_REFILL` equals 0, then timer will be set to `TIMER_INITIAL`.

If you are using scripting, use `TIMER_TIME_FLAG` and `TIMER_ZERO_FLAG` to copy the values of `timer` and `timer_zero` to a couple of flags.

Steady Shooters
---------------

```c
	#define ENABLE_STEADY_SHOOTERS
	#define STEADY_SHOOTERS_BASE_SPRID		44
	#define STEADY_SHOOTER_KILLABLE
```

- Type 5 (0x5).
- Place them. Relation between (x1, y1) and (x2, y2) defines direction. Attribute defines # of seconds between shoots.
- Cells are from `STEADY_SHOOTERS_BASE_SPRID` onwards, from left, clockwise: left up right down.
- `STEADY_SHOOTER_KILLABLE` make steady shooters destructible. But beware! They are not counted as killable by `eneexp3.exe`'s option `gencounter`. You can count them in using the `genallcounters` option in addition to `gencounter` and adding `KILLABLE_ENEMS_prefix + MAX_ENEMS_TYPE_5_prefix`.

Springs
-------

```c
	#define ENABLE_SPRINGS
	#define SPRING_TILE 					10
	#define SPRING_SPIKE_TILE 				11
	//#define SPRINGS_ON_BY_DEFAULT	
```

- When the player touches the cell above a placed `SPRING_TILE`, a `SPRING_SPIKE_TILE` is put in that cell.
- Springs will only work if `springs_on`. `springs_on` is set from the beginning if `SPRINGS_ON_BY_DEFAULT` is set.
