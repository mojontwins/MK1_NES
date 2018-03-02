Cadàveriön
==========

Quick and dirty port from the 2013 ZX Spectrum original used mainly to test the scripting system. 

How I made this
===============

This isn't enough to be called a tutorial, but it may be interesting. The original game used scripting to achieve a sort of sokoban-like gameplay in which you had to place statues over pedestals so a gate in each screen would open. There's a special "retry square" you can push to force a screen redraw whiuch will place statues in their original position. There's also a timer. The game is divided into 4 "floors". You must complete each floor before being warped to the next. If you run out of time you start over from the beginning of the current floor.

It needs these items of gameplay:

- Pushable statues.
- Detection on where you push a statue.
- Trigger a script to do stuff when you place a statue.
- Detecting where some stuff is located on screen (mainly a gate and the retry square).
- When you are out of time, display a message and get back to the beginning.
- Warp to the end of each floor to the beginning of the next.

That's how this is done:

Configuration
-------------

To make things easier and save lots of code, we'll be custom-feeding the scripting engine with precalculated values. The script will need to know three things for each screen unless you want to hardcode them (that's how it was done in the ZX Spectrum original from 2013 :-S ):

1. Where's the "retry square".
2. Where's the gate.
3. How many statues we have to place.

The easiest way to calculate such stuff and feed the info to the scripting engine is to add custom code to `my/map_renderer_custom.h` which will loop through the `map_buff` array to gather the info and write them to a number of flags:

1. **Flags 4, 5** will contain the (x, y) coordinates of the "retry square".
2. **Flags 6, 7** will contain the (x, y) coordinates of the gate.
3. **Flag 9** will get the # of stautes (increase everytime tile 14 is found on the screen).

So when the gameplay for each screen begins, flags 4-9 contain screen-related information directly extracted from what's on screen, which will make our life easier and our code smaller and less error prone.

**Enable the timer** and configure it so it starts at 99 (maximum), there's refills (`HOTSPOT_TYPE_TIME`) of 30 seconds, there's a sound when timer goes below 10, and the timer is on from the beginning:

```c
    #define HOTSPOT_TYPE_TIME       5

    [...]

    #define ENABLE_TIMER
    #define TIMER_INITIAL           99
    #define TIMER_START_ON
    #define TIMER_REFILL            30
    //#define TIMER_RESET_ON_ENTER
    //#define TIMER_TIME_FLAG       0
    //#define TIMER_ZERO_FLAG       1
    #define TIMER_SOUND             10 
```

Note that we don't need to really know about the time in the script - the `ON_TIMER_OFF` section will be triggered when zero is reached anyways, so the `TIMER_*_FLAG` directives remain commented out. We don't need the timer to reset per screen, either, so `TIMER_RESET_ON_ENTER` is commented out as well.

As mentioned, when timer goes off when scripting is enabled, the `ON_TIMER_OFF` section will be run. We need to display a message ("TIME OVER!"), wait for input, and reset the hotspots so refills will respawn. Such stuff can't be performed from the script directly; therefore, we will issue a `EXTERN 0` which will run custom C code we have to write. Just populate the `do_extern_action` function in `my/extern.h` to react to `n == 0`. Check it out now. 

**Pushed tile scripting** on, so the engine copies some values to some flags and lauched the `PRESS_FIRE` actions whenever you push a block. Our pushable block will be, of course, the statue.

```c
    #define ENABLE_PUSHED_SCRIPT
    #define PUSHED_TILE_FLAG        1
    #define PUSHED_TO_X_FLAG        2
    #define PUSHED_TO_Y_FLAG        3
    #define PUSHED_FIRE
```

So everytime you push a statue, the tile being overwritten gets copied to flag 1 (so you know when you place the statue over the pedestal) and its coordinates to flags 2 and 3.

In the script, we will add code to the `PRESS_FIRE ON ANY` section which will check if the overwritten tile was the pedestal tile, increase a counter if it was, and check if all statues are in place to open the gate.

**Setting up the game progress** using a flag. There are 20 screens in the game containing 19 gates. The order is not lineal: first right, then left, then right, then left. This is a representation of the map. The numbers indicate in which screens we have a gate puzzle and its order:

```
      0    1    2    3    4
      9    8    7    6    5
     10    ·   11   12   13
     18   17   16   15   14
```

So if flag 8 (`$FINISHED_SCREENS`) equals "6" it means that gate puzzles 0, 1, 2, 3, 4 and 5 have been cleared.

**Activate simple warpers** so we can make the player warp to the end of a floor to the beginning of the next easily. Remember that simple warpers are placed as enemy type `0xff` in the spot you want them to appear. They will carry the player to the screen specified in the `attr` field to coordinates specified in `s1` (in YX hexadecimal format). Check the .ene file in *ponedor*.

```c
    #define ENABLE_SIMPLE_WARPERS
    #define SIMPLE_WARPERS_BASE_SPRID       (32+((frame_counter>>2)&3))
    //#define SIMPLE_WARPERS_FIRE_BUTTON
```

There's an expression in `SIMPLE_WARPERS_BASE_SPRID` so it animates (it's a spinning spyral). We want them to transport the player when simply touched, so `SIMPLE_WARPERS_FIRE_BUTTON` is commented out.

- Timer enabled.
- If timer enabled *and* script active, call ON_TIMER_OFF section when timer reaches zero.
- Use of EXTERN to show "TIMER OVER" screen.
- These:

```c
    #define ENABLE_PUSHED_SCRIPT
    #define PUSHED_TILE_FLAG        1
    #define PUSHED_TO_X_FLAG        2
    #define PUSHED_TO_Y_FLAG        3
    #define PUSHED_FIRE
```

`PUSHED_FIRE` means that the fire scripts will be ran when you push a tile. In the script, `JUST_PUSHED` will evaluate to true. The pushable tile coordinates will be copied to the pair of flags `PUSHED_TO_X_FLAG` and `PUSHED_TO_Y`, and the overwritten tile will be put to flag `PUSHED_TILE_FLAG`.

Last but not least, we'll be using 17 different flags in this game, so don't forget to set:

```c
    #define MAX_FLAGS               17
```


Scripting
---------

I prefer to read labers rather than numbers, so I create an Alias for each flag I'll be using.

```
    DEFALIAS
        $FIN_SCR_CPY        0
        $PUSHABLE_OVER      1
        $PUSHABLE_X         2
        $PUSHABLE_Y         3
        $RETRY_X            4
        $RETRY_Y            5
        $GATE_X             6
        $GATE_Y             7
        $FINISHED_SCREENS   8
        $STATUES_TO_PLACE   9
        $STATUES_IN_PLACE   10
        $GATE_OPEN          11
        $RETURN_TO_SCREEN   12
        $RETURN_TO_X        13
        $RETURN_TO_Y        14
        $FLOOR              15
        $MOTORBIKE_FOR_SALE 16
    END
```

I like to initialize my stuff:

```
    ENTERING GAME
        IF TRUE
        THEN
            SET_TIMER 99
            SET $FINISHED_SCREENS = 0
        END IF
    END
```

Next, the `ON_TIMER_OFF` section. It will play a sound, execute `EXTERN 0`, reset timer, substract one live, reset `$FINISHED_SCREENS` to the value at the beginning of current floor, and warp the player there.

```
    ON_TIMER_OFF
        IF TRUE
        THEN
            SOUND 2
            EXTERN 0
            SET_TIMER 99
            DEC LIFE 1
            SET $FINISHED_SCREENS = $FIN_SCR_CPY
            WARP_TO #$RETURN_TO_SCREEN, #$RETURN_TO_X, #$RETURN_TO_Y
        END
    END
```

When entering screens, reset the `$STATUES_IN_PLACE` counter and tentatively consider that the gate is closed:

```
    ENTERING ANY
        IF TRUE
        THEN
            SET $STATUES_IN_PLACE = 0
            SET $GATE_OPEN = 0
        END
    END
```

Remember that the `PRESS_FIRE` scripts are triggered everytime you push a statue as you have configured the engine to do so. We want this executed regardless of the screen. If this section gets run after pushing a statue, `IF JUST_PUSHED` will evaluate to true so you know you just pushed a statue. Note how useful it is that we have `$STATUES_TO_PLACE`, `$RETRY_X` and `$RETRY_Y` precalculated from the engine:

```
    PRESS_FIRE AT ANY
        # Push over a pedestal.
        # Tile pushed over is storet at $PUSHABLE_OVER
        # Coordinates at $PUSHABLE_X, $PUSHABLE_Y

        IF JUST_PUSHED
        IF #$PUSHABLE_OVER = 13
        THEN
            INC $STATUES_IN_PLACE, 1
            SOUND 3
            SET TILE (#$PUSHABLE_X, #$PUSHABLE_Y) = 30
        END

        # All in place?

        IF #$STATUES_IN_PLACE = #$STATUES_TO_PLACE
        IF #$GATE_OPEN = 0
        THEN
            SET $GATE_OPEN = 1
            SET TILE (#$GATE_X, #$GATE_Y) = 0
            SOUND 7
            INC $FINISHED_SCREENS, 1

            # Turn off retry
            SET $RETRY_Y = 13

            TEXT "GATE OPEN!                   "
        END

        # Prevent nasty shit (retrying while pushing!)
        IF JUST_PUSHED
        THEN
            BREAK
        END

        # Retry tile
        IF PLAYER_TOUCHES #$RETRY_X, #$RETRY_Y
        THEN
            SOUND 2
            REENTER
        END
    END
```

Most screens are the same. Just check if the current amount of finished gates is bigger than the current gate puzzle (check the small diagram I showed earlier) and open the gate if so. Again, the value of `$GATE_X` and `$GATE_Y` comes from the engine.

```
    ENTERING SCREEN 1
        IF #$FINISHED_SCREENS > 1
        THEN
            SET TILE (#$GATE_X, #$GATE_Y) = 0
            SET $GATE_OPEN = 1
        END
    END
```

On the first screen in each floor (this is, screens 0, 9, 10 and 19) we have a couple more things to set up, for example:

```
    ENTERING SCREEN 9
        IF #$FINISHED_SCREENS > 5
        THEN
            SET TILE (#$GATE_X, #$GATE_Y) = 0
            SET $GATE_OPEN = 1
            BREAK
        END

        IF #$FINISHED SCREENS = 5
        THEN
            SET_TIMER 99
            TEXT "YOU GET TO THE SECOND FLOOR!"
            SET $RETURN_TO_SCREEN = 9
            SET $RETURN_TO_X = 14
            SET $RETURN_TO_Y = 1
            SET $FIN_SCR_CPY = 5
        END IF
    END
```

This sets where to return when time is out. `$FIN_SCR_CPY` contains the # of finished gate puzzles before this screen, so when you are thrown back when time is out the correct value is restored to `$FINISHED_SCREENS`.

Finally, you win the game via a fire zone which can be only accessed then the gate is cleared:

```
    ENTERING SCREEN 15
        IF #$FINISHED_SCREENS > 18
        THEN
            SET TILE (#$GATE_X, #$GATE_Y) = 0
            SET $GATE_OPEN = 1
        END IF

        IF TRUE
            # EXIT VIA FIRE ZONE
            SET_FIRE_ZONE 16, 192, 31, 207
        END
    END

    PRESS_FIRE AT SCREEN 15
        IF PLAYER_TOUCHES 1, 12
        THEN
            WIN GAME
        END
    END
```

And that's it!
