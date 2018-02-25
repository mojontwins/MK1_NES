// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// PUNCHIES are shooting patrollers. This code executes
// right after that of the patrollers, overriding some stuff.

if (en_rawv [gpit] & 2) {
	if (_en_ct) {
		// Is in shooting animation
		_en_x = rdx; _en_y = rdy;
		en_spr = _en_s + _en_facing + 2;
		_en_ct --;
	} else {
		// Attempt to shoot
		if (PUNCH_FREQ) {
			if (_en_facing) 
				rdx = _en_x + 16 - PUNCHIES_PUNCH_OFFS_X - 8;
			else 
				rdx = _en_x + PUNCHIES_PUNCH_OFFS_X;
			rdy = _en_y + PUNCHIES_PUNCH_OFFS_Y;
			
			_en_ct = 16;

			// Collide w/player
			if (pstate == EST_NORMAL && 
				rdx + 7 >= prx && rdx <= prx + 7 && 
				rdy + 7 + PLAYER_COLLISION_VSTRETCH_FG >= pry && 
				rdy <= pry + 13
			) {
				pkill = 1;
				en_spr = _en_s + _en_facing + 2;
			}
		} else en_spr = _en_s + _en_facing + en_fr;
	}
}
