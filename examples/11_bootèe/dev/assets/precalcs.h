// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// LUTs

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
