// NES MK1 v2.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// extra_inits.h
// Extra stuff you have to initialize before the level starts

// Testing the "several engines for vertical in sideview mode"

// vertical_engine_type lets you choose, but the chosen engine
// must be activated in config.h.

// Level 0: Jump, Level 1: Jet Pac.
// vertical_engine_type = (level ? ENGINE_TYPE_JET_PAC : ENGINE_TYPE_JUMP);

// Level 0: Jump, Level 1: Swim.
// vertical_engine_type = (level ? ENGINE_TYPE_SWIM : ENGINE_TYPE_JUMP);

// Level 0: Auto Jump, Level 1: Jet Pac
// vertical_engine_type = (level ? ENGINE_TYPE_JET_PAC : ENGINE_TYPE_AUTO_JUMP);
