// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// spritesets

// Player metasprites
const unsigned char * const spr_player [] = {
	// Facing right (offset 0)
	sspl_00,								// Standing (offset 0)
	sspl_01, sspl_02, sspl_03, sspl_04,		// Walk cycle (offset 1)
	sspl_05,								// Pushing

	// Facing left (offset 6)
	sspl_06,
	sspl_07, sspl_08, sspl_09, sspl_0A,
	sspl_0B,

	// Facing down (offset 12)
	sspl_0C,
	sspl_0D, sspl_0E, sspl_0F, sspl_10,
	sspl_11,

	// Facing up (offset 18)
	sspl_12,
	sspl_13, sspl_14, sspl_15, sspl_16,
	sspl_17,
	
	// Use animation (offset 24)
	sspl_18, sspl_19, sspl_1A, sspl_1B,		// Up to down
	sspl_1C, sspl_1D,						// After this, *interact*
	sspl_1D, sspl_1C, sspl_1B, sspl_1A,		// Down to up
	sspl_19, sspl_18,
	sspl_1E, 								// Final, good pose
	sspl_1F									// Final, wrong pose
};

// Enemy metasprites
// Framesets are right {anim1, anim2, attacking, dying}, right {anim1, anim2, attacking, dying}.
// Not every kind of enemy has every cell, just use 0 when N/A. dying cell will be used only if ENEMS_ENABLE_DYING_FRAME
const unsigned char * const spr_enems0 [] = {
	// Linear enems and platforms (ids 1-4)
	ssen_00_a, ssen_01_a, 0, 0, ssen_00_b, ssen_01_b, 0, 0, // 1
	ssen_02_a, ssen_03_a, 0, 0, ssen_02_b, ssen_03_b, 0, 0, // 2
	ssen_04_a, ssen_05_a, 0, 0, ssen_04_b, ssen_05_b, 0, 0, // 3
	ssen_06_a, ssen_07_a, 0, 0, ssen_06_b, ssen_07_b, 0, 0,	// 4

	// Warp (offset 32)
	sswarp_00, sswarp_01, sswarp_02, sswarp_03
};

// Item metasprites
// nothing, nothing, nothing, refill, nothing, time
const unsigned char * const spr_hs [] = {
	0, 0, 0, ssit_00, 0, ssit_01
};

