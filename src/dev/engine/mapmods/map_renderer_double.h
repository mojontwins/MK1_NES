// NES MK1 v2.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Map renderer double uses map_renderer_complex.

void draw_scr (void) {
	nametable_base = 0x2000;
	buff_offset = attr_table_offset = 0;
	draw_half_scr ();

	#ifdef SINGLE_SCREEN_SUPPORT
		if (scr_single) return;
	#endif
	
	++ n_pant;
	nametable_base = 0x2400;
	buff_offset = 192;
	attr_table_offset = 64;
	draw_half_scr ();
	// n_pant &= 0xfe; 
	-- n_pant;
}
