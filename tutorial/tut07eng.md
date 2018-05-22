Cutting and importing graphics
==============================

*Note: the files referenced in this article are in the folder `/tutorial/assets/Sir Ababol DX/gfx`*

As designers, we have always worked on having a system which would allow us to worry only about pushing pixels, freeing ourselves from the chores of manual converting, reorganizing, etc. to put the graphics into the game.

This system, which has been evolving slowly since mid 2015, is a *Frankenstein* app called **mkts**. It's a command line tool which is capable of reading a script with instructions on how to build the binary which will eventually become the CHR-ROM of our cartridge. It also generates several kinds of metadata needed to actually make use of the imported entities.

Although **mkts** does much more things, we are going to cut just four kinds of entities:

- **Charsets**: the tool generates a series of 8x8 pixels patterns from a rectangle in the input image. This kind of entity doesn't generate metadata.
- **Metatilesets**: collections of tiles which will be used to build the screens of our map. They are bits of scenery measuring 16x16 pixels (2x2 patterns) with an associated palette. When cutting metatilesets, **mkts** will try and reuse already cut patterns. It also generates metadata: a *map* for each tile telling which 4 patterns are used to draw it, plus the associated palette.
- **Metasprites**: metasprites are related collections of sprites (remember that the PPU uses just one 8x8 pattern to draw each sprite) which move together to create the impression of a bigger sprite. **mkts** cuts metasprites attempting to reuse already cut patterns and generates metadata indicating the arrangement of the sprites which compose such metasprites.
- **Metaspritesets**: Collections of metasprites with the same features.

Before explaining how **mkts** works, let's take a glance at the graphics we will be cutting and importing for **Sir Ababol**:

The charset
-----------

> **AGNES** uses charsets made of 64 characters located at the beginning of bank 0 in CHR-ROM (i.e. - they have indexes 0 to 63). They are ordered the same as ASCII, but character 0 is in fact ASCII 32, so to find a character in the pattern set you should substract 32 to its ASCII code. Charsets include symbols, numbers and the uppercase alphabet. It's common to substitute symbols we won't use for bits and pieces of graphics to make better use of the limited space.

This is the charset we will be using in **Sir Ababol**, `font.png`. As you will notice, there are several symbols which will be used to make text boxes in the **DX** version.

[[font.png]]

The tileset
-----------

Depending on the map format we will be using (more on this in the corresponding chapter), the tileset of our level may have 16, 32 or up to 256 different tiles. The term *tile*, in this context, refers to the bits of 16x16 pixels the engine uses to compose the screens of our map.

**Sir Ababol** uses 16 tiles. There are four extra tiles used by a custom renderer to substitute the ones referenced in the map data in when certain conditions are met (more about this in the corresponding chapter). Finally, there are 9 extra tiles used to render text boxes in the **DX** version.

[[ts0.png]]

The spriteset
-------------

**AGNES** uses three spritesets for three different kinds of entities:

- The **main character**, with different cells for all the actions the main character may do, depending on which modules are active in the game.
- **Enemies**, covering all kinds of enemies which appear in the game. Moving platforms are considered **enemies** in **AGNES** (harmless, of course).
- **Items & interactives** - collectables, keys, refills, items, characters...

> In **AGNES**, the main character is represented by a 8x16 pixel rectangle, and enemies, items, etc by 16x16 pixel rectangles. Such rectangles are used for reference coordinates, placement and collision. Obviously, the actual metasprites may be bigger, that's why we call such rectangles *logical rectangles*.

In the spriteset for **Sir Ababol** we have cells to represent Sir Ababol idling, walking, jumping and swimming. In its basic form we won't be using the swimming cells, but we'll be importing them anyways.

Next we'll find enemies and moving platforms. In the first row we can find three types of enemies and a moving platform, plus an explosion which will be shown when Sir Ababol crushes enemies - we won't be using this either in the basic **Sir Ababol**, but we will be importing it anyways.

In the next row we have three extra kinds of enemies which will be used in the **DX**, enhanced version of the game.

Finally there's a collection of cells used for items: an Ababol flower (collectible item), a key (used to open locks), a chicken leg (which serves as a life refill) and two extra types used in the **DX** game: a pair of boots (which will enable the ability to jump on enemies to crush them) and a sign (which will be used to show some bits of text to the player).

[[ss.png]]

Imports and palettes
====================

It's important to remember that we have 4 different palettes with 3 colours each for the background and 4 more for the sprites, plus a global background colour.

For **mkts** to work, it has to *know* which palettes it should recognice in the graphics you tell it to cut and import. As PPU generated colours vary quite a lot from each piece of HW to the next, **mkts** is designed to *understand* this exact representation of the NES palette:

[[fullnespal.png]]

(Besides, **mkts** can work in *Aseprite mode*, recognising the mysterious *NES* palette which comes bundled with *Aseprite*).

So in order for everything to work, we have to draw our graphics using the colours as found in the provided `fullnespal.png`. Besides, we'll have to greate what we call *palette images*. 

*Palette images* are 16x1 pixels png files. From each set of 4 pixels, the first is the shared background colour (that is, index 0 of each sub-palette is the background colour), followed by the three colours of each subpalette. For instance, this is a 16x magnification of the image palette we'll be using for the background tiles:

[[palts0_big.png]]

Sir Ababol uses a *palette image* for the background, `palts0.png`, and another one for the sprites, `palss0.png`.

How mkts works
==============

**mkts** can function in several ways; we will be using the *scripting mode*. **mkts** will read commands from a script and will build a binary we'll use as CHR-ROM, besides a collection of C-code files with associated metadata.

**mkts** can generate full CHR-ROM images (8K in two 4K banks, named bank 0 and bank 1) or partial binaries (for use in more complex mappers). To generate full CHR-ROM images we'll be using special commands to pad the data correctly. Besides, **mkts** adds every new pattern to an internal *pool* so it can recognize already imported patterns and reuse them. There's a command to reset the pool which we'll be using then we finish cutting patterns for bank 0 and start cutting for bank 1.

The pattern cutting script for **Sir Ababol DX** can be found at `/tutorial/assets/Sir Ababol DX/gfx/import_patterns.spt`, but we'll be building it step by step in this chapter.

> The first thing you should do is go inside `/gfx/`, find `import_patterns.spt`, and delete the default script so you can write yours. Also, delete all `png` files in `/gfx/` and copy those from `/tutorial/assets/Sir Ababol DX/gfx`.

**mkts** works by cutting rectangles defined by a pair of origin coordinates and dimmensions, and runs from left to right and from top to bottom.

Before we start, *coordinates* and *entity sizes* are measured in *patterns* (8x8 pixels), *set dimensions* are measured in *entities* (i.e. 2x2 tiles, 3x2 metasprites ...) and *offsets* (as used in metasprites) are measured in pixels.

Basic commands
--------------

This is a list of the basic commands:

```
	OPEN image.png
```

Will load `image.png`. Every cutting command from now on will refer to this image until a new image is loaded.

```
	PALS pal.png
```

Will open the *palette image* `pal.png` and will extract the palette data from it. Every cutting command from now on will use the extracted palette until a new palette is loaded.

```
	MAPFILE metadata.h
```

Defines the output file for the generated metadata. All metadata generated while cutting graphics will be written to that file, until a new output file is especified.

```
	LABEL label
```

Metadata is generated as C arrays. The identifiers of such arrays will use the defined string as a prefix. 

```
	RESETPOOL
```

Clears the used patterns pool. We will use it when we finish cutting patterns for bank 0 and are about to start bank 1.

```
	FILL to
```

Pads with zeroes until the output binary length is `to` bytes. We'll be using it to pad until the end of bank 0 (`FILL 4096`) and to the end of bank 1 (`FILL 8192`).

Bank 0, charset
---------------

First thing we'll be importing is the set of 64 patterns **ANES** uses to display text from `font.png`. The command used is `CHARSET`, whose syntax is:

```
	CHARSET x0, y0, w, h
```

Where `x0, y0` are the origin coordinates inside the png and `w, h` are the dimensions of the rectangle we want to cut. In `font.png` the 64 patterns are arranged as a 32x2 rectangle, so:

```
	OPEN font.png
	PALS palts0.png
	CHARSET 0, 0, 32, 2
```

As charset don't generate metadata, we won't need `MAPFILE` or `LABEL`.

Bank 0, tilset
--------------

In this initial version we'll be completing bank 0 with the patterns extracted from the tileset contained in `ts0.png`. To cut tilesets we use the `MAPPED` command, whose syntax is:

```
	MAPPED x0, y0, wm, hm, w, h [, max]
```

here `x0, y0` are the origin coordinates inside the png and `w, h` are the dimensions of the rectangle we want to cut **in tiles**. `wm, hw` are the size *in patterns* of each entity - in **AGNES** such dimmensins are **always** `2, 2`, as map tiles are 2x2 patterns. `max` is used if we want to cut fewer tiles than the rectangle contains.

In our example we want to cut 41 tiles from `ts0.png` which contains an array of 16x3 tiles. Before we start cutting, we need to set the file which will store the metadata first. As **mkts** will be called from the `/gfx/` folder, we add a relative path to the file leading to the `/dev/assets/` folder, which is where **AGNES** expects the metadata to reside.

```
	MAPFILE ..\dev\assets\tiledata.h
```

And them the cutting command:

```
	OPEN ts0.png
	PALS palts0.png
	LABEL ts0
	MAPPED 0, 0, 2, 2, 16, 3, 41
```

(the `PALS` line isn't really necessary as the palette is already active, but personally I like to have it always present in case I change it or move stuff around).

This code makes **mkts** open `ts0.png`, read the palette from `palts0.png`, take `ts0` as the prefix to generate metadata, and cut 41 2x2 patterns tiles from `0, 0` from a rectangle of 16x3 tiles.

In the metadata generated and stored in `tiledata.h`, **mkts+* will generate two arrays for each `MAPPED` command: `<prefix>_pals`, with the palette used by each tile, and `<prefix>_tmaps`, with the four patterns used to render each tile. In our case, the two arrays are called `ts0_pals` and `ts0_tmaps`.

> If our game will use just one tileset, the `LABEL` should be `ts0`.

Bank 0, closure
---------------

We won't need anything else on bank 0, so we pad with zeroes, then reset the pool

```
	RESETPOOL
	FILL 4096
```

By now the output binary is 4Kb and contains a whole first bank of CHR-ROM.

Bank 1, preparations
--------------------

As all metadata related to sprites will go into the same file, all sprites are in `ss.png`, and all of them use the same palette `palss0.png`, this section starts with:

```
	OPEN ss.png
	PALS palss0.png
	MAPFILE ..\dev\assets\spritedata.h	
```

Bank 1, main character (Sir Ababol)
-----------------------------------

[[ss0-detalle-player.png]]

Remember that, in **AGNES**, the main character is a 8x16 pixels rectangle. The actual metasprite for Sir Ababol is 16x24 pixels. 

> All cells don't have to be of the same size.

What **AGNES** does is having an offset to know where to place the actual metasprite in regards to the position of the *logical rectangle*, as you can see in this picture:

[[logical-metasprite-offset.png]]

Generally, we want the *logical rectangle* bottom-centered in the metasprite. The six cells used to animate Sir Ababol (1 x *idle*, 4 * *walking*, 1 * *airborne*) share the same offsets and the same size, so we can cut them all in one go using the command `METASPRITESET`, whose syntax is:

```
	METASPRITESET x0, y0, wm, hm, w, h, offs_x, offs_y[, MAX] [FLIPPED]
```

here `x0, y0` are the origin coordinates inside the png and `w, h` are the dimensions of the rectangle we want to cut *in metasprites*. `wm, hm` are the dimmensions of each metasprite *in patterns*.

`FLIPPED` will make **mkts** generate an horizontal flipped version automaticly. Sprite flipping is performed by HW in the PPU, so no extra patterns are stored for the flipped version. 

> The conventios is having every sprite cell facing right, so the generated *flipped* versions are facing left.

So:

```
	LABEL sspl
	METASPRITESET 1, 1, 2, 3, 6, 1, -4, -8 FLIPPED
```

The `-4, -8` offset mean that the metasprite will be drawn 4 pixels left, 8 pixels over the actual coordinates of the *logical rectangle*.

Metadata generated by this command consists in a set of 12 arrays containing the 6 cells we have cut plus their flipped versions. `sspl`will be used as a prefix, and the generated identifiers will have the format:

```
	sspl_XX_F
```

Where `XX` is an index and `F` is `a` for the normal version (by convention, facing right) and `b` for the horizontally flipped version (by convention, facing left). Therefore, the list of generated arrays will have these identifiers:

```c
	sspl_00_a, sspl_00_b, sspl_01_a, sspl_01_b, 
	sspl_02_a, sspl_02_b, sspl_03_a, sspl_03_b, 
	sspl_04_a, sspl_04_b, sspl_05_a, sspl_05_b
```

Next we'll be cutting three extra cells for the swimming animation, which will be used in the **DX** version of the game. In this case, the offset is slightly different: the *logical rectangle* is centered inside the metasprite, so the offset is `-4, -4`:

```
	LABEL sspl2
	METASPRITESET 13, 1, 2, 3, 3, 1, -4, -4 FLIPPED
```

This command will generate six extra arrays as metadata:

```
	sspl2_00_a, sspl2_00_b, 
	sspl2_01_a, sspl2_01_b, 
	sspl2_02_a, sspl2_02_b 
```

Bank 1, enemies
---------------

[[ss0-detalle-enems.png]]

> Generally, in **AGNES**, the metaspriteset for each enemy contains 8 different cells: 4 facing each direction (left, right). Of those 4, the first two are used as the "walking" animation, and the two remaining as an "attack" frame and a "dying" frame. The latter are only used in special configurations. You can even repeat the same cell in several slots if you don't need the enemies facing left or right, or if you don't need animation.

Remember that **AGNES** uses a 16x16 pixels *logical rectangle* for enemies and moving platforms.

We'll start cutting the differently sized enemies using several `METASPRITESET` commands.

First we find two different enemies with two animation cells each, measuring 2x3 patterns (or 16x24 pixels). We won't be generating flipped versions as they are designed to appear to be facing the camera.

```
	LABEL ssena
	METASPRITESET 1, 4, 2, 3, 4, 1, 0, -8
```

The *logical rectangle* is 16x16 and our enemies are 16x24. The metasprite should be painted 8 pixels above the actual coordinates, hence the `0, -8`.

Next enemy, a 24x16 pixels bat, that makes 2x3 patterns:

```
	LABEL ssenb
	METASPRITESET 9, 4, 3, 2, 2, 1, -4, 0
```

Centering the 16x16 *logical rectangle* with the 24x16 metasprite means that we should paint the metasprite 4 pixels to the left of the actual coordinates, hence the `-4, 0`.

Platforms are 2x2 patterns:

```
	LABEL ssplat
	METASPRITESET 15, 4, 2, 2, 2, 1, 0, 0
```

In this case, the *logical rectangle* is the same size of the metasprite, so the offsets are `0, 0`.

Now it's the turn of the extra three kinds of enemies we'll be using for the enhanced **DX** version. The first two won't need flipped versions. The last one, which is a fish facing right, will. So we cut them in two steps. All of them are 16x16:

```
	LABEL ssenc
	METASPRITESET 1, 7, 2, 2, 4, 1, 0, 0

	LABEL ssend
	METASPRITESET 9, 7, 2, 2, 2, 1, 0, 0 FLIPPED
```

Finally, an extra cell for the explosion (also used in **DX**):

```
	LABEL ssexpl
	METASPRITESET 19, 4, 2, 2, 1, 1, 0, 0
```

All those commands will generate quite a few arrays in the metadata file:

```c
	ssena_00, ssena_01, ssena_02, ssena_03, 
	ssenb_00, ssena_01, 
	ssplat_00, ssplat_01,
	ssenc_00, ssenc_01, ssenc_02, ssenc_03, 
	ssend_00_a, ssend_00_b, ssend_01_a, ssend_01_b,
	ssexpl
```

Bank 1, items / extra
---------------------

We'll be cutting 5 metasprites in this section: the Ababol flower (collectible item), a key (used to open locks), a chicken leg (which serves as a life refill), a pair of boots (which will enable the ability to jump on enemies to crush them in **DX**) and a sign (which will be used to show some bits of text to the player, also for **DX**).

Remember that items/extra stuff have a *logical rectangle* of 16x16 pixels, so:

```
	LABEL ssit
	METASPRITESET 1, 9, 2, 2, 5, 1, 0, 0
```

And the generated arrays are:

```
	ssit_00, ssit_01, ssit_02, ssit_03, ssit_04
```

Bank 1, closure
---------------

We just need to pad until the 8K mark:

```
	FILL 8192
```

With this last command, the output binary is exactly 8192 bytes long, with pattern data aligned to two sections of 4096 bytes each.

Running mkts
============

Let's try and run **mkts** to check that everything is alright. Open a command line interpreter window (have you tried [conEmu](https://conemu.github.io/)?), move to the `/gfx/` folder in our project and run:

```batch
	$ ..\utils\mkts.exe mode=scripted in=import_patterns.spt out=..\dev\tileset.chr
```

This will output some text in the console (that's **mkts** working and being verbose; you can prevent that adding `silent` to the end of the command line) and will generate `tileset.chr` in `/dev/` and the metadata files `/dev/assets/tiledata.h` and `/dev/assets/spritedata.h`, which is where **AGNES** expects them.

Let's check if the generated `tileset.chr` is correct by running:

```batch
	$ ..\utils\chrview.exe ..\dev\tileset.chr
```

You should get this:

[[contenido-de-tileset.png]]

> In NROM mode (basic), the file with the CHR-ROM image should be called `tileset.chr`. In CNROM mode, there should be 4 distinct files called `tilesetX.chr` with X ranging from 0 to 3.

-

In the next installment we'll learn how to organize references of all the metasprite arrays in metaspritesets for **AGNES**. Until them you can try to [keep me awake](https://www.buymeacoffee.com/nath) ;-) .
