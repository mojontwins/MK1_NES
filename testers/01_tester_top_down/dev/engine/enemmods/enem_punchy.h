// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// PUNCHIES are shooting patrollers. This code executes
// right after that of the patrollers, overriding some stuff.

#ifdef DOUBLE_WIDTH
if (on_screen)
#endif
{
	if (_en_t & 0x40) {
		if (_en_ct) {
			// Is in shooting animation
			_en_x = rdx; _en_y = rdy;
			en_spr = _en_s + _en_facing + 2;
			-- _en_ct;
		} else {
			// Attempt to punch
			if (PUNCH_FREQ) {
				if (_en_facing) 
					PUNCH_RDX = EN_X_ABSOLUTE + 16 - PUNCHIES_PUNCH_OFFS_X - 8;
				else 
					PUNCH_RDX = EN_X_ABSOLUTE + PUNCHIES_PUNCH_OFFS_X;
				rdy = _en_y + PUNCHIES_PUNCH_OFFS_Y;
				
				_en_ct = 16;
				sfx_play (SFX_HITTER, 1);

				// Collide w/player
				if (
					#ifdef PLAYER_FLICKERS
						!pflickering &&
					#endif
					#ifdef PLAYER_BOUNCES
						!pbouncing && 
					#endif
					PUNCH_RDX + 7 >= prx && PUNCH_RDX <= prx + 7 && 
					rdy + 7 + PLAYER_COLLISION_VSTRETCH_FG >= pry && 
					rdy <= pry + 13
				) {
					pkill = 1;
					en_spr = _en_s + _en_facing + 2;
				}
			} else en_spr = _en_s + _en_facing + en_fr;
		}
	}
}
