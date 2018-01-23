// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017

// spritesets

// Player metasprites
const unsigned char * const spr_player [] = {
	// Facing right (offset 0)
	sspl_00_a,										// Standing (offset 0)
	sspl_01_a, sspl_02_a, sspl_03_a, sspl_04_a,		// Walk cycle (offset 1)
	sspl_05_a,										// Airborne (offset 5)
	sspl2_00_a, sspl2_01_a, sspl2_02_a, sspl2_01_a,	// Swim cycle (offset 6)

	// Facing left (offset 10)
	sspl_00_b,
	sspl_01_b, sspl_02_b, sspl_03_b, sspl_04_b,
	sspl_05_b,
	sspl2_00_b, sspl2_01_b, sspl2_02_b, sspl2_01_b
};

// Enemy metasprites
const unsigned char * const spr_enems0 [] = {
	// Linear enems and platforms (ids 1-4)
	ssena_00, ssena_01, ssena_00, ssena_01,			// Enem id 1, right, left, two cells (duplicated)
	ssena_02, ssena_03, ssena_02, ssena_03,			// Enem id 2, right, left, two cells (duplicated)
	ssenb_00, ssenb_01, ssenb_00, ssenb_01,			// Enem id 3, right, left, two cells (duplicated)
	ssplat_00, ssplat_01, ssplat_00, ssplat_01,		// Platform, right, left, two cells (duplicated)

	// Explosion (offset 16)
	ssexpl
};

const unsigned char * const spr_enems1 [] = {
	// Linear enems and platforms (ids 1-4)
	ssenc_00, ssenc_01, ssenc_00, ssenc_01,			// Enem id 1, right, left, two cells (duplicated)
	ssenc_02, ssenc_03, ssenc_02, ssenc_03,			// Enem id 2, right, left, two cells (duplicated)
	ssend_00_a, ssend_01_a, ssend_00_b, ssend_01_b,	// Enem id 3, right, left, two cells 
	ssplat_00, ssplat_01, ssplat_00, ssplat_01,		// Platform, right, left, two cells (duplicated)

	// Explosion (offset 16)
	ssexpl
};

// Item metasprites
// nothing, object, key, refill, boots
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
