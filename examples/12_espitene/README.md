# Espitene

A rather interesting port of Sonic Bad (which originally runs on a rather modified MK2 engine) using only "legal" techniques to expand functionality, i.e., code injection points, even a couple of custom modules.

# How I made this

One of the things I like about **AGNES** is that it's fun (well, at least for me) thinking about how to implement what you have in mind using what the engine offers. **Sonic Bad** was a joke game which tried to mimmick the gameplay in Sonic (well, kind of). It had these gameplay items:

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

The implementation of the ring is fairly simple, just a bouncing object which can collide with the background. It uses a couple of shortcuts (for example, it repurposes the `collide` function, which is used for enemies) and focus has been put on making the code compact and fast (so sorry, it's not very readable). On `my/ring.h` there are two functions:

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

When `ENABLE_TILE_GET` is on, the player can *get* tiles with behaviour 34 from the screen. The engine only takes care of deleting the tile and granting persistence (if activated), but does nothing else. If we want the engine to react to the player collecting rings, we have to inject code via `my/on_tile_got.h`. We are going to simply increase the rings counter:

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

In a similar way, when the player is hit by a spike (or killing tile), the code in `my/on_player_spike.h` is included before checking, on `en_sg_2`, if the player should receive damage. Here we don't have to check `en_sg_2` which is not set beforehand (as all collisions with spikes should result on the player getting hit, normally), so we only have to check for rings. Also, we don't have to explicitly make the player bounce this time, as the code which deals with spikes already takes care of doing so, but we'll play the "player hit" sound and set the state to "pbouncing" so the sprite cell changes for 16 game frames.

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

## Falling bridge bits

I've used the `my/on_tile_break.h` code injection point. This code is executed whenever a breakable tile is broken *and disappears*. What I do is spawn a simple moving object I've coded in the custom module `bridge.h`. The first thing to do is `#include` the code in `my/extra_modules.h`:

```c
    #include "my/bridge.h"
```

`bridge.h` contains a rather simple *falling object* implementation using integer math and dully simulating gravity acceleration.  The objects are stored in a circular buffer with just 4 slots (no need for more). Active bridge bits are overwritten with new instances if necessary. The implementation needs some variables:

```c
    unsigned char bridge_idx;       // Breakable bridges
    unsigned char bridge_x [4];
    unsigned char bridge_y [4];
    unsigned char bridge_f [4];
```

`bridge.h` contains just two functions: `bridge_create` creates a new falling bit at _x, _y. This is the function we have to call from `my/on_tile_break.h`:

```c
    // NES MK1 v1.0
    // Copyleft Mojon Twins 2013, 2015, 2017, 2018

    // Add here your code. A tile has just broken.
    // Tile is at _x, _y. DO NOT MODIFY!

    bridge_create ();
```

`bridge_do` is the other function. This just checks if bridge bits are active and animates them. This needs to be called every game frame so we add the function call to `extra_routines.h`:

```c 
    // Animate bridges 
    bridge_do ();
```

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

Something else is required: The engine is wired in a way so that if the enemies life gauge is defined as 1, code related to substracting life and checking if it reached 0 is simply not included. We have life gauge defined as 1, so the engine doesn't support en_life = 8! We can explicitly tell the engine to include life gauge management by defining `NEEDS_LIFE_GAUGE_LOGIC` in config.h.

The boss in the Bosque de Badajoz Zone (third world) is specially interesting as it uses the `EXTERN N` keyword in the script. This keyword makes the engine to make a call to `do_extern_action` in `my/extern.h` passing the number N as a parameter.

In the screen there are two Boioiong type enemies. The engine is configured so Boioiong are not active. Whenever the boss gets to the lowest points of its trajectory, it will call `EXTERN 0` or `EXTERN 1` which will reinit and activate one of the Boioiongs in the correct spot so it seems that the ship with Somari is throwing the bombs.

Scripting compiled enemies is pretty straightforward. You define a section for each, then they map to the ARG you define when placing the enemy in `ponedor` (0 is the first).

Compiled enemies script must be loops: they should start with a label and end with `RETURN label`. Commands in between instruct the enemy to move around, wait for N half-seconds, shoot, or change speed.

## Underwater

Level 4 has underwater sections. In those (when `n_pant >= 10` on 5x5 screens levels), the timer is on and counting down. Time refills are "oxygen recharges" and will be custom-respawned everytime the player enters a screen. On timer off, the player will be killed. 

We have created an `underwater` variable in `my/extra_vars.h` to make detection easier between different modules. We also add variables to move around a little bubble, which helps the player knowing we are underwater.

```c
    unsigned char underwater;
    unsigned char bubble_x; 
    unsigned char bubble_y;
```

The variable is to be given a value everytime time player enters a new screen, so we add this to `my/on_entering_screen.h`:

```c
    if (level_world == 3) {
        // Set underwater screens
        timer_on = underwater = (level_act == 2 || n_pant >= 10);

        pal_bg (underwater ? palts3a : palts3);
    }
```

In `level_world` 3, in the whole act 2 or in screens >= 10 in acts 0 and 1, `underwater` is on. In such cases, the timer, which we will use as an oxygen indicator, is on too. Besisdes, the palette is changed if we are under water.

Speaking of the timer, this is the configuration in `config.h`:

```c
#define ENABLE_TIMER
#define TIMER_INITIAL                   20
//#define TIMER_START_ON
#define TIMER_REFILL                    0 
//#define TIMER_RESET_ON_ENTER
#define TIMER_SOUND                     10 
```

Note how we have configured the timer to be 20 initially. Also, it won't start automaticly (`TIMER_START_ON` is commented out). Time refills should refill the timer completely (back to `TIMER_INITIAL`, or 20), so `TIMER_REFIL` has been given a value of 0. `TIMER_SOUND` will make ticks audible for the last 10 seconds.

To work with timer, a very appropriate place to add or code is in the `my/on_timer_tick.h` code injection point. Code added there will be executed everytime the timer ticks, that is, when its value changes.

If underwater, we'll spawn a bubble (for eye candy) and check if the timer reached 0, in such case we kill the player:

```c
    if (underwater) {
        // spawn a bubble
        bubble_x = prx; bubble_y = pry - 8; 

        // Kill player.
        if (timer_zero) {
            psprid = CELL_HIT;
            pkill = 1;
        }
    }
```

Finally, we want the bubble to animate, and we also want to make Espitene move slower. To make the main character move slower, we are using a rather cheesy sollution which seems to work quite nicely for this game: arbitrarily limiting the maximum speed in both axes. Remember the code injection point for routines that have to run every frame is `my/extra_routines.h`. We add this:

```c
    // Underwater
    if (underwater) {
        // slower movements. Cheesy but kinda works!
        if (pvx < -96) pvx = -96;
        else if (pvx > 96) pvx = 96;

        if (pvy > 96) pvy = 96;
        if (pvy < -160 && !ptrampoline) pvy = -160;

        // Update bubble
        if (bubble_y) {
            rda = rand8 ();

            bubble_y --;            
            bubble_x += ((rda & 2) - 1);

            oam_index = oam_spr (
                bubble_x, bubble_y + SPRITE_ADJUST, 
                0x0c + (rda & 1),
                2, 
                oam_index
            );
        }

        // Update countdown
        if (timer && timer < 10) oam_index = oam_spr (
            prx, pry - 16 + SPRITE_ADJUST - (12 - (timer_frames >> 2)), 
            2 + timer, 
            2, 
            oam_index
        );

        [...]
    }
```

We added a nice countdown if the timer is < 10, using sprites.

We also want to respawn type 5 hotspots (time refills) automaticly:

```c
    // Underwater
    if (underwater) {
        
        [...]

        // Respawn time refills
        if (ht [n_pant] == HOTSPOT_TYPE_TIME && timer < 10) {
            hrt = HOTSPOT_TYPE_TIME;
            hact [n_pant] = 1;
        }
    }
```

## Toggle switches in Crap Brain Zone Act 2

I've implemented simple toggle switches which open/close gates labelled "A" and "B". The implementation is simple: Gates "A" and "B" are painted or not depending on the value of toggle_switch (0 means "A" is open, 1 means "B" is open), so gates and toggle switches can't coexist in the same room (as gate fiddling is performed off-room).

Toggle switches will be implemented using hotspots. We need two states, so we'll use two hotspot values: `HOTSPOT_TYPE_SWITCH_OFF` and `HOTSPOT_TYPE_SWITCH_ON`.

In ponedor, switches are set using the value `HOTSPOT_TYPE_SWITCH_OFF`. `HOTSPOT_TYPE_SWITCH_ON` will be ignored by the engine. When the player gets a `HOTSPOT_TYPE_SWITCH_OFF` hotspot, the value changes to `HOTSPOT_TYPE_SWITCH_ON` (on screen) and a timer is started. When the timer goes off, the value is changed back to `HOTSPOT_TYPE_SWITCH_OFF`.

On `extra_hotspots.h`:

```c
    case HOTSPOT_TYPE_TOGGLE_OFF:
        hrt = HOTSPOT_TYPE_TOGGLE_ON;
        toggle_timer = 50;
        toggle_switch = !toggle_switch;     // Toggle!
        sfx_play (SFX_STEPON, 0);
    case HOTSPOT_TYPE_TOGGLE_ON:
        rda = 0;                            // Do not clear hotspot!
        break;
```

`toggle_timer` is managed in `extra_routines.h`:

```c    
    // toggle switch timer:
    if (toggle_timer) {
        toggle_timer --;
        if (toggle_timer == 0) {
            hrt = HOTSPOT_TYPE_TOGGLE_OFF;
            sfx_play (SFX_STEPON, 0);
        }
    }  
```

Make sure `toggle_timer` is reset when entering a new room, `on_entering_screen.h`:

```c
    // Reset toggle timer
    toggle_timer = 0;
```

And add an extra rule to `map_renderer_customization.h` so Gates "A" and "B" are drawn or not upon the value of `toggle_switch`. Tiles are 2 (gate "A") or 4 (gate "B"):

```c
        case 4:
        case 2:
        case 3:
            [...]

                if (level_world == 3) {
                    // Wet Ruins Zone embellishments
                    [...]   

                } else {
                    // Crap Brains embellishments
                    [...]

                    if (level_act == 1) {
                        if (rdt == 2 && toggle_switch == 0) rdt = 0;
                        if (rdt == 4 && toggle_switch) rdt = 0;
                    }
                }
```

## Electric barriers in Ola K Ase Zone

Electric barriers are wires of lighting which turn or on off periodically. They are harmfull when on. We have implemented them via a custom tile detection and a custom routine which uses the ticker to switch states.

The ticker ticks 50 times every second (regardless of PAL/NTSC). Everytime the ticker equals 0, a second has passed. Everytime the ticker equals 0 or 25, half a second has passed. We use this to time the tates of the wires.

First of all, we define some variables and constant arrays in `extra_vars.h`:

```c
    unsigned char elec_state;
    unsigned char elec_state_ct;
    const unsigned char elec_state_max_ct [] = { 6, 1, 4 };
```

The array contains the amount of half seconds every state will be active before stepping to the next. State control is performed in `extra_routines.h`:
   
```c 
    // Electricity
    if (level_world == 5) {
        if (ticker == 0 || ticker == 25) {
            if (elec_state_ct) -- elec_state_ct; else {
                ++ elec_state; if (elec_state == 3) elec_state = 0;
                elec_state_ct = elec_state_max_ct [elec_state];
                if (elec_state == 0) pal_bg (palts5);
                else if (elec_state == 1) pal_col (11, 0x18);
            }
        }
    }
```

When `elec_state` equals 2, the electric barriers are "active" - harmful. There's also a blinking effect achieved alternating palettes and colour emphasis bits. `effects.h` gets executed every TV frame (50 or 60 times per second), not every game frame, (which is always running at 50 fps), so it's the place to add this kind of effects:

```c
    // Level 5 flashing should be 50Hz in PAL and 60Hz in NTSC
    // So we put it here.
    if (level == 15) {
        if (elec_state == 2) {
            pal_bg (half_life ? palts5 : palts5a);
            ppu_mask (0x1e);
            if (real_frame_counter & 1) sfx_play (1, 2);
        } else {
            ppu_mask (0xfe); 
        }
    }
```

Finally, to make them really harmful, we inject code via `custom_center_detections.h`, which let's you add code to react to the tile detected at the dot located in the middle of the player's collision rectangle. We have chose beh 66 as it's unused and does not interfere with existing tile behaviours.

```c
    if (at1 == 66) {
        // Custom: electricity rays
        if (elec_state == 2) {
            
            if (prings) {
                ring_create ();
                prings = 0;

                // Simulate hit
                pvx = -pvx; pvy = -64; 
                pbouncing = 16;
                pflickering = 100;
            } else pkill = 1;

        } 
    }
```

This code is not included automaticly, we have to enable `CUSTOM_CENTER_DETECTIONS` in `config.h`.

## Last boss

The boss for the last stage tries to mimmick the of Sonic for SMS. To do that, it uses a a combination of standard AGES features and custom coding.

The last boss is composed by several elements:

### The orbs and ray.

The orbs which shoot a ray when advancing and seem to "recharge" while recoiling are created using a single linear enemy with a big vertical metasprite. The "facing left" sprites have a ray between the orbs, the "facing right" don't. The metasprite handle is located at the top, so the collision box is around the upper orb. This orb is inside non walkable tiles, so you can't really collide with this enemy.

Collision is handled by a custom routine in `custom_routines.h`. If the orbs are moving left (mx = -2), it is collidable and will kill the player:

```c
    // Final boss
    if (level == 17 && n_pant == 2) {

        [...]

        // Sound
        if (en_mx [0] < 0 && half_life) sfx_play (1, 2);    

        // Collision with ray
        if (pflickering == 0 &&
            en_mx [0] < 0 &&
            prx + 3 >= en_x [0] &&
            prx <= en_x [0] + 7
        ) {
            pkill = 1;
        }
    }

```

Note that we only check the horizontal axis as there's no way you can go over or under the orbs (as they cover the whole non-obstacle area of a corridor).

### The shooters

From time to time, two "cannons" appear and shoot towards the player. This one was simple: they are two monococos.

### Somari

With the three enemy slots already taken, The only way to feature somari on screen (and make it collidable) was via extra routines. First of all, a custom module `somari.h` is `#include`d from `extra_modules.h`. This module implements a simple fsm. Somari can have 4 states:

- State 0: Normal. A cyclic animation is displayed.
- State 1: Touched. Somari flashes showing his skeleton until a counter expires, then goes back to state 0.
- State 2: Dying. Somari stays in skeleton form until a counter expires, then goes to state 3.
- State 3: Dead. Only used so the engine can detect the level is finished.

```c
    void somari_do (void) {

        switch (somari_state) {
            case 0:
                en_spr = 42 + ((frame_counter >> 3) & 3);
                break;

            case 1:
                en_spr = half_life ? 44 : 46;
                if (somari_ct) somari_ct --; else somari_state = 0;
                break;

            case 2:
                en_spr = 46;
                if (somari_ct) somari_ct --; else somari_state = 3;
                break;
        }

        oam_index = oam_meta_spr (
            232, SPRITE_ADJUST + 128, 
            oam_index, 
            spr_enems [en_spr]
        );

    }
```

The `somari_do` function is called from `extra_routines.h`. Collision with the player (which is the only way to change from state 0 to state 1) is handled there as well:

```c
    // Final boss
    if (level == 17 && n_pant == 2) {
        somari_do ();   

        // Collision with somari:
        if (prx > 216) {
            prx = 216; px = prx << FIXBITS; pvx = 0;
            if (pspin) {
                pvx = -512;         
                pflickering = 30;
                if (somari_life) {
                    somari_life --;
                    somari_state = 1;   
                } else {
                    somari_state = 2;
                }
                somari_ct = 120;
                sfx_play (SFX_BREAKB, 1);
            }
        }

        [...]
    }
```

Collision is detected checking if the player's X coordinate goes past 216. If the player is spinning, Somari will register a hit.

## Also of interest

### Custom renderer

The game features a nice custom renderer, like most AGNES games. For worlds 0 and 1 (all acts) it adds clouds to the background selecting a random location and checking if there's room (but it doesn't try very hard).

world 1 also feature a strip if water tiles at the bottom of each screen in the bottom row of the map.

Worlds 2 to 4 have several tile substitutions for embellishments. Worlds 3 and 4 substitute single tiles by 2x2 tiles clusters automaticly. This saves tons of bytes in RLE compressed maps, but such 2x2 tiles are always rendered in coordinates which are multiples of two.

World 5 has a custom bg: a 192 bytes array with 1 byte per tile. Everytime a 0 is found in the map, it is substituted by the value of the corresponding tile in the bg array.

```c
set_rand (n_pant + 1);

switch (level_world) {
    case 1:
        // Strip of water at the bottom of the level
        if (level != 3 || n_pant > 14) {
            for (gpit = 176; gpit < 192; gpit ++) map_buff [gpit] = 18;
        }
    case 0:
        // Random clouds, find a nice spot but don't try hard
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
    case 4:
        rdc = (map_buff [0] != 4);  // Cheap way to distinguish between outside / inside
    case 2:
    case 3:
            
        for (gpit = 0; gpit < 192; gpit ++) {
            rdt = map_buff [gpit];
            rdb = rand8 ();

            if (level_world == 2) {
                // Bosque de Badajoz Zone embellishments

                if (gpit > 16) {
                    rda = map_buff [gpit - 16];
                    if (rdt == 8) {
                        if (rda == 4) rdt = 16;
                        else if (rda == 6) rdt = 17;
                    } else if (rdt == 10 && rda == 13) rdt = 20;
                    else if (rdt == 2 && rda != 2 && rda != 19) rdt = 3;
                }
                if (gpit < 176) {
                    rda = map_buff [gpit + 16];
                    if (rdt == 0 && rda == 2) rdt = 19;
                    else if (rdt == 2 && rda != 2) rdt = 18;
                }
            } else {                
                if (level_world == 3) {
                    // Wet Ruins Zone embellishments
                    if (rdt == 1) rdt = 16 + (rdb & 3);
                } else {
                    // Crap Brains embellishments
                    if (rdt == 1 || rdt == 3) {
                        rda = map_buff [gpit - 1];
                        if (rda != rdt && rda != rdt + 16) rdt = rdt + 16;
                        else {
                            rda = map_buff [gpit + 1];
                            if (rda != rdt) rdt = rdt + 17;
                        }
                    }

                    if (level_act == 1) {
                        if (rdt == 2 && toggle_switch == 0) rdt = 0;
                        if (rdt == 4 && toggle_switch) rdt = 0;
                    }
                }

                // Wet Ruins Zone & Crap Brains embellishments
                switch (rdt) {                  
                    case 8:
                    case 12:
                        rdt = 16 + rdt + (gpit & 1) + ((gpit >> 3) & 2);
                        break;
                }
            }
        
            if (rdt == 0 && (rdb & 7) == 1 && (rdc || level_world < 4)) rdt = 21;

            map_buff [gpit] = rdt;
        }
        break;

    case 5:
        for (gpit = 0; gpit < 192; gpit ++) {
            rdt = map_buff [gpit];

            if (rdt == 0) rdt = level5_bg [gpit]; 
            else {
                if ((rdt == 8 || rdt == 10) && map_buff [gpit - 1] == rdt) rdt += 17;
            }

            map_buff [gpit] = rdt;
        }
        break;
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

The first and third acts in World 3 (level 6) are 1-screen wide levels. We have to add code to `my/custom_flickscreen.h` to override the normal connection between screens so you can't exit the screens on the left or right sides.

```c
    if (level == 6 || level == 8) {
        flick_override = 1;

        // Just detect up/down connections
        flickscreen_do_vertical ();
    }
```

(if you don't set `flick_override`, the engine will call `flickscreen_do_horizontal` and `flickscreen_do_vertical`).

### Bottom of the map kills in some levels

This was achieved pretty easily via a custom flickscreen rule:

```c
    // On level world 5, get killed by the bottom of the map

    if ((level == 15 && n_pant >= c_map_w)
        [...]
        ) {
        if (pry >= 192 && pvy > 0) {
            pkill = 1;
            flick_override = 1;
        }
    }
```

### Moving water in Broken Fridge Zone

This is achieved by a sprite-0 based split. You can read about it in the [nesdev wiki](https://wiki.nesdev.com/w/index.php?title=PPU_OAM&redirect=no#Sprite_zero_hits). The sprite is set up in `my/on_entering_screen.h`. The following line puts it in place if there's a water strip on screen (see the custom renderer code above), or "invisible" (with y = 240) otherwise:

```c
    water_strip = (level_world == 1 && (level != 3 || n_pant > 14));
    oam_spr (128, water_strip ? 211 : 240, 1, 2, 0);
```

The actual split is performed at `my/effects.h`, which gets included right before waiting for the next frame:

```c
    // Do a split in the fridge
    if (water_strip) split (real_frame_counter, 0);   
```

Of course you need to define `water_strip` in `my/extra_vars.h`:

```c
    unsigned char underwater;       // To make our life easier in Wet Ruins Zone
```

### Moving water in Bosque de Badajoz Zone

This is achieved via a palette cycle. We need an array to copy the three colours in the subpalette we want to cycle, defined as always in `my/extra_vars.h`:

```c
    unsigned char pal_cycle [3];
```

First thing to do is copy the last subpalette to our array. In `my/extra_inits.h`:

```c
    pal_cycle [0] = c_pal_bg [13];
    pal_cycle [1] = c_pal_bg [14];
    pal_cycle [2] = c_pal_bg [15];
```

The actual palette cycling is performed in `my/effects.h`. This ensure the effect is smooth and doesn't artifact in NTSC systems:

```c
    // Do a palette cycle in the jungle
    if (level_world == 2 && (real_frame_counter & 7) == 0) {
        rda = pal_cycle [2];
        pal_cycle [2] = pal_cycle [1]; pal_col (15, pal_cycle [2]);
        pal_cycle [1] = pal_cycle [0]; pal_col (14, pal_cycle [1]);
        pal_cycle [0] = rda;           pal_col (13, pal_cycle [0]);
    }
```
