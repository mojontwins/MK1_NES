// All hail the monococo!

// x1, y1 is where it appears.
// mx is state. my is counter.

// Counter & state change
-- _en_ct; if (!_en_ct) {
	_en_state = (_en_state + 1) & 3; _en_ct = monococo_state_times [_en_state] - (rand8 () & 0x15);
}

// Shoot
if (_en_state == 2 && _en_ct == MONOCOCO_FIRE_COCO_AT) {
	COCO_RDX = EN_X_ABSOLUTE + 4; rdy = _en_y + 4; cocos_shoot_aimed ();
}

// Sprite
//_en_facing = (prx < rdx) ? 4 : 0;
rda = (prx < rdx); enems_facing ();
#ifdef MONOCOCO_TYPE_A
	if (
		((_en_state == 1 || _en_state == 3) && half_life) ||
		_en_state == 2
	) {
		en_spr = MONOCOCO_BASE_SPRID + _en_facing + ((frame_counter >> 5) & 1);
	} 
#else
	en_spr = MONOCOCO_BASE_SPRID + _en_facing;
	switch (_en_state) {
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
