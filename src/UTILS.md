MK1_NES / AGNES Utilities
=========================

This directory contains most of the toolchain used to convert and prepare data for games created using MK1_NES / AGNES, including source code, Windows and Linux executables (for example, for the **mkts** utility, `mkts.bas` has the sources, `mkts.exe` is the Windows executable, and `mkts` is the Linux executable).

- The root directory contains tools currently used in the toolchain (as of v1.0).
- The legacy directory contains tools used in old versions of the engine. They are there so examples 1-6 can be compiled.
- The UNROM directory contains tools which will be used in the future UNROM version of MK1_NES / AGNES.

Current toolchain
=================

binpaster
---------

Glues a lot of binary files together and creates C code with an index of offsets to the beginning of each binary. Useful to stuff data in CHR-ROM, for example.

```
	$ binpaster [pad_to=XXXX] index=index.h out=out.bin files=file1,file2,...
```

`index` contains the path of the (output) index file, `out` the path to the output binary, and `files` a list of paths to the files you want to glue together. `pad_to` will fill with zeroes to the specified file size, can be omitted if you don't need padding.

chrview
-------

Shows a `tileset.chr` (a CHR-ROM image) on a graphical window. It can save the image to a png file (pressing S). You can hover over the patterns to know the pattern number.

```
	$ chrview tileset.chr
```

eneexp3
-------

Generates C code from a .ene file. .ene files contain level object placement (enemies and hotspots).

```
	$ eneexp3 enems.ene out.h prefix [yadjust] [nohotspots] [gencounter] [genallcounters] [bin]
```

* `enems.ene` is the input file.
* `out.h` is the generated C code you have to add to your game.
* `prefix` is a text prefix used to generate labels in the code.
* `yadjust` is a **value**. If present, all Y tile values are incremented by such value. MK1_NES / AGNES requires a value of **1**
* `nohotspots` don't generate code for hotspots, even if hotspots are found in the input file.
* `gencounter` generate a counter `KILLABLE_ENEMS_<prefix>` of killable enemies (just adds the amount of type 0x01, 0x02, 0x03, 0x41, 0x42, 0x43, 0x81, 0x82, 0x83, 0x06, 0x09, 0x0B and 0x14 enemies found).
* Normally, the tool generates a counter `MAX_ENEMS_TYPE_<N>_<PREFIX>` for each time of enemy found. With `genallcounters`, a counter for each type < 32 will be generated, no matter if it's 0.
* `bin` makes the tool work in binary mode. Enemy and hotspots data is output to a binary file `out.h.bin`. Offsets and constants are output to `out.h`. Useful if you want to stuff your object placement data in `CHR-ROM`.

fillto
------

Pads a binary file with zeroes until it gets to the specified size.

```
	$ fillto file.bin size
```

mapcnvnes2
----------

Converts Mappy's raw .MAP files to data using legacy formats (packed: 2 tiles per byte; unpacked: 1 tile per byte). Outputs decorations, if present and active, in the old format (as a separate entity with its own index).

* packed maps are directly usable by MK1_NES / AGNES.
* unpacked maps have been used in custom renderers for a couple of examples.

```
	$ mapcnvnes2 in.map out.h w h tlock prefix [offset] [packed]
```

* `in.map` is the raw output of Mappy.
* `out.h` is the output filename with the map data.
* `w` and `h` are the map dimmensions in screens.
* `tlock` which tile to detect as "lock". Use an out of bounds value if you don't want the tool to detect locks.
* `prefix` is a text prefix used to generate labels in the code.
* `offset` is a **value** which gets substracted from the value of each tile read from `in.map`. If metatile 0 is not a black square, Mappy will add a black square at the beginning of the tileset and offset all metatiles by one unit. If so, use "1" as offset when converting.
* `packed` makes the output be in the `PACKED` format, otherwise the output is `UNPACKED`.

mkts
----

Graphics cutter and importer. Too complex to describe here. Look in the tutorials when available.

mscmk1
------

Scripts compiler. Reads a custom MK1_NES / AGNES script and generates a bytecode an a specified interpreter.

```
	$ mscmk1 input.spt output.h maxpants [debug] [bin]
```

* `input.spt` is the input script.
* `output.h` is the output file with the interpreter and with the bytecode (not in bin mode).
* `maxpants` is a **value** and should be the same as `MAP_W*MAP_H` in `config.h`.
* `bin` activates the bin mode. No use (yet).

namgen
------

Converts a png file into a nametable using an already defined `tileset.chr` (CHR-ROM image)

```
	$ namgen in=screen.png out=file.nam pals=pals.png chr=tileset.chr [bank=B]
```

* `in` - the input filename. Should be 256x240 png.
* `out` - the output filename, a 1024 bytes nametable.
* `pals` - the palette image. Should be a 16x1 png with all the bg palette entries. Pick the colours from `fullnespal.png`!
* `chr` - the CHR-ROM image file with the patterns.
* `bank=B` - uses the second bank in the CHR-ROM image instead of the first in the conversion.

pencompiler
-----------

Compiles programable enemy behaviours.

```
	$ pencompiler in.spt out.h
```

* `in.spt` is the filename of the script with the enemy behaviours.
* `out.h` is the output filename for the generated C code.

ponedor
-------

Simple GUI driven utility to build and edit .ene files (i.e. - to place enemies and hotspots on your level).  Too complex to describe here. Look in the tutorials when available.

rle44mapMK1 & rle54mapMK1
-------------------------

Map converters. Convert Mappy's raw .MAP files to data using RLE-like algorithms (4 bit runs of 4 bit tile indexes, or 3 bit runs of 5 bit tile indexes). Outputs decorations, if present and active, in the new format (appended to each screen's RLE string).

```
	$ rle44mapMK1 in.map out.h w h tlock prefix [offset] [scrsizes] [nodecos] [bin]
	$ rle53mapMK1 in.map out.h w h tlock prefix [offset] [scrsizes] [nodecos] [bin]
```

* `in.map` is the raw output of Mappy.
* `out.h` output filename.
* `w` and `h` are the map dimensions in screens.
* `tlock` which tile to detect as "lock". Use an out of bounds value if you don't want the tool to detect locks.
* `prefix` is a text prefix used to generate labels in the code.
* `offset` is a **value** which gets substracted from the value of each tile read from `in.map`. If metatile 0 is not a black square, Mappy will add a black square at the beginning of the tileset and offset all metatiles by one unit. If so, use "1" as offset when converting.
* `nodecos` makes the converter to completely ignore decorations.
* `bin` activates bin mode. In bin mode, `out.h` will contain constants and indexes, and `out.h.bin` a binary with the map data.

rle44mapchrrom & rle53mapchrrom
-------------------------------

Map converters. Convert Mappy's raw .MAP files to data using RLE-like algorithms (4 bit runs of 4 bit tile indexes, or 3 bit runs of 5 bit tile indexes). Outputs decorations, if present and active, in the new format (appended to each screen's RLE string).

These map converters take a list of maps with parameters and attempt to generate a series of CHR-ROM images and an index. This is used to put maps in CHR-ROM.

```
	$ rle44mapchrrom in=list.txt bin=chr.bin out=map.h chr=1
	$ rle53mapchrrom in=list.txt bin=chr.bin out=map.h chr=1
```

* `in` is a list of map filenames and parameters, see below.
* `bin` is an output filename **prefix**.
* `out` is an output filename to generate code for the indexes and constants.
* `chr` the index of the first CHR-ROM bank generated.

This is designed to be used with CNROM type mappers. It fills complete 8K binaries with the converted map data and generates an index. The amount of generated banks depends of course in the size of the generated data.

The order in list.txt is respected. The format of each entry in list.txt is:

```
	filename.map,w,h,l0[-...][,fixmappy]
```

* `filename.map` is the raw output of Mappy.
* `w` and `h` are the map dimensions in screens.
* `l0[-...]` is a list of lock tiles. If theres more than one use `-` to separate.
* `fixmappy` substracts 1 from each tile read. If metatile 0 is not a black square, Mappy will add a black square at the beginning of the tileset and offset all metatiles by one unit.

Legacy utilities that may be of some use
========================================

binsubsts
---------

A binary substitutor. Substitutes byte N for byte M. Takes three parameters:

```
	$ binsubsts in.bin out.bin substs.txt
```

`substs.txt` contains a list of numbers. In the process, byte N of `in.bin` is substituted by value #N in the list. The results are written to `out.bin`.

(...)