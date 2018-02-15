// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Add here your code. An object has just been used.
// You may add checks and set variables and stuff.

// Player used object rda on hotspot rdb.

// If object 6 is placed on screen 9 -> enable propellers

if (ht [9] == 6 + 2*HS_USE_OFFS) propellers_on = 1;
