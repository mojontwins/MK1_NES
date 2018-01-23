// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017

// breakable.h
// Manages breakable blocks and animations

#ifdef BREAKABLE_ANIM
void __fastcall__ process_breakable (void) {
	do_process_breakable = 0;
	gpit = MAX_BREAKABLE; while (gpit --) {
		if (brkf [gpit]) {
			brkf [gpit] ++;
			if (brkf [gpit] == MAX_BREAKABLE_FRAMES) {
				brkf [gpit] = 0;
				map_set (brkx [gpit], brky [gpit], BREAKABLE_ERASE);	
			} else {
				do_process_breakable = 1;
			}
		}
	}
}
#endif

void break_wall (unsigned char x, unsigned char y) {
	gpaux = x + (y << 4);
	if (brk_buff [gpaux] < BREAKABLE_WALLS_LIFE) {
		brk_buff [gpaux] ++;
	} else {
#ifdef BREAKABLE_ANIM
		// Unsafe but short & fast. Adjust MAX_BREAKABLE if this breaks your game
		gpit = MAX_BREAKABLE; while (gpit --) {
			if (!brkf [gpit]) {
				do_process_breakable = 1;
				brkf [gpit] = 1;
				brkx [gpit] = x;
				brky [gpit] = y;
				map_set (x, y, BREAKABLE_BREAKING);	// Break tile!
			}
		}
#else
		map_set (x, y, BREAKABLE_ERASE);
#endif		
	}
}
