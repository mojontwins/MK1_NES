// NES MK1 v2.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// LUTs

const unsigned char bitmasks [] = {0xfc, 0xf3, 0xcf, 0x3f};

#ifdef ENABLE_MONOCOCOS
	const unsigned char monococo_state_times [] = {
		MONOCOCO_BASE_TIME_HIDDEN, MONOCOCO_BASE_TIME_APPEARING, MONOCOCO_BASE_TIME_ONBOARD, MONOCOCO_BASE_TIME_APPEARING
	};
#endif

#if defined (ENABLE_COCOS) && defined(COCOS_ENABLE_LINEAR)
	// left, up, right, down
	const signed int coco_dx [] = { -COCO_V, 0, COCO_V, 0 };
	const signed int coco_dy [] = { 0, -COCO_V, 0, COCO_V };
#endif

#if defined (ENABLE_COMPILED_ENEMS)
	// Directions are
	// LEFT DOWNLEFT DOWN DOWNRIGHT RIGHT UPRIGHT UP UPLEFT
	// 0    1        2    3         4     5       6  7
	const signed char endx [] = {-1, -1, 0, 1, 1, 1, 0, -1};
	const signed char endy [] = {0, 1, 1, 1, 0, -1, -1, -1};
#endif

#if defined (ENABLE_TILE_CHAC_CHAC) || defined (ENABLE_CHAC_CHAC)
	const unsigned char chac_chacs_times [] = {
		0, CHAC_CHAC_IDLE_2, CHAC_CHAC_IDLE_3, CHAC_CHAC_IDLE_4, CHAC_CHAC_IDLE_3, CHAC_CHAC_IDLE_3
	};

	const unsigned char chac_chacs_t1 [] = {
		CHAC_CHAC_BASE_TILE + 6, CHAC_CHAC_BASE_TILE, CHAC_CHAC_BASE_TILE + 2, CHAC_CHAC_BASE_TILE + 5, CHAC_CHAC_BASE_TILE + 2, CHAC_CHAC_BASE_TILE
	};

	const unsigned char chac_chacs_t2 [] = {
		CHAC_CHAC_BASE_TILE + 6, CHAC_CHAC_BASE_TILE + 6, CHAC_CHAC_BASE_TILE + 6, CHAC_CHAC_BASE_TILE + 4, CHAC_CHAC_BASE_TILE + 6, CHAC_CHAC_BASE_TILE + 6
	};

	const unsigned char chac_chacs_t3 [] = {
		CHAC_CHAC_BASE_TILE + 6, CHAC_CHAC_BASE_TILE + 1, CHAC_CHAC_BASE_TILE + 3, CHAC_CHAC_BASE_TILE + 5, CHAC_CHAC_BASE_TILE + 3, CHAC_CHAC_BASE_TILE + 1
	};
#endif

const unsigned char bits [] = {
	1, 2, 4, 8, 16, 32, 64, 128
};

#ifdef ENEMS_MAY_DIE
	const unsigned char jitter [] = { 0,1,1,0,0,1,0,1,1,0,0,0,1,0,1,1 };
#endif

