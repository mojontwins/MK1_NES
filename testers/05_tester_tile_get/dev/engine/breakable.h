// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// breakable.h
// Manages breakable blocks and animations

#ifdef BREAKABLE_ANIM
	void breakable_do_anim (void) {
		do_process_breakable = 0;
		gpit = BREAKABLE_MAX; while (gpit --) {
			if (brkf [gpit]) {
				++ brkf [gpit];
				if (brkf [gpit] == BREAKABLE_MAX_FRAMES) {
					brkf [gpit] = 0;
					_x = brkx [gpit]; _y = brky [gpit]; _t = BREAKABLE_ERASE;
					map_set ();
				} else {
					do_process_breakable = 1;
				}
			}
		}
	}
#endif

void breakable_break (unsigned char x, unsigned char y) {
	gpaux = COORDS (x, y);
	#ifndef BREAKABLES_SOFT
		if (brk_buff [gpaux] < BREAKABLE_LIFE) {
			++ brk_buff [gpaux];
		} else 
	#endif
	{
		sfx_play (SFX_BREAKH, 1);
#ifdef BREAKABLE_ANIM
		// Unsafe but short & fast. Adjust BREAKABLE_MAX if this breaks your game
		gpit = BREAKABLE_MAX; while (gpit --) {
			if (!brkf [gpit]) {
				do_process_breakable = 1;
				brkf [gpit] = 1;
				_x = brkx [gpit] = x;
				_y = brky [gpit] = y;
				_t = BREAKABLE_BREAKING;
				map_set ();					// Break tile!
				sfx_play (SFX_BREAKB, 1);
				break;						// HA HA HA
			}
		}
#else
		_x = x; _y = y; _t = BREAKABLE_ERASE;
		map_set ();
#endif		
	}
}
