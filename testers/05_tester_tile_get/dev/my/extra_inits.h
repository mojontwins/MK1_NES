// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// extra_inits.h
// Extra stuff you have to initialize before the level starts

prings = 0; oprings = 0xff;

pal_cycle [0] = c_pal_bg [13];
pal_cycle [1] = c_pal_bg [14];
pal_cycle [2] = c_pal_bg [15];

timer = 20;

toggle_switch = 0;

elec_state_ct = 5;
elec_state = 0;

somari_state = somari_ct = 0;
somari_life = 8;
