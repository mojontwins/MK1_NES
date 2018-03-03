// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// spritesets

// Player metasprites
const unsigned char * const spr_player [] = {
	// Facing right (offset 0)
	sspl_00_a,										// Standing (offset 0)
	sspl_01_a, sspl_02_a, sspl_03_a, sspl_04_a,		// Walk cycle (offset 1)
	sspl_05_a, sspl_06_a,							// Airborne, up/down (offset 5, 6)
	
	// Animation (offset 8, with facing!)
	sspl_00_a, sspl_09_a, sspl_0A_a, sspl_0B_a,		// Action here!
	sspl_0B_a, sspl_0A_a, sspl_09_a, 
	sspl_08_a, sspl_08_a,							// Last two cells: OK, NO_OK
	
	// Facing left (offset 16)
	sspl_00_b,										// Standing (offset 0)
	sspl_01_b, sspl_02_b, sspl_03_b, sspl_04_b,		// Walk cycle (offset 1)
	sspl_05_b, sspl_06_b,							// Airborne, up/down (offset 5, 6)
	
	// Animation (offset 8, with facing!)
	sspl_00_b, sspl_09_b, sspl_0A_b, sspl_0B_b,		// Action here!
	sspl_0B_b, sspl_0A_b, sspl_09_b, 
	sspl_08_b, sspl_08_b,							// Last two cells: OK, NO_OK
	
};

// Enemy metasprites
// Framesets are right {anim1, anim2, attacking, dying}, right {anim1, anim2, attacking, dying}.
// Not every kind of enemy has every cell, just use 0 when N/A. dying cell will be used only if ENEMS_ENABLE_DYING_FRAME
const unsigned char * const spr_enems0 [] = {
	// Linear enems and platforms (ids 1-4)
	ssen_00_a, ssen_01_a, 0, 0, ssen_00_b, ssen_01_b, 0, 0, // 1
	ssen_02_a, ssen_03_a, 0, 0, ssen_02_b, ssen_03_b, 0, 0, // 2
	ssen_04_a, ssen_05_a, 0, 0, ssen_04_b, ssen_05_b, 0, 0, // 3
	ssplat_00, ssplat_01, 0, 0, ssplat_00, ssplat_01, 0, 0,	// Platform (4)
};

// Item metasprites
// EMPTY, object, key, refill
// potion, eye, skull, puff
const unsigned char * const spr_hs [] = {
	ssit_03, ssit_00, ssit_01, ssit_02,
	ssit_04, ssit_05, ssit_06, ssit_07
};
