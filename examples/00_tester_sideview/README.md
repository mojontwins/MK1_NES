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
- Enemy type monococo
- Easy objects (Inventory system & hotspot based Containers)

To Fix
======

All fixed.

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

Easy objects
============

Provides infraestructure to place N objects in hotspots, N places to use such objects, and places to add custom code that is executed everytime you get an object and everytime you use an object.

Objects and places to use such objects are represented by hotspots, so just 1 per screen. 

hotspot values representing objects must fit in a range, defined by `HS_OBJ_MIN` and `HS_OBJ_MAX`.

hotspot values representing where to use each object must fit in the range defined by `HS_OBJ_MIN + HS_USE_OFFS` and `HS_OBJ_MAX + HS_USE_OFFS`.

So if you get object `N` you have to use it in the hotspot with value `N + HS_USE_OFFS`.

Every time you get an object, code included in `mainloop/on_object_got.h` (empty by default) is executed.

Every time you use an object successfully, code included in `mainloop/on_object_used.h` (empty by default) is executed.

There are two types of behaviour: Type A and Type B. If you define `HS_TYPE_A`, you enable Type A. If you undefine it, you enable Type B.

With type A, hotposts where you use objects are represented by the empty item, that is, `HS_OBJ_EMPTY`. When you use an object, the hotspot will use the objects's graphic.

With type B, everything must have its own graphic: objects, place to use them, and objects used on places. If object is item `N`, then where to use is `N + HS_USE_OFFS` and object used = `N + 2*HS_USE_OFFS`. Very important when define the `spr_it` array.

Combine this with `WIN_LEVEL_CUSTOM`! if you define `WIN_LEVEL_CUSTOM`, the level with end as soon as you make `win_level = 1`.

You can check the ending condition at  `mainloop/on_object_got.h` or `mainloop/on_object_used.h`, for instance, and raise `win_level`. You can use the `level` variable in multilevel games for different conditions.

