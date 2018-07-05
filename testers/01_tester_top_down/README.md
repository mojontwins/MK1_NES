What's this?
============

I've made LOTS of changes to MK1 in order to make it ready for release and to polish stuff and remove shit I didn't like, or enhance some modules and dah dah dah.

The thing is that I'm sure I have broken everything.

Porting old games to the new version will help me debug the engine.

Interesting bits
----------------

This game uses a lot of the new features found in AGNES 1.0 to make gameplay smoother. It also features new graphics. But I'm writing this because this is the only game in the set which features a custom level order for the scripts.

Usually, you can stuff the scripts for several levels in the script file. Say you add three level sections (finishing each section with `END_OF_LEVEL` and stuff). An array of pointers to level script binaries will be automaticly generated which maps script X to level X, so when loading level N the script with index N in the array will be used as the level script. 

In this game, we need the same script to run for both level, so we need a custom mapping mechanism so when level 1 is set up, script 0 is set to be used as well. This is a new feature I just wrote days before releasing this (I didn't think of it at all but accidentally found the issue when doing some last time tests using this game) which is achieved by adding a command somewhere near the top of your script file which reads as:

```spt
	CUSTOM_SCRIPT_ORDER A, B, C, D, ...
```

Which will map script #A to level 0, script #B to level1, script # C to level 2, and so forth. For this game it is

```spt
	CUSTOM_SCRIPT_ORDER 0, 0
```

So a proper array of pointers to level script binaries is created, containing two pointers to the level 0 script binary. That way when you run level 1, script 0 will be used.

Ain't this cool?
