// metasprites.h
// Metaspriteset.

#include "assets/spr_pla.h"
#include "assets/spr_plb.h"
#include "assets/spr_lin.h"
#include "assets/spr_enf.h"
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
	spr_pla_00_a, spr_pla_01_a, spr_pla_02_a, spr_pla_03_a, spr_pla_04_a, spr_pla_05_a, spr_pla_06_a, spr_pla_07_a, 
	spr_pla_08_a, spr_pla_09_a, spr_pla_0A_a, spr_pla_0B_a, spr_pla_0C_a, spr_pla_0D_a, spr_pla_0E_a, spr_pla_0F_a,
	spr_pla_00_b, spr_pla_01_b, spr_pla_02_b, spr_pla_03_b, spr_pla_04_b, spr_pla_05_b, spr_pla_06_b, spr_pla_07_b, 
	spr_pla_08_b, spr_pla_09_b, spr_pla_0A_b, spr_pla_0B_b, spr_pla_0C_b, spr_pla_0D_b, spr_pla_0E_b, spr_pla_0F_b,
	spr_plb_00, spr_plb_01, spr_plb_02, spr_plb_03, spr_plb_04, spr_plb_05, spr_plb_06, spr_plb_07
};

const unsigned char rinse_cycle [] = { 5, 6, 7, 6 };

// ID R1 R2 R3 R4 F1 F2 F3
// U1 U2 U3 U4 U5 U6 HI JP

// CUSTOM {
// 0xAB, A = cell offset (add to 0x20), B = frame counter; 0 means 1 second, anything else 1 frame.
const unsigned char tt_anim [] = {
	0xff, // dummy to save code.
	0x00, 0x10,
	0x2f, 0x3e, 0x4d, 0x1c,
	0x2b, 0x3a, 0x49, 0x18,
	0x28, 0x38, 0x47, 0x17,
	0x26, 0x36, 0x45, 0x15,
	0x24, 0x34, 0x43, 0x13,
	0x22, 0x32, 0x42, 0x12,
	0x21, 0x31, 0x41, 0x11,
	0x21, 0x31, 0x41, 0x11,
	0x21, 0x31, 0x41,
	0x54, 0x62, 0x72,
	0xff
};
// } END_OF_CUSTOM

const char *spr_enems [] = {
	spr_lin_00_a, spr_lin_01_a, spr_lin_00_b, spr_lin_01_b,
	spr_lin_02_a, spr_lin_03_a, spr_lin_02_b, spr_lin_03_b,
	spr_lin_04_a, spr_lin_05_a, spr_lin_04_b, spr_lin_05_b,
	spr_lin_06_a, spr_lin_07_a, spr_lin_06_b, spr_lin_07_b,
	spr_enf_00, spr_enf_01, spr_enf_02, spr_enf_03, spr_enf_04, spr_enf_05
};

/*
	0 Obj A
	1 Obj B
	2 Key
	3 Refill
*/
const char *spr_hs [] = {
	spr_hs_00, spr_hs_01, spr_hs_02, spr_hs_03, spr_hs_04, spr_hs_05, spr_hs_06, spr_hs_07
};
