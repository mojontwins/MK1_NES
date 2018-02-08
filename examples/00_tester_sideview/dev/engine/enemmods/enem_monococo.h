// All hail the monococo!

// x1, y1 is where it appears.
// mx is state. my is counter.

// Counter & state change
MONOCOCO_COUNTER [gpit] --; if (!MONOCOCO_COUNTER [gpit]) {
	MONOCOCO_STATE [gpit] = (MONOCOCO_STATE [gpit] + 1) & 3; MONOCOCO_COUNTER [gpit] = monococo_state_times [MONOCOCO_STATE [gpit]] - (rand8 () & 0x15);
}

// Shoot
if (MONOCOCO_STATE [gpit] == 2 && MONOCOCO_COUNTER [gpit] == MONOCOCO_FIRE_COCO_AT) {
	rdx = MONOCOCO_X [gpit] + 4; rdy = MONOCOCO_Y [gpit] + 4; cocos_shoot_aimed ();
}

// Sprite
en_facing [gpit] = (prx < rdx) ? 4 : 0;
#ifdef MONOCOCO_TYPE_A
	if (
		((state == 1 || state == 3) && half_life) ||
		state == 2
	) {
		en_spr = MONOCOCO_BASE_SPRID + en_facing [gpit] + ((frame_counter >> 5) & 1);
	} 
#else
	en_spr = MONOCOCO_BASE_SPRID + en_facing [gpit];
	switch (MONOCOCO_STATE [gpit]) {
		case 0:
			en_spr += 9;
			break;
		case 1:
		case 3:
			en_spr += 8;
			break;
		case 2:
			en_spr += ((frame_counter >> 5) & 1);
			break;
	}
#endif
