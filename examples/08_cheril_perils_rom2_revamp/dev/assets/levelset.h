
// Initial screen & position, per level

const unsigned char l_scr_ini [] = 				{ 0, 16, 20, 18, 12 };
const unsigned char l_ini_x [] = 				{ 2, 3, 2, 3, 8 };
const unsigned char l_ini_y [] = 				{ 2, 3, 6, 3, 2 };
const unsigned char l_map_w [] = 				{ 6, 8, 4, 6, 6 };

// Ending screen & position, per level (comment if N/A)

/*
const unsigned char l_scr_end [] = 				{ 0, 0 };
const unsigned char l_end_x [] = 				{ 3, 3 };
const unsigned char l_end_y [] = 				{ 3, 3 };
*/

// # of objects. 

const unsigned char l_player_max_objects [] =	{ 0xff, 0xff, 0xff, 0xff, 0xff };

// # of killable enemies 

const unsigned char l_player_max_enems [] = 	{ KILLABLE_ENEMS_4, KILLABLE_ENEMS_1, KILLABLE_ENEMS_3, KILLABLE_ENEMS_0, KILLABLE_ENEMS_2 };

// Palettes

const unsigned char * const l_pal_bg [] = 		{ palts1, palts1, palts3, palts0, palts2 };
const unsigned char * const l_pal_fg [] = 		{ palss1, palss1, palss3, palss0, palss2 };

// Metatiles

const unsigned char * const l_ts_tmaps [] = 	{ ts1_tmaps, ts1_tmaps, ts3_tmaps, ts0_tmaps, ts2_tmaps };
const unsigned char * const l_ts_pals [] = 		{ ts1_pals, ts1_pals, ts3_pals, ts0_pals, ts2_pals };
const unsigned char * const l_behs [] = 		{ behs1, behs1, behs3, behs0, behs2 };

// Enemy spriteset

const unsigned char * const * const l_spr_enems [] =
												{ spr_enems1, spr_enems1, spr_enems3, spr_enems0, spr_enems2 };

// Map data

#ifdef MAP_FORMAT_PACKED
	const unsigned char * const l_map [] =		{ map_4, map_1, map_3, map_0, map_2 };
#endif
#ifdef MAP_FORMAT_RLE16
	const unsigned char * const * const l_map [] =
												{ map_4, map_1, map_3, map_0, map_2 };
#endif
#ifdef MAP_FORMAT_RLE53
	const unsigned char * const * const l_map [] =	
												{ map_4, map_1, map_3, map_0, map_2 };
#endif
#ifdef MAP_FORMAT_CHRROM
	const unsigned char l_map_chr_rom_bank [] = { MAP_04_CHRROM, MAP_01_CHRROM, MAP_03_CHRROM, MAP_00_CHRROM, MAP_02_CHRROM };
	const unsigned int * const l_map [] = 		{ map_04_scr_offsets, map_01_scr_offsets, map_03_scr_offsets, map_00_scr_offsets, map_02_scr_offsets };
#endif
												

#ifdef MAP_WITH_DECORATIONS
	const unsigned char * const * const l_decos [] =
												{ 0, 0, 0, 0, 0 };
#endif

#ifndef DEACTIVATE_KEYS
	const unsigned char * const l_locks [] = 	{ 0, 0, map_03_locks, map_00_locks, 0 };
	const unsigned char l_n_bolts [] = 			{ 1, 1, MAP_03_N_LOCKS, MAP_00_N_LOCKS, 1 };
#endif

// Objects

const unsigned char * const l_enems [] = 		{ enems_4, enems_1, enems_3, enems_0, enems_2 };
const unsigned char * const l_hotspots [] = 	{ hotspots_4, hotspots_1, hotspots_3, hotspots_0, hotspots_2 };

// CHR-ROM bank, for CNROM

#ifdef CNROM
	const unsigned char l_chr_rom_bank [] = 	{ 0, 0, 1, 0, 1 };
#endif

// Interactives

#if defined (ENABLE_INTERACTIVES) && defined (INTERACTIVES_FROM_CODE)
	const unsigned char * const l_interactives [] = 
												{ 0, 0, interactives2 };
#endif
