<img src="http://www.mojontwins.com/dev/wp-content/uploads/2018/04/bleg.jpg" alt="" width="953" height="269" class="aligncenter size-full wp-image-57" />

Before we start designing a game for a retro platform it is necessary to know how it works in the inside. At least, a bare minimum. We are not going to dive a lot in this short article (if you are interested, there's plenty of information as the **NES** is one of the best documented retro platforms), but you should know and understand some key concepts about how the system works.

*Disclaimer: some information contained here is simplified. For more accurate / correct information, it's advised to read [the Nesdev wiki](https://wiki.nesdev.com/) throroughly*

The **NES** has two main characteristics:

- It has two separate address spaces: one for the CPU, and one for the PPU (which is the processor which is in charge of the display). This means that the CPU can't access the PPU memory directly.
- The system is not complete until you plug in a cartridge.

While this is true for many cartridge based game consoles, those of you who have used computers may find this quite different to what you are used. In a computer you normally have *plenty* of RAM, and programs are loaded onto it from an external peripheral. In the **NES** (and other game consoles), as you plug in a cartridge, you are actually completing the system with the memory which contains the software.

Main components
===============

Inside the **console** we can find:

- The **CPU**, which contains a custom processor based in the 6502 plus the audio hardware.
- The **PPU**, which is in charge of generating the video display. This chip contains several registers so the **CPU** can communicate with it plus 256 bytes of memory where sprite related information is stored.
- 2Kb of **RAM** in the CPU address space, mapped from $0000 to $07FF.
- 2Kb of **VRAM** in the PPU address space, mapped from $2000 to $27FF, and mirrored from $2800 to $2FFF.
- 32 bytes of palette data in the PPU address space, from $3F00 and $3F1F, mirrored many times.
- Extra circuitry in charge of I/O and stuff like that.

Inside the **cartridge** there is:

- **PRG-ROM**: 32K of ROM in the CPU address space, mapped from $C000 to $FFFF. This memory contains the program and static data.
- **CHR-ROM**: 8K of ROM in teh PPU address space, mapped from $0000 to $1FFF, containing two tables of *patterns*, which are tiny 8x8 pixels bits of graphics the PPU uses to draw backgrounds and sprites. *patterns* are 2 bit depth, meaning they can use 4 colours (in fact, 3 plus a the transparent colour).
- ... or **CHR-RAM**: some carts contain a RAM chip instead of CHR-ROM, meaning that the program stored in PRG-ROM must write the patterns itself - this is how most 8 and 16 bits game consoles work, by the way.

32K + 8K may seem limited. We have all seen NES games which are much bigger. Those games have extra circuitry in the cart, called the *mapper* (which can be as simple as a bare counter or as complex as a dedicated ASIC), which scans the bus for several kinds of signals (generally, writes to the ROM space) and "changes" what the CPU or PPU "see" as "connected". This is called *paging*. Note that neither the CPU nor the PPU actually notice what just happened when the *mapper* changes which portion of memory is *paged in*. 

The most simple roms are those which don't contain any extra circuitry (i.e. don't contain a *mapper*). Those are known as NROM. From those who contain a *mapper*, the most simple are those known as CNROM type (which contain hardware to change the whole 8K of CHR-ROM) and GNROM type (which, besides, contain hardware to change the whole 32K of PRG-ROM). Another interesting mapper we'll mention in the tutorial is UNROM (with hardware to change 16 from the 32K and CHR-RAM instead of CHR-ROM).

How graphics work
=================

The image is composed by:

- A background, or *nametable*, which is a grid of 32x30 cells. Each cell contains a 8x8 pixels *pattern* which uses 3 colours + transparent. Each group of 2x2 cells (a macrocell in a 16x15 subgrid) can be shown using one of four background palettes.
- The sprite, or *object* layer. Each sprite is an entity which can be placed anywhere in the 256x240 pixels screen and which will show a *pattern* (or two, depending on a global PPU configuration) using 3 colours + transparent using any of the four sprite palettes. Besides, each sprite has a number of attributes so the HW knows if the pattern should be horizontally or vertically flipped, of if it should be drawn *behind* the background.

Note that:

- *Behind* the background, that is, through the transparent pixels in the patterns placed in the nametable, you will see the global background colour (which is the first colour in the palette), or the sprites configured to be rendered behind the background.
- Sprites can be 8x8 or 8x16 depending on a global PPU configuration (i.e. *all* sprites will be 8x8 OR *all* sprites will be 8x16). Each configuration has its pros and cons.
- The hardware can only display 8 sprites in each pixel line. The 9th and following sprites won't show.

Palettes
--------

The system supports 8 different palettes with 3 colours each plus a global background colour (25 colours in total) (this is not 100% true, but it's all you need to know if you are drawing graphics for the NES). The first 4 palettes are used in the background, and the 4 remaining for the sprites.

Colours come from a table of 64 entries which looks like this:

<img src="http://www.mojontwins.com/dev/wp-content/uploads/2018/04/fullnespal.png" alt="" width="128" height="32" class="aligncenter size-full wp-image-42" />

Note I've written "looks like". No NES palette you find in the internet is 100% accurate as every piece of hardware tends to render slightly different colours.

Pattern tables
--------------

As mentioned, *patterns* are the tiny 8x8 pixel, 4 colours bits of graphics used to build backgrounds and render sprites. Patterns are organized in two tables of 256 patterns each. Patterns may be in RAM (in a so called CHR-RAM chip) or ROM (CHR-ROM, most commonly), depending on the kind of cart or mapper used. Hardware lets you choose which pattern to use for backgrounds and which for sprites. You can even use the same table for both backgrounds and sprites.

Nametables
----------

Each *nametable* takes 1 Kb. The first 960 bytes contain the 960 patterns which fit in the grid of 32x30 cells. The last 64 bytes are known as *attributes*. Each attributes contains information about which background palette to use in 4 macrocells of 2x2 cells (or 16x16 pixels). This is the reason why you can only assign a palette as per 16x16 block.

NES hardware supports up to 4 different nametables, mapped in the PPU address space from $2000 to $23FF, $2400 to $27FF, $2800 to $2BFF and $2C00 to $2FFF. Nevertheless, as mentioned, the system only contains 2Kb of VRAM to store *nametables*, which means only *two* nametables can be stored.

Luckily, the hardware allows the cartridge to somehow choose which 2 of the 4 possible nametables actually exist by configuring how VRAM is *mirrored* in the address space. To make things simple, let's say the two nametables can be arranged vertically or horizontally. The two missing nametables are copies (more precisely, *mirroris*) of those who are present. That's why the vertical arrangement is called *horizontal mirroring* and vice-versa.

You can learn more about mirroing in the [Nesdev wiki](https://wiki.nesdev.com/w/index.php/Mirroring).

Scroll
------

**MK1_NES/AGNES** games don't scroll, but it's useful to know how it works. The 4 nametables are set up in a 2x2 arrangement. The image shown on screen is a 256x240 pixels window to such 512x480 area. The position of the window is determined by the *scroll coordinates*. The arrangement is circular: if the window exits from one side, it enters from the opposite.

Gosh, what a mess
=================

I know this is a bunch of information, but I wanted to make some concepts clear before starting the tutorial. In sum:

- PRG-ROM: where program and data is stored.
- CHR-ROM: where patterns are stored.
- CHR-RAM: here be patterns also, but written from PRG-ROM. Used with several mappers.
- Pattern table: a table containing 256 patters. The system uses two.
- Nametable: A grid of 32x30 patterns and 64 related attributes used to render the background.
- Sprite: the bits which move around.

In the next entry we'll start to talk about **MK1_NES/AGNES**, I promise ;-)

(header picture from http://blipretro.com/cleaning-famiclone-carts/)
