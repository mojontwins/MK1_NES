// hotspots.h
// Hotspot management

void __fastcall__ hotspots_load (void) {
	// Copies hotspots from ROM to RAM and initializes them
	gp_gen = (unsigned char *) c_hotspots;
	for (gpit = 0; gpit < MAP_W * MAP_H; gpit ++) {
#ifndef HOTSPOTS_WONT_CHANGE
		hxy [gpit] = *gp_gen ++;
		ht [gpit] = *gp_gen ++;
#endif
		hact [gpit] = 1;
	}	
}

// v1.0: Simple. Modify/expand when needed.
void __fastcall__ hotspots_create (void) {
	hrt = 0;

#ifndef HOTSPOTS_WONT_CHANGE
	if (ht [n_pant] && hact [n_pant]) {
		if (hact [n_pant]) {
			hrt = ht [n_pant];
		} 
	}

	if (hrt) {
		hrx = (hxy [n_pant] >> 4) << 4;
		hry = (hxy [n_pant] & 15) << 4;

		if (hrt == HOTSPOT_TYPE_RESONATOR && res_on) {
			oam_meta_spr (hrx, hry + SPRITE_ADJUST, 176, spr_hs [3]);
		} else {
			oam_meta_spr (hrx, hry + SPRITE_ADJUST, 176, spr_hs [hrt - 1]);
		}
	}	
#else
	gp_gen = (unsigned char *) c_hotspots; gp_gen += (n_pant << 1);
	rda = *gp_gen ++; rdb = *gp_gen;
	if (rdb && hact [n_pant]) {
		hrt = rdb;
		hrx = rda & 0xf0; hry = rda << 4;
		if (hrt == HOTSPOT_TYPE_RESONATOR && res_on) {
			oam_meta_spr (hrx, hry + SPRITE_ADJUST, 176, spr_hs [3]);
		} else {
			oam_meta_spr (hrx, hry + SPRITE_ADJUST, 176, spr_hs [hrt - 1]);
		}
	} else oam_meta_spr (0, 240, 176, spr_empty);
#endif


}
