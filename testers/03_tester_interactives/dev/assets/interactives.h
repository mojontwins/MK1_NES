// NES MK1 v2.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Interactives from code

// For each interactive: NP YX F  (single width)
//                       NP Y X F (double width)

#ifdef DOUBLE_WIDTH
	const unsigned char interactives_0 [] = {
		0, 0x09, 0x02,        8,	// screen 0, SPR Cheril
		2, 0x08, 0x08, 0x80 | 5,	// screen 1, CONT pedestal
		4, 0x08, 0x04, 0x80 | 1,	// screen 2, CONT bottle
		6, 0x04, 0x0e, 0x80 | 2,	// screen 3, CONT eye
		8, 0x02, 0x01, 0x80 | 3,	// screen 4, CONT skull
		8, 0x03, 0x0d, 0x80 | 4,	//       and CONT puff of smoke
		8, 0x09, 0x1c,        8,	//       and SPR Cheril
		0xff
	};
#else
	const unsigned char interactives_0 [] = {
		0, 0x92,        8,		// screen 0, SPR Cheril
		2, 0x88, 0x80 | 5,		// screen 1, CONT pedestal
		4, 0x84, 0x80 | 1,		// screen 2, CONT bottle
		6, 0x4e, 0x80 | 2,		// screen 3, CONT eye
		8, 0x21, 0x80 | 3,		// screen 4, CONT skull
		8, 0x3d, 0x80 | 4,		//       and CONT puff of smoke
		9, 0x9c,        8,		//       and SPR Cheril
		0xff
	};
#endif
