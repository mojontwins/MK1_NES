// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017

// PUNCHIES are shooting patrollers. This code executes
// right after that of the patrollers, overriding some stuff.

if (en_rawv [gpit] & 2) {
	if (en_ct [gpit]) {
		// Is in shooting animation
		en_x [gpit] = rdx; en_y [gpit] = rdy;
		en_spr = en_s [gpit] + en_facing [gpit] + 2;
		en_ct [gpit] --;
	} else {
		// Attempt to shoot
		if (PUNCH_FREQ) {
			if (en_facing [gpit]) 
				rdx = en_x [gpit] + 16 - PUNCHIES_PUNCH_OFFS_X - 8;
			else 
				rdx = en_x [gpit] + PUNCHIES_PUNCH_OFFS_X;
			rdy = en_y [gpit] + PUNCHIES_PUNCH_OFFS_Y;
			
			en_ct [gpit] = 16;

			// Collide w/player
			if (pstate == EST_NORMAL && 
				rdx + 7 >= prx && rdx <= prx + 7 && 
				#ifdef TALL_PLAYER
					rdy + 7 >= pry - 16 + PLAYER_COLLISION_TOP &&
				#else			
					rdy + 7 >= pry && 
				#endif
				rdy <= pry + 12
			) {
				pkill = 1;
				en_spr = en_s [gpit] + en_facing [gpit] + 2;
			}
		} else en_spr = en_s [gpit] + en_facing [gpit] + en_fr;
	}
}
