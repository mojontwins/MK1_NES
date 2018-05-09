The Tools
=========

There's a good bunch of useful tools in the package. I've divided them into two groups: external tools and internal tools. **MK1 NES/AGNES** is built around legacy versions of the compiler **cc65** and the library **neslib** (one of the future milestones to reach is to port the framework to a modern **cc65** / **neslib** combo). We have opted for including all you need to develop games in this package.

External Tools
==============

cc65 v2.13
----------

The latest official version released of this compiler prior to the current fork. The original **neslib** was written with this version in mind. In future releases, **MK1 NES/AGNES** will be adapted to use the most recent **cc65** version and the latest **neslib**. There are several features in **neslib** which have changed drastically since **MK1 NES/AGNES** was designed, specially relating to the data structure used when updating VRAM during VBLANK. That's why we are using the old **cc65** / **neslib** combo.

**cc65** resides in `/cc65_2.13.2/bin/`. In order for the compiler to work, this path should be added to the beginning of the *PATH* system variable. There are several ways to achieve this - I prefer the *portable* approach of having a .bat file I run at the beginning of every coding session:

```
	@echo off
	set path=c:\AGNES\cc65_2.13.2\bin;%path%
	cc65 --version
```

(this example assumes you instaled AGNES in c:\AGNES).

The last line, `cc65 --version`, should print the compiler version in the command line - just to test everything went all right.

Famitracker 0.4.6
-----------------

We are using **famitone2** for sound, which was designed to work alongside Famitracker 0.4.6. That's why we have added a copy of this version in `/ext_utils/Famitracker0.4.6`.

Mappy (Mapwin)
--------------

A good old tiny app useful to design maps. We have added a customized version in `/ext_utils/Mappy/` mainly 'cause the download link in the [official web page](https://tilemap.co.uk/mappy.php) hasn't worked for a long time.

Nes Screen Tool 1.43
--------------------

This version of **NESst** is rather old, but should be more than enough for what we need it: to convert raw nametables to RLE-compressed data directly usable by **neslib**.

In future versions of **MK1 NES/AGNES** a special command line tool to compress nametables will be included.

Sublime Addons
--------------

If you use Sublime Text 3, there's a simple addon to provide syntax highlighting for **MK1 NES/AGNES** script files.

Internal Tools
==============

Most of them are command line based, *toolchain* ready tools created inhouse:

Binary Paster binpaster.exe
---------------------------

Pastes a list of binary files together and generates an index in code. Used for example to store data in CHR-ROM and have a readily available index.

CHR Viewer chrview.exe
----------------------

CHR-ROM visualizer. Opens a window showing the patterns in a CHR-ROM binary image. 

ene Exporter 3 eneexp3.exe
---------------------------

Converts .ene files with enemy and hotspots placement to code directly usable by the engine, or binary files directly usable from CHR-ROM.

Zero filler fillto.exe
----------------------

Let's you resize a binary file, truncating it or padding it with zeroes to any size.

Map Converter NES 2 mapcnvnes2.exe
----------------------------------

Map converter. Takes **Mappy** .MAP files and exports code. Generates maps in unpacked (1 byte per tile, 256 different tiles, 192 bytes per screen) or packed (2 tiles per byte, 16 different tiles, 96 bytes per screen) formats. More info in the chapter about maps. 

Map Converter in RLE53 rle53mapMK1.exe y rle53mapcchrrom.exe
------------------------------------------------------------

Map converter. The output format is RLE53, which uses runs of 3 bits (1 to 8) of 5 bits values (so the maximum is 32 different tiles). This is the optimum format in most cases. More info in the chapter about maps. 

`rle53mapMK1.exe` generates PRG-ROM ready code files,  `rle53mpachrrom.exe` generates CHR-ROM ready binary files plus an index.

Map Converter in RLE legacy rlemap2.exe
---------------------------------------

Map converter. The output format is RLE16, which produces fairly good results for maps which use mainly 16 tiles. More info in the chapter about maps.  

Graphics importer mkts.exe
--------------------------

Graphics converter and importer. Generates patterns for CHR-ROM and metadata. Used to convert charsets, tilesets, or spritesets. Fairly complex and powerful, we will devote a whole chapter to this tool.

Nametable generator namgen.exe
------------------------------

Generates a 1024 bytes nametable + attributes from a .png file, a palette file, and a CHR-ROM image. The .nam file generated can be loaded into **NES Screen tool** for fine-tuning and exporting a RLE-compressed array representation of the image.

Enemy Compiler pencompiler.exe
------------------------------

Compiles enemy behaviour scripts used with *compiled* enemies.

Enemy/hotspots placement editor ponedor.exe
-------------------------------------------

Graphical interface to place enemies and hotspots in the map. Generates .ene files.

Importador de textos text2carray.exe
------------------------------------

Just generates C arrays from text files.

-

In the next chapter we will start creating a new game in **MK1_NES/AGNES** step by step: we will build a port of the ZX Spectrum game **Sir Ababol DX**. Until then, [remember that I love coffee](https://www.buymeacoffee.com/nath) ;-)
