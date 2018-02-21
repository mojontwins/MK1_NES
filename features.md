Engine features
===============

Sorry, this is all you have as documentation until I write the tutorials or something better.

Quicksands
----------

```c
	#define ENABLE_QUICKSANDS
```

Tiles with `beh == 2` make the player sink slowly.

Breakable
---------

```c
	// Breakable, beh & 16
	#define ENABLE_BREAKABLE				// Breakable walls
	#define BREAKABLE_LIFE	2				// Amount of hits to break wall
	#define BREAKABLE_ANIM					// Show explosion when breaking
	#define BREAKABLE_MAX 			4		// Max # of concurrent explosions
	#define BREAKABLE_MAX_FRAMES	8		// Frames to show explosion
	#define BREAKABLE_ERASE			0		// Tile to erase broken tiles
	#define BREAKABLE_BREAKING		8		// Tile to display while breaking
	#define BREAKABLE_WALKABLE				// If defined (side view), tiles break when stepped on
```

- Tiles with `beh & 16` are breakable. 
- They have to be shoot/hit `BREAKABLE_LIFE` times to break. 
- If `BREAKABLE_ANIM` is set, an intermediate tile (breaking, or explosion, id `BREAKABLE_BREAKING`) is shown for `BREAKABLE_MAX_FRAMES` game frames. 
- Broken tiles are replaced with tile id `BREAKABLE_ERASE`.
- If `BREAKABLE_WALKABLE` is set, tiles break if you walk over them.

Conveyors
---------

```c
	// Conveyors, beh & 32 [+1] (must be & 8!)
	// For player movement values, see section 4
	// (PLAYER_VX_CONVEYORS)

	#define ENABLE_CONVEYORS
```

Tiles for side-view which push the player left or right when walked over. Use beh & 32 (pushes right) or beh & 33 (pushes left).

Slippery
--------

```c
	// Slippery, beh & 64. (must be & 12!)
	// For player movement values, see section 4
	// (PLAYER_AX_ICE & PLAYER_RX_ICE)

	#define ENABLE_SLIPPERY
```

Tiles for side-view which are slippery when walked over. Use beh & 64.

The occluding frame
-------------------

Saws and Pezons need an "occluding frame" to do the sprite priority trick. It should be one frame using the darkest colour in the selected palette to mask areas of the sprite which should be hidden when appearing. Every 8x8 sprite in the metasprite should have at lease 1 #FF00FF pixel for the converter to autodetect it. See example 00.

Saws
----

- Type 8
- You define the travelling direction with start/end points in ponedor.
- It will emerge left/up if the attribute is `0`, right down if it is `2`.
- It needs two, not flipped frames, from `SAW_BASE_SPRID`.

Fanties / Homing fanties
------------------------

- Type 6
- Just define the position in ponedor.
- Use two frames from `FANTY_BASE_SPRID`, or four (right1, right2, left1, left2) if you define `FANTY_WITH_FACING`.

Chac-Chacs
----------

- Type 10 (0xA)
- 3 tiles tall, placed in ponedor marking the topmost tile.
- Attribute in ponedor is delay between shutter
- Needs 7 contiguous tiles in the tileset (can be outside the 16 first).
- Needs the behaviours of each of the 7 tiles to be properly set: `0, 0, 1, 1, 1, 1, 0`.

Monococos
---------

This was really bad implemented in Yun (very patchy). Let's do this properly this time.

A monococo is an enemy which

1. Is out of the screen for A frames.
2. Appears during B frames
3. When out, shoots one coco, and stays for C frames.
4. Then disappears during D frames.

We have two types.

Type A: step 1 shows no sprite. steps 2 & 4 shows sprite animation (2 cells) normally, but flickering via ` + half_life`.
Type B: step 1 shows a special "hidden" cell. steps 2 & 4 shows a special "appear / disappear" frame.

We will use 4 frame definitions: `FRAME_A`, `FRAME_B`, `APPEARING`, `HIDDEN`. If defined `MONOCOCO_TYPE_A`, cells 3 & 4 are ignored. You can even take them away from your metasprites array.

Monococo has a timer which is controlled by a number of "state times" in a 4 elements array, but controlled only by three #defines in config.h:

```c
const unsigned char monococo_state_times [] = {
	MONOCOCO_BASE_TIME_HIDDEN,
	MONOCOCO_BASE_TIME_APPEARING,
	MONOCOCO_BASE_TIME_ONBOARD, 
	MONOCOCO_BASE_TIME_APPEARING
};
```

- Type 11 (0xB)
- Just define the position in ponedor.

Cocos frames are (order may be off, but helps with saving bytes)

(RIGHT)  `FRAME_A`, `FRAME_B`, (LEFT)   `FRAME_A`, `FRAME_B`,
[(RIGHT) `APPEARING`, `HIDDEN`, (LEFT) `APPEARING`, `HIDDEN`]

Easy objects
------------

Provides infraestructure to place N objects in hotspots, N places to use such objects, and places to add custom code that is executed everytime you get an object and everytime you use an object.

Objects and places to use such objects are represented by hotspots, so just 1 per screen. 

hotspot values representing objects must fit in a range, defined by `HS_OBJ_MIN` and `HS_OBJ_MAX`.

hotspot values representing where to use each object must fit in the range defined by `HS_OBJ_MIN + HS_USE_OFFS` and `HS_OBJ_MAX + HS_USE_OFFS`.

So if you get object `N` you have to use it in the hotspot with value `N + HS_USE_OFFS`.

Every time you get an object, code included in `mainloop/on_object_got.h` (empty by default) is executed.

Every time you use an object successfully, code included in `mainloop/on_object_used.h` (empty by default) is executed.

There are two types of behaviour: Type A and Type B. If you define `HS_TYPE_A`, you enable Type A. If you undefine it, you enable Type B.

With type A, hotposts where you use objects are represented by the empty item, that is, `HS_OBJ_EMPTY`. When you use an object, the hotspot will use the objects's graphic.

With type B, everything must have its own graphic: objects, place to use them, and objects used on places. If object is item `N`, then where to use is `N + HS_USE_OFFS` and object used = `N + 2*HS_USE_OFFS`. Very important when define the `spr_it` array.

Combine this with `WIN_LEVEL_CUSTOM`! if you define `WIN_LEVEL_CUSTOM`, the level with end as soon as you make `win_level = 1`.

You can check the ending condition at  `mainloop/on_object_got.h` or `mainloop/on_object_used.h`, for instance, and raise `win_level`. You can use the `level` variable in multilevel games for different conditions.

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
	#define PLAYER_PUNCHES					// When on floor
	#define PLAYER_PUNCH_OFFS_X				15
	#define PLAYER_PUNCH_OFFS_Y				-7

	#define PLAYER_KICKS					// While airborne
	#define PLAYER_KICK_OFFS_X				12
	#define PLAYER_KICK_OFFS_Y				-3
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
	#define PROPELLERS_MAX 					4
	#define PROPELLERS_BASE_PATTERN			64
	#define PROPELLERS_MAX_LENGTH			6
	#define PROPELLER_TILE					14
```

Place in your map using tile `PROPELLER_TILE`. Propeller animation will alternate two 2x1 patterns cells defined in the bg tileset from pattern `PROPELLERS_BASE_PATTERN`. Place as much as `PROPELLERS_MAX` per screen.

If you define `PROPELLERS_MAX_LENGTH` the propeller will affect an area max N tiles high, otherwise it will reach the top of the game area.

Vertical movement is then controlled by these:

```c
	#define PLAYER_AY_FLOAT			16	
	#define PLAYER_VY_FLOAT_MAX		256
```

Simple Warpers
--------------

Simple warpers are portal which take you from where they are placed to an arbitrary position (screen, Y, X).

```c
	#define ENABLE_SIMPLE_WARPERS
	#define SIMPLE_WARPERS_BASE_SPRID	56
	#define SIMPLE_WARPERS_FIRE_BUTTON
```

They are placed *as enemies*, type = 0xff. Use `attr` for the destination screen number and `s1` for the YX coordinate (high nibble Y, low nibble X).

`SIMPLE_WARPERS_BASE_SPRID` must be defined as the base sprite id (in the enemy metasprites array) for warpers to display. Of course, you can use an expression depending on `half_life` or `frame_counter` if you want animation.

If `SIMPLE_WARPERS_FIRE_BUTTON` is defined, you have to press B to activate the portal.

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

