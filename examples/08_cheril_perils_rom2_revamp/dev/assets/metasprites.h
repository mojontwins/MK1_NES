// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// spritesets

// Player metasprites
const unsigned char * const spr_player [] = {
	// Facing right (offset 0)
	sspl_00_a,										// Standing (offset 0)
	sspl_01_a, sspl_02_a, sspl_03_a, sspl_04_a,		// Walk cycle (offset 1)
	sspl_05_a,										// Airborne (offset 5)
	
	// Facing left (offset 6)
	sspl_00_b,										// Standing (offset 0)
	sspl_01_b, sspl_02_b, sspl_03_b, sspl_04_b,		// Walk cycle (offset 1)
	sspl_05_b,										// Airborne (offset 5)
		
	// Use animation (offset 12)
	sspl_06_a, sspl_07_a, sspl_08_a, sspl_09_a,		// Up to down
	sspl_0A_a, sspl_0B_a, 							// After this, *interact*
	sspl_0B_a, sspl_0A_a, sspl_09_a, sspl_08_a,		// Down to up
	sspl_07_a, sspl_06_a,
	sspl_0C_a										// Final frame
};

// Enemy metasprites
// Framesets are right {anim1, anim2, attacking, dying}, right {anim1, anim2, attacking, dying}.
// Not every kind of enemy has every cell, just use 0 when N/A. dying cell will be used only if ENEMS_ENABLE_DYING_FRAME
const unsigned char * const spr_enems0 [] = {
	// Linear enems and platforms (ids 1-4)
	ssen_00_a, ssen_01_a, 0, 0, ssen_00_b, ssen_01_b, 0, 0, // 1
	ssen_02_a, ssen_03_a, 0, 0, ssen_02_b, ssen_03_b, 0, 0, // 2
	ssen_02_a, ssen_03_a, 0, 0, ssen_02_b, ssen_03_b, 0, 0, // 3 (duplicated 2, won't use)
	ssplat_00, ssplat_01, 0, 0, ssplat_00, ssplat_01, 0, 0,	// Platform (4)

	// Fanty (offset 32)
	ssfanty_00_a, ssfanty_01_a, 0, 0, ssfanty_00_b, ssfanty_01_b, 0, 0,

	// Saw (offset 40)
	sssaw_00, sssaw_01,

	// Occlusion (offset 42)
	ssocl_00,

	// Explosion (offset 43)
	ssit_05,

	// Steady shooters (offset 44)
	ssss_00, ssss_01, ssss_02, ssss_03
};

const unsigned char * const spr_enems1 [] = {
	// Linear enems and platforms (ids 1-4)
	ssen_04_a, ssen_05_a, 0, 0, ssen_04_b, ssen_05_b, 0, 0, // 1
	ssen_06_a, ssen_07_a, 0, 0, ssen_06_b, ssen_07_b, 0, 0, // 2
	ssen_06_a, ssen_07_a, 0, 0, ssen_06_b, ssen_07_b, 0, 0, // 3 (duplicated 2, won't use)
	ssplat_00, ssplat_01, 0, 0, ssplat_00, ssplat_01, 0, 0,	// Platform (4)

	// Fanty (offset 32)
	ssfanty_00_a, ssfanty_01_a, 0, 0, ssfanty_00_b, ssfanty_01_b, 0, 0,

	// Saw (offset 40)
	sssaw_00, sssaw_01,

	// Occlusion (offset 42)
	ssocl_00,

	// Explosion (offset 43)
	ssit_05,

	// Steady shooters (offset 44)
	ssss_00, ssss_01, ssss_02, ssss_03
};


// Item metasprites
const unsigned char * const spr_hs [] = {

	// nothing, empty, key, refill, res_off, res_on	
	0, ssit_00, ssit_01, ssit_02, ssit_03, ssit_04,

	// 6 onwards:
	// 	item 1,  item 2,  item 3,  item 4,  item 5,  0, 0, 0,
	// 	place 1, place 2, 0,       0,       0,       0, 0, 0, 
	// 	used 1,	 used 2,  0,       0,       0,       0, 0, 0
	sssit_00, sssit_03, sssit_06, sssit_07, sssit_08, 0, 0, 0,	// N (total = 8)
	sssit_01, sssit_04, 0, 0, 0, 0, 0, 0, 						// N + HS_USE_OFFS (N+8)
	sssit_02, sssit_05, 0, 0, 0, 0, 0, 0 						// N + 2*HS_USE_OFFS (N+16)

	//  6 = crank/key
	//  7 = ?
	//  8 = fork
	//  9 = zurully
	// 10 = key
	// 11 = ?
	// 12 = ?
	// 13 = ?
	// 14 = propellers starter
	// 15 = ?
	// 22 = propellers started + crank (used)
	// 23 = ? + ? (used)
};
