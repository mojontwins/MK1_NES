// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Add here your code.  This gets run right before checking
// for the level's end condition. The previous frame is on
// screen. 

// Do whatever you need. Everything you add here gets executed
// each frame!

// Whenever you want to WIN the game just set win_level = 1
// (Don't forget to define WIN_LEVEL_CUSTOM in config.h)

win_level = pstars;

// Custom sprite

if (n_pant == 0 && pobjs < MAX_HOTSPOTS_TYPE_1_0) {
	oam_index = oam_meta_spr (
		0x78, 0x64 + SPRITE_ADJUST,
		oam_index,
		ssit_05
	);
}

// Palette cycling

if ((frame_counter & 3) == 0) {
	rda = pal_cycle [2];
	pal_cycle [2] = pal_cycle [1];	
	pal_cycle [1] = pal_cycle [0];	
	pal_cycle [0] = rda;			

	pal_col (13, pal_cycle [0]);
	pal_col (14, pal_cycle [1]);
	pal_col (15, pal_cycle [2]);
}
