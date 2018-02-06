// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017

// spritesets

// Player metasprites
const unsigned char * const spr_player [] = {
	// Facing right (offset 0)
	sspl_00_a,										// Standing (offset 0)
	sspl_01_a, sspl_02_a, sspl_03_a, sspl_04_a,		// Walk cycle (offset 1)
	sspl_05_a, sspl_06_a,							// Airborne, up/down (offset 5, 6)
	0,												// I like powers of two
	

	// Facing left (offset 8)
	sspl_00_b,										// Standing (offset 0)
	sspl_01_b, sspl_02_b, sspl_03_b, sspl_04_b,		// Walk cycle (offset 1)
	sspl_05_b, sspl_06_b,							// Airborne, up/down (offset 5, 6)
	0,												// I like powers of two
	
};

// Enemy metasprites
const unsigned char * const spr_enems0 [] = {
	// Linear enems and platforms (ids 1-4)
	ssen_00_a, ssen_01_a, ssen_00_b, ssen_01_b,			// Enem id 1, right, left, two cells (duplicated)
	ssen_02_a, ssen_03_a, ssen_02_b, ssen_03_b,			// Enem id 2, right, left, two cells (duplicated)
	ssen_04_a, ssen_05_a, ssen_04_b, ssen_05_b,			// Enem id 3, right, left, two cells (duplicated)
	ssplat_00, ssplat_01, ssplat_00, ssplat_01,		// Platform, right, left, two cells (duplicated)

	// Explosion (offset 16)
	ssexpl
};

// Item metasprites
// nothing, object, key, refill
const unsigned char * const spr_hs [] = {
	0, ssit_00, ssit_01, ssit_02
};

// Fixed stuff
const unsigned char spr_pl_empty [] = {
#ifdef TALL_PLAYER
	-4, -16, 0xff, 0, 4, -16, 0xff, 0,
#endif	
	-4, -8, 0xff, 0, 4, -8, 0xff, 0,
	-4, 0, 0xff, 0, 4, 0, 0xff, 0,
	-4, 8, 0xff, 0, 4, 8, 0xff, 0,
	128
};
