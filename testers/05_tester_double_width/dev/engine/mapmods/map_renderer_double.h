// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Map renderer double uses map_renderer_complex.

void draw_scr (void) {
	w_pant = n_pant;
	nametable_base = 0x2000;
	buff_offset = 0;
	draw_half_scr ();
	n_pant ++;
	nametable_base = 0x2400;
	buff_offset = 192;
	draw_half_scr ();
	n_pant = w_pant; 
}
