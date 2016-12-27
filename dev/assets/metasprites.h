// metasprites.h
// Metaspriteset.

#include "assets/spr_pl.h"
#include "assets/spr_en.h"
#include "assets/spr_hs.h"

// Extra sprites:

const unsigned char spr_empty [] = {
	0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 128
};

const unsigned char spr_pl_empty [] = {
	0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 0, 0, 255, 0, 128
};

// Special: coverup low priority sprite to cover up saws and other stuff
// (Saws have high priority, but are located further on in the OAM than these)
const char spr_black [] = {
        0x00, 0x00, 0xFF, 0x23, 0x08, 0x00, 0xFF, 0x23,
        0x00, 0x08, 0xFF, 0x23, 0x08, 0x08, 0xFF, 0x23,
        0x80	
};

// spritesets

const char *spr_player [] = {
	spr_pl_00, spr_pl_01, spr_pl_02, spr_pl_03, spr_pl_04, spr_pl_05, spr_pl_06, spr_pl_07, 
	spr_pl_08, spr_pl_09, spr_pl_0A, spr_pl_0B, spr_pl_0C, spr_pl_0D, spr_pl_0E, spr_pl_0F
};

const char *spr_enems [] = {
	spr_en_00, spr_en_01, spr_en_02, spr_en_03, spr_en_04, spr_en_05, spr_en_06, spr_en_07, 
	spr_en_08, spr_en_09, spr_en_0A, spr_en_0B, spr_en_0C, spr_en_0D, spr_en_0E, spr_en_0F,
	spr_en_12, spr_en_13, spr_en_14, spr_en_15, spr_en_16, spr_en_17, spr_en_18, spr_en_19
};

/*
	0 Key
	1 Life
	2 Reso off
	3 Reso on
	4 Star
*/
const char *spr_hs [] = {
	spr_hs_00, spr_hs_01, spr_hs_02, spr_hs_03, spr_hs_04
};
