# Code injection points

**MK1_NES / AGNES** has several code injection points, places where special files in the `my` folder are `#included` so you can expand or modify the default behaviour of the engine.

## Basic code injection points

### extra vars

The file `my/extra_vars.h` lets you define new variables in the BSS segment. For example:

```c
    unsigned char myvar;
```

### extra modules

We can add our own library of functions to the project by `#include`ing the code directly in `my/extra_modules.h`.

### extra inits

The file `my/extra_inits.h` is included right after the code which initializes the level before playing. You can use this code injection point to initialize your custom variables:

```c
    myvar = 0;
```

Or to do custom inizialization, for example activate springs in level 1:

```c
    springs_on = (level == 1);
```

### on entering screen

The code in `my/on_entering_screen.h` is run everytime the player enters a new screen, right after everything is initialized so you can expand on it or even do modifications.

For example, [Cheril Perils Classic](https://github.com/mojontwins/MK1_NES/tree/master/examples/07_cheril_perils_classic) injects code in this point to disable resonators and delete hotspots with resonators if `mode_no_resonators` (a custom variable) is set:

```c
    if (mode_no_resonators) {
        res_disable = 1;
        if (hrt == HOTSPOT_TYPE_RESONATOR) hrt = 0;
    }
```

`hrt` is the value of the current, active hotspot.

### extra routines

If you need to call extra functions of perform tasks every frame, add your code to `my/extra_routines.h`. This code is included right after every actor has updated and changes made there will be output in the current frame.

As it is run each game frame, you can use it for example to cycle a palette:

```c
if (level < 2 && (frame_counter & 7) == 0 ) {
    
    rda = pal_cycle [2];
    pal_cycle [2] = pal_cycle [1];  
    pal_cycle [1] = pal_cycle [0];  
    pal_cycle [0] = rda;            
    
    pal_col (13, pal_cycle [2]);
    pal_col (14, pal_cycle [1]);
    pal_col (15, pal_cycle [0]);
}
```

### extra checks

The file `my/extra_checks.h` is included in the main loop, so the code you add here will be run each game frame. This is the place, for example, to add the "win level" condition if you defined `WIN_LEVEL_CUSTOM` in `config.h`. Just set `win_level` to true if the win level condition is met. For example:

```c
    win_level = (c_max_enems == pkilled);
```

Will end the level if the player has killed all enemies in this level.

## Adding support for new hotspot types.

You can add your own custom hotspot types easily by adding code to `my/extra_hotspots.h`. For each custom hotspot type you want to create, just add a code block like this:

```c
    case HOTSPOT_TYPE_MY_OWN:
        rda = SFX_OBJECT;
        // do stuff
        break;
```

If `rda != 0`, the engine will play such sound on interaction.

## Related to collisions

If the player is collided by cocos, enemies or spikes, special code can be executed.

### Colliding with cocos

When the player collides with cocos, `en_sg_2` will be set and the code in `my/on_player_coco.h` will be executed. You can do extra checks or perform extra actinons. If you reset `en_sg_2`, the player won't get hurt.

### Colliding with enemies

When the player collides with enemies, `en_sg_2` will be set and `en_sg_1` will be reset, then the code in `my/on_player_hit.h` is executed. You can do extra checks or perform extra actions. If you reset `en_sg_2` the player won't get hurt. If you set `en_sg_1`, the enemy will get hurt.

### Colliding with spikes

When the player collides with spikes, `en_sg_2` will be set and the code in `my/on_player_spike.h` will be executed. You can do extra checks or perform extra actinons. If you reset `en_sg_2`, the player won't get hurt.

In sum, `en_sg_1` controls the enemy being hurt (if applies) and `en_sg_2` controls the player getting hurt.

## Related to objects

### on object got

If `ENABLE_ONLY_ONE_OBJECT` or `ENABLE_EASY_OBJECTS` are on, whenever the player gets an object the code in `my/on_object_got.h` will be run. Here you can do all the checks you want. Interesting variables or arrays you may need:

* `pinv` - the object the player is carrying. 
* `ht []` array - the contents of each hotspot in the game. Indexed by screen number, i.e. ht [N] is the value of the hotspot in screen N.
* `level` - current level number.

### on object used

When `ENABLE_EASY_OBJECTS` is on, the player can use the object N in a hotspot with value `N + HS_USE_OFFS`. When such a thing happens, the code in `my/on_object_used.h` is executed.

If `HS_FIX_ON_USE` is set in `config.h`, once the player uses object `N` in hotspot `N + HS_USE_OFFS`, the value of the hotspot becomes `N + 2 * HS_USE_OFFS` and the object can't be taken again.

For example, in [Cheril the writer](https://github.com/mojontwins/MK1_NES/tree/master/examples/08_cheril_the_writer) we needed to activate propellers and show a text box when the player uses object 6 in screen 9 of level 3.:

```c
    if (level == 3 && ht [9] == 6 + 2*HS_USE_OFFS) {
        propellers_on = 1;
        // Make player know 
        oam_hide_rest (0);
        gp_gen = text_propellers;
        textbox_do ();
    }
```

`ht [9]` is *the contents of the hotspot in screen 9*; `6` is the number of the object, so `6 + 2*HS_USE_OFFS` means *object 6 used*.

## Timer

If you have the timer on, everytime the timer ticks the code in `my/on_timer_ticks.h` will be executed. 

The timer current value is in `timer`. `timer_zero` will be set when the timer reaches zero.

## Related to interactives

### on interactive

Whenever a user interacts with an interactive, the code in `my/on_interactive.h` is executed. At this point, the `rdc` variable encodes which interactive:

* If bit 7 of `rdc` is set `(rdc & 0x80)`, the user just interacted with container number `rdc & 0x7f`.
* If bit 7 of `rdc` is reset `!(rdc & 0x80)`, the user just interacted with the sprite number `rdc`.

For example, if the player just interacted with the container bound to flag 5, then `rdc = 0x80|5`:

```c
    if (rdc == 0x80|5) {
        // Player interacted with container 5
    }
```

Remember that the contents of containers equals the contents of the flag they are bound to, so the contents of such container 5 will be `flag [5]`:

```c
    if (rdc == 0x80|5 && flag [5] == 6) {
        // Player just interacted with container 5 and
        // put object 6 inside.
    }
```

## Changing rooms

You can override the normal changing of rooms by injecting code to `my/custom_flickscreen`, which gets executed before the normal "player exits the current room" code. This helps you creating custom connections. 

Setting `flick_override` to 1 will override the normal "player exits the current room" code for the current frame.

## Things you may want to do in code injections

### Display text boxes:

```
    gp_gen = string_pointer;
    textbox_do ();
```

`string_pointer` points to a string. You can define your strings in `assets/custom_texts.h`. Remember that text boxes use `%` for line break, and that lines shouldn't have more than 18 characters, excluding the `%`. For example:

```c
    // assets/custom_texts.h

    const unsigned char text_open_gate [] = 
        "AHOY! OBJECTS ARE%"
        "IN PLACE... NOW%"
        "YOU CAN HEAR A%"
        "RUSTY FENCE OPENING%"
        "ELSEWHERE.%";
```

```c
    // my/on_object_got

    if (
        level == 1 && 
        level1_gate == 0 && 
        ht [0x12] == 9 && ht [0x16] == 10 && ht [0x0c] == 8
    ) {
        level1_gate = 1;
        gp_gen = text_open_gate;
        textbox_do ();
    }
```

### Change/check the value of containers

Containers are bound to flags, so container N will contain the object represented to the value of flag N.

### Change/check the value of hotspots

Remember that `ht [N]` is the hotspot in screen N, which you can read or write at will.

The current, active hotspot is in `hrt`, located at `hrx, hry`. You can read or write such variables at will.

### Change/check the player values:

* `pobjs` is the current number of collectibles.
* `pkeys` is the current number of keys.
* `pinv` is the object the player is carrying.
* `plife` is the player life gauge.
* `pkilled` the amount of enemies killed.

Of course, the more you know the inner works of the engine the more you can do using code injection points. If you think of something you don't know exactly how to do yo can always [buy me a coffee](https://www.buymeacoffee.com/nath) and ask me ;)

### Warp to another level

Setting `warp_to_level` will make the engine to restart the main loop, reloading all level assets. That means that if you change n_pant, px, py or level you can actually warp to another level, or to another location in the same level. For example, warp to level 2 at screen 5, tile location (3, 4) when interacting with sprite ID = 6, `my/on_interactive.h`:

```c
    if (rdc == 6) {
        level = 2;
        n_pant = 5;
        px = 3 << 10;
        py = 4 << 10;
        warp_to_level = 1;
    }
```

### Enemies exploding

If you set your `win_level` condition depending on killing enemies, it's ugly that the screen fades out as soon as you hit the last enemy. To delay that a bit (and show the enemy's *dying animation*) you can use `en_cttouched`, which is an array which stores a count for each enemy on screen if the enemy is exploding. If we wait until all `en_cttouched` are 0 the effect of finishing the game is more pleasant to the eye. We can see this in action in [Cheril the writer](https://github.com/mojontwins/MK1_NES/tree/master/examples/08_cheril_the_writer):

```c
    if (c_max_enems == pkilled) {
        if (
            en_cttouched [0] == 0 &&
            en_cttouched [1] == 0 &&
            en_cttouched [2] == 0
        ) win_level = 1;
    }
```

Note how if all enemies have been killed, `win_level` wont be set while there's a exploding animation going on.
