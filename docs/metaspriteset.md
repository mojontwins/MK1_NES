AGNES' metaspriteset
====================

AGNES' metaspriteset is the collection of metasprites used in the game, and is divided into three distinct groups, each one in a different array:

- **Player metaspriteset** - all metasprites representing every cell the main character uses.
- **Enemies metaspritesets** - metasprites representing all enemies. You can have diferent enemies metaspritesets as per level.
- **Items / interactives metaspritesets** - Used to paint items, keys, NPCs, interactive objects, etcetera.

Metaspritesets are defined in `dev/assets/metasprites.h`.

Player metaspriteset
--------------------

Which cells are needed to animate a player is not set in stone. You'll have to write actual code to map the player actions and state to actual animation cells (metasprites). So the main player cell mapping can be performed in a number of ways - we will describe the one we use. 

The player metaspriteset is an array defined in `dev/assets/metasprites.h` with the name `spr_player`. In its most simple form, it will cover the states and "idle", "walking", "airborne", facing right then facing left. In more complex configurations it may include metasprite sequences for more states, such as "using", "climbing", etcetera. You can check the example **Cheril the Writer** to see a rather complex definition.

For example, suppose we have imported 9 animation cells facing right and 9 more facing left. The identifiers are `sspl_XX_F` (the first 6) and `sspl2_YY_F` (the last 3), with XX ranging from `00` to `05`, YY ranging from `00` to `02` and F being `a` for cells facing right and `b` for cells facing left. The metasprites were cut from left to right, so:

- `sspl_00_a` and `sspl_00_b`: shows the player's *idle* state (standing up, stopped).
- `sspl_01_a` to `sspl_04_a` and their corresponding `*_b`s: *walking* state.
- `sspl_05_a` and `sspl_05_b`: *airborne* state, jumping or falling.
- `sspl2_00_a` to `sspl_02_a` and their corresponding `*_b`s: *swimming* animation.

We just put them all together in an array, first facing right, then facing left:

```c
	// Player metasprites
	const unsigned char * const spr_player [] = {
		// Facing right (offset 0)
		sspl_00_a,										// Standing (offset 0)
		sspl_01_a, sspl_02_a, sspl_03_a, sspl_04_a,		// Walk cycle (offset 1)
		sspl_05_a, 										// Airborne (offset 5)
		sspl2_00_a, sspl2_01_a, sspl2_02_a,				// Swimming cycle (offset 6) (for DX)
		

		// Facing left (offset 9)
		sspl_00_b,										// Standing (offset 0)
		sspl_01_b, sspl_02_b, sspl_03_b, sspl_04_b,		// Walk cycle (offset 1)
		sspl_05_b, 										// Airborne (offset 5)
		sspl2_00_b, sspl2_01_b, sspl2_02_b				// Swimming cycle (offset 6) (for DX)
	};
```

As you see, we take note of the relative offsets of each metasprite representing the first cell of each state. We will need such offsets to write the code which maps metasprites to actions / states, so it's a good idea to write constants for them. Such constants are defined in `config.h`, near the end, in a list of `#define`s in the form `CELL_*`. Let's define a list for our games (you will find more in the default `config.h`. You can delete the ones you won't be using if you like):
 
```c 
	// config.h, line ~574
	#define CELL_FACING_RIGHT	0
	#define CELL_FACING_LEFT	9

	#define CELL_IDLE			0
	#define CELL_WALK_CYCLE		1
	#define CELL_AIRBORNE		5
	#define CELL_SWIM_CYCLE		6
```

Enemies metaspritesets
----------------------

As mentioned, you can have several enemies metaspritesets and choose which one to use for each level in your game.

Save for some exceptions, each type of enemy uses a fixed set of 8 metasprites, 4 facing right, 4 facing left. From each subset, the first two are used to animate the baddie (they can contain the same value if you don't need animation), the third is the *attack* cell, and the fourth the *hit* cell. Those last two will only be used under certain circumstances; commonly we'll leave them to 0.

Let's suppose we have three types of enemies and moving platforms. The enemies were cut and imported in two goes: one for 16x24 pixels enemies and one for 24x16 pixel enemies. The moving platforms (which are 16x16) were cut & imported separately as well. We didn't generate metasprites with horizontally flipped versions of the cells for any of those, so we'll repeat the same info for the "facing right" and "facing left" subsets. The generated arrays are:

- `ssena_00` and `ssena_01` contain the 2 animation cells for type 1 enemies (a skeleton).
- `ssena_02` and `ssena_03` contain the 2 animation cells for type 2 enemies (knight with sword).
- `ssenb_00` y `ssenb_01` contain the 2 animation cells for type 3 enemies (a bat).
- `ssplat_00` y `ssplat_01` contain the 2 animation cells for moving platforms (which in fact are type 4 *enemies*).

> By default, in **AGNES**, the first 4 subsets of enemies metasprites in each array correspond to enemy types 1-3 and moving platforms for side-view games and to enemy types 1-4 for top-down games. All of them are linear moving "patrollers" which describe a path between two pivots. 

The first enemies metasprite array is as follows:

> If you are making a single-level game and don't want to fiddle too much with the default code in `src` just name this array `spr_enems0`.

```c
	const unsigned char * const spr_enems0 [] = {
		// Linear enems and platforms (ids 1-4)
		ssena_00, ssena_01, 0, 0, ssena_00, ssena_01, 0, 0, // 1
		ssena_02, ssena_03, 0, 0, ssena_02, ssena_03, 0, 0, // 2
		ssenb_00, ssenb_01, 0, 0, ssenb_00, ssenb_01, 0, 0, // 3
		ssplat_00, ssplat_01, 0, 0, ssplat_00, ssplat_01, 0, 0, // Platform (4)
	};
```

Note how, as we didn't generate horizontally flipped versions for these entities, we just repeat cells in the 'facing right' subset (first four entries in each line) and the `facing left` subset (last four entries in each line). As mentioned, we used zeroes to cover the entries we won't be using.

Now we'll add a second enemies metasprites array named `spr_enems1` for a different level.

Let's suppose the enemies we imported for the second level are all 16x16 pixels (2x2 patterns). The first two weren't flipped, but the third was: we commanded `mkts.exe` to generate horizontally flipped versions. The generated arrays are:

- `ssenc_00` and `ssenc_01` contain the 2 animation cells for type 1 enemies (frog).
- `ssenc_02` and `ssenc_03` contain the 2 animation cells for type 2 enemies (squid).
- `ssend_00_a`, `ssend_01_a` and their corresponding `*_b`s contain the 2 animation cells for type 3 enemies (fish).

So the array for this level would be:

```c
	const unsigned char * const spr_enems1 [] = {
		// Linear enems and platforms (ids 1-4)
		ssenc_00, ssenc_01, 0, 0, ssenc_00, ssenc_01, 0, 0, // 1
		ssenc_02, ssenc_03, 0, 0, ssenc_02, ssenc_03, 0, 0, // 2
		ssend_00_a, ssend_01_a, 0, 0, ssend_00_b, ssend_01_b, 0, 0, // 3
	};
```

Note how the third row contains `_a` cells first (facing right), and `_b` cells second (facing left). Rows 1 and 2 have the same metasprites for both directions.

There's no more definitions as this second level will only contain enemies 1-3.

Metaspritesets for items / etc
------------------------------

This last set is somewhat custom, up to a degree at least. Anyways, if you don't want to fiddle too much, it's a good idea to reserve the first four slots to "nothing", "collectable", "key" and "life refill". "nothing" may be a proper metasprite (if you are using *easy objects* for example) or just "0" if it isn't used.

After the fourth entry, we can do more or less whatever we want. In our case we'll save positions 5 and 6 for extra, custom items related to hotspots:

```c
	const unsigned char * const spr_hs [] = {
		0, ssit_00, ssit_01, ssit_02, ssit_03, ssit_04
	};
```
