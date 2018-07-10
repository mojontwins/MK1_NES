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

```
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

```
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

```
    IF FLAG 5 = #3
    IF #5 = FLAG 3
    IF #5 = #3
```

All would evaluate to true if the contents of flags 3 and 5 are the same.

## Aliases

Having to keep track of which flags contains what can be a chore. That's why it's possible to define aliases for flags. You can define and redefine the aliases as much as you want, they can be used right after the code defining them and until new code redefines them. Quite useful for multi-level games.

Aliases begin with character $ followed by an identifier. For example, we can bind alias $LLAVE to flag 2 and use this:

```
    IF $LLAVE = 1
```

Or even

```
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

```
    IF TRUE
```

## Conditions and commands related to flags

Most of your script will have to do with checking the values in flags and setting values to flags. There are quite a few conditions and commands related to flags:

### Conditions

```
    IF VALUE1 = VALUE2          Will be true if VALUE1 and VALUE2 are equal.

    IF VALUE1 < VALUE2          Will be true if VALUE1 is less than VALUE2.

    IF VALUE1 > VALUE2          Will be true if VALUE1 is more than VALUE2.

    IF VALUE1 <>|!= VALUE2      Will be true if VALUE1 and VALUE2 are different.

```

VALUE1 and VALUE2 can be numbers or flags. Flags can be expressed as FLAG N, #N or $ALIAS. For difference, you can use either <> or !=.

For example:

```
    IF $APPLES = 4
    IF FLAG 2 = FLAG 3
    IF $TIMES_PLAYED > $MINIMUM
```

### Commands

```
    SET FLAG = VALUE            Will put VALUE inside FLAG.

    INC|ADD FLAG, VALUE         Will increment the value inside FLAG VALUE times.

    DEC|SUB FLAG, VALUE         Will decrement the value inside FLAG VALUE times.

    FLIPFLOP FLAG               If 1 is inside FLAG, it will be changed to 0, 
                                and vice-versa.

    SWAP FLAG1, FLAG2           Will interchange the contents of FLAG1 and FLAG2.
```

Where FLAG can be either FLAG N, #N or $ALIAS. VALUE can be a number or a FLAG. 

For example:

```
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

```
    IF PLAYER_HAS_OBJECTS       Will be true if the player has *any* objects.

    IF OBJECT_COUNT = VALUE     Will be true if the count of collectible
                                objects equals VALUE
```

VALUE can be a number or a flag. Flags can be expressed as FLAG N, #N or $ALIAS.

There's also some commands:

```
    INC OBJECTS, VALUE          Increases the collectible items count by VALUE

    DEC OBJECTS, VALUE          Decreases the collectible items count by VALUE
```

### Body counts

If the player can kill enemies, there's a couple of conditions available:

```
    IF ALL_ENEMIES_DEAD         True if player killed all enemies on level.

    IF ENEMS_KILLED = VALUE     True if player killed VALUE enemies.
```

## Conditions and commands related to position

### Conditions about coordinates

```
    IF PLAYER_TOUCHES (x, y)    True if player touches tile at (x, y)
                                x and/or y can be either numbers of flags

    IF PLAYER_IN_X x1, x2       True if player is between pixel coordinates
                                x1 and x2, horizontally.

    IF PLAYER_IN_Y x1, x2       True if player is between pixel coordinates
                                y1 and y2, vertically.
```

`PLAYER_IN_X` and `PLAYER_IN_Y` have the pseudoaliases `PLAYER_IN_X_TILES` and `PLAYER_IN_Y_TILES` which take parameters in tile coordinates but generate `PLAYER_IN_X|Y` commands with calculated pixels coordinates.

### Commands to change position

```
    SET X = VALUE               Changes the player to tile coordinate X,
                                horizontally.

    SET Y = VALUE               Changes the player to tile coordinate Y,
                                vertically.

    SET XY = VALUE1, VALUE2     Changes the player to tile coordinates
                                VALUE1, VALUE2.
```

VALUE, VALUE1 and VALUE2 can be either numbers or flags. Flags can be expressed as FLAG N, #N or $ALIAS.

### Conditions related to the current screen

```
    IF NPANT = VALUE            True if player is on screen VALUE.

    IF NPANT <>|!= VALUE        True if player is NOT on screen VALUE.
```

VALUE can be a number or a flag. Flags can be expressed as FLAG N, #N or $ALIAS.

### Changing to a different screen

```
    WARP_TO N, X, Y             Changes the player to screen N at tile
                                coordinates X, Y.
```

Either N, X, or Y can be numbers or flags. Flags can be expressed as FLAG N, #N or $ALIAS.

### Changing levels

Not ready yet!

### Redraw the screen

You have the ability to redraw the screen as it is stored in the current screen buffer `scr_buff`. Note that breaking or pushing tiles around actually modify `scr_buff`, so this command will NOT revert the screen.

```
    REDRAW
```

### Reenter the screen

This command will indeed reenter the screen: the background will be drawn from map data (thus broken or pushed tiles will be reverted), and enemies will be reloaded, etc.

```
    REENTER
```

## Modifying current screen

### Modifying the game area

Printing tiles in the game area actually modifies the screen, as printed tiles will set its behaviour on the cell they are printed to. Note that `REDRAW` will conserve the changes, `REENTER` will destroy them.

```
    SET TILE (X, Y) = VALUE     Puts tile VALUE at coordinates X, Y.
```

Either VALUE, X, or Y can be numbers or flags. Flags can be expressed as FLAG N, #N or $ALIAS.

If you have several (three or more) tiles to print, you can save space using decorations, which are triads of numeric values X, Y, T. Each triad will set tile T at (X, Y) and lies on a separate line. The end of the list is marked by `END`.

```
    DECORATIONS
        X, Y, T
        ...
    END
```

### Changing just the behaviour

Not ready yet!

### Printing anywhere

This facilty let's you print anytile anywhere in the screen, not just the game area, and uses character coordinates. Obviously, those print don't put anything on the game area, so cell behaviours will not be affected at all.

```
    PRINT_TILE_AT (x, y) = n    Prints tile n at CHARACTER coordinates x, y
```

This can be useful for many tasks, but for example you can hide secret passages after apparently non walkable tiles by printing tiles over gaps using this command. The area will still be walkable as the cell behaviours won't be affected.

### Show changes

```
    SHOW
```

This will make changes visible. This is may be desirable as `SET TILE` won't send the changes to the screen until next NMI.

## Player life

## The timer

## Interactives 

## End of game

## The fire zone

## Extern code

## Safe spots

## Miscellaneous
