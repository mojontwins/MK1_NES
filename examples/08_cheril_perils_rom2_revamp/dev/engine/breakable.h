// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017

// breakable.h
// Manages breakable blocks and animations

#ifdef BREAKABLE_ANIM
	void breakable_do_anim (void) {
		do_process_breakable = 0;
		gpit = BREAKABLE_MAX; while (gpit --) {
			if (brkf [gpit]) {
				brkf [gpit] ++;
				if (brkf [gpit] == BREAKABLE_MAX_FRAMES) {
					brkf [gpit] = 0;
					map_set (brkx [gpit], brky [gpit], BREAKABLE_ERASE);	
				} else {
					do_process_breakable = 1;
				}
			}
		}
	}
#endif

void breakable_break (unsigned char x, unsigned char y) {
	gpaux = COORDS (x, y);
	if (brk_buff [gpaux] < BREAKABLE_LIFE) {
		brk_buff [gpaux] ++;
	} else {
#ifdef BREAKABLE_ANIM
		// Unsafe but short & fast. Adjust BREAKABLE_MAX if this breaks your game
		gpit = BREAKABLE_MAX; while (gpit --) {
			if (!brkf [gpit]) {
				do_process_breakable = 1;
				brkf [gpit] = 1;
				brkx [gpit] = x;
				brky [gpit] = y;
				map_set (x, y, BREAKABLE_BREAKING);	// Break tile!
				break;								// HA HA HA
			}
		}
#else
		map_set (x, y, BREAKABLE_ERASE);
#endif		
	}
}
