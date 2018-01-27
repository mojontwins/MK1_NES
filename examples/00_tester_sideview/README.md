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

Next

- Enemy type monococo (add)

To Fix
======

[ ] Strange behaviour when jumping between laterally connected screens.
[ ] Glitches when respawning in a different screen (player gets repositioned too early - easy to fix?)

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