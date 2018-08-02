
// Initial screen & position, per level

const unsigned char l_scr_ini [] = 				{ 
													20, 0, 0, 	// Benny Hill Zone
													15, 24, 0, 	// Broken Fridge Zone
												};
const unsigned char l_ini_x [] = 				{ 
													3, 2, 3,	// Benny Hill Zone
													2, 12, 3, 	// Broken Fridge Zone
												};
const unsigned char l_ini_y [] = 				{ 	
													3, 3, 3,	// Benny Hill Zone
													3, 3, 3,	// Broken Fridge Zone
												};
const unsigned char l_map_w [] = 				{ 
													10, 10, 3,	// Benny Hill Zone 
													15, 25, 3,	// Broken Fridge Zone
												};
const unsigned char l_map_h [] = 				{
													3, 3, 1,	// Benny Hill Zone
													2, 1, 1, 	// Broken Fridge Zone
												};

// Ending screen & position, per level (comment if N/A)

/*
const unsigned char l_scr_end [] = 				{ 0, 0 };
const unsigned char l_end_x [] = 				{ 3, 3 };
const unsigned char l_end_y [] = 				{ 3, 3 };
*/

// # of objects. 

const unsigned char l_player_max_objects [] =	{ 
													MAX_HOTSPOTS_TYPE_1_00, MAX_HOTSPOTS_TYPE_1_01, 0, 	// Benny Hill Zone
													MAX_HOTSPOTS_TYPE_1_10, MAX_HOTSPOTS_TYPE_1_11, 0, 	// Broken Fridge Zone
												};

// # of killable enemies 

const unsigned char l_player_max_enems [] = 	{ 
													KILLABLE_ENEMS_00, KILLABLE_ENEMS_01, 1, // Benny Hill Zone
													KILLABLE_ENEMS_10, KILLABLE_ENEMS_11, 1, // Broken Fridge Zone
												};

// Palettes

const unsigned char * const l_pal_bg [] = 		{ 
													palts0, palts0, palts0, // Benny Hill Zone
													palts1, palts1, palts1, // Broken Fridge Zone
													palts2, palts2, palts2,
													palts3, palts3, palts3,
												};
const unsigned char * const l_pal_fg [] = 		{ 
													palss0, palss0, palss0, // Benny Hill Zone
													palss0, palss0, palss0, // Broken Fridge Zone
													palss0, palss0, palss0,
													palss0, palss0, palss0
												};

// Metatiles

const unsigned char * const l_ts_tmaps [] = 	{	
													ts0_tmaps, ts0_tmaps, ts0_tmaps, // Benny Hill Zone
													ts1_tmaps, ts1_tmaps, ts1_tmaps, // Broken Fridge Zone
													ts2_tmaps, ts2_tmaps, ts2_tmaps,  
													ts3_tmaps, ts3_tmaps, ts3_tmaps,  
												};
const unsigned char * const l_ts_pals [] = 		{ 
													ts0_pals, ts0_pals, ts0_pals, // Benny Hill Zone
													ts1_pals, ts1_pals, ts1_pals, // Broken Fridge Zone
													ts2_pals, ts2_pals, ts2_pals, 
													ts3_pals, ts3_pals, ts3_pals, 
												};
const unsigned char * const l_behs [] = 		{
													behs0, behs0, behs0, // Benny Hill Zone
													behs1, behs1, behs1, // Broken Fridge Zone
												};

// Enemy spriteset

const unsigned char * const * const l_spr_enems [] =
												{
													spr_enems0, spr_enems0, spr_enems0, // Benny Hill Zone
													spr_enems1, spr_enems1, spr_enems1, // Broken Fridge Zone
												};

// Map data

#ifdef MAP_FORMAT_PACKED
	const unsigned char * const l_map [] =		{ map_0, map_1 };
#endif
#if defined (MAP_FORMAT_RLE16) || defined (MAP_FORMAT_RLE53) || defined (MAP_FORMAT_RLE44)
	const unsigned char * const * const l_map [] =	
												{ map_0, map_1 };
#endif
#if defined (MAP_FORMAT_RLE53_CHRROM) || defined (MAP_FORMAT_RLE44_CHRROM)
	const unsigned char l_map_chr_rom_bank [] = { 
													MAP_00_CHRROM, MAP_01_CHRROM, MAP_02_CHRROM,  // Benny Hill Zone
													MAP_03_CHRROM, MAP_04_CHRROM, MAP_05_CHRROM,  // Broken Fridge Zone
												};
	const unsigned int * const l_map [] = 		{
													map_00_scr_offsets, map_01_scr_offsets, map_02_scr_offsets, // Benny Hill Zone
													map_03_scr_offsets, map_04_scr_offsets, map_05_scr_offsets, // Broken Fridge Zone
												};
#endif

#ifdef MAP_WITH_DECORATIONS
	#if defined (MAP_FORMAT_PACKED) || defined (MAP_FORMAT_RLE16) 
		const unsigned char * const * const l_decos [] =
												{ map_0_decos, map_1_decos };
	#else
		// 0 = no decos in this level's map, 1 = decos.
		const unsigned char l_decos [] = 		{ 1, 1 };												
	#endif
#endif

#ifndef DEACTIVATE_KEYS
	const unsigned char * const l_locks [] = 	{ map_0_locks, map_1_locks };
	const unsigned char l_n_bolts [] = 			{ N_BOLTS_0, N_BOLTS_1 };
#endif

// Objects

#ifdef ENEMS_IN_CHRROM
	// change 'base_address' for the actual base address in CHR-ROM

	const unsigned char l_enems_chr_rombank [] = 	{ 
														3, 3, 3, // Benny Hill Zone
														3, 3, 3, // Broken Fridge Zone
													};
	const unsigned int l_enems [] = 				{ 
														4096 + WORK_ENEMS00_H_BIN_OFFS,	// Benny Hill Zone
														4096 + WORK_ENEMS01_H_BIN_OFFS,
														4096 + WORK_ENEMS02_H_BIN_OFFS,
														4096 + WORK_ENEMS10_H_BIN_OFFS,	// Broken Fridge Zone
														4096 + WORK_ENEMS11_H_BIN_OFFS,
														4096 + WORK_ENEMS12_H_BIN_OFFS,
													};
	const unsigned int l_hotspots [] = 				{ 
	 													4096 + WORK_ENEMS00_H_BIN_OFFS + HOTSPOTS_OFFSET_00, // Benny Hill Zone
														4096 + WORK_ENEMS01_H_BIN_OFFS + HOTSPOTS_OFFSET_01,
														4096 + WORK_ENEMS02_H_BIN_OFFS + HOTSPOTS_OFFSET_02,
														4096 + WORK_ENEMS10_H_BIN_OFFS + HOTSPOTS_OFFSET_10, // Benny Hill Zone
														4096 + WORK_ENEMS11_H_BIN_OFFS + HOTSPOTS_OFFSET_11,
														4096 + WORK_ENEMS12_H_BIN_OFFS + HOTSPOTS_OFFSET_12,
													};
#else
const unsigned char * const l_enems [] = 		{ enems_0, enems_1 };
const unsigned char * const l_hotspots [] = 	{ hotspots_0, hotspots_1 };
#endif

// CHR-ROM bank, for CNROM

#ifdef CNROM
	const unsigned char l_chr_rom_bank [] = 	{ 
													0, 0, 0, // Benny Hill Zone 
													0, 0, 0, // Broken Fridge Zone 
												};
#endif

// Interactives

#if defined (ENABLE_INTERACTIVES) && defined (INTERACTIVES_FROM_CODE)
	const unsigned char * const l_interactives [] = 
												{ interactives0, interactives1 };
#endif

// Music track

const unsigned char l_music [] = 				{ 0, 0, 0 };
