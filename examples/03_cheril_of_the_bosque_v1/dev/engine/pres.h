// Cutscene

unsigned char cutsi, cutc, cutf, cutff;
unsigned char *cuts_text;

void cutscene (unsigned char *cuts_tsmap, unsigned char *cuts_pals, unsigned char *cuts_text) {
	cls ();
	
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

void title (void) {
	// Shows title screen.
	// Returns 1 = normal, 0 = hidden game
	cls ();
	pal_bg (mypal_title);
	tsmap = (unsigned char *) (tstitle_tmaps);
	tileset_pals = (unsigned char *) (tstitle_pals);
	un_rle_screen ((unsigned char *) scr_rle_0);
	pr_str (10, 18, "PRESS START!");
	pr_str (4, 27, "(C) 2015 THE MOJON TWINS");
	pr_str (8, 28 , "MUSIC BY DAVIDIAN");

	pal_bright (0);
	ppu_on_all ();
	while (pad_poll (0));

	fade_delay = 4;
	fade_in ();

	music_play (m_title);
	while (1) {
		i = pad_poll (0);
		if (i & PAD_START) { gpit = 1; break; }
	}
	music_stop ();

	fade_out ();
	ppu_off ();
}

unsigned char game_over_scr (void) {
	cls ();
	pal_bg (mypal_title);
	pal_spr (mypal_game_fg0);
	
	rda = 0;
	
	if (pcontinues) {
		pr_str (11, 13, "GAME OVER!");
		pr_str (11, 15, "CREDITS ");
		vram_put (16 + (pcontinues / 10));
		vram_put (16 + (pcontinues % 10));
		pr_str (7, 17, "CONTINUE?  YES  NO"); 
		// Horz: NO = 12*8, YES = 17*8
	} else pr_str (11, 15, "GAME OVER!");

	pal_bright (0);
	ppu_on_all ();
	while (pad_poll (0));
	
	fade_delay = 4;
	fade_in ();

	music_play (m_gover);
	if (pcontinues) {
		while (1) {
			i = pad_poll (0);
			if ((i & PAD_LEFT) && !rda) rda = 1;
			if ((i & PAD_RIGHT) && rda) rda = 0;
			if (i & PAD_SELECT) rda = 1 - rda;
			if ((i & PAD_A) || (i & PAD_B) || (i & PAD_START)) break;

			rdb = 1 - rda;
			oam_spr ((17 + (rdb << 2) + rdb) << 3, 126, 164, 0, 0);
			ppu_waitnmi ();
		}
	} else {
		while (0 == pad_poll (0));
	}

	music_stop ();

	fade_out ();
	ppu_off ();

	return rda;
}


// Mojontwins logo
const unsigned char spr_mt_logo [] = {
	0, 0, 152, 3, 8, 0, 153, 3, 16, 0, 154, 2, 24, 0, 155, 2, 32, 0, 156, 3, 40, 0, 157, 3,
	0, 8, 158, 3, 8, 8, 159, 3, 16, 8, 160, 2, 24, 8, 161, 2, 32, 8, 162, 3, 40, 8, 163, 3,
	128	
};
signed int lower_end;
void __fastcall__ credits (void) {
	pal_bg (mypal_title);
	pal_spr (mypal_title);
	cls ();
	oam_clear (); scroll (0, 0);
	
	lower_end = 0; rdy = 240;

	pr_str (6, 22, "CHERIL OF THE BOSQUE");
	pr_str (0, 24, "COPYLEFT 2015 BY THE MOJON TWINS");
	pr_str (0, 25, "DESIGN & PORT BY THE MOJON TWINS");
	pr_str (3, 26, "NESLIB & SFX CODE BY SHIRU");
	pr_str (7, 27, "NES OGT BY DAVIDIAN");
	
	pal_bright (0);
	ppu_on_all ();
	fade_delay = 4;
	fade_in ();
	while (!(pad_poll (0) & PAD_START) && lower_end < 300) {
		oam_meta_spr (102, rdy, 0, spr_mt_logo);
		if (rdy > 112) rdy --;
		ppu_waitnmi ();
		lower_end ++;
	};
	fade_out ();
	
	ppu_off ();
	oam_clear ();
	cls ();
}
