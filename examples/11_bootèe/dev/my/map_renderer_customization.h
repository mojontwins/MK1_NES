// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Use this alongisde map_renderer_complex.h
// (#define MAP_RENDERER_COMPLEX)

// Reaching this point, map_buff has the decompressed map screen.
// You can write as many modifications as you like here:

gpit = 192; while (gpit --) {
	rda = map_buff [gpit];

	if (rda < 5 && (rand8 () & 1)) rda += 16;

	map_buff [gpit] = rda;
}

// Clear monkey when all coins got
if (n_pant == 0 && pobjs == MAX_HOTSPOTS_TYPE_1_0) map_buff [0x68] = 29;
