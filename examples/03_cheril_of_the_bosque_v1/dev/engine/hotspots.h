// hotspots.h
// Hotspot management

void __fastcall__ hotspots_load (void) {
	// Copies hotspots from ROM to RAM and initializes them
	gp_gen = (unsigned char *) c_hotspots;
	for (gpit = 0; gpit < MAP_W * MAP_H; gpit ++) {
		hxy [gpit] = *gp_gen ++;
		ht [gpit] = *gp_gen ++;
		hact [gpit] = 1;
	}	
}

// v1.0: Simple. Modify/expand when needed.
void __fastcall__ hotspots_create (void) {
	hrt = 0;
	
	if (ht [n_pant] && hact [n_pant]) {
		if (hact [n_pant]) {
			hrt = ht [n_pant];
		} 
	}
	
	if (hrt) {
		hrx = (hxy [n_pant] >> 4) << 4;
		hry = (hxy [n_pant] & 15) << 4;
		draw_tile (hrx >> 3, TOP_ADJUST + (hry >> 3), 16 + hrt);
	}
}
