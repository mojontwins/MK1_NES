// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017

// Shooties are shooting patrollers. This code executes
// right after that of the patrollers, overriding some stuff.

if (en_rawv [gpit] & 1) {
	if (en_facing == 2) en_facing = 3;

	if (en_ct [gpit]) {
		// Is in shooting animation
		en_x [gpit] = rdx; en_y [gpit] = rdy;
		en_spr = en_s [gpit] + en_facing + 2;
		en_ct [gpit] --;
	} else {
		// Attempt to shoot
		if (SHOOT_FREQ) {
			if (en_facing) 
				rdx = en_x [gpit] + 16 - SHOOTIES_SHOOT_OFFS_X - 8;
			else 
				rdx = en_x [gpit] + SHOOTIES_SHOOT_OFFS_X;
			rdy = en_y [gpit] + SHOOTIES_SHOOT_OFFS_Y;
			cocos_shoot_linear ();
			en_ct [gpit] = 16;
		}

		en_spr = en_s [gpit] + en_facing + en_fr;
	}
}
