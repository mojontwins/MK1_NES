// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// hotspots.h
// Hotspot management

void hotspots_load (void) {
	// Copies hotspots from ROM to RAM and initializes them
#ifdef ENEMS_IN_CHRROM
	bankswitch (l_enems_chr_rombank [level]);
	vram_adr (c_hotspots);
	rda = VRAM_READ; 	// Dummy read.
#else
	gp_gen = (unsigned char *) c_hotspots;
#endif

	for (gpit = 0; gpit < MAP_SIZE; gpit ++) {
#ifdef ENEMS_IN_CHRROM
		ht [gpit] = VRAM_READ;
		hyx [gpit] = VRAM_READ;
#elif defined (HOTSPOTS_DYNAMIC)
		ht [gpit] = *gp_gen ++;
		hyx [gpit] = *gp_gen ++;
#endif
		hact [gpit] = 1;
	}
}

void hotspots_paint (void) {
	#if defined (ENABLE_EASY_OBJECTS) && defined (HS_TYPE_A)
		if (hrt >= HS_OBJ_MIN + 2*HS_USE_OFFS && hrt <= HS_OBJ_MAX + 2*HS_USE_OFFS) 
			rda = hrt - 2*HS_USE_OFFS;
		else if (hrt >= HS_OBJ_MIN + HS_USE_OFFS && hrt <= HS_OBJ_MAX + HS_USE_OFFS) 
			rda = HS_OBJ_EMPTY;
		else
	#endif

	rda = hrt;

	#ifdef ENABLE_RESONATORS
		if (hrt == HOTSPOT_TYPE_RESONATOR && res_on) rda = HOTSPOT_TYPE_RESONATOR_ON;
	#endif

	oam_index = oam_meta_spr (
		#ifdef DOUBLE_WIDTH
			hrx - scroll_x,
		#else
			hrx, 
		#endif
		hry + SPRITE_ADJUST, 
		oam_index, 
		spr_hs [rda]
	);
}

void hotspots_create (void) {	
	#if defined (HOTSPOTS_DYNAMIC) || defined (ENEMS_IN_CHRROM)
		if (ht [HS_CUR_PANT] && hact [HS_CUR_PANT]) {
			hrt = ht [HS_CUR_PANT];
			hrx = hyx [HS_CUR_PANT] << 4;
			hry = hyx [HS_CUR_PANT] & 0xf0;
		} else hrt = 0;
	#else
		gp_gen = (unsigned char *) (c_hotspots + (HS_CUR_PANT << 1));
		//hrt = *gp_gen ++; rda = *gp_gen; 
		SET_FROM_PTR (hrt, gp_gen); gp_gen ++;
		SET_FROM_PTR (rda, gp_gen);
		if (hrt && hact [HS_CUR_PANT]) {
			hry = rda & 0xf0; hrx = rda << 4;
		} else hrt = 0;
	#endif
}
