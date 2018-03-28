Cheril Perils ROM2 Revamp
=========================

Back in 2015 we were making the ill-fated big Cheril game and Anjuel created a gorgeous tileset for the factory level which was sitting in my hard drive since then. 

I've decided it will be great to use it in a short game to test out some stuff and re-add some of the custom stuff as selectable components.

This is a multilevel games which utilizes quite a lot of MK1 stuff so it may be interesting to disect. First of all, those are the main features:

- **Full featured CNROM**, this includes extra pattern sets and maps stored in spare CHR-ROM pages. This game uses CHR0 and CHR1 for ingame graphics, CHR2 for titles & cutscenes, and CHR3 to store the map data.

- **Full featured platformer**, with ladders, jumping on enemies, use animation, easy objects, interactive sprites, programmed enemies, quicksands ...

- **Adventure elements**, talk to characters, find and carry items, use them, solve simple puzzles, etc.

- **Sorta final boss**, in fact a normal enemy which is a bit of a pain to kill, but it works.

- **Five levels with different conditions**, so you can learn how to manage lots of stuff in a multi-level environment.

- **Lots of code injection**: almost every facility in the `my/` folder is populated.

How I made this
===============

I'd check compile.bat to see how every asset is built and placed before compiling. Notice the `-D CNROM` directives. Read [features.md](https://github.com/mojontwins/MK1_NES/blob/master/features.md) for a proper description.

Let's examine the `config.h` file first, then build each level one by one.

Configuration
-------------

Let's talk about some choices inside `config.h` running from top to bottom:

Of course, this is a multi level game:

```c 
    #define MULTI_LEVEL 
    #define MAX_LEVELS              5
```

This game uses resonators, and implements them as type 4 hotspots. Resonators are special devices which paralyze enemies for 10 seconds. I've added them to the engine for no reason. 

```c
    #define HOTSPOT_TYPE_RESONATOR  4
```

Each level's "win condition" is custom, as we'll see later.

```c
    #define WIN_LEVEL_CUSTOM
```

Maps are in CHR-ROM:

```c
    #define MAP_FORMAT_CHRROM
```

No *collect'em'all items* (objects) ([check this!](https://github.com/mojontwins/MK1_NES/tree/master/examples/10_che_man)), so it's good practice to leave out all related code:

```c 
    #define DEACTIVATE_OBJECTS
```

We need quicksands (map tiles with `beh = 2`):

```c
    #define ENABLE_QUICKSANDS
```

And conveyors (map tiles with beh & (8 | 32)):

```c
    #define ENABLE_CONVEYORS
```

And ladders (map tiles with beh == 32). More on them latter, as half of their functionality is customized when selecting the main character cell.

```c
    #define ENABLE_LADDERS
```

And propellers. 

```c 
    #define ENABLE_PROPELLERS
    #define PROPELLERS_MAX                  6
    #define PROPELLERS_BASE_PATTERN         64
    #define PROPELLERS_MAX_LENGTH           5
    #define PROPELLER_TILE                  24
    //#define PROPELLERS_ON_BY_DEFAULT
```

Propeller tile 24 will place a propeller. This is a multilevel game so does it mean that tile 24 will spawn propellers in every level? Yes and no. `PROPELLERS_ON_BY_DEFAULT` is commented out, so unless you make `propellers_on = 1` *manually*, no propellers will be placed.

In fact, we will be using propellers only on level 4 (the factory), and they are off until we use a special item. We will set `propellers_on` manually when that happens.

We will need shines, as well, further explained [here](https://github.com/mojontwins/MK1_NES/blob/master/features.md).

```c
    #define ENABLE_SHINES
    #define SHINES_MAX                      8
    #define SHINES_BASE_PATTERN             10
    #define SHINES_PALETTE                  3
    #define SHINING_TILE                    23
```

Shines are always on no matter what (as of MK1 v1.0), so map tile 23 will always spawn shines. This is not really a problem as every level has a fitting tile (spikes, hooks, or even icy water) which I've placed as tile #23 in the metatileset.

And we'll need resonators, further explained [here](https://github.com/mojontwins/MK1_NES/blob/master/features.md):

```c
    #define ENABLE_RESONATORS
    #define RESONATOR_BASE_PATTERN          0
    #define RESONATOR_PALETTE               3
    #define RESONATOR_COUNTER_OFFS_X        4
    #define RESONATOR_COUNTER_OFFS_Y        7
    //#define RESONATOR_CHANGE_BG_PAL       paltsgrey
    #define RESONATOR_CHANGE_SPR_PAL        palssgrey   
```

Cheril will say "NO!" when you attempt to clear a lock without a key, further explained [here](https://github.com/mojontwins/MK1_NES/blob/master/features.md):

```c 
    #define ENABLE_NO 
    #define NO_METASPRITE                   ssit_06
    #define NO_OFFS_X                       0
    #define NO_OFFS_Y                       -24
```

And a "use animation", further explained [here](https://github.com/mojontwins/MK1_NES/blob/master/features.md)

```c
    #define ENABLE_USE_ANIM
    #define USE_ANIM_MAX_FRAMES             13
    #define USE_ANIM_INTERACT_ON            7
    #define USE_ANIM_FRAMES_PER_STEP        4
```

We will be using full-fledged text boxes, complete with optional portraits and dialogue support (more on this later):

```c
    #define ENABLE_TEXT_BOX
    #define TEXT_BOX_FRAME_TILE_OFFSET      0x20
    #define TEXT_BOX_WITH_PORTRAITS
    #define TEXT_BOX_DIALOGUES
```

In this game, levels are won if all enemies are killed, so it's important that they don't respawn if we reenter a screen. It's also important that the engine remembers where they were last time we visited:

```c
    #define PERSISTENT_ENEMIES
    #define PERSISTENT_DEATHS
```

We will need simple fanties:

```c
    #define ENABLE_FANTY
    [...]
```

And saws, further explained [here](https://github.com/mojontwins/MK1_NES/blob/master/features.md)

```c
    #define ENABLE_SAW
    [...]
```

And steady shooters, further explained [here](https://github.com/mojontwins/MK1_NES/blob/master/features.md)

```c 
    #define ENABLE_STEADY_SHOOTERS
    [...]
```

And compiled enemies, further explained [here](https://github.com/mojontwins/MK1_NES/blob/master/features.md)

```c 
    #define ENABLE_COMPILED_ENEMS
    [...]
```

For item management, we will need `easy objects`. Check [here](https://github.com/mojontwins/MK1_NES/blob/master/features.md), and [this tester](https://github.com/mojontwins/MK1_NES/tree/master/testers/00_tester_sideview).

```c
    #define ENABLE_EASY_OBJECTS

    #define HS_OBJ_EMPTY                    1
    #define HS_OBJ_MIN                      6
    #define HS_OBJ_MAX                      13
    #define HS_USE_OFFS                     8
    //#define HS_TYPE_A
    #define HS_FIX_ON_USE
```

Notice the numbers:

* `spr_hs [1]` contains the "empty" item.
* `spr_hs [6]` to `spr_hs [13]` contain every item in the game.
* `spr_hs [14]` to `spr_hs [21]` contain where to use each item.
* `spr_hs [22]` to `spr_hs [29]` contains "object N used".

As "type B" is defined, if you place object `N` in hotspot `N + HS_USE_OFFS`, it will "work", and the sprite `N + 2*HS_USE_OFFS` will be displayed.

That's why `spr_hs [6]` contains a winding key, `spr_hs [14]` (6+8=14) contains the hole to place the key, and `spr_hs [22]` (6+2*8) contains the winding key placed in such hole.

More on this later.

We will need interactives, but just to display sprites, as we are doing item management via *easy objects*.

```c
    #define ENABLE_INTERACTIVES
    #define INTERACTIVES_MAX                4
    //#define FLAG_INVENTORY
    #define INTERACTIVES_ONLY_SPRITES
```

And finally:

```c
    #define PLAYER_HAS_JUMP 
    [...]
    #define PLAYER_STEPS_ON_ENEMS
```

Game items and characters
-------------------------

Items and characters (displayed via interactive sprites) are stored, as you know, in the `spr_hs` metasprite array, which contains (check `assets/metasprites.h`:

```
    Index   Contents
    ------------------------
    0       (nothing)
    1       empty item

    Basic hotspots:
    2       keys
    3       refills
    4       resonators (on)
    5       resonators (off)

    Easy objects.
    Plain items:
    6       Winding key (factory)
    7       
    8       fork (pepinoni)
    9       zurully (pepinoni)
    10      silver key (pepinoni)
    11
    12
    13
    Places to use items:
    14      Winding key hole (factory)
    15
    16      empty (*)
    17      empty (*)
    18      empty (*)
    19
    20
    21
    Items used on places:
    22      Key hole with winding key (factory)
    23
    24      empty (*)
    25      empty (*)
    26      empty (*)
    27
    28
    29

    Characters
    30      Amador the lumberjack

```

(*) In pepinoni, the puzzle is solved arranging items 8, 9 and 10 in three hotspots, so you aren't really "using" them (hotspots 16, 17, 18 don't exist, so there's nowhere to use them). That's why there's no metasprite assigned to `spr_hs [16..18]` and `spr_hs [24..26]`.

Level by level: level 0
-----------------------

Level 0, the first level, is a short level to make the player learn how to jump and contains a trivial "activate resonator then jump on baddie" situation.

The level begins with 4 screens with simple platform arrangements, then a screen with Amador the Lumberjack, who will tell us about our mission, and a final screen with an enemy and a resonator. The enemy and the resonator are locked behind a gate. The gate will open once Cheril talks with Amador.

Amador the lumberjack will be implemented as an interactive sprite. Interacting with Amador will display a dialogue and set a custom variable. The gate will be cleared if such custom variable is set via custom map rendering. There's also a nice palette cycle to make quicksands look pretty.

First of all, we need to add the custom variables to `my/extra_vars.h`:

```c
    unsigned char pal_cycle [3];
    unsigned char level0_gate;
```

Such variables need to be initialized in `my/extra_inits.h`:

```c
    pal_cycle [0] = palts1 [13];
    pal_cycle [1] = palts1 [14];
    pal_cycle [2] = palts1 [15];

    level0_gate = 0;
```

Let's place the interactive sprite. `assets/interactives.h` has an array defined for each level. Amador is sprite #30 in `spr_hs` and we need to place it at X = 8, Y = 9. We won't need more interactive sprites in this level. We had the decency of defining `SPR_AMADOR` as "30" in `assets/metasprites.h`. This will make the code more readable.

```c 
    const unsigned char interactives0 [] = {
        4, 0x98, SPR_AMADOR,        // 0
        0xff
    };
```

So the engine will place amador on screen 4 at (8, 9) and make it interactive. When the player collides with such sprite and presses the B button, the code in `my/on_interactive.h` will be run.

We have to define a dialogue. Just three lines of text displayed in sequence. This is done in `assets/custom_texts.h`. The three lines of text are `dialogue_0_0`, `dialogue_0_1` and `dialogue_0_2`. All three lines of text will be said by Amador, so:

```c
    const unsigned char dialogue_portraits [] = {
        PORTRAIT_AMADOR, PORTRAIT_AMADOR, PORTRAIT_AMADOR,
        [...]
    };

    const unsigned char * const dialogue_texts [] = {
        dialogue_0_0, dialogue_0_1, dialogue_0_2,
        [...]
    };
```

The code in `my/on_interactive.h` will display such piece of dialogue and set the custom variable `level0_gate`. 

```c 
    if (level == 0 && rdc == SPR_AMADOR) {
        textbox_dialogue_do (0, 2);
        level0_gate = 1;
    }
```

The gate has to be cleared on entering screen 5. This is achieved via custom rendering, that is, adding code to `my/map_rendered_customization.h`. There's code for decorations and stuff, but the important part is this:

```c
    switch (level) {
        case 0:
        case 1:
            [...]

            // Clear gate if already talked to Amador
            if (level == 0 && n_pant == 5 && level0_gate) {
                map_buff [0x94] = 0;
                map_buff [0xA4] = 0;
            }
            break;        
        [...]
    }
```

This will clear tiles X, Y = (4, 9) and (4, 10) on level 0, screen 5, if level0_gate is set.

Remember the custom level ending condition. We need to make it happen when every enemy in the level is killed. So we add some code to `my/extra_checks.h`:

```c
    win_level = (c_max_enems == pkilled);
```

Finally, the nice pal cycling code, in `my/extra_checks.h` as well, which is a bit of a hack but works:

```c 
    if (level < 2 && (frame_counter & 7) == 0 ) {
        rda = pal_cycle [2];
        pal_cycle [2] = pal_cycle [1];  pal_col (13, pal_cycle [2]);
        pal_cycle [1] = pal_cycle [0];  pal_col (14, pal_cycle [1]);
        pal_cycle [0] = rda;            pal_col (15, pal_cycle [0]);
    }
```

And we are done.
