Tester Interactives
===================

Made to test script driven and code driven interactives.

Probably you know about **easy objects**, a simple system to carry objects around and use them using the *hotspots* system. If you don't, you can see it in action in `testers/00_tester_sideview` and `examples/08_cheril_perils_rom2_revamp`.

**Interactives** are a much more powerful system of managing objects and can be used alongside the scripting system or from code, depending on your needs.

This tester contains code to generate the same experience using both script driven and code driven interactives. To select one or another just comment or uncomment `#define ACTIVATE_SCRIPTING` in `config.h`.

**As of v1.0 you acn't use scripting *and* interactives from code at the same time.** - When scripting is on, interactives are created from the script only and all code related to interactives in the engine won't be included.

Flags
-----

There is a `flags []` array in the engine which is mainly used by the scripting system and other minor things. The amount of flags available depends on your needs so you can configure it using `MAX_FLAGS` in `config.h`.

Flags are value containers. They can contain values from 0 to 127 (values 128 to 255 may have special *internal* meanings, as you will eventually see).

Interaction
-----------

We'll be using this concept a lot. "To interact" means that the player metasprite overlaps with the interactive object and the player presses B.

Types of interactives
---------------------

Interactives are just stuff on screen you can interact with. They are rendered as metasprites using definitions in the `spr_it` array. As of v1.0, there are TWO types of interactives:

- Simple interactives (called *sprites*): Just sprites you can interact with.

- *Containers*: They are bound to a flag, and the displayed metasprite is that corresponding to the value stored in the flag. On interaction, the object you carry (which is a value) and the value stored in the flag are swapped.

If you know about **easy objects**, you probably know that the object you are carrying is stored in the `pinv` variable. When using interactives, the object you are carrying is stored in a flag. Which flag is again configured in `config.h` via the `FLAG_INVENTORY` directive.

So when you interact with a container, the value in the flag bound with the container and the value of `flags [FLAG_INVENTORY]` are swapped.

Display
-------

Interactives are created when you enter a new screen. They will exist on that screen only. The maximum number of interactives you can create in a screen is defined in `config.h` using `INTERACTIVES_MAX`. Note that this isn't a limit on how many interactives you have in your level.

Interactives have three attributes: X, Y and F. 

All interactives are defined to appear at a fixed position using tile coordinates (X, Y).

The graphic they display (from the `spr_it` metatile array) depends on the type of interactive:

- *Sprites* will show the metasprite defined as `spr_it [F]`.
- *Containers* will show the metasprite defined as `spr_it [flags [F]]`.

Initialization
--------------

**Containers make flags interactuable**. You can use containers and flags to place objects around the map you can get and carry, and also places where you can drop what you are carrying.

So everytime the game starts you must give initial values to the flags you are planning to bind with containers.

For example, if you want a rock to appear in screen 7, tile coordinates (3, 4) and the metasprite of such rock is on `spr_it [5]`, you have to:

- Choose a free flag to bind with the container. Let's say `flag [2]`.
- When the game starts, put the rock in the container: *assign "5" to `flag [2]`*.
- When entering screen 7, you create a new container with X = 3, Y = 4, and F = 2.

That way, when the player enters screen 7 for the first time, the engine will render `spr_it [flags [F]] = spr_it [flags [2]] = spr_it [5]` = the rock at (3, 4).

If the player is carrying a flower, which is the metasprite on `spr_it [6]`, and the player interacts with the container, the values of flags [2] and flags [FLAG_INVENTORY] will be swapped, and the engine will render `spr_it [flags [2]] = spr_it [6]` = the flower at (3, 4).

And this is how it works.

Interactives using scripting
============================

To use interactives and scripting...

```c
	#define ACTIVATE_SCRIPTING
	[...]
	#define ENABLE_INTERACTIVES
```

<TODO>

Interactives from C code
========================

To use interactives from C code...

```c
	//#define ACTIVATE_SCRIPTING
	[...]
	#define ENABLE_INTERACTIVES
```

<TODO>