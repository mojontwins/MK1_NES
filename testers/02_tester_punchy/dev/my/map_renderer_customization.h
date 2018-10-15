// NES MK1 v2.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Use this alongisde map_renderer_complex.h
// (#define MAP_RENDERER_COMPLEX)

// Reaching this point, map_buff has the decompressed map screen.
// You can write as many modifications as you like here:

rda = n_pant & 1;
gp_gen = bgs [level] + (rda << 7) + (rda << 6);
gpit = 192; while (gpit --) {
	if (map_buff [gpit] == 0) map_buff [gpit] = gp_gen [gpit];
}
