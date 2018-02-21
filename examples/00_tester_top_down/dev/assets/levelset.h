
// Initial screen & position, per level

const unsigned char l_scr_ini [] = 				{ 23, 23 };
const unsigned char l_ini_x [] = 				{ 12, 12 };
const unsigned char l_ini_y [] = 				{ 9, 9 };
const unsigned char l_map_w [] = 				{ 1, 1 };

// Ending screen & position, per level (comment if N/A)

/*
const unsigned char l_scr_end [] = 				{ 0, 0 };
const unsigned char l_end_x [] = 				{ 3, 3 };
const unsigned char l_end_y [] = 				{ 3, 3 };
*/

// # of objects. (Comment if N/A)

const unsigned char l_player_max_objects [] =	{ MAX_HOTSPOTS_TYPE_1_0, MAX_HOTSPOTS_TYPE_1_1 };

// # of killable enemies 

const unsigned char l_player_max_enems [] = 	{ 0, 0 };

// Palettes

const unsigned char * const l_pal_bg [] = 		{ palts0, palts1 };
const unsigned char * const l_pal_fg [] = 		{ palss0, palss0 };

// Metatiles

const unsigned char * const l_ts_tmaps [] = 	{ ts0_tmaps, ts0_tmaps };
const unsigned char * const l_ts_pals [] = 		{ ts0_pals, ts0_pals };
const unsigned char * const l_behs [] = 		{ behs0, behs0 };

// Enemy spriteset

const unsigned char * const * const l_spr_enems [] =
												{ spr_enems0, spr_enems0 };

// Map data

#ifdef MAP_FORMAT_PACKED
	const unsigned char * const l_map [] =
#endif
#ifdef MAP_FORMAT_RLE16
	const unsigned char * const * const l_map [] =	
#endif
#ifdef MAP_FORMAT_RLE53
	const unsigned char * const * const l_map [] =	
#endif
												{ map_0, map_1 };

#ifdef MAP_WITH_DECORATIONS
	const unsigned char * const * const l_decos [] =
												{ map_0_decos, map_1_decos };
#endif

#ifndef DEACTIVATE_KEYS
	const unsigned char * const l_locks [] = 	{ map_0_locks, map_1_locks };
	const unsigned char l_n_bolts [] = 			{ N_BOLTS_0, N_BOLTS_1 };
#endif

// Objects

const unsigned char * const l_enems [] = 		{ enems_0, enems_1 };
const unsigned char * const l_hotspots [] = 	{ hotspots_0, hotspots_1 };
