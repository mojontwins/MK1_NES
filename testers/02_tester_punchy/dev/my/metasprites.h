// NES MK1 v2.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// spritesets

// Player metasprites
const unsigned char * const spr_player [] = {
	// Facing right (offset 0)
	sspl_00_a,										// Standing (offset 0)
	sspl_01_a,										// Pre-walk (vx < MIN)
	sspl_02_a, sspl_03_a, sspl_04_a, sspl_05_a,		// Walk cycle (offset 1)
	sspl_06_a, sspl_07_a,							// Airborne, up/down (offset 5, 6)
	sspl_08_a,										// Punch (standing)
	sspl_09_a,										// Kick (airborne)
	
	// Facing left (offset 10)
	sspl_00_b,										// Standing (offset 0)
	sspl_01_b,										// Pre-walk (vx < MIN)
	sspl_02_b, sspl_03_b, sspl_04_b, sspl_05_b,		// Walk cycle (offset 1)
	sspl_06_b, sspl_07_b,							// Airborne, up/down (offset 5, 6)
	sspl_08_b,										// Punch (standing)
	sspl_09_b,										// Kick (airborne)	
	
	// offset 20
	sspl_0A, sspl_0B, sspl_0C, sspl_0B 		 		// Climbing
};

// Enemy metasprites
// Framesets are right {anim1, anim2, attacking, dying}, right {anim1, anim2, attacking, dying}.
// Not every kind of enemy has every cell, just use 0 when N/A. dying cell will be used only if ENEMS_ENABLE_DYING_FRAME
const unsigned char * const spr_enems0 [] = {
	// Linear enems and platforms (ids 1-4)
	ssena_00_a, ssena_01_a, 0, 0, ssena_00_b, ssena_01_b, 0, 0, // 1
	ssena_00_a, ssena_01_a, 0, 0, ssena_00_b, ssena_01_b, 0, 0, // won't use; failsafe copy of 1
	ssena_00_a, ssena_01_a, 0, 0, ssena_00_b, ssena_01_b, 0, 0, // won't use; failsafe copy of 1
	ssplat_00, ssplat_00, 0, 0, ssplat_00, ssplat_00, 0, 0,  	// Platform (4)

	// Punchy (offset 32)
	ssenpu_00_a, ssenpu_01_a, ssenpu_03_a, ssenpu_02_a,
	ssenpu_00_b, ssenpu_01_b, ssenpu_03_b, ssenpu_02_b,

	// Shooty (offset 40)
	ssensh_00_a, ssensh_01_a, ssensh_02_a, ssensh_03_a,
	ssensh_00_b, ssensh_01_b, ssensh_02_b, ssensh_03_b,

	// Portals (warpers) (offset 48)
	ssport_00, ssport_01, ssport_02, ssport_03
};



// Item metasprites
// nothing, object, nothing, refill
const unsigned char * const spr_hs [] = {
	0, ssit_00, 0, ssit_01
};

