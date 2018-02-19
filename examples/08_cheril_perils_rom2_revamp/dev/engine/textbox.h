// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Simple text box which works with ppu_on.

// Use the correct metatile values for your game!
// Remember that the frame is 12 tiles wide. 
// Please respect the 0xff byte padding.
const unsigned char box_buff [] = {
	0xff, 0xff, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0xff, 0xff,
	0xff, 0xff, 0x03, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x05, 0xff, 0xff, 
	0xff, 0xff, 0x03, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x05, 0xff, 0xff, 
	0xff, 0xff, 0x06, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x08, 0xff, 0xff,
};

void textbox_frame (void) {
	// Draws or clears the frame upon the value of rdm
	rdct = 0; rdx = 0; rdy = 12;
	gp_ram = rdm ? ((unsigned char *) box_buff) : (map_buff + (((rdy - TOP_ADJUST) >> 1) << 4));
	gpit = 64; while (gpit --) {
		rdt = *gp_ram ++; 
		if (rdct == 0) clear_update_list ();
		if (rdt != 0xff) { update_list_tile (rdx, rdy, rdt + rdm); }
		rdx = (rdx + 2) & 0x1f; if (rdx == 0) rdy = rdy + 2;
		rdct ++; if (rdct == 4) { ppu_waitnmi (); rdct = 0; }
	}
}

void textbox_draw_text (void) {
	// Paints text inside the frame.
	// Text must be pointed at by gp_gen.
	// Text lines can be broken by %. 
	// If a line is more than 20 chars wide, ¡BUM!
	// Max is 6 lines.

	rda = 1; // New line marker!
	rdy = 13;
	while (rdt = *gp_gen ++) {
		if (rda) { clear_update_list (); rda = 0; gp_addr = 0x2000 + 6 + (rdy << 5); }
		if (rdt == '%') rda = 1; else ul_putc (rdt - 32);
		if (rda) { ppu_waitnmi (); rdy ++; }
	}	
}

void textbox_do (void) {
	// Textbox example, simple.
	// Text must be pointed at by gp_gen.
	rdm = TEXT_BOX_FRAME_TILE_OFFSET; textbox_frame ();
	textbox_draw_text ();
	while (1) {
		ppu_waitnmi ();
		pad_read (); if (pad_this_frame & (PAD_A|PAD_B)) break;
	}
	rdm = 0; textbox_frame ();
	clear_update_list ();
}
