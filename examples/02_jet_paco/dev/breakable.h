// NES MK1 v0.1
// Copyleft Mojon Twins 2013

// breakable.h
// Manages breakable blocks and animations

#ifdef BREAKABLE_ANIM
void __fastcall__ process_breakable (void) {
	do_process_breakable = 0;
	for (gpit = 0; gpit < MAX_BREAKABLE; gpit ++) {
		if (brkf [gpit]) {
			brkf [gpit] ++;
			if (brkf [gpit] == MAX_BREAKABLE_FRAMES) {
				brkf [gpit] = 0;
				map_set (brkx [gpit], brky [gpit], ts_offs);	
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
		for (gpit = 0; gpit < MAX_BREAKABLE; gpit ++) {
			if (!brkf [gpit]) {
				do_process_breakable = 1;
				brkf [gpit] = 1;
				brkx [gpit] = x;
				brky [gpit] = y;
				map_set (x, y, 47);	// Break tile!
			}
		}
#else
		map_set (x, y, ts_offs);
#endif		
	}
}
