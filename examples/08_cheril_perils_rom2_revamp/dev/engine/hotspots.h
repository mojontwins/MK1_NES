// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// hotspots.h
// Hotspot management

void hotspots_load (void) {
	// Copies hotspots from ROM to RAM and initializes them
#ifdef ENEMS_IN_CHRROM
	bankswitch (l_enems_chr_rombank [level]);
	vram_adr (c_hotspots);
	rda = *((unsigned char *) (0x2007)); 	// Dummy read.
#else
	gp_gen = (unsigned char *) c_hotspots;
#endif

	for (gpit = 0; gpit < MAP_SIZE; gpit ++) {
#ifdef ENEMS_IN_CHRROM
		ht [gpit] = *((unsigned char *) (0x2007));
		hyx [gpit] = *((unsigned char *) (0x2007));
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
	if (hrt == HOTSPOT_TYPE_RESONATOR && res_on) rda ++;
#endif

	oam_index = oam_meta_spr (
		hrx, hry + SPRITE_ADJUST, 
		oam_index, 
		spr_hs [rda]
	);
}

void hotspots_create (void) {
	
#if defined (HOTSPOTS_DYNAMIC) || defined (ENEMS_IN_CHRROM)

	if (ht [n_pant] && hact [n_pant]) {
		hrt = ht [n_pant];
		hrx = hyx [n_pant] << 4;
		hry = hyx [n_pant] & 0xf0;
	} else hrt = 0;

#else

	gp_gen = (unsigned char *) (c_hotspots + (n_pant << 1));
	hrt = *gp_gen ++; rda = *gp_gen; 
	if (hrt && hact [n_pant]) {
		hry = rda & 0xf0; hrx = rda << 4;
	} else hrt = 0;

#endif
}
