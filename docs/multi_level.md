# Multiple level support

This document shortly describes how AGNES handles multiple levels. For examples you can check [Cheril Perils Classic](https://github.com/mojontwins/MK1_NES/tree/master/examples/07_cheril_perils_classic), [Cheril the Writer](https://github.com/mojontwins/MK1_NES/tree/master/examples/08_cheril_the_writer), [Che Man](https://github.com/mojontwins/MK1_NES/tree/master/examples/10_che_man), [Bootèe](https://github.com/mojontwins/MK1_NES/tree/master/examples/11_boot%C3%A8e) or [Espitene](https://github.com/mojontwins/MK1_NES/tree/master/examples/12_espitene).

For a better understanding of this document, be sure to understand most of what's written [here](https://github.com/mojontwins/MK1_NES/blob/master/docs/features.md), [here](https://github.com/mojontwins/MK1_NES/blob/master/docs/engine_config.md) and [here](https://github.com/mojontwins/MK1_NES/blob/master/docs/mkts.md).

## Levels in AGNES

A Level is AGNES is a collection of related assets and parameter values as defined in `assets/levelset.h` in a set of arrays. Arrays are arranged so element N in the arrays correspond to the values and assets for level N. The arrays are:

### Initial screen `l_scr_ini` and Initial coordinates (X, Y) `l_ini_x` / `l_ini_y`.

Those values tell the engine where to place the main character when the level begins.

### Level map size `l_map_w` and `l_map_h`.

The size of the map for each level in the game, **in screens**.

### Number of objects `l_player_max_objects`

The number of type 1 items in each level. When a .ene file is exported, the exporter `eneexp3` creates `#define`s to help you fill this array. The amount of type 1 hotspots in a level exported with *prefix* `N` would be `MAX_HOTSPOTS_TYPE_1_N`.

### Number of killable enemies `l_player_max_enems`

The total amount of killable enemies for each level. The exporter `eneexp3` creates `#defines` for them as well: for *prefix* `N` it would be `KILLABLE_ENEMS_N`.

### Background and foreground palettes `l_pal_bg` and `l_pal_fg`.

Which palette to use for background and foreground for each level.

### Metatile definitions `l_ts_tmaps`, `l_ts_pals` and `l_behs`.

Here you must reference the tilemap and palette arrays corresponding to the metatileset for each level, plus the behaviours array (which you must create by hand in `assets/behs.h`).

### Metaspriteset for enemies `l_spr_enems`.

Which `spr_enems*` array (defined in `assets/metasprites.h` to use for each level.  

### Map data `l_map` / `l_map_chr_rom_bank`. 

Select which map array / address to use for each level. Note that there are several sets of arrays - you must fill that which corresponds to your map format / location.

### Decorations `l_decos`

Fill in this section if your maps uses decorations. The array contains an 1 for *there are decorations in this level* or 0 if there are not. Note that the format is completely different depending if you are using `MAP_FORMAT_PACKED` or `MAP_FORMAT_RLE16` (the legacy formats) - in this case this is an array of pointers. Most likely you won't be using legacy map formats so you may kindly ignore this.

### Locks `l_locks`, `l_n_bolts`.

If your game uses keys and locks, `l_locks` is an array of references to lock arrays (as exported alongside map data by the map exporters with the name `map_N_locks`, where `N` is the *prefix* you fed to the exporter) and `l_n_bolts` contains the total number of locks in the level (`#define`d as `N_BOLTS_N` by the exporters).

### Enemies and hotspots `l_enems`, `l_hotspots` / `l_enems_chr_rombank`

If your enemies are in PRG-ROM (default), `l_enems` and `l_hotspots` are arrays referencing the `enems_N` and `hotspots_N` arrays exported by `eneexp3`.

If your enemies are in CHR-ROM (`ENEMS_IN_CHRROM`), `l_enems` and `l_hotspots` are arrays of CHR-ROM addresses. You can use the `ENEMSN_H_BIN_OFFS` and `HOTSPOTS_OFFSET_N` `#define`s, and you must properly substitue `base_address` in the code for the real address where the data begins in your CHR-ROM. 

### CHR-ROM bank, `l_chr_rom_bank`

Which CHR-ROM bank to page in for graphics for each level.

### Interactives, `l_interactives`

Array of references to the interactives arrays for each level. Interactives arrays are created in `assets/interactives.h`.

### Music, `l_music`

Which music track to use in each level.

## How to add levels

Here's a short checklist to add levels.

### Level assets must be available

You have to export your level data (`.map` and `.ene` files) and make the resulting code available in your source tree and `#include`d from `game.c`. We normally store the files in `assets`, as you may check in all the examples.

### Reference level assets & values from the arrays in `assets/levelset.h`.

As explained.

### It is good practice to tell the engine

Give a proper value to `MAX_LEVELS` in `config.h`.

## This should be obvious

If your game has 4 levels, all the arrays should have 4 items. 
