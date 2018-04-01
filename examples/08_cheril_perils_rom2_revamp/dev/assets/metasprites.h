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
	sspl_06, sspl_07, sspl_08, sspl_09,				// Up to down
	sspl_0A, sspl_0B, 								// After this, *interact*
	sspl_0B, sspl_0A, sspl_09, sspl_08,				// Down to up
	sspl_07, sspl_06,
	sspl_0C,										// Final frame

	// Climbing ladder (offset 25)
	sspl_0D, sspl_0E, sspl_0F, sspl_0E,

	// Half ladder (offset 29)
	sspl_0A
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
	ssit_05
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

	// Alternate, big fanty (offset 44)
	ssbigf_00_b, ssbigf_01_b, 0, 0, ssbigf_00_b, ssbigf_01_b, 0, 0, 
};

const unsigned char * const spr_enems2 [] = {
	// Linear enems and platforms (ids 1-4)
	ssen2_00_a, ssen2_01_a, 0, 0, ssen2_00_b, ssen2_01_b, 0, 0, // 1
	ssen2_02_a, ssen2_03_a, 0, 0, ssen2_02_b, ssen2_03_b, 0, 0, // 2
	ssen2_04_a, ssen2_05_a, 0, 0, ssen2_04_b, ssen2_05_b, 0, 0, // 3 (duplicated 2, won't use)
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
	ssss_00, ssss_01, ssss_02, ssss_03,

	// Precompiled (offset 48)
	sscomp_00_a, sscomp_01_a, sscomp_02_a, sscomp_03_a, 
	sscomp_00_b, sscomp_01_b, sscomp_02_b, sscomp_03_b, 
};

const unsigned char * const spr_enems3 [] = {
	// Linear enems and platforms (ids 1-4)
	ssen3_00_a, ssen3_01_a, 0, 0, ssen3_00_b, ssen3_01_b, 0, 0, // 1
	ssen3_02_a, ssen3_03_a, 0, 0, ssen3_02_b, ssen3_03_b, 0, 0, // 2
	ssen3_04_a, ssen3_05_a, 0, 0, ssen3_04_b, ssen3_05_b, 0, 0, // 3 (duplicated 2, won't use)
	ssplat_00, ssplat_01, 0, 0, ssplat_00, ssplat_01, 0, 0,	// Platform (4)

	// Fanty (offset 32)
	ssfanty_00_a, ssfanty_01_a, 0, 0, ssfanty_00_b, ssfanty_01_b, 0, 0,

	// Saw (offset 40)
	sssaw_00, sssaw_01,

	// Occlusion (offset 42)
	ssocl_00,

	// Explosion (offset 43)
	ssit_05
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
	sssit_02, sssit_05, 0, 0, 0, 0, 0, 0,						// N + 2*HS_USE_OFFS (N+16)

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

	// 30 onwards: Characters
	sschar1_00, 					// Amador / Jack
	sschar1_01,						// Mandee
	sschar2_00,						// Girl
};

// Some custom defines

#define SPR_AMADOR			30
#define SPR_MANDEE			31
#define SPR_GIRL			32
