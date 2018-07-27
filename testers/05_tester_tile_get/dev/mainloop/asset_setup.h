// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Rule of thumb: if you make a single level cart, all your assets
// are level 0.

#ifdef MULTI_LEVEL

	c_pal_bg = 		l_pal_bg [level];
	c_pal_fg = 		l_pal_fg [level];
	
	c_ts_tmaps = 	l_ts_tmaps [level];
	c_ts_pals = 	l_ts_pals [level];
	c_behs = 		l_behs [level];
	spr_enems = 	l_spr_enems [level];

	c_map = 		l_map [level];
	#if defined (MAP_FORMAT_RLE53_CHRROM) || defined (MAP_FORMAT_RLE44_CHRROM)
		c_map_chr_rom_bank = 
					l_map_chr_rom_bank [level];
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

	c_max_enems = 	l_player_max_enems [level];

	c_map_w = 		l_map_w [level];

	#if defined (ENABLE_INTERACTIVES) && defined (INTERACTIVES_FROM_CODE)
		c_interactives = 
					l_interactives [level];
	#endif
	
#else

	c_pal_bg = 		palts0;
	c_pal_fg = 		palss0;
	
	c_ts_tmaps = 	ts0_tmaps;
	c_ts_pals = 	ts0_pals;
	c_behs = 		behs0;
	spr_enems = 	spr_enems0;

	#if defined (MAP_FORMAT_RLE53_CHRROM) || defined (MAP_FORMAT_RLE44_CHRROM)
		c_map = 	map_00_scr_offsets;
		c_map_chr_rom_bank = 
					MAP_00_CHRROM;
	#else
		c_map = 	map_0;
	#endif		

	#ifdef MAP_WITH_DECORATIONS
		c_decos = 	map_0_decos;
	#endif
	#ifndef DEACTIVATE_KEYS
		c_locks = 	map_0_locks;
		c_max_bolts = 	
					N_BOLTS_0;
	#endif
	
	c_enems = 		enems_0;
	c_hotspots = 	hotspots_0;
	
	#ifdef KILLABLE_ENEMS_0
		c_max_enems = 	
					KILLABLE_ENEMS_0;
	#else
		c_max_enems = 	99;
	#endif

	c_map_w = MAP_W;

	#if defined (ENABLE_INTERACTIVES) && defined (INTERACTIVES_FROM_CODE)
		c_interactives =
					interactives_0;
	#endif

#endif
