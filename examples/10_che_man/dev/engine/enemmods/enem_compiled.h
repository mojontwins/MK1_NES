// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Compiled enemies!
// needs a const unsigned char *en_behptr [ENEMS_MAX];
// needs endx [8], endy [8] from precalcs.h

if (_en_ct) {
	// Do 

	switch (en_alive [gpit]) {
		case 0:
			// Idling

			// Idle animation, cells 2, 3.
			en_spr = _en_s + _en_facing + 2 + 
				((frame_counter >> 3) & 1); 
			break;
		case 1:
			// Moving

			// TODO Add collision
			rdx = _en_x; _en_x += _en_mx;
			rdy = _en_y; _en_y += _en_my;

			// Moving animation, cells 0, 1.
			en_spr = _en_s + _en_facing + en_fr;
			break;
	}

	_en_ct --;

} else {
	// Choose. Beware, this assumes you will never consume
	// the whole array and that a proper RETURN is issued!

	rda = *en_behptr [gpit] ++;
	en_alive [gpit] = 0;

	rdc = (rda & 0x38) >> 3;
	rdt = rda & 0x07;

	switch (rda & 0xc0) {	// Command
		case 0x00:
			// IDLE
			rdb = 1; while (rdt --) rdb += 25;
			_en_ct = rdb;
			break;

		case 0x40:
			// ADVANCE
			_en_mx = endx [rdc]; _en_my = endy [rdc];
					
			if (_en_mx < 0) _en_facing = 4;
			else if (_en_mx > 0) _en_facing = 0;
			
			_en_ct = rdt << 4; en_alive [gpit] = 1;
			break;

		case 0x80:
			// FIRE
			rdx = _en_x + 4; rdy = _en_y + 4; cocos_shoot_aimed ();
			break;

		case 0xC0:
			// RETURN
			en_behptr [gpit] -= ((rda & 0x3f) + 1);
			break;
	}

	en_spr = _en_s + _en_facing;
}

