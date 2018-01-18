// NES MK1 v0.7
// Copyleft Mojon Twins 2013, 2015, 2016

// spritesets

// Player metasprites
const unsigned char *spr_player [] = {
	// Facing right (offset 0)
	sspl_00_a,										// Standing (offset 0)
	sspl_01_a, sspl_02_a, sspl_03_a, sspl_04_a,		// Walk cycle (offset 1)
	sspl_05_a,										// Airborne (offset 5)
	sspl_06_a, sspl_07_a, sspl_08_a, sspl_07_a,		// Swim cycle (offset 6)

	// Facing left (offset 10)
	sspl_00_b,
	sspl_01_b, sspl_02_b, sspl_03_b, sspl_04_b,
	sspl_05_b,
	sspl_06_b, sspl_07_b, sspl_08_b, sspl_07_b,	
};

// Enemy metasprites
const unsigned char *spr_enems [] = {
	// Linear enems and platforms (ids 1-4)
	ssena_00, ssena_01, ssena_00, ssena_01,			// Enem id 1, right, left, two cells (duplicated)
	ssena_02, ssena_03, ssena_02, ssena_03,			// Enem id 2, right, left, two cells (duplicated)
	ssenb_00, ssenb_01, ssenb_00, ssenb_01,			// Enem id 3, right, left, two cells (duplicated)
	ssplat_00, ssplat_01, ssplat_00, ssplat_01,		// Platform, right, left, two cells (duplicated)

	// Explosion (offset 16)
	ssexpl
};

// Item metasprites
// nothing, object, key, refill, boots
const char *spr_hs [] = {
	0, ssit_00, ssit_01, ssit_02, ssit_03
};

// Fixed stuff
const unsigned char spr_pl_empty [] = {
	-4, -8, 0xff, 0, 4, -8, 0xff, 0,
	-4, 0, 0xff, 0, 4, -8, 0xff, 0,
	-4, 8, 0xff, 0, 4, 8, 0xff, 0,
	128
};
