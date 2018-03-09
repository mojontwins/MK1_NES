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

Interactives are just stuff on screen you can interact with. They are rendered as metasprites using definitions in the `spr_hs` array. As of v1.0, there are TWO types of interactives:

- Simple interactives (called *sprites*): Just sprites you can interact with.

- *Containers*: They are bound to a flag, and the displayed metasprite is that corresponding to the value stored in the flag. On interaction, the object you carry (which is a value) and the value stored in the flag are swapped.

If you know about **easy objects**, you probably know that the object you are carrying is stored in the `pinv` variable. When using interactives, the object you are carrying is stored in a flag. Which flag is again configured in `config.h` via the `FLAG_INVENTORY` directive.

So when you interact with a container, the value in the flag bound with the container and the value of `flags [FLAG_INVENTORY]` are swapped.

Display
-------

Interactives are created when you enter a new screen. They will exist on that screen only. The maximum number of interactives you can create in a screen is defined in `config.h` using `INTERACTIVES_MAX`. Note that this isn't a limit on how many interactives you have in your level.

Interactives have three attributes: X, Y and F. 

All interactives are defined to appear at a fixed position using tile coordinates (X, Y).

The graphic they display (from the `spr_hs` metatile array) depends on the type of interactive:

- *Sprites* will show the metasprite defined as `spr_hs [F]`.
- *Containers* will show the metasprite defined as `spr_hs [flags [F]]`.

Initialization
--------------

**Containers make flags interactuable**. You can use containers and flags to place objects around the map you can get and carry, and also places where you can drop what you are carrying.

So everytime the game starts you must give initial values to the flags you are planning to bind with containers.

For example, if you want a rock to appear in screen 7, tile coordinates (3, 4) and the metasprite of such rock is on `spr_hs [5]`, you have to:

- Choose a free flag to bind with the container. Let's say `flag [2]`.
- When the game starts, put the rock in the container: *assign "5" to `flag [2]`*.
- When entering screen 7, you create a new container with X = 3, Y = 4, and F = 2.

That way, when the player enters screen 7 for the first time, the engine will render `spr_hs [flags [F]] = spr_hs [flags [2]] = spr_hs [5]` = the rock at (3, 4).

If the player is carrying a flower, which is the metasprite on `spr_hs [6]`, and the player interacts with the container, the values of flags [2] and flags [FLAG_INVENTORY] will be swapped, and the engine will render `spr_hs [flags [2]] = spr_hs [6]` = the flower at (3, 4).

And this is how it works.

In config.h
-----------

```c
#define ENABLE_INTERACTIVES 
#define INTERACTIVES_MAX        4
#define FLAG_INVENTORY          0
```

As mentioned, the object the player carries is stored in `flag [FLAG_INVENTORY]` instead of `pinv`. 

`INTERACTIVES_MAX` means the maximum amount of interactives present *on screen* at the same time. It's good practice to adjust this number as low as possible (i.e. if the screen in your game which contains the most interactives contains just 2, set `INTERACTIVES_MAX` to 2).

```c
    #define HS_INV_X                136 
    #define HS_INV_Y                11  
```

If you want that the object you are carrying appears on screen (in the HUD), define these two directives. Their values are the (X, Y) coordinates (in pixels) of the sprite.

In metasprites.h
----------------

Interactives are rendered on screen using metasprites in the `spr_hs` array defined in `assets/metasprites.h`. The numbers used are the actual index of the metasprite in the array.

Empty containers
----------------

There's no special case to represent "empty" containers. The "empty" state is just simulated using a normal value. Just define an "empty" metasprite in `spr_hs` and its index in the array will mean "empty container" or "empty carried object".

This example / tester
=====================

There are 5 screens laid out vertically. I've placed five containers, four of them contain items, and one of them is empty. There's a monkey closing your path at the top. You have to place one of the objects (the skull) in the empty container so the monkey disappears and you can exit the tower. Cheril will appear in the same screen as the monkey and will tell you what to do if you interact with her. The empty container where you can place objects will show a text every time you interact with it.

Interactives using scripting
============================

Here we will implement the game using scripting.

To use interactives and scripting...

```c
    #define ACTIVATE_SCRIPTING
    [...]
    #define ENABLE_INTERACTIVES
```

Handy aliases
-------------

First of all you should define aliases for all the containers and the object the player is carrying in your script. For example:

```spt
    DEFALIAS
        $PINV           0
        $CONT_BOTTLE    1
        $CONT_EYE       2
        $CONT_SKULL     3
        $CONT_PUFF      4
        $CONT_OFFER     5
    END
```

In the example we are using $PINV to point to the flag which represents object the player is carrying. This mean that there should be a matching `#define FLAG_INVENTORY 0` in `config.h`.

Populating containers
---------------------

Containers will be bound to flags when placed, and such flags should contain a proper value. The $PINV flag should be initialized as well. 

In this example, we have four objects: bottle, eye, skull and puff of smoke (!). We also have the "empty" object, and a metatile representing Cheril. Metatiles for them are laid out in the `spr_hs` as follows:

```
    spr_hs array:

    index   contents
    ------- -------------------------------
    0       the "empty" object 
    1       hotspot collectibles (not used)
    2       hotspot keys (not used)
    3       life refills
    4       a bottle
    5       an eye
    6       a skull
    7       a puff of smoke
    8       a nice sprite of Cheril
```

Initializations are performed in the `ENTERING GAME` section in your script:

```spt
    ENTERING GAME
        IF TRUE
        THEN
            # Inventory: Empty
            SET $PINV = 0

            # Containers with stuff:
            SET $CONT_BOTTLE = 4
            SET $CONT_EYE = 5
            SET $CONT_SKULL = 6
            SET $CONT_PUFF = 7

            # An empty container
            SET $CONT_OFFER = 0
        END IF
    END
```

Note how we assign the objects to their flags, and the empty object to both the inventory and $CONT_OFFER, which is the flag which will be bound to the container over the pedestal where we should place the "skull" object.

Placing interactives
--------------------

Interactives (both plain sprites and containers) have to be placed on screen. That is generally done in the `ENTERING SCREEN x` section in the script, but may be added anywhere (imagine you have to make a sprite appear when some condition is met in the `PRESS_FIRE_AT SCREEN x` section, for example).

To place a sprite:

```spt
    ADD_SPRITE S, X, Y
```

Where S is the metasprite index in `spr_hs` and (X, Y) the coordinates (tile coordinates, X = 0..15; Y = 0..12).

To place a container:

```spt
    ADD_CONTAINER F, X, Y
```

Where F is the flag bound to the container (or an alias to it), and (X, Y) the coordinates (tile coordinates, X = 0..15; Y = 0..12).

Checking containers
-------------------

You can check the contents of a container normally as they are bound to flags. Of course, you can do it even if the container will be placed in a different screen. See the screen by screen analysis later.

Checking interaction
--------------------

When the player interacts with an interactive, the interactive will be referenced by the special script variable `ARG`, `JUST_INTERACTED` will evaluate to `TRUE` and the `PRESS_FIRE` sections in the script will be run. Sprites and containers are checked for differently (due to simple implementation and 8bits stuff):

To check if the player interacted with sprite S:

```spt
    IF JUST_INTERACTED
    IF ARG = S
```

To check if the player interacted with container F (executed after the objects have been swapped):

```spt
    IF JUST_INTERACTED
    IF ARG ~ F
```

Screen by screen analysis
-------------------------

The game ends when the player exits the tower using the top screen, to the right. The monkey who is blocking the way out is placed in the map, so when the conditions are met, we should remove it. Such conditions is that item "skull" (value `6`) is placed on the pedestal (container bound to `$CONT_OFFER`).

The map is 1 column wide, so "exit the tower" will be detected if the player enters the last column of tiles to the right.

As the action of placing the skull in the pedestal will be performed in a different screen, we do the checks and the tile map modification in the `ENTERING` section.

```spt
    ENTERING SCREEN 0
        # If there's a skull in the pedestal...
        IF $CONT_OFFER = 6
        THEN
            SET TILE (8, 6) = 3
            SET_FIRE_ZONE_TILES 15, 0, 15, 6
        END

        IF TRUE
        THEN
            ADD_SPRITE 8, 2, 9
        END
    END
```

So if `$CONT_OFFER` contains object 6 (the skull), then remove the monkey via overwritting it with a tile 3 (see the tileset in `gfx/`) and create the fire zone which will autorun the `PRESS_FIRE` scripts when the player enters such area.

We also are adding Cheril (metasprite 8 in `spr_hs`) to (2, 9) - and making her interactuable.

The "win game" condition check and the interaction with Cheril are of course in the `PRESS_FIRE` section of screen 0:

```spt
    PRESS_FIRE AT SCREEN 0
        IF PLAYER_IN_X_TILES 15, 15
        THEN
            WIN GAME
        END

        IF JUST_INTERACTED
        IF ARG = 8
        THEN
            TEXTBOX 0
        END
    END
```

Note the `IF JUST_INTERACTED : IF ARG = 8`. Remember that interaction with sprites is checked using `=`. The above clausules will evaluate to true if and only if the player entered the scripting system by interacting with interactive sprite 8 (Cheril). Note how it doesn't really matter where you placed it. You can, of course, check for the player position after a `JUST_INTERACTED`, if you need so.

In this case, interaction will trigger a text box with text 0. Text boxes should be covered elsewhere. Just remember to activate and configure them in `config.h` and to create an `assets/custom_texts.h` file with the strings and an index to them called `custom_texts`. Note as well how the tiles used to draw the text box frame are in the tileset.

The next screen, going donwards, screen 1, contains the pedestal where you have to place the skull to solve the game. The pedestal itself is in the map at coordinates (8, 9), and all we have to do is place a container bound to flag `$CONT_OFFER` on top of it.

```spt
    ENTERING SCREEN 1
        IF TRUE
        THEN
            ADD_INTERACTIVE $CONT_OFFER, 8, 8
        END
    END
```

We want to display a text after the player swaps their inventory with the object contained in $CONT_OFFER, so we add a detection:

```spt
    PRESS_FIRE AT SCREEN 1
        IF JUST_INTERACTED
        IF ARG ~ $CONT_OFFER
        THEN
            TEXTBOX 1
        END
    END
```

Note the `IF JUST INTERACTED: IF ARG ~ $CONT_OFFER`. Remember that interaction with containers is checked using `~`. The above clausules will evaluate to true if and only if the player entered the scripting system by interacting with the container bound to `$CONT_OFFER`.

In such case we display a text box with text 1.

Screens 2, 3 and 4 are pretty straightforward. We just place 1 container on screens 2 and 3, and 2 on screen 4.

```spt
    ENTERING SCREEN 2
        IF TRUE
        THEN
            ADD_INTERACTIVE $CONT_BOTTLE, 4, 8
        END IF
    END IF

    ENTERING SCREEN 3
        IF TRUE
        THEN
            ADD_INTERACTIVE $CONT_EYE, 14, 4
        END IF
    END IF

    ENTERING SCREEN 4
        IF TRUE
        THEN 
            ADD_INTERACTIVE $CONT_SKULL, 1, 2
            ADD_INTERACTIVE $CONT_PUFF, 13, 3
        END IF
    END IF
```

Interactives from C code
========================

Here we will implement the game using just C code.

To use interactives from C code...

```c
    //#define ACTIVATE_SCRIPTING
    [...]
    #define ENABLE_INTERACTIVES
```

Using interactives from within C code implies adding code to some files in the `my/` folder, is generally simpler, but of course less powerful / more fiddly. If you are not a C coder you'd better use the scripting system.

Besides, if you are already using the scripting system for your game (`ACTIVATE_SCRIPTING` is defined), you *cant* use interactives from C code. Both techniques are **mutually exclusive**.

Also, dont' use `ENABLE_EASY_OBJECTS` alongside `ENABLE_INTERACTIVES`. Those are **mutually exclusive** as well.

Populating containers
---------------------

Containers will be bound to flags when placed, and such flags should contain a proper value. The `FLAG_INVENTORY` flag should be initialized as well. 

In this example, we have four objects: bottle, eye, skull and puff of smoke (!). We also have the "empty" object, and a metatile representing Cheril. Metatiles for them are laid out in the `spr_hs` as follows:

```
    spr_hs array:

    index   contents
    ------- -------------------------------
    0       the "empty" object 
    1       hotspot collectibles (not used)
    2       hotspot keys (not used)
    3       life refills
    4       a bottle
    5       an eye
    6       a skull
    7       a puff of smoke
    8       a nice sprite of Cheril
```

The initialization is performed in `my/interactives_setup.h`:

```c
    flags [FLAG_INVENTORY] = 0; // Carrying nothing

    flags [1] = 4;              // Container for bottle
    flags [2] = 5;              // Container for eye
    flags [3] = 6;              // Container for skull
    flags [4] = 7;              // Container for puff

    flags [5] = 0;              // An empty container
```

Note how we assign the objects to their flags, and the empty object to both the inventory and `flags [5]`, which is the flag which will be bound to the container over the pedestal where we should place the "skull" object.

Placing interactives
--------------------

The interactives are placed around the map using arrays defined in `assets/interactives.h`, one per level. As our example only has one level, we only need an array. 

The array will contain screen number, coordinates (YX) and **value** for each interactive.

If the interactive is a **sprite**, **value** should be the index to the desired metasprite in the `spr_hs` array.

If the interactive is a **container**, the value should be `0x80 | F`, where F is the flag the container is bound to.

So, in our example:

```c 
    const unsigned char interactives_0 [] = {
        0, 0x92,        8,      // screen 0, SPR Cheril
        1, 0x88, 0x80 | 5,      // screen 1, CONT pedestal
        2, 0x84, 0x80 | 1,      // screen 2, CONT bottle
        3, 0x4e, 0x80 | 2,      // screen 3, CONT eye
        4, 0x21, 0x80 | 3,      // screen 4, CONT skull
        4, 0x3d, 0x80 | 4,      //       and CONT puff of smoke

        0xff
    };
```

Interaction
-----------

When an interactive is interacted with (!), the code containerd in `my/on_interactive.h` is run. The interactive **value** will be copied to variable `rdc`. Remember:

If the interactive is a **sprite**, `rdc` will contain the index to the desired metasprite in the `spr_hs` array.

If the interactive is a **container**,  `rdc` will contain `0x80 | F`, where F is the flag the container is bound to.

So, in our example:

```c
    if (rdc == 8) {
        gp_gen = custom_text0; textbox_do (); 
    }

    if (rdc == (0x80 | 5)) {
        gp_gen = custom_text1; textbox_do (); 
    }
```

Which translates to:

- If the user interacted with sprite '8' (Cheril), display a textbox with `custom_text0`.

- If the user interacted with container '5' (the pedestal), display a textbox with `custom_text1`.

And that's it!.
