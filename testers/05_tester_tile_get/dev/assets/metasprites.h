// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// spritesets

// Player metasprites
const unsigned char * const spr_player [] = {
	// Facing right (offset 0)
	sspl_01_a,										// Standing (offset 0)
	sspl_01_a, sspl_02_a, sspl_01_a, sspl_00_a,		// Walk cycle (offset 1)
	sspl_04, sspl_05, sspl_04, sspl_06,				// Spin
	sspl_03_a,										// Hit
	

	// Facing left (offset 10)
	sspl_01_b,										// Standing (offset 0)
	sspl_01_b, sspl_02_b, sspl_01_b, sspl_00_b,		// Walk cycle (offset 1)
	sspl_04, sspl_05, sspl_04, sspl_06,				// Spin
	sspl_03_b,										// Hit
	
};

// Enemy metasprites
// Framesets are right {anim1, anim2, attacking, dying}, right {anim1, anim2, attacking, dying}.
// Not every kind of enemy has every cell, just use 0 when N/A. dying cell will be used only if ENEMS_ENABLE_DYING_FRAME
const unsigned char * const spr_enems0 [] = {
	// Linear enems and platforms (ids 1-4)
	ssen_00_a, ssen_01_a, 0, 0, ssen_00_b, ssen_01_b, 0, 0, // 1
	ssen_02_a, ssen_03_a, 0, 0, ssen_02_b, ssen_03_b, 0, 0, // 2
	ssen_04_a, ssen_05_a, 0, 0, ssen_04_b, ssen_05_b, 0, 0, // 3
	ssplat_00, ssplat_01, 0, 0, ssplat_00, ssplat_01, 0, 0, // Platform (4)

	// Explosion (offset 32)
	ssexpl_00, 

	// Monococo (type A) (offset 33)
	ssmc_00_a, ssmc_01_a, ssmc_00_b, ssmc_01_b, 
};

// Item metasprites
// nothing, object, key, refill
const unsigned char * const spr_hs [] = {
	0, ssit_00, ssit_01, ssit_02
};

