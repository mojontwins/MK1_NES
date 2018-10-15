// NES MK1 v2.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Interactives from code

// For each interactive: NP YX F  (single width)
//                       NP Y X F (double width)
#ifdef DOUBLE_WIDTH
const unsigned char interactives_0 [] = {
	0, 0, 0,

	0xff
};
#else
const unsigned char interactives_0 [] = {
	0, 0, 0, 0,
	0xff
};
#endif
