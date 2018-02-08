// All hail the monococo!

// x1, y1 is where it appears.
// mx is state. my is counter.

// Counter & state change
MONOCOCO_COUNTER --; if (!MONOCOCO_COUNTER) {
	MONOCOCO_STATE = (MONOCOCO_STATE + 1) & 3; MONOCOCO_COUNTER = monococo_state_times [MONOCOCO_STATE] - (rand8 () & 0x15);
}

// Shoot
if (MONOCOCO_STATE == 2 && MONOCOCO_COUNTER == MONOCOCO_FIRE_COCO_AT) {
	rdx = _en_x + 4; rdy = _en_y + 4; cocos_shoot_aimed ();
}

// Sprite
_en_facing = (prx < rdx) ? 4 : 0;
#ifdef MONOCOCO_TYPE_A
	if (
		((MONOCOCO_STATE == 1 || MONOCOCO_STATE == 3) && half_life) ||
		MONOCOCO_STATE == 2
	) {
		en_spr = MONOCOCO_BASE_SPRID + _en_facing + ((frame_counter >> 5) & 1);
	} 
#else
	en_spr = MONOCOCO_BASE_SPRID + _en_facing;
	switch (MONOCOCO_STATE) {
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
