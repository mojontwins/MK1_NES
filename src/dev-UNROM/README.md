MK1_NES / AGNES for UNROM-like mappers
======================================

Support UNROM or SNROM (in the future) need some extra tools and several changes. I'll be maintaining this parallel `dev-UNROM` folder which will make use of the modules in `dev` in a new structure.

In UNROM there's no CHR-RAM and 16Kb of PRG-ROM are switchable. We'll target UNROM64 or UNROM128 with a configuration switch. In UNROM64 mode, banks 0 and 1 will store the "library" of assets and banks 2 and 3 will contain code. In UNROM128 mode, banks 0-3 will be the library, banks 4 and 5 will be free (for custom code/data), and banks 6 and 7 will contain code.

One of the main goals will be having this version directly ported to GB or SMS.

See you in v2.0!

~~

I've putting a lil' package of toolchain applications for this, namely:

- eneexp3.exe         - can export enemies as a binary file.
- rle53mapMK1.exe     - can export map, map index, and locks as a binary file.
- mscmk1.exe          - can put all the script pools to a binary file
- UNROM/librarian.exe - creates libraries of binary assets as C files with constants and indexes.
- UNROM/lzsscomp.exe  - used to compress pattern data (or anything that goes to VRAM).

~~

In UNROM (bin) mode, msc needs `locate_res` to page in the correct bank and retrieve the correct pointer in `gp_gen`. The resource is stored as `SCRIPTS_RES`, which must have the correct value at compile time.