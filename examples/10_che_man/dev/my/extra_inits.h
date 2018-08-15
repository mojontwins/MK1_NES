// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// extra_inits.h
// Extra stuff you have to initialize before the level starts

pfacing = CELL_FACING_LEFT;
first_text = 1;
first_batucada = 1;
first_medallion = 1;
first_totem = 1;
c_max_enems = MAX_ENEMS_TYPE_3_0;
pjewels = 0;
opjewels = 0xff;

springs_on = (level == 1);

// Remove part of the hud for level 0
if (level == 0) { _x = 1; _y = 4; pr_str ("        "); }
