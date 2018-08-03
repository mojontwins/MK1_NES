# Espinete Bad

A rather interesting port of Sonic Bad (which originally runs on a rather modified MK2 engine) using only "legal" techniques to expand functionality, i.e., code injection points, even a 100% custom module.

# How I made this

One of the things I like about **AGNES** is that it's fun (well, at least for me) thinking about how to implement what you have in mind using what the engine offers. **Sonic Bad** was a joke game which tried to mimmick the gameplay in Sonic (well, kind of). It has those gameplay items:

* The main character has low acceleration, low friction, but high max speed.
* The main character collects rings, which are part of the background. The game has to remember which rings have been already collected. Luckily **AGNES** supports this, using the unused nametable to store which tiles have been "got" in each screen.
* When the main character is hit by a spike or an enemy, and carries at least 1 ring, rings are lost. If no rings are being carried, one life is lost and the level is restarted.
* The player "throws" a ring when hit, and such ring should bounce around the screen and can be taken.
* Levels are organized in worlds/acts, and there's a special item (an emmerald) in the second act of each world.

## Creating the ring


The ring has been implemented using a custom module written for this game. The module is `my/ring.h`. To make **AGNES** include extra code you have to write `#includes` in `my/extra_modules.h`:

```c
    // Ring implementation, Sonic SMS style:
    #include "my/ring.h"
```

The ring needs several custom variables we have to create in `my/extra_vars.h`...

```c
    signed int ring_x, ring_y;      // Ring fixed point coordinates (10.6)
    signed int ring_vx, ring_vy;    // Ring fixed point velocities (10.6)
    unsigned char ring_timer;       // Ring timer + "is on" flag (when != 0)
    unsigned char prings, oprings;  // Rings collected.

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

## Getting rings

When `ENABLE_TILE_GET` is on, the player can *get* tiles of behaviour 34 from the screen. The engine only takes care of deleting the tile and granting persistence (if activated), but does nothing else. If we want the engine to react to the player collecting rings, we have to inject code via `my/on_tile_got.h`. We are going to simply increase the rings counter:

```c
    prings ++;
```

But we need to refine this a bit further: whenever the player gets 100 coins, the counter should be reset and an extra life earned:

```c
    if (prings == 100) {
        prings = 0;
        plife ++;
        sfx_play (SFX_USE, 0);
    }
```

## Player hit with and without rings

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

## Make the ring move!

We have already created the ring, but we have to tell the engine it should call its update function to make it, well, happen. There's a code injection point which gets included right after every actor has updated. As our ring is an actor, we'll add the hook to the `ring_do` function in `my/extra_routines.h`:

```c
    ring_do ();
```

## Emmeralds

Another special feature are emmeralds. Game levels are divided into "worlds" and "acts" (this division is purely arbitrary and the engine doesn't really know about it). In the second act of every world there's a hidden emmerald. If you get all emmeralds, the real ending will show when you finish the game.

The first thing we should do is keep track of "world" and "act". We could use just "level" and some math, but the NES is not very good at math. So we start adding two new variables in `my/extra_vars.h`:

```c
    unsigned char level_world;      // We need those
    unsigned char level_act;
```

We will then modify `game.c` to manage `level_world` and `level_act` alongside `level`. First of all, set both to 0 when a new game is started:

```c
    [...]
    while (1) { 
        pres (palts0, scr_title);

        #ifdef MULTI_LEVEL      
            level = 0;
        #endif
        plife = PLAYER_LIFE;

        // Custom for this game
        level_world = level_act = 0;    
        pemmeralds = 0;
        //
        [...]
    }
```

And roll/advance when level is incremented:

```c
    #ifdef MULTI_LEVEL
        if (warp_to_level) continue;
        level ++;
        
        // Custom for this game
        level_act ++; if (level_act == 3) {
            level_world ++; level_act = 0;
        }
        //

        if (level == MAX_LEVELS) 
    #endif
```

Now we have our infrastructure.

To store which emmeralds have been collected, we'll use the bits in a single byte variable, so if bit N is set, that means that the emmerald in `level_word` N has been collected. So first of all create the variable in `my/extra_vars.h`:

```c
    unsigned char pemmeralds;       // Emmeralds collected, bitwise.
```

In `assets/precalcs.h` there's a `bits` array which will help you doing bitwise operations fast: `bits [i]` is 2<sup>i</sup>.

Emmeralds will be implemented as type 4 hotspots. Because I like constants, I add a new one for emmeralds in `my/extra_vars.h` (which looks like a good place to do so):

```c
    #define HOTSPOT_TYPE_EMMERALD   4
```

To extend the engine to support this new kind of hotspot, we just have to add code to `my/extra_hotspots.h`, creating a case for `HOTSPOT_TYPE_EMMERALD`. In the code, we just instruct the engine to play `SFX_FANFARE` and raise the correct bit based upon `level_word`:

```c
    case HOTSPOT_TYPE_EMMERALD:
        rda = SFX_FANFARE;

        // Fill in the correct bit. Levels with emmeralds
        // are 1, 4, 7 and 10.
        pemmeralds |= bits [level_world];

        break;
```

So whenever the player touches a type 4 hotspot, `pemmeralds` will be modified accordingly.

But what would happen if the player gets the emmerald, then gets killed? The emmerald remains "collected", so we have to take it down. As the whole level is reloaded when the player is respawned, the emmerald will show again if we don't take action.

What we will do is add a simple check to `my/on_entering_screen.h`. If the hotspot in the new screen is `HOTSPOT_TYPE_EMMERALD`, we check in `pemmeralds` if we already collected it, and if so, we just take it down:

```c
    // Invalidate emmeralds
    if (
        hrt == HOTSPOT_TYPE_EMMERALD && 
        pemmeralds & bits [level_world]
    ) hrt = 0;
```

There's also code to show which emmeralds have been collected in the "level title" screens, you can check it out at `my/pres.h`.

## Bosses

Bosses are implemented as simple compiled enemies. Dull, but it works and you don't have to modify the engine or add your own enemy types.

Bosses behaviours are scripted in `/script/enembehs.spt`. We created simple patterns, one for each boss.

The gotcha is here: the engine is configured so enemies need just one strike to be defeated. As the engine is not yet capable of managing independent life gauges as per enemy type (except for fanties), we have to patch it via code injection. 

Via `my/on_entering_screen.h` we'll just set the life gauge to 8 if we detect a compiled enemy:

```c
    // Make bosses more resistant
    gpit = 3; while (gpit --) if (en_t [gpit] == 0x14) 
        en_life [gpit] = 8;
```

Or, assuming (correctly) that the boss will be enemy 0 on the screen:

```c
    if (en_t [0] == 0x14) en_life [0] = 8;
```

The boss in the Bosque de Badajoz Zone (third world) is specially interesting as it uses the `EXTERN N` keyword in the script. This keyword makes the engine to make a call to `do_extern_action` in `my/extern.h` passing the number N as a parameter.

In the screen there are two Boioiong type enemies. The engine is configured so Boioiong are not active. Whenever the boss gets to the lowest points of its trajectory, it will call `EXTERN 0` or `EXTERN 1` which will reinit and activate one of the Boioiongs in the correct spot so it seems that the ship with Somari is throwing the bombs.

## Also of interest

### Custom renderer

The game features a nice custom renderer, like most AGNES games. For worlds 0 and 1 (all acts) it adds clouds to the background selecting a random location and checking if there's room (but it doesn't try very hard).

Worlds 0 and 1 correspond to levels 0 to 5. Levels 3 to 5 (world 1) also feature a strip if water tiles at the bottom of each screen in the bottom row of the map.

```c
switch (level) {
    case 3:
    case 4:
    case 5:
        if (level != 3 || n_pant > 14) {
            for (gpit = 176; gpit < 192; gpit ++) map_buff [gpit] = 18;
        }
    case 0:
    case 1:
    case 2:
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
    [...]
    }
```

### Win level condition

Then **win level condition** is also pretty custom. In normal levels, the level is finished when the player touches the sign, which has been coded as collectible objects (hotspots of type 1). So in `my/extra_checks.h`:

```c
    if (
        l_player_max_objects [level] && 
        pobjs == l_player_max_objects [level]
    ) win_level = 1;
```

Boss levels (the third in each world, that is, `level_act == 2`) don't have hotspots of type 1. They should end when the boss is defeated. The condition is over simplified and relies on several shortcuts, so it's mandatory that:

* Boss levels just have 1 enemy (the boss).
* Boss levels don't have hotspots of type 1.

We will use `pkilled`, which contains the number of enemies the player has killed. If it isn't 0, then the boss is down, as there are no more enemies in the level. Also, we want the explosion which appears when you kill the boss to stay a bit before the level ends. We can use `en_cttouched [0]`, which will get to 0 eventually when the explossion ends:

```c
    if (level_act == 2 && pkilled && en_cttouched [0] == 0) win_level = 1;
```

### Special rules for flicking the screen

The first act in World 3 (level 6) is a 1-screen wide level. We have to add code to `my/custom_flickscreen.h` to override the normal connection between screens so you can't exit the screens on the left or right sides.

```c
    if (level == 6) {
        flick_override = 1;

        // Just detect up/down connections
        flickscreen_do_vertical ();
    }
```

(if you don't set `flick_override`, the engine will call `flickscreen_do_horizontal` and `flickscreen_do_vertical`).

