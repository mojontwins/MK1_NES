// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Compiled enemies!
// needs a const unsigned char *en_behptr [ENEMS_MAX];
// needs endx [8], endy [8] from precalcs.h

if (_en_ct) {
	switch (_en_state) {
		case 0:
			// Idling
			break;
		case 1:
			// Moving

			// TODO Add collision
			rdx = _en_x; _en_x += _en_mx;
			rdy = _en_y; _en_y += _en_my;

			break;
	}

	-- _en_ct;

} else {
	// Choose. Beware, this assumes you will never consume
	// the whole array and that a proper RETURN is issued!

	rda = *en_behptr [gpit] ++;
	_en_state = 0;
	
	rdc = (rda & 0x38) >> 3;
	rdt = rda & 0x07;

	switch (rda & 0xc0) {	// Command
		case 0x00:
			// IDLE / EXTERN
			if (rdt == 0) {
				// EXTERN
				do_extern_action (*en_behptr [gpit] ++);				
			} else {
				// IDLE
				rdb = 0; while (rdt --) rdb += 25;
				_en_ct = rdb;
				en_rawv [gpit] = _en_s + 2 + _en_facing;
			}
			break;

		case 0x40:
			// ADVANCE
			_en_mx = endx [rdc] << _en_x1; _en_my = endy [rdc] << _en_x1;
			
			if (_en_mx < 0) _en_facing = 4;
			else if (_en_mx > 0) _en_facing = 0;
			// If _en_mx == 0, no change!
			en_rawv [gpit] = _en_s + _en_facing;
			
			_en_ct = (rdt << 4) >> _en_x1; _en_state = 1;
			break;

		case 0x80:
			// FIRE & SPEED
			rdb = rda & 0x3f;
			if (rdb > 0x3b) {
				// SPEED
				_en_x1 =0x3f - rda; // 0 for 1, 1 for 2, 2 for 4, 3 for 8
			} else {
				// FIRE
				rdx = _en_x + 4; rdy = _en_y + 4; cocos_shoot_aimed ();
			}
			
			break;

		case 0xC0:
			// RETURN
			en_behptr [gpit] -= ((rda & 0x3f) + 1);

			break;
	}
}

en_spr = en_rawv [gpit];
switch (_en_state) {
	case 0:
		en_spr += ((frame_counter >> 3) & 1); 
		break;
	case 1:
		en_spr += en_fr;
		break;
}
