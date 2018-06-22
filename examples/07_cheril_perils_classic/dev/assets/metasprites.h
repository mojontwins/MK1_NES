// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// spritesets

// Player metasprites
const unsigned char * const spr_player [] = {
	// Facing right (offset 0)
	sspl_00_a,										// Standing (offset 0)
	sspl_00_a, sspl_01_a, sspl_02_a, sspl_03_a,		// Walk cycle (offset 1)
	sspl_04_a, 										// Airborne (offset 5)
	0, 0,											// I like powers of two
	

	// Facing left (offset 8)
	sspl_00_b,										// Standing (offset 0)
	sspl_00_b, sspl_01_b, sspl_02_b, sspl_03_b,		// Walk cycle (offset 1)
	sspl_04_b, 										// Airborne (offset 5)
	0, 0,											// I like powers of two
	
};

// Enemy metasprites
// Framesets are right {anim1, anim2, attacking, dying}, right {anim1, anim2, attacking, dying}.
// Not every kind of enemy has every cell, just use 0 when N/A. dying cell will be used only if ENEMS_ENABLE_DYING_FRAME
const unsigned char * const spr_enems0 [] = {
	// Linear enems and platforms (ids 1-4)
	ssen_00_a, ssen_01_a, 0, 0, ssen_00_b, ssen_01_b, 0, 0, // 1
	ssen_02_a, ssen_03_a, 0, 0, ssen_02_b, ssen_03_b, 0, 0, // 2
	ssen_04_a, ssen_05_a, 0, 0, ssen_04_b, ssen_05_b, 0, 0, // 3
	ssen_0E_a, ssen_0F_a, 0, 0, ssen_0E_b, ssen_0F_b, 0, 0, // Platform (4)

	ssen_0C_a, ssen_0D_a, 0, 0, ssen_0C_b, ssen_0D_b, 0, 0, // Fanty (offset 32)

	// Explosion (offset 40)
	ssen_10
};

const unsigned char * const spr_enems1 [] = {
	// Linear enems and platforms (ids 1-4)
	ssen_06_a, ssen_07_a, 0, 0, ssen_06_b, ssen_07_b, 0, 0, // 1
	ssen_08_a, ssen_09_a, 0, 0, ssen_08_b, ssen_09_b, 0, 0, // 2
	ssen_0A_a, ssen_0B_a, 0, 0, ssen_0A_b, ssen_0B_b, 0, 0, // 3
	ssen_0E_a, ssen_0F_a, 0, 0, ssen_0E_b, ssen_0F_b, 0, 0, // Platform (4)

	ssen_0C_a, ssen_0D_a, 0, 0, ssen_0C_b, ssen_0D_b, 0, 0, // Fanty (offset 32)

	// Explosion (offset 40)
	ssen_10
};

// Item metasprites
// nothing, nothing, key, refill, res_on, res_off
const unsigned char * const spr_hs [] = {
	0, 0, ssit_01, ssit_00, ssit_02, ssit_03
};

