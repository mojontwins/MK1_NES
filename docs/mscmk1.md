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



