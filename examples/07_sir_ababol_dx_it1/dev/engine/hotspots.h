// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017

// hotspots.h
// Hotspot management

void hotspots_load (void) {
	// Copies hotspots from ROM to RAM and initializes them
	gp_gen = (unsigned char *) c_hotspots;
	for (gpit = 0; gpit < MAP_W * MAP_H; gpit ++) {
#ifndef HOTSPOTS_WONT_CHANGE
		ht [gpit] = *gp_gen ++;
		hyx [gpit] = *gp_gen ++;
		gp_gen ++;
#endif
		hact [gpit] = 1;
	}	
}

void hotspots_paint (void) {
	oam_index = oam_meta_spr (
		hrx, hry + SPRITE_ADJUST, 
		oam_index, 
		spr_hs [hrt]
	);
}

void hotspots_create (void) {
	
#ifdef HOTSPOTS_WONT_CHANGE

	gp_gen = (unsigned char *) (c_hotspots + (n_pant << 1));
	hrt = *gp_gen ++; rda = *gp_gen; 
	if (hrt && hact [n_pant]) {
		hry = rda & 0xf0; hrx = rda << 4;
	} else hrt = 0;

#else

	hrt = 0;

	if (ht [n_pant] && hact [n_pant]) {
		if (hact [n_pant]) {
			hrt = ht [n_pant];
		} 
	}

	if (hrt) {
		hrx = hyx [n_pant] >> 4;
		hry = hyx [n_pant] & 0xf0;
	}	

#endif
}
