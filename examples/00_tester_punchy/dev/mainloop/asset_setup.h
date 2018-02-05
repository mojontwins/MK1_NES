// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017

// Rule of thumb: if you make a single level cart, all your assets
// are level 0.

#ifdef MULTI_LEVEL

	c_pal_bg = 		l_pal_bg [level];
	c_pal_fg = 		l_pal_fg [level];
	
	c_ts_tmaps = 	l_ts_tmaps [level];
	c_ts_pals = 	l_ts_pals [level];
	c_behs = 		l_behs [level];
	spr_enems = 	l_spr_enems [level];

	#ifdef MAP_FORMAT_RLE16
	c_map = 		l_map [level];
	#endif
	#ifdef MAP_FORMAT_PACKED
	c_map = 		l_map [level];
	#endif
	#ifdef MAP_WITH_DECORATIONS
		c_decos = 	l_decos [level];
	#endif
	#ifndef DEACTIVATE_KEYS
		c_locks = 	l_locks [level];
		c_max_bolts =
					l_n_bolts [level];
	#endif
	
	c_enems = 		l_enems [level];
	c_hotspots = 	l_hotspots [level];	

#else

	c_pal_bg = 		palts0;
	c_pal_fg = 		palss0;
	
	c_ts_tmaps = 	ts0_tmaps;
	c_ts_pals = 	ts0_pals;
	c_behs = 		behs0;
	spr_enems = 	spr_enems0;

	c_map = 		map_0;
	#ifdef MAP_WITH_DECORATIONS
		c_decos = 	map_0_decos;
	#endif
	c_locks = 		map_0_locks;
	c_max_bolts = 	N_BOLTS_0;
	
	c_enems = 		enems_0;
	c_hotspots = 	hotspots_0;
	
#endif
