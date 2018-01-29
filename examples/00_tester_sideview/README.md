Tester Sideview
===============

Simple project to test/add/debug MK1 engine stuff.

Things, so far:

- Quicksands
- Slippery platforms/obstacles
- Conveyor platforms/obstacles
- Pushable boxes
- Can fire/breakable
- Enemy type fanty
- Enemy type homing fanty
- Enemy type saw
- Enemy type pezon
- Enemy type chac chac
- Enemy type monococo (add)

Next

- Inventory system & Containers

To Fix
======

[X] Strange behaviour when jumping between laterally connected screens.
[X] Glitches when respawning in a different screen (player gets repositioned too early - easy to fix?)

Things to note for possible documentation
=========================================

The occluding frame
-------------------

Saws and Pezons need an "occluding frame" to do the sprite priority trick. It should be one frame using the darkest colour in the selected palette to mask areas of the sprite which should be hidden when appearing. Every 8x8 sprite in the metasprite should have at lease 1 #FF00FF pixel for the converter to autodetect it. See example 00.

Saws
----

- Type 8
- You define the travelling direction with start/end points in ponedor.
- It will emerge left/up if the attribute is `0`, right down if it is `2`.
- It needs two, not flipped frames, from `SAW_BASE_SPRID`.

Fanties / Homing fanties
------------------------

- Type 6
- Just define the position in ponedor.
- Use two frames from `FANTY_BASE_SPRID`, or four (right1, right2, left1, left2) if you define `FANTY_WITH_FACING`.

Chac-Chacs
----------

- Type 10
- 3 tiles tall, placed in ponedor marking the topmost tile.
- Attribute in ponedor is delay between shutter
- Needs 7 contiguous tiles in the tileset (can be outside the 16 first).
- Needs the behaviours of each of the 7 tiles to be properly set: `0, 0, 1, 1, 1, 1, 0`.

Monococos
---------

This was really bad implemented in Yun (very patchy). Let's do this properly this time.

A monococo is an enemy which

1. Is out of the screen for A frames.
2. Appears during B frames
3. When out, shoots one coco, and stays for C frames.
4. Then disappears during D frames.

We have two types.

Type A: step 1 shows no sprite. steps 2 & 4 shows sprite animation (2 cells) normally, but flickering via ` + half_life`.
Type B: step 1 shows a special "hidden" cell. steps 2 & 4 shows a special "appear / disappear" frame.

We will use 4 frame definitions: `FRAME_A`, `FRAME_B`, `APPEARING`, `HIDDEN`. If defined `MONOCOCO_TYPE_A`, cells 3 & 4 are ignored. You can even take them away from your metasprites array.

Monococo has a timer which is controlled by a number of "state times" in a 4 elements array, but controlled only by three #defines in config.h:

```c
const unsigned char monococo_state_times [] = {
	MONOCOCO_BASE_TIME_HIDDEN,
	MONOCOCO_BASE_TIME_APPEARING,
	MONOCOCO_BASE_TIME_ONBOARD, 
	MONOCOCO_BASE_TIME_APPEARING
};
```

- Type 11
- Just define the position in ponedor.

Cocos frames are (order may be off, but helps with saving bytes)

(RIGHT)  `FRAME_A`, `FRAME_B`, (LEFT)   `FRAME_A`, `FRAME_B`,
[(RIGHT) `APPEARING`, `HIDDEN`, (LEFT) `APPEARING`, `HIDDEN`]

