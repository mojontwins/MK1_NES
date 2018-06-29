Che-Man
=======

This game was made as a present to @Octopusjig. It is a reimagining of Trabajo Basura, ported to the NES as "Dire Job" using a early version of MK2 and included in "Mojon Twins Gran Sabiduría" and "Mojonian Tales" as a filler.

This incarnation has been built from the ground up using **MK1/AGNES** in an attempt of testing the scripting system and the code injection points in `my/` working together. The graphics have been redone, and the map and enemy placement have been horizontally mirrored.

The purpose of the game is smashing all the batucadas (those hairy men with drums) and carrying all the helloween medallions to the ugly totems located in the central stairs, one by one. There are 25 batucadas and 15 medallions.

Only One Object (at a time)
===========================

**MK1/AGNES** supports several types of behaviours related to objects (or items), as you may know:

- The **basic**, legacy *collect'em'all* stype: There's a bunch of identical items scattered around the map. They are all placed using hotspots of type 1 (one per screen). When the player gets one, the `pobjs` counter is increased. In the default configuration, the player will win the game if all items are collected. "items" are traditionally called "objects" (hence the `pobjs` variable name, for instance).

- **One at a time**: items are still placed as hotspots of type 1, but the player can only carry one of them at a time. The player can't get a new item until its *inventory* is cleared, by external means. This method is not complete, therefore, as the coder must provide a way to free the player's *inventory*. We'll be discussing this behaviour in this document.

- **Easy objects**: items are still placed using hotspots, but there are several kind of objects, and the player can swap the one he or she is carrying for a new one, which will stay in place, or use them elsewhere (also using hotspots). This behaviour is better seen in action in `00_tester_sideview` or `08_cheril_perils_rom2_revamp`.

- **Interactives**: items are placed inside *interactives* of type *container*. Learn about them in `00_tester_interactives`.

As mentioned, the **One at a time** behaviour relies on an *external* actor to remove the item from the player inventory. This can be performed in a number of ways: from injected code or from the script. This game uses scripting to do so.

To activate this behaviour:

```c
    #define ENABLE_ONLY_ONE_OBJECT
```

You will need some preparations: first of all, you have to define a metasprite to represent "empty", and store it at index 0 in the `spr_hs` metasprite array. Also, you need to define where in the hud to display the player's *inventory* via the `HS_INV_X` and `HS_INV_Y` directives in `config.h`.

This is what happens when the player collides with a hotspot of type 1:

- If the player is carrying nothing, the player gets the item (hotspot is cleared). The code in `my/on_object_got.h` is executed.

- If the player is carrying an item, nothing happens.

By default, current object is stored to variable `pinv`. You can then use code injection to detect / react. To free up the player's *inventory* after doing *something*, you can add this code to `my/extra_checks.h`:

```c
    if (<some condition>) {
        // Free the player's inventory
        pinv = 0;

        // Do some stuff, for example...
        pobjs ++;
    }
```

In this game will be using the script to remove the item from the player's *inventory*. To make that possible, the object the player is carrying must be made available in a flag, so you can read or set it from your script.

```c
    #define ONLY_ONE_OBJECT_FLAG            0
```

If you define this, the engine will use the specified flag *instead of* `pinv`. Of course, you can still access the contents of the flag from injected code: it will be on `flags [ONLY_ONE_OBJECT_FLAG]`.

How I made this
===============

As mentioned, this game uses a mixture of scripting and code injection. The ZX Spectrum original used just scripting, the MK2 version custom modifications to the MK2 engine (which doesn't have "code injection" points like **MK1/AGNES**).

This implementation could have been done either way, but I chose this implementation trying to be didactic.

Configuration
-------------

Configuration in this game is pretty basic. On to the important parts...

First of all, we will decide when the player wins the game via code injection, so:

```c
    #define WIN_LEVEL_CUSTOM
```

We need text boxes to display several bits of info during the first try. Remember that text boxes need 9 special tiles in the active tileset (I *know* this is cumbersome, but may change in the future), from tile $20 (32) onwards, as configured:

```c
    #define ENABLE_TEXT_BOX
    #define TEXT_BOX_FRAME_TILE_OFFSET      0x20
```

Killable enemies (the batucadas) will need two hits to be exterminated. We also need them to stay dead (game win condition depends on it). Also, we'll need an explosion, which is in the `spr_en` metasprite array at index 32:

```c
    #define ENEMS_LIFE_GAUGE                2
    [...]
    #define PERSISTENT_DEATHS
    [...]
    #define ENEMS_EXPLODING_CELL            32
```

Activate the **one at a time** behaviour, using flag 0 to store the player's *inventory*:

```c
    #define ENABLE_ONLY_ONE_OBJECT          
    #define ONLY_ONE_OBJECT_FLAG            0
```

Activate scripting. All scripting options off

```c
    #define ACTIVATE_SCRIPTING
    //#define CLEAR_FLAGS
    //#define FIRE_ON_KILL
    //#define ENABLE_EXTERN_CODE
    //#define ENABLE_FIRE_ZONE  
```

Player has jump, player steps on enemies, but only make enemies type 3 (the batucadas) killable. As there are enemies which you can't kill, make them **jumpable-over**:

```c
    #define PLAYER_STEPS_ON_ENEMS
    #define PLAYER_STEPS_MIN_KILLABLE       3
    #define PLAYER_SAFE_LANDING 
```

Scripting
---------

Scripting in this game is in charge of checking if totems are used when the player is carrying an item, and free the inventory and increase the objects count if so.

```spt

    PRESS_FIRE ON SCREEN 2, 7, 12, 17
        IF PLAYER_TOUCHES (7, 10)
        IF $PINV = 1
        THEN
            SET $PINV = 0
            INC OBJECTS, 1
            SOUND 2
        END
    END

```

`$PINV` is an alias defined to represent `FLAG 0`.

Code injection
--------------

We use code injection for three things. The most important is checking the 'win game condition'. Code in `my/extra_checks.h` is run each frame, so it's the place to add:

```c
    win_level = (pobjs == MAX_HOTSPOTS_TYPE_1_0 && pkilled == MAX_ENEMS_TYPE_3_0);
```

That is, win_level will be true if all (`MAX_HOTSPOTS_TYPE_1_0`) items have been carried to the ugly totems and if all (`MAX_ENEMS_TYPE_3_0`) batucadas have been smashed.

The second thing we'll be injecting is a nice effect applied as a sprite overlay over the totems if the player is carrying a medallion, as if they were 'reacting' to them. Just paint a metasprite in the correct coordinates if we are in the screens containing totems and we are carrying something:

```c
    if (n_pant == 2 || n_pant == 7 || n_pant == 12 || n_pant == 17) {
        if (flags [ONLY_ONE_OBJECT_FLAG] && (half_life)) {
            oam_index = oam_meta_spr (
                112, 183, 
                oam_index, 
                ssit_04
            );
        }
    }
```

The third is displaying text boxes which will act as a tutorial of sorts. This will only happen during the first game, so:

```c
    if (first_game) {
        // Code here will only run during the first game
    }
```

We will define three custom variables to store if we already displayed each of the four text boxes. That's done in `my/extra_vars.h`:

```c
    unsigned char first_text;
    unsigned char first_batucada;
    unsigned char first_medallion;
    unsigned char first_totem;
```

And the code is:

```c
    if (first_game) {
        gp_gen = 0;

        if (ppossee && first_text) {
            first_text = 0;
            gp_gen = custom_text0;
        }

        if (pkilled && first_batucada) {
            first_batucada = 0;
            gp_gen = custom_text1;
        }

        if (flags [ONLY_ONE_OBJECT_FLAG] && first_medallion) {
            first_medallion = 0;
            gp_gen = custom_text2;
        }

        if (pobjs && first_totem) {
            first_totem = 0;
            gp_gen = custom_text3;
        }

        if (gp_gen) {
            oam_index = 4;
            player_render ();
            hud_update ();
            oam_hide_rest (oam_index); 
            oam_index = 4;
            textbox_do ();  
        }
    }
```

That that's how it's done!
