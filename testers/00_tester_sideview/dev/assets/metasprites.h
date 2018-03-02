// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// spritesets

// Player metasprites
const unsigned char * const spr_player [] = {
	// Facing right (offset 0)
	sspl_00_a,										// Standing (offset 0)
	sspl_01_a, sspl_02_a, sspl_03_a, sspl_04_a,		// Walk cycle (offset 1)
	sspl_05_a, sspl_06_a,							// Airborne, up/down (offset 5, 6)
	0,												// I like powers of two
	

	// Facing left (offset 10)
	sspl_00_b,										// Standing (offset 0)
	sspl_01_b, sspl_02_b, sspl_03_b, sspl_04_b,		// Walk cycle (offset 1)
	sspl_05_b, sspl_06_b,							// Airborne, up/down (offset 5, 6)
	0,												// I like powers of two
	
};

// Enemy metasprites
// Framesets are right {anim1, anim2, attacking, dying}, right {anim1, anim2, attacking, dying}.
// Not every kind of enemy has every cell, just use 0 when N/A. dying cell will be used only if ENEMS_ENABLE_DYING_FRAME
const unsigned char * const spr_enems0 [] = {
	// Linear enems and platforms (ids 1-4)
	ssen_00_a, ssen_01_a, 0, 0, ssen_00_b, ssen_01_b, 0, 0, 
	ssen_02_a, ssen_03_a, 0, 0, ssen_02_b, ssen_03_b, 0, 0, 
	ssen_04_a, ssen_05_a, 0, 0, ssen_04_b, ssen_05_b, 0, 0, 
	ssplat_00, ssplat_01, 0, 0, ssplat_00, ssplat_01, 0, 0, 

	// Fanty (offset 32) 
	ssfanty_00_a, ssfanty_01_a, 0, 0, ssfanty_00_b, ssfanty_01_b, 0, 0, 

	// Pezon (offset 40), 0, 1
	sspezon_00, sspezon_01, 0, 0, 0, 0, 0, 0, 

	// Saw (offset 48), 0, 1
	sssaw_00, sssaw_01, 0, 0, 0, 0, 0, 0, 

	// Monococo, full (offset 56) 
	ssmono_00_a, ssmono_01_a, 0, 0, ssmono_00_b, ssmono_01_b, 0, 0, 
	ssmono_02_a, ssmono_03_a, 0, 0, ssmono_02_b, ssmono_03_b, 0, 0,  	// extra frames for full monococos

	// Occluding cell (offset 72) (for pezon, saw)
	sseocc,

	// Explosion (offset 73)
	ssexpl
};

// Item metasprites
// nothing, object, key, refill
// object 1, object 2, empty
const unsigned char * const spr_hs [] = {
	0, ssit_00, ssit_01, ssit_02,
	// HS_OBJ_EMPTY = 4, HS_OBJ_MIN = 5, HS_OBJ_MAX = 6
	ssobjs_00, ssobjs_01, ssobjs_02
};

