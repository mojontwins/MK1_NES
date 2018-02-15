Cheril Perils ROM2 Revamp
=========================

Back in 2015 we were making the ill-fated big Cheril game and Anjuel created a gorgeous tileset for the factory level which was sitting in my hard drive since then. 

I've decided it will be great to use it in a short game to test out some stuff and re-add some of the custom stuff as selectable components, such as:

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
