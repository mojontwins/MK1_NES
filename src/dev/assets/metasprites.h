// NES MK1 v0.7
// Copyleft Mojon Twins 2013, 2015, 2016

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
	ssen_00, ssen_01, ssen_00, ssen_01, 			// Enem id 1, right, left, two cells (duplicated)
	ssen_02, ssen_03, ssen_02, ssen_03,				// Enem id 2, right, left, two cells (duplicated)
	ssen_04, ssen_05, ssen_04, ssen_05, 			// Enem id 3, right, left, two cells (duplicated)
	ssen_06, ssen_07, ssen_06, ssen_07,				// Enem id 4, right, left, two cells (duplicated)

	// Explosion (offset 16)
	ssexpl
};

// Item metasprites
// nothing, object, key, refill, ammo
const unsigned char * const spr_hs [] = {
	0, ssit_00, ssit_01, ssit_02, ssit_03
};

// Fixed stuff
const unsigned char spr_pl_empty [] = {
	-4, -8, 0xff, 0, 4, -8, 0xff, 0,
	-4, 0, 0xff, 0, 4, -8, 0xff, 0,
	-4, 8, 0xff, 0, 4, 8, 0xff, 0,
	128
};
