// NES MK1 v2.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// spritesets

// Player metasprites
const unsigned char * const spr_player [] = {
	sspl_00, sspl_01,	// Right, 2 cells
	sspl_02, sspl_03,	// Left, 2 cells
	sspl_04, sspl_05,	// Up, 2 cells
	sspl_06, sspl_07	// Down, 2 cells
};

// Enemy metasprites
const unsigned char * const spr_enems0 [] = {
	// Linear enems and platforms (ids 1-4)
	ssen_00, ssen_01, 0, 0, ssen_00, ssen_01, 0, 0, // 1
	ssen_02, ssen_03, 0, 0, ssen_02, ssen_03, 0, 0, // 2
	ssen_04, ssen_05, 0, 0, ssen_04, ssen_05, 0, 0, // 3
	ssen_06, ssen_07, 0, 0, ssen_06, ssen_07, 0, 0,	// 4

	// Explosion (offset 32)
	ssexpl
};

// Item metasprites
// nothing, object, key, refill, ammo
const unsigned char * const spr_hs [] = {
	0, ssit_00, ssit_01, ssit_02, ssit_03
};

