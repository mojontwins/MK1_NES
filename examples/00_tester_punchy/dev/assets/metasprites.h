// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017

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
	sspl_09_b										// Kick (airborne)	
	
};

// Enemy metasprites
const unsigned char * const spr_enems0 [] = {
	// Linear enems and platforms (ids 1-4)
	ssena_00_a, ssena_01_a, ssena_00_b, ssena_01_b, // Linear 1
	ssena_00_a, ssena_01_a, ssena_00_b, ssena_01_b, // Linear 1
	ssena_00_a, ssena_01_a, ssena_00_b, ssena_01_b, // Linear 1 (won't use)
	ssplat_00, ssplat_00, ssplat_00, ssplat_00, 	// Platform

	// Punchy (offset 16), 6 frames
	ssenpu_00_a, ssenpu_01_a, ssenpu_02_a,			// W1, W2, PUNCH (right)
	ssenpu_00_b, ssenpu_01_b, ssenpu_02_b,			// W1, W2, PUNCH (left)

	// Shooty (offset 22), 6 frames
	ssensh_00_a, ssensh_01_a, ssensh_02_a,			// W1, W2, SHOOT (right)
	ssensh_00_b, ssensh_01_b, ssensh_02_b,			// W1, W2, SHOOT (left)

	// Explosion (offset 28)
	ssexpl

};



// Item metasprites
// nothing, object, nothing, refill
const unsigned char * const spr_hs [] = {
	0, ssit_00, 0, ssit_01
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
