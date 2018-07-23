# mscmk1 - Mojon Script Compiler for MK1

**mscmk1** means Mojon Script Compiler for MK1.

**mscmk1** is a scripts compiler. It generates a bytecode with your scripts plus a minimal interpreter (with code to interpret just the commands and conditions you have used in your script).

**mscmk1** is run from the command line:

```cmd
    $ msc3 file.spt N
```

Where N is the max number of screens as defined in `config.h` - the value of `MAP_SIZE`, this is, `MAP_W * MAP_H`. As you know, even if levels in the game have different sizes, these constants should account for the biggest level in the game.

## Levels

If your game has several levels, you can define different scripts for each level in your script file. Each level script ends with:

```spt
    END_OF_LEVEL
```

Then, you can add code for the next level.

By default, levels in the script file are 'exported' in order, starting with level script 0, and are mapped N:N, so level 0 will run script 0, level 1 will run script 1, etc., but you can change how scripts are mapped to levels:

```spt
    CUSTOM_SCRIPT_ORDER A, B, C, D, ...
```

That will map script A to level 0, script B to level 1, etcetera. For example, the remake of [Sgt. Helmet's Training Day](https://github.com/mojontwins/MK1_NES/tree/master/testers/01_tester_top_down) has two levels which need to run the same script, that is, level 0 and level 1 have to run script 0, thus:

```spt
    CUSTOM_SCRIPT_ORDER 0, 0
```

## Sections

The script for each level is organized in sections. Each section will be executed in a precise moment and / or screen. There are several kinds of sections:

```spt
    ENTERING SCREEN x
```

Will run each time the player enters screen X.

```spt
    ENTERING GAME
```

Will run when a new game starts, and before entering the first screen.

```spt
    ENTERING ANY
```

Will run when entering *any* screen, and right *before* `ENTERING SCREEN x`.

```spt
    PRESS_FIRE AT SCREEN x
```

Will run when the player presses **B** while on screen X.

```spt
    PRESS_FIRE AT ANY
```

Will run when the player presses **B**, no matter in which screen he is, and right before `PRESS_FIRE AT SCREEN x`

```spt
    ON_TIMER_OFF
```

Will run when the timer reaches 0.

```spt
    PLAYER_GETS_COIN
```

Not used in AGNES as of v1.0, will be supported soon.

```spt
    PLAYER_KILLS_ENEMY
```

Run everytime the player kills an enemy.

The `PRESS_FIRE` sections will run also when these conditions are met:

* If `ENABLE_FIRE_ZONE` is defined in `config.h`, and the player enters the hot zone defined by a `SET FIRE_ZONE` command in the script.
* If `PLAYER_PUSH_BOXES` and `ENABLE_PUSHED_SCRIPT` are defined in `config.h` and the player pushes a pushable tile.
* If the player interacts with an interactive. [See this](https://github.com/mojontwins/MK1_NES/blob/master/testers/03_tester_interactives/README.md).

## Clausules

All sections described above contain a list of clausules. Each clausule is just a list of conditions followed by a list of commands. The interpreter will first iterate the list of conditions. If one fails, the interpreter discards the clausule and moves to the next, if any. If all conditions are true, the interpreter will run the commands list.

The syntax is:

```spt
    IF <condition>
    ...
    THEN
        <command>
        ...
    END
```

The interpreter will keep consuming all the clausules in the section in order, unless it reaches a commands section in a clausule with a BREAK, GAME OVER or WIN GAME command.

BREAK is quite useful if you don't need more checks after the current clausule successes. 

```spt
    IF $SOMEVAL = 0
    THEN
        SET TILE (2, 4) = 2
        SET $SOMEVAL = 1
        
        # BREAK is necessary so the next clausule doesn't
        # run, as $SOMEVAL = 1 now!
        BREAK
    END 

    IF $SOMEVAL = 1
    THEN
        SET TILE (2, 4) = 3
    END
```

Notice how, in this situation, you can save a `BREAK` just being clever when organizing your clausules:

```spt
    IF $SOMEVAL = 1
    THEN
        SET TILE (2, 4) = 3
    END

    IF $SOMEVAL = 0
    THEN
        SET TILE (2, 4) = 2
        SET $SOMEVAL = 1
    END
```

## Flags

The scripting engine works with a set of *flags*, which are *value containers*. The value can range from 0 to 127. The amount of available flags depend on the value of the `MAX_FLAGS` constant in `config.h`. Flags are referenced in the code using `FLAG N` or `#N` with N the flag number, or using aliases.

For example those are equivalent.

```spt
    IF FLAG 5 = #3
    IF #5 = FLAG 3
    IF #5 = #3
```

All would evaluate to true if the contents of flags 3 and 5 are the same.

## Aliases

Having to keep track of which flags contains what can be a chore. That's why it's possible to define aliases for flags. You can define and redefine the aliases as much as you want, they can be used right after the code defining them and until new code redefines them. Quite useful for multi-level games.

Aliases begin with character $ followed by an identifier. For example, we can bind alias $LLAVE to flag 2 and use this:

```spt
    IF $LLAVE = 1
```

Or even

```spt
    IF $STATUES_IN_PLACE = $STATUES_TO_PLACE
```

To define a set of alias, create a `DEFALIAS` section:

```spt
    DEFALIAS
        $ALIAS = N
        ...
    END
```

You can define as many aliases as you need. N is a flag number. 

## Dynamic interpreter

As mentioned, mscmk1 generates a minimal interpreter, which means that only the code needed to interpret the conditions and commands you have used will be generated. That means that the less different conditions and commands you use, the smaller the interpreter will be. Also, some commands and conditions are much bigger than others.

Sometimes there are several ways to achieve the same goal. Don't take this as a rule of thumb, but many times it's better to have more code if you can avoid using a new condition or command.

## Tautology

There's a condition which always evaluates to true:

```spt
    IF TRUE
```

## Conditions and commands related to flags

Most of your script will have to do with checking the values in flags and setting values to flags. There are quite a few conditions and commands related to flags:

### Conditions

```spt
    IF VALUE1 = VALUE2          Will be true if VALUE1 and VALUE2 are equal.

    IF VALUE1 < VALUE2          Will be true if VALUE1 is less than VALUE2.

    IF VALUE1 > VALUE2          Will be true if VALUE1 is more than VALUE2.

    IF VALUE1 <>|!= VALUE2      Will be true if VALUE1 and VALUE2 are different.

```

VALUE1 and VALUE2 can be numbers or flags. Flags can be expressed as FLAG N, #N or $ALIAS. For difference, you can use either <> or !=.

For example:

```spt
    IF $APPLES = 4
    IF FLAG 2 = FLAG 3
    IF $TIMES_PLAYED > $MINIMUM
```

### Commands

```spt
    SET FLAG = VALUE            Will put VALUE inside FLAG.

    INC|ADD FLAG, VALUE         Will increment the value inside FLAG VALUE times.

    DEC|SUB FLAG, VALUE         Will decrement the value inside FLAG VALUE times.

    FLIPFLOP FLAG               If 1 is inside FLAG, it will be changed to 0, 
                                and vice-versa.

    SWAP FLAG1, FLAG2           Will interchange the contents of FLAG1 and FLAG2.
```

Where FLAG can be either FLAG N, #N or $ALIAS. VALUE can be a number or a FLAG. 

For example:

```spt
    SET $BOILER = 2
    SET $STATE = $OLD_STATE
    INC $RABBITS, 2
    DEC $GAUGE, $STRENGTH
    FLIPFLOP FLAG 2
    SWAP $OLD, $NEW
    SWAP #2, #4
```

## Stuff in the engine which may modify flags.

The game engine is able to communicate with the scripting engine. It can be configured to write and read some game state values in flags. As of v1.0 (more to come):

### Pushed tile

If `ENABLE_PUSHED_SCRIPT` is set in `config.h`, everytime the player pushes a pushable tile the `PRESS_FIRE` scripts in the current screen will be run, but before, the engine will:

* copy the value of the overlapped (disappearing) tile to the flag configured in `PUSHED_TILE_FLAG`.
* copy the coordinates (X, Y)  of the pushable tile after being pushed at `PUSHED_TO_X_FLAG` and `PUSHED_TO_Y_FLAG`.  

This is used in the game [Cadàveriön](https://github.com/mojontwins/MK1_NES/tree/master/examples/09_cad%C3%A0veri%C3%B6n). In the README.md you can read an explanation on how this works.

### Timer

*Deprecated*: If the constant `TIMER_TIME_FLAG` is defined in `config.h`, the value of the timer will be copied to the flag # `TIMER_TIME_FLAG`. Note that, from the script, this is a read-only value. Setting the flag from the script will have no effect in the timer whatsoever.

To interact with the timer properly, you have a whole set of conditions and commands which makes the need of using `TIMER_TIME_FLAG` obsolete. 

More on the timer later.

### Only one object flag

If the constant `ONLY_ONE_OBJECT_FLAG` is defined in `config.h`, the value 1 or 0 is copied to such flag if the player is carrying a collectible or not.

### Easy objects

Interaction with easy objects is still TODO

### Interactives

The `FLAG_INVENTORY`flag will contain the value of the object the player is carrying. Besides, interactives are graphical representations of flags: the engine will draw the defined interactives on screen using a sprite depending on the contents of the flag.

More on interactives later.

### Counting collectibles.

Related to the count of collected items (hotspots type 1), you have these conditions:

```spt
    IF PLAYER_HAS_OBJECTS       Will be true if the player has *any* objects.

    IF OBJECT_COUNT = VALUE     Will be true if the count of collectible
                                objects equals VALUE
```

VALUE can be a number or a flag. Flags can be expressed as FLAG N, #N or $ALIAS.

There's also some commands:

```spt
    INC OBJECTS, VALUE          Increases the collectible items count by VALUE

    DEC OBJECTS, VALUE          Decreases the collectible items count by VALUE
```

### Body counts

If the player can kill enemies, there's a couple of conditions available:

```spt
    IF ALL_ENEMIES_DEAD         True if player killed all enemies on level.

    IF ENEMS_KILLED = VALUE     True if player killed VALUE enemies.
```

### Player life

There are commands to increase, decrease, or completely refill the player's life:

```spt
    INC LIFE, VALUE             Increments player's life by VALUE

    DEC LIFE, VALUE             Decrements player's life by VALUE

    REFILL                      plife = PLAYER_LIFE (full refill)
```

## Conditions and commands related to position

### Conditions about coordinates

```spt
    IF PLAYER_TOUCHES (x, y)    True if player touches tile at (x, y)
                                x and/or y can be either numbers of flags

    IF PLAYER_IN_X x1, x2       True if player is between pixel coordinates
                                x1 and x2, horizontally.

    IF PLAYER_IN_Y x1, x2       True if player is between pixel coordinates
                                y1 and y2, vertically.
```

`PLAYER_IN_X` and `PLAYER_IN_Y` have the pseudoaliases `PLAYER_IN_X_TILES` and `PLAYER_IN_Y_TILES` which take parameters in tile coordinates but generate `PLAYER_IN_X|Y` commands with calculated pixels coordinates.

### Commands to change position

```spt
    SET X = VALUE               Changes the player to tile coordinate X,
                                horizontally.

    SET Y = VALUE               Changes the player to tile coordinate Y,
                                vertically.

    SET XY = VALUE1, VALUE2     Changes the player to tile coordinates
                                VALUE1, VALUE2.
```

VALUE, VALUE1 and VALUE2 can be either numbers or flags. Flags can be expressed as FLAG N, #N or $ALIAS.

### Conditions related to the current screen

```spt
    IF NPANT = VALUE            True if player is on screen VALUE.

    IF NPANT <>|!= VALUE        True if player is NOT on screen VALUE.
```

VALUE can be a number or a flag. Flags can be expressed as FLAG N, #N or $ALIAS.

### Changing to a different screen

```spt
    WARP_TO N             Changes the player to screen N
```

N may be a number or a flag. Flags can be expressed as FLAG N, #N or $ALIAS.

Note that `WARP_TO` doesn't reposition the player! To do so, use `SET X`, `SET Y` or `SET XY` alongside `WARP_TO`.

Note that the construction `WARP_TO N, X, Y` is accepted, but it generates a sequence of:

```spt
    SET XY = X, Y
    WARP_TO N
```

### Which level?

This may look redundant, but it's not. In normal situations you *know* which level you are in, but the same script *can* be shared by several levels, so,

```spt
    IF LEVEL = VALUE            True if level is N.

    IF LEVEL <>|!= VALUE        True if level is not N.
```

VALUE can be a number or a flag. Flags can be expressed as FLAG N, #N or $ALIAS.

### Changing levels

```spt
    WARP_TO_LEVEL L, N          Changes the player to screen N of level L
```

Either L or N are numbers or flags. Flags can be expressed as FLAG N, #N or $ALIAS.

Note that `WARP_TO_LEVEL` doesn't reposition the player! To do so, use `SET X`, `SET Y` or `SET XY` alongside `WARP_TO`.

Note that the construction `WARP_TO_LEVEL L, N, X, Y` is accepted, but it generates a sequence of:

```spt
    SET XY = X, Y
    WARP_TO N
```

### Redraw the screen

You have the ability to redraw the screen as it is stored in the current screen buffer `scr_buff`. Note that breaking or pushing tiles around actually modify `scr_buff`, so this command will NOT revert the screen.

```spt
    REDRAW
```

### Reenter the screen

This command will indeed reenter the screen: the background will be drawn from map data (thus broken or pushed tiles will be reverted), and enemies will be reloaded, etc.

```spt
    REENTER
```

## Conditions about movement

```spt
    IF VX|VY < VALUE            True if VX / VY is less than VALUE

    IF VX|VY > VALUE            True if VX / VY is more than VALUE

    IF VX|VY = VALUE            True if VX / VY is VALUE

    IF VX|VY !=|<> VALUE        True if VX / VY is not VALUE
```

VALUE can be a number or a flag.  Flags can be expressed as FLAG N, #N or $ALIAS.

## Modifying current screen

### Modifying the game area

Printing tiles in the game area actually modifies the screen, as printed tiles will set its behaviour on the cell they are printed to. Note that `REDRAW` will conserve the changes, `REENTER` will destroy them.

```spt
    SET TILE (X, Y) = VALUE     Puts tile VALUE at coordinates X, Y.
```

Either VALUE, X, or Y can be numbers or flags. Flags can be expressed as FLAG N, #N or $ALIAS.

If you have several (three or more) tiles to print, you can save space using decorations, which are triads of numeric values X, Y, T. Each triad will set tile T at (X, Y) and lies on a separate line. The end of the list is marked by `END`.

```spt
    DECORATIONS
        X, Y, T
        ...
    END
```

### Changing just the behaviour

Not ready yet!

### Printing anywhere

This facilty let's you print anytile anywhere in the screen, not just the game area, and uses character coordinates. Obviously, those print don't put anything on the game area, so cell behaviours will not be affected at all.

```spt
    PRINT_TILE_AT (x, y) = n    Prints tile n at CHARACTER coordinates x, y
```

This can be useful for many tasks, but for example you can hide secret passages after apparently non walkable tiles by printing tiles over gaps using this command. The area will still be walkable as the cell behaviours won't be affected.

### Show changes

```spt
    SHOW
```

This will make changes visible. This is may be desirable as `SET TILE` won't send the changes to the screen until next NMI.

## The timer

The timer will count down in seconds from an initial value if it is on, and is able to execute things when it reaches zero. From the script, you can set and read the timer. There's also a special section which is executed when the timer reaches zero, `ON_TIMER_OFF`.

In the future: `ON_TIMER_TICK` section, not ready yet.

### Conditions

```spt
    IF TIMER = VALUE            True if TIMER equals VALUE

    IF TIMER <>|!= VALUE        True if TIMER does not equal VALUE

    IF TIMER >= VALUE           True if TIMER >= VALUE

    IF TIMER <= VALUE           True if TIMER <= VALUE
```

VALUE can be a number or a flag. Flags can be expressed as FLAG N, #N or $ALIAS.

### Commands

```spt
    SET TIMER = VALUE           Sets the TIMER to VALUE

    TIMER START                 Starts the timer.

    TIMER STOP                  Stops the timer.
```

VALUE can be a number or a flag. Flags can be expressed as FLAG N, #N or $ALIAS.

## Interactives 

As mentioned, Interactives are graphicas stuff (in fact, metasprites) you can put on screen and interact with. There are two kinds of interactives:

* Simple interactives (called *sprites*): Just sprites you can interact with.
* *Containers*: Graphical representation of flags - Containers are bound to flags and show its contents. On interaction, the object the player is carrying (which is a value) and the value stored in the flag are swapped.

The object the player is carrying is stored in a flag, as defined by the constant `FLAG_INVENTORY` in `config.h`. It's good practice to have this in your `config.h`:

```c
    #define FLAG_INVENTORY 1
```

And this in your script:

```spt
    DEFALIAS
        $PINV 1
        ...
    END
```

And always reference the object the player is carrying using `$PINV`.

### Display

Interactives are created when you enter a new screen. They will exist on that screen only. The maximum number of interactives you can create in a screen is defined in `config.h` using `INTERACTIVES_MAX`. Note that this isn't a limit on how many interactives you have in your level.

Interactives have three attributes: X, Y and F. (X, Y) defines where they appear in the game area. F is related to the displayed metasprite, which always comes from the `spr_hs` metasprite array:

- *Sprites* will show the metasprite `spr_hs [F]`.
- *Containers* will show the metasprite `spr_hs [flags [F]]`.

### Initialization

**Containers make flags interactuable**. You can use containers and flags to place objects around the map you can get and carry, and also places where you can drop what you are carrying.

So everytime the game starts you must give initial values to the flags you are planning to bind with containers. That's usually done in the `ENTERING_GAME` section in the current level.

```spt
    DEFALIAS
        $CONT1 1
        $CONT2 2
    END

    ENTERING GAME
        IF TRUE
        THEN
            # Initialize containers to initially contain
            # a flower (metasprite 8) and a quill (metasprite 9)
            SET $CONT1 = 8
            SET $CONT2 = 9
        END
    END
```

### Placement

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

### Checks with containers

As containers are bound to flags, you just have to check the contents of the appropriate flags. For example, if you are using $CONT_CHEST to implement a chest, you can check if it contains the quill (metasprite 9, for example) as easy as:

```spt
    IF $CONT_CHEST = 9
```

### Interaction

When the player interacts with an interactive, the interactive will be referenced by the special script variable `ARG`, `JUST_INTERACTED` will evaluate to `TRUE` and the `PRESS_FIRE` sections in the script will be run. Sprites and containers are checked for differently.

To check if the player interacted with sprite S:

```spt
    IF JUST_INTERACTED
    IF ARG = S
```

For example:

```spt
    IF JUST_INTERACTED
    IF ARG = 7
```

Will evaluate to true if the player just interacted with sprite with F = 7 (showing spr_hs [7]).

To check if the player interacted with container F (executed **after the objects have been swapped**):

```spt
    IF JUST_INTERACTED
    IF ARG ~ F
```

For example:

```spt
    IF JUST_INTERACTED
    IF ARG ~ $CONT_CHEST
```

So if the player interacted with the chest, `PRESS_FIRE` is run, and there, the above code will evaluate to true as `ARG` will *reference* `$CONT_CHEST`. If the script was fired 'cause the player interacted with a different container, `IF ARG ~ $CONT_CHEST` will evaluate to false.

You can check [Tester interactives](https://github.com/mojontwins/MK1_NES/tree/master/testers/03_tester_interactives) for a good example on using interactives from the script.

## End of game

You can finish the game from the script in two ways:

```spt
    GAME OVER                   Make the player lose the game

    WIN GAME                    Make the player win the game
```

The `WIN GAME` command will end the current level successfully.

## The fire zone

The *fire zone* is a rectangle on screen which will launch the `PRESS_FIRE` script if the player overlaps it. You have to enable them in `config.h`:

```c
    #define ENABLE_FIRE_ZONE
```

To define a fire zone you can do it in pixel or tile coordinates. This can be done in any commands section. The fire zone will be active until the player moves to a different screen.

```spt
    SET FIRE_ZONE x1, y1, x2, y2

    SET FIRE_ZONE_TILES x1, y1, x2, y2
```

Note that `SET FIRE_ZONE_TILES` will be internally converted to a play `SET FIRE_ZONE` by the compiler.

To clear the fire zone, just move it to an unreachable area, for example:

```spt
    SET_FIRE_ZONE 255, 255, 255, 255
```

## Extern code

If you need to do something which can't be directly achieved from the script, you can use the extern code facility. Extern code support has to be enabled in `config.h`.

```spt
    ENABLE_EXTERN_CODE
```

That will make the contents of `my/extern.h` to be included in your game. In that file, there's a main entry point:

```c
    void do_extern_action (unsigned char n) {
        //
    }
```

When the interpreter finds this command in the script:

```spt
    EXTERN N
```

It will call `do_extern_action (n)`, with n = N a number. You can then add your own c code to `do_extern_action` to perform tasks depending on `n`. 

For example, in [Cadàveriön](https://github.com/mojontwins/MK1_NES/tree/master/examples/09_cad%C3%A0veri%C3%B6n) it is called from `ON_TIMER_OF` to do the *time over* sequence (hide the sprites, show 'TIME OVER!' on screen, wait for input, and reload the hotspots).

## Safe spots

If `DIE_AND_RESPAWN` is defined in `config.h`, whenever the player jumps (while on a safe spot), the coordinates will be stored as 'safe', and when the player dies, it will be respawned on such coordinates.

You can set such spot manually from the script:

```spt
    SET SAFE HERE               Store current player coordinates as 
                                'safe spot'.

    SET SAFE N, X, Y            Set the 'safe spot' at screen N, coordinates
                                (X, Y).
```

## Text output

There are two ways to display text, the legacy "line of text" and the new text box.

```spt
    TEXT "SOME TEXT"            Shows some text in the line of text area
```

The location of the line of text area is defined by the constants `LINE_OF_TEXT` (Y coordinate) and `LINE_OF_TEXT_X` (X coordinate) in `config.h`.

As this is quite limited, we added a proper text box:

```spt
    TEXTBOX N                   Display a text box showing text N.
```

Texts are defined in `assets/custom_texts.h`:

```c
    const unsigned char custom_text0 [] = 
        "THIS IS A FILLER!%";
        
    const unsigned char * const custom_texts [] = {
        custom_text0
    };
```

Add your strings as `custom_textN` arrays, then reference them in the `custom_texts` list. The value N in the script references to this array. So if you have:

```c
    const unsigned char custom_text0 [] = 
        "HELLO, MY FRIEND!%"
        "IT'S A NICE DAY!%";

    const unsigned char custom_text1 [] = 
        "BEWARE!!!%"
        "HUNGRY PIRANHAS%";
        
    const unsigned char * const custom_texts [] = {
        custom_text0
    };
```

Then this:

```spt
    TEXTBOX 1
```

Will display a text box with the text:

```
    BEWARE!!!
    HUNGRY PIRANHAS
```

In the strings, % is a line break. Lines shouldn't be longer than 18 characters (excluding the line break). Strings should end with a line break.

## Miscellaneous

```spt
    FLICKER                     Makes the player flicker for a while

    PAUSE|DELAY VALUE           Will pause for VALUE frames.

    MUSIC OFF|STOP              Stops the background music.

    MUSIC VALUE                 Plays the music VALUE

    SOUND VALUE                 Plays the sound effect VALUE
```
