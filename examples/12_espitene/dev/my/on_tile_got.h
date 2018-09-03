// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Add here your code. An TILE_GET has just been got.
// You may add checks and set variables and stuff.

// The value of the tile just got is QTILE (cx1, cy1)

prings ++;
if (prings == 100) {
	prings = 0;
	plife ++;
	sfx_play (SFX_USE, 0);
}
