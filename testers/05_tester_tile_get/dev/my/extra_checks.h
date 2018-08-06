// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Add here your code.  This gets run right before checking
// for the level's end condition. The previous frame is on
// screen. 

// Do whatever you need. Everything you add here gets executed
// each frame!

// Whenever you want to WIN the game just set win_level = 1
// (Don't forget to define WIN_LEVEL_CUSTOM in config.h)

if (l_player_max_objects [level] && pobjs == l_player_max_objects [level]) win_level = 1;
if (level_act == 2 && en_flags [0] && en_cttouched [0] == 0) win_level = 1;

// Respawn time refills
if (level_world == 3 && ht [n_pant] == HOTSPOT_TYPE_TIME) hact [n_pant] = 1;
