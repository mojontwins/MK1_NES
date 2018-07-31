Espinete Bad
============

A rather interesting port of Sonic Bad (which originally runs on a rather modified MK2 engine) using only "legal" techniques to expand functionality, i.e., code injection points, even a 100% custom module.

How I made this
===============

One of the things I like about **AGNES** is that it's fun (well, at least for me) thinking about how to implement what you have in mind using what the engine offers. **Sonic Bad** was a joke game which tried to mimmick the gameplay in Sonic (well, kind of). It has those gameplay items:

* The main character has low acceleration, low friction, but high max speed.
* The main character collects rings, which are part of the background. The game has to remember which rings have been already collected. Luckily **AGNES** supports this, using the unused nametable to store which tiles have been "got" in each screen.
* When the main character is hit by a spike or an enemy, and carries at least 1 ring, rings are lost. If no rings are being carried, one life is lost and the level is restarted.
* The player "throws" a ring when hit, and such ring should bounce around the screen and can be taken.

Creating the ring
-----------------

The ring has been implemented using a custom module written for this game. The module is `my/ring.h`. To make **AGNES** include extra code you have to write `#includes` in `my/extra_modules.h`:

```c
	// Ring implementation, Sonic SMS style:
	#include "my/ring.h"
```

The ring needs several custom variables we have to create in `my/extra_vars.h`...

```c
	signed int ring_x, ring_y;		// Ring fixed point coordinates (10.6)
	signed int ring_vx, ring_vy;	// Ring fixed point velocities (10.6)
	unsigned char ring_timer;		// Ring timer + "is on" flag (when != 0)
	unsigned char prings, oprings;	// Rings collected.

	// To animate the ring we will use those metasprites:

	const unsigned char *const spr_ring [] = {
		ssit_00, ssit_04, ssit_05, ssit_06
	};
```

And initialize minimally in `my/extra_inits.h`:

```c
	prings = 0; oprings = 0xff;
```

Besides, when entering a new screen, if a ring was active in the previous one, it should be deactivated. So in `my/on_entering_screen.h`:

```c
	// Turn off ring if it is on
	ring_timer = 0;
```

We also want to show how many rings the player collected in the HUD. To do so, just add code to `my/extra_hud_update.h`:

```c
	if (oprings != prings) {
		oprings = prings;
		_x = 0x1d; _y = 4; _n = prings; p_t ();
	}
```

The implementation of the ring is a fairly simple one, just a bouncing object which can collide with the background. It uses a couple of shortcuts (for example, it repurposes the `collide` function, which is used for enemies) and focus has been put on making the code compact and fast (so sorry, it's not very readable). On `my/ring.h` there are two functions:

```c
	void ring_create (void) {
		// Creates an animated ring at px, py
	}

	void ring_do (void) {
		// If the ring is active, animate it
	}
```

Now it's time to add hooks for those functions in the engine.

Getting rings
-------------

When `ENABLE_TILE_GET` is on, the player can *get* tiles of behaviour 34 from the screen. The engine only takes care of deleting the tile and granting persistence (if activated), but does nothing else. If we want the engine to react to the player collecting rings, we have to inject code via `my/on_tile_got.h`. We are going to simply increase the rings counter:

```c
	prings ++;
```

Player hit with and without rings
---------------------------------

The main gameplay hook is that you can save your life if you are carrying rings when hit. To achieve this custom behaviour we have to make use of two code injection points: `my/on_player_hit.h` and `my/on_player_spike.h`.

When an enemy hits a player, the engine does some stuff, then sets two flags `en_sg_1` and `en_sg_2`. The first one tells if the enemy involved in the collision should receive damage. It's usually 0, but in several cases. The second tells if the player should receive damage. It's usually 1.

Before checking `en_sg_*` and acting accordingly, the engine includes the code in `my/on_player_hit`. This way you can modify the behaviour. In our case, we are going to check if the player was actually hit (`en_sg_2` is set) and if it has rings (`prings` is not zero). If so, we are going to simulate a hit but make sure the player won't get damage (setting `en_sg_2` to zero):

```c
	if (en_sg_2 && prings) {

		// Create animated ring:
		ring_create ();
		prings = 0;

		// Simulate hit
		pvx = ADD_SIGN (_en_mx, PLAYER_V_REBOUND); _en_mx = ADD_SIGN (_en_x - prx, ABS (_en_mx));
		pvy = ADD_SIGN (_en_my, PLAYER_V_REBOUND); if (!_en_mx) _en_my = ADD_SIGN (_en_y - pry, ABS (_en_my));
		touched = 1; 
		sfx_play (SFX_PHIT, 0);
		pbouncing = 16;

		// But don't hurt player!
		en_sg_2 = 0;
	}
```

Note how we are creating the animated ring here via `ring_create`, and setting `prings` to zero.

In a similar way, when the player is hit by a spike (or killing tile), the code in `my/on_player_spike.h` is included before checking, on `en_sg_2`, if the player should receive damage. Here we don't have to check `en_sg_2` which is not set beforehand (as all collisions with spikes should result on the player getting hit, normally), so we only have to check for rings. Also, we don't have to explicitly make the player bounce this time, as the code which deals with spikes already takes care of doing so.

```c
if (prings) {

	// Create animated ring:
	ring_create ();
	prings = 0;

	// Simulate hit
	sfx_play (SFX_PHIT, 0);
	pbouncing = 16;

	// But don't hurt player!
	en_sg_2 = 0;
}
```

Make the ring move!
-------------------

We have already created the ring, but we have to tell the engine it should call its update function to make it, well, happen. There's a code injection point which gets included right after every actor has updated. As our ring is an actor, we'll add the hook to the `ring_do` function in `my/extra_routines.h`:

```c
	ring_do ();
```

Emmeralds
---------

Also of interest
----------------

The game features a nice custom renderer, like most AGNES games. For levels 0 and 1 it adds clouds to the background selecting a random location and checking if there's room (but it doesn't try very hard):

```c
	case 0:
	case 1:
		gpit = 4; while (gpit --) {
			gpjt = 4; while (gpjt --) {
				rdx = rand8 () & 0xf;
				rdy = rand8 () & 0xf;
				rda = (rdy << 4) | rdx;
				if (map_buff [rda] == 0 && map_buff [rda + 1] == 0 && rdy < 12) {
					map_buff [rda] = 16;
					map_buff [rda + 1] = 17;
					break;
				}
			}
		}
		break;
```

