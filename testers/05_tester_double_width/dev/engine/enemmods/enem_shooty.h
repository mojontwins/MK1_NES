// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Shooties are shooting patrollers. This code executes
// right after that of the patrollers, overriding some stuff.

if (_en_t & 0x80) {
	if (_en_ct) {
		// Is in shooting animation
		_en_x = rdx; _en_y = rdy;
		en_spr = _en_s + _en_facing + 2;
		-- _en_ct;
	} else {
		// Attempt to shoot
		if (SHOOT_FREQ) {
			if (_en_facing) {
				COCO_RDX = EN_X_ABSOLUTE + 16 - SHOOTIES_SHOOT_OFFS_X - 8;
				rda = 0;
			} else {
				COCO_RDX = EN_X_ABSOLUTE + SHOOTIES_SHOOT_OFFS_X;
				rda = 2;
			}
			rdy = _en_y + SHOOTIES_SHOOT_OFFS_Y;
			cocos_shoot_linear ();
			_en_ct = 16;
		}

		en_spr = _en_s + _en_facing + en_fr;
	}
}
