// Cutscene

unsigned char cutsi, cutc, cutf, cutff;
unsigned char *cuts_text;

void cutscene (unsigned char *cuts_tsmap, unsigned char *cuts_pals, unsigned char *cuts_text) {
	cls ();
	
	pal_bg (mypal_cuts);

	for (cutsi = 0; cutsi < 64; cutsi ++) attr_table [cutsi] = 0xff;
	if (cuts_tsmap && cuts_pals) {
		// Draw cutscene graphics
		rdx = 8; rdy = 6;
		tsmap = cuts_tsmap;
		tileset_pals = cuts_pals;
		for (cutsi = 0; cutsi < 32; cutsi ++) {
			draw_tile (rdx, rdy, cutsi);
			rdx += 2; if (rdx == 24) { rdx = 8; rdy += 2; }
		}
	}
	vram_write (attr_table, 0x23c0, 64);

	pal_bright (0);
	ppu_on_all ();
	while (pad_poll (0));

	fade_delay = 4;
	fade_in ();

	// do
	gp_tmap = cuts_text;
	cutf = 1;
	update_index = 0;
	clear_update_list ();
	rdx = 1; rdy = 16; cutff = 0;
	
	set_vram_update (UPDATE_LIST_SIZE, update_list);
	music_play (m_cuts);

	while (cutc = *gp_tmap ++) {
		if (cutc == '*') {
			rdy += 2;
			rdx = 1;
			if (rdy == 16 + 10) {
				while (1) {
					ppu_waitnmi ();
					
					clear_update_list ();	
					
					i = pad_poll (0);
					if (i & PAD_A || i & PAD_B) break;
					if (i & PAD_START) {
						cutf = 0;
						break;
					}
				}
				while (1) {
					ppu_waitnmi ();
					i = pad_poll (0);	
					if (!(i & PAD_A || i & PAD_B || i & PAD_START)) break;	
				}
				for (cutsi = 0; cutsi < 5; cutsi ++) {
					update_index = 0;
					gp_addr = 0x2000 + ((cutsi + cutsi + 16) << 5) + 1;
					for (rdx = 0; rdx < 30; rdx ++) {
						update_list [update_index ++] = MSB (gp_addr);
						update_list [update_index ++] = LSB (gp_addr ++);
						update_list [update_index ++] = 0;
					}
					ppu_waitnmi ();
				}
				rdy = 16;
				rdx = 1;
				cutff = 0;
				update_index = 0; 
				clear_update_list ();				
			}
		} else {
			gp_addr = 0x2000 + (rdy << 5) + rdx;
			update_list [update_index++] = MSB(gp_addr);		
			update_list [update_index++] = LSB(gp_addr);
			update_list [update_index++] = cutc - 32;
			rdx = rdx + 1;

			if (cutff) {
				if (update_index == 30) {
					ppu_waitnmi ();
					update_index = 0;
					clear_update_list ();
				}
			} else {
				for (cutsi = 0; cutsi < 4; cutsi ++) ppu_waitnmi ();
				update_index = 0;
				clear_update_list ();		
			}
		}

		i = pad_poll (0);
		if (i & PAD_A || i & PAD_B) cutff = 1;
		if (i & PAD_START || !cutf) { 
			cutf = 0;
			break;	
		}
	}
	ppu_waitnmi (); // Show rogue chars

	set_vram_update (0, 0);

	// Wait button
	while (cutf) {
		i = pad_poll (0);
		if (i & PAD_A || i & PAD_B || i & PAD_START) break;
		ppu_waitnmi ();
	}

	music_stop ();
	fade_out ();
	ppu_off ();
}

unsigned char game_over_scr (void) {
	cls ();
	pal_bg (mypal_cuts);
	
	rda = 0;
	
	pr_str (11, 15, "GAME OVER!");

	pal_bright (0);
	ppu_on_all ();
	while (pad_poll (0));
	
	fade_delay = 4;
	fade_in ();

	music_play (m_gover);
	while (0 == pad_poll (0));
	
	music_stop ();

	fade_out ();
	ppu_off ();

	return rda;
}
