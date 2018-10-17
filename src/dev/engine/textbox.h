// NES MK1 v2.0
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
	rdct = 0; _y = 12;
	#ifdef DOUBLE_WIDTH
		_x = rdit ? 32 : 0;
		gp_ram = rdm ? 
			((unsigned char *) box_buff) : 
			((rdit ? 192 : 0) + map_buff + (((_y - TOP_ADJUST) >> 1) << 4));
	#else
		_x = 0; 
	gp_ram = rdm ? ((unsigned char *) box_buff) : (map_buff + (((_y - TOP_ADJUST) >> 1) << 4));
	#endif
	
	gpit = 64; while (gpit --) {
		rdt = *gp_ram ++; 
		if (rdct == 0) clear_update_list ();
		if (rdt != 0xff) { 
			_t = rdt + rdm;
			update_list_tile (); 
		}
		#ifdef DOUBLE_WIDTH
			_x = _x + 2; if (_x == 32 || _x == 64) { _x = rdit ? 32 : 0; _y += 2; }
		#else
		_x = (_x + 2) & 0x1f; if (_x == 0) _y += 2;
		#endif
		++ rdct; if (rdct == 4) { ppu_waitnmi (); rdct = 0; }
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
		#ifdef TEXT_BOX_WITH_PORTRAITS
			if (rda) { clear_update_list (); rda = 0; gp_addr = NAMETABLE_BASE + rdm + (rdy << 5); }
		#else
			if (rda) { clear_update_list (); rda = 0; gp_addr = NAMETABLE_BASE + 6 + (rdy << 5); }
		#endif
		if (rdt == '%') rda = 1; else { _n = rdt - 32; ul_putc (); }
		if (rda) { ppu_waitnmi (); ++ rdy; }
	}	
}

void textbox_do (void) {
	// Textbox example, simple.
	// Text must be pointed at by gp_gen.
	// if defined TEXT_BOX_WITH_PORTRAITS, portrait # is in rdd
	// rdd == 0 means no portrait.

	// In double width mode, scroll to the correct screen
	#ifdef DOUBLE_WIDTH
		player_to_pixels ();
		rdit = !!(prx & 0x100);
		rds16 = rdit ? 256 : 0;
		scroll_to ();
	#endif

	rdm = TEXT_BOX_FRAME_TILE_OFFSET; textbox_frame ();
	#ifdef TEXT_BOX_WITH_PORTRAITS
	if (rdd) {
		oam_meta_spr (
			44, 103,
			256-32,
			spr_hs [rdd]);
		rdm = 8;
	} else rdm = 6;
	#endif	

	#ifdef DOUBLE_WIDTH
		NAMETABLE_BASE = rdit ? 0x2400 : 0x2000;
	#endif

	textbox_draw_text ();
	while (1) {
		ppu_waitnmi ();
		pad_read (); if (pad_this_frame & (PAD_A|PAD_B)) break;
	}
	#ifdef TEXT_BOX_WITH_PORTRAITS
	if (rdd) oam_hide_rest (256-32);
	#endif
	rdm = 0; textbox_frame ();
	clear_update_list ();

	#ifdef DOUBLE_WIDTH
		rds16 = prx - 124;
		if (rds16 < 0) rds16 = 0;
		if (rds16 > 256) rds16 = 256;
		rds16 &= 0xfff8;
		scroll_to ();
	#endif
}

#ifdef TEXT_BOX_DIALOGUES
	void textbox_dialogue_do (unsigned char dfrom, unsigned char dto) {
		for (gpjt = dfrom; gpjt <= dto; gpjt ++) {
			rdd = dialogue_portraits [gpjt];
			gp_gen = dialogue_texts [gpjt];
			textbox_do ();
		}
	}
#endif
