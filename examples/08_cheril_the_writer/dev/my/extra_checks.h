// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Add here your code.  This gets run right before checking
// for the level's end condition. The previous frame is on
// screen. 

// Do whatever you need. Everything you add here gets executed
// each frame!

// Whenever you want to WIN the game just set win_level = 1
// (Don't forget to define WIN_LEVEL_CUSTOM in config.h)

if (c_max_enems == pkilled) {
	if (
		en_cttouched [0] == 0 &&
		en_cttouched [1] == 0 &&
		en_cttouched [2] == 0
	) win_level = 1;
}

// Some palete cycling

if (level < 2 && (frame_counter & 7) == 0 ) {
	
	rda = pal_cycle [2];
	pal_cycle [2] = pal_cycle [1];	
	pal_cycle [1] = pal_cycle [0];	
	pal_cycle [0] = rda;			
	
	pal_col (13, pal_cycle [2]);
	pal_col (14, pal_cycle [1]);
	pal_col (15, pal_cycle [0]);
}
