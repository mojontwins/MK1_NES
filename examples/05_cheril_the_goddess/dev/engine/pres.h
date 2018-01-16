// Cutscene

void fixed_screen_wait (void) {
	batin ();
	while (0 == pad_poll (0));
	batout ();
	bankswitch (0);
}

void game_over_scr (void) {
	cls ();
	pal_bg (mypal_cuts);
	bankswitch (1);
	rda = 0;
	pr_str (11, 15, "GAME OVER!");
	fixed_screen_wait ();
}

void manual (void) {
	pal_bg (mypal_cuts);
	bankswitch (2);
	unrle_vram (manual_rle, 0x2000);
	fixed_screen_wait ();
}

void title (void) {
	pal_bg (mypal_cuts);
	bankswitch (1);

	unrle_vram (title_rle, 0x2000);

	pr_str (10, 21, "PRESS START!");
	if (game_on) pr_str (4, 22, "PRESS SELECT TO CONTINUE");
	pr_str (4, 27, "(C) 2015 THE MOJON TWINS");

	batin ();

	music_play (m_intro);
	reset_all = 1;
	while (1) {
		i = pad_poll (0);
		if (i & PAD_START) break;
		if (i & PAD_SELECT) {
			if (game_on) {
				reset_all = 0;
				break;
			}
		}
	}
	music_stop ();

	batout ();
	bankswitch (0);
}

unsigned char cutsi, cutc, cutf, cutff;
unsigned char *cuts_text;

void cutscene (const unsigned char *cuts_text, const unsigned char *rle) {
	pal_bg (mypal_cuts);
	
	unrle_vram (rle, 0x2000);

	batin ();	
	music_play (m_cuts);

	// do
	gp_tmap = (unsigned char *) cuts_text;
	cutf = 1;
	clear_update_list ();
	rdx = 1; rdy = 16; cutff = 0;
	
	set_vram_update (UPDATE_LIST_SIZE, update_list);
	//music_play (m_cuts);

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
					i = pad_poll (0); if (!(i & PAD_A || i & PAD_B || i & PAD_START)) break;	
				}
				for (cutsi = 0; cutsi < 5; cutsi ++) {
					update_index = 0;
					gp_addr = 0x2000 + ((cutsi + cutsi + 16) << 5) + 1;
					for (rdx = 0; rdx < 30; rdx ++) {
						ul_putc (0);
					}
					ppu_waitnmi ();
				}
				rdy = 16;
				rdx = 1;
				cutff = 0;
				clear_update_list ();				
			}
		} else {
			gp_addr = 0x2000 + (rdy << 5) + rdx;
			ul_putc (cutc - 32);
			rdx = rdx + 1;

			if (cutff) {
				if (update_index == 30) {
					ppu_waitnmi ();
					clear_update_list ();
				}
			} else {
				for (cutsi = 0; cutsi < 4; cutsi ++) ppu_waitnmi ();
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
	batout ();	
}

// Mojontwins logo
const unsigned char spr_mt_logo [] = {
	0, 0, 0, 0, 8, 0, 1, 0, 16, 0, 2, 1, 24, 0, 3, 1, 32, 0, 4, 0, 40, 0, 5, 0,
	0, 8, 6, 0, 8, 8, 7, 0, 16, 8, 8, 1, 24, 8, 9, 1, 32, 8, 10, 0, 40, 8, 11, 0,
	128	
};
signed int lower_end;
void __fastcall__ credits (void) {
	bankswitch (1);
	pal_bg (mypal_cuts);
	pal_spr (mypal_cuts);
	cls ();
	oam_clear (); scroll (0, 0);
	
	lower_end = 0; rdy = 240;

	pr_str (7, 22, "CHERIL THE GODDESS");
	pr_str (0, 24, "COPYLEFT 2015 BY THE MOJON TWINS");
	pr_str (0, 25, "DESIGN & PORT BY THE MOJON TWINS");
	pr_str (3, 26, "NESLIB & SFX CODE BY SHIRU");
	
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
	bankswitch (0);
}
