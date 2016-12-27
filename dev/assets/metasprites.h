// NES MK1 v0.7
// Copyleft Mojon Twins 2013, 2015, 2016

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
        0x00, 0x00, 0xFF, 0x20, 0x08, 0x00, 0xFF, 0x20,
        0x00, 0x08, 0xFF, 0x20, 0x08, 0x08, 0xFF, 0x20,
        0x80	
};

// spritesets

const char *spr_player [] = {
	spr_pl_00_a, spr_pl_01_a, spr_pl_02_a, spr_pl_03_a, spr_pl_04_a, spr_pl_05_a, spr_pl_06_a, spr_pl_07_a, 
	spr_pl_08_a, spr_pl_09_a, spr_pl_0A_a, spr_pl_0B_a, spr_pl_0C_a, spr_pl_0D_a, spr_pl_0E_a, spr_pl_0F_a,
	spr_pl_00_b, spr_pl_01_b, spr_pl_02_b, spr_pl_03_b, spr_pl_04_b, spr_pl_05_b, spr_pl_06_b, spr_pl_07_b, 
	spr_pl_08_b, spr_pl_09_b, spr_pl_0A_b, spr_pl_0B_b, spr_pl_0C_b, spr_pl_0D_b, spr_pl_0E_b, spr_pl_0F_b
};

// ID U1 U2 D1 D2 CR RL FI

const char *spr_enems [] = {
	spr_en_00_a, spr_en_01_a, spr_en_00_b, spr_en_01_b,
	spr_en_02_a, spr_en_03_a, spr_en_02_b, spr_en_03_b,
	spr_en_04_a, spr_en_05_a, spr_en_04_b, spr_en_05_b,
	spr_en_06_a, spr_en_07_a, spr_en_06_b, spr_en_07_b,
	spr_en_08_a, spr_en_09_a, spr_en_0C_a, spr_en_0D_a
};

/*
	0 Refill
	1 Object (Coin)
	2 Key
	3 Star
*/
const char *spr_hs [] = {
	spr_hs_00, spr_hs_01, spr_hs_02, spr_hs_03, spr_hs_04, spr_hs_05, spr_hs_06, spr_hs_07, 
	spr_hs_08
};
