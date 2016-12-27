// NES MK1 v0.2
// Copyleft Mojon Twins 2013, 2015

// printer.h
// Draw map, print text, etcetera.


// fade out
void  fade_out (void) {
	for (fader = 4; fader > -1; fader --) {
		pal_bright (fader);
		delay (fade_delay);
	}	
}

// fade in
void  fade_in (void) {
	for (fader = 0; fader < 5; fader ++) {
		pal_bright (fader);
		delay (fade_delay);
	}	
}
/*
void  fade_in_fast (void) {
	pal_bright (2); delay (1);
	pal_bright (4); delay (1);
}

void  fade_out_fast (void) {
	pal_bright (2); delay (1);
	pal_bright (0); delay (1);
}
*/
// Clear update list
void  clear_update_list (void) {
	for (i = 0; i < UPDATE_LIST_SIZE * 3; i ++)
		update_list [i] = 0;	
}

void  cls (void) {
	vram_adr(0x2000);
	vram_fill(255,0x3c0);
	vram_adr (0x23c0);
	vram_fill(0xff,64);
}

const unsigned char bitmasks [] = {0xfc, 0xf3, 0xcf, 0x3f};
unsigned char attr_table [64];

void draw_tile (unsigned char x, unsigned char y, unsigned char tl) {
	rdc = (x >> 2) + ((y >> 2) << 3);
	rdb = ((x >> 1) & 1) + (((y >> 1) & 1) << 1);
	rda = attr_table [rdc];
	rda = (rda & bitmasks [rdb]) | (tileset_pals [tl] << (rdb << 1));
	attr_table [rdc] = rda;
	
	gp_tmap = tsmap + (tl << 2);
	gp_addr = ((y<<5) + x + 0x2000);
	vram_adr (gp_addr++);
	vram_put (*gp_tmap++);
	vram_put (*gp_tmap++);
	gp_addr+=31;
	vram_adr (gp_addr++);
	vram_put (*gp_tmap++);
	vram_put (*gp_tmap);	
}

void update_list_tile (unsigned char x, unsigned char y, unsigned char tl) {
	// Attribute
	rdc = (x >> 2) + ((y >> 2) << 3);
	rdb = ((x >> 1) & 1) + (((y >> 1) & 1) << 1);
	rda = attr_table [rdc];
	rda = (rda & bitmasks [rdb]) | (tileset_pals [tl] << (rdb << 1));
	attr_table [rdc] = rda;
	
	gp_addr = 0x23c0 + rdc;
	update_list [update_index++] = MSB(gp_addr);
	update_list [update_index++] = LSB(gp_addr);
	update_list [update_index++] = rda;
	
	// tiles
	//tl = (16 + tl) << 2;
	gp_tmap = tsmap + (tl << 2);
	gp_addr = ((y<<5) + x + 0x2000);
	update_list [update_index++] = MSB(gp_addr);
	update_list [update_index++] = LSB(gp_addr++);
	update_list [update_index++] = *gp_tmap++;
	update_list [update_index++] = MSB(gp_addr);
	update_list [update_index++] = LSB(gp_addr);
	update_list [update_index++] = *gp_tmap++;
	gp_addr += 31;
	update_list [update_index++] = MSB(gp_addr);
	update_list [update_index++] = LSB(gp_addr++);
	update_list [update_index++] = *gp_tmap++;
	update_list [update_index++] = MSB(gp_addr);
	update_list [update_index++] = LSB(gp_addr);
	update_list [update_index++] = *gp_tmap;
}

void p_t (unsigned char x, unsigned char y, unsigned char n) {
	gp_addr = (y << 5) + x + 0x2000;
	update_list [update_index++] = MSB (gp_addr);
	update_list [update_index++] = LSB (gp_addr++);
	update_list [update_index++] = (n / 10) + 16;
	update_list [update_index++] = MSB (gp_addr);
	update_list [update_index++] = LSB (gp_addr);
	update_list [update_index++] = (n % 10) + 16;
}

void draw_game_tile (unsigned char x, unsigned char y, unsigned char t) {
	rdit = x + (y << 4);
	map_buff [rdit] = t;
	map_attr [rdit] = tbehs [t];
#ifdef BREAKABLE_WALLS
	brk_buff [rdit] = 1;
#endif
	//if (t == 0 && (rand8 () & 15) == 1) t = 16;	// ALT BG
	draw_tile (x + x, TOP_ADJUST + y + y, t);
}

void draw_scr (void) {
	// Draw current screen
	gp_gen = (unsigned char *) (c_map) + n_pant * 96; rdx = 0; rdy = TOP_ADJUST;
	
	for (rdit = 0; rdit < 192; rdit ++) {
		if ((rdit & 1) == 0) {
			rdt = gp_gen [rdit >> 1] >> 4;
		} else {
			rdt = gp_gen [rdit >> 1] & 15;
		}
		
		map_buff [rdit] = rdt;		
		map_attr [rdit] = tbehs [rdt];		
#ifdef BREAKABLE_WALLS
		brk_buff [rdit] = 1;
#endif
		if (rdt == 0 && (rand8 () & 15) == 1) rdt = 16;	// ALT BG

		draw_tile (rdx, rdy, rdt);
		
		rdx = (rdx + 2) & 31; if (!rdx) rdy +=2;
	}

	// Draw decorations
	if (c_decos [n_pant]) {
		gp_gen = (unsigned char *) c_decos [n_pant];

		while (rdt = *gp_gen ++) {
			if (rdt & 0x80) {
				rdt &= 0x7F;
				rdct = 1;
			} else {
				rdct = *gp_gen ++;
			}
			while (rdct --) {
				rda = *gp_gen ++;
				rdx = rda >> 4; rdy = rda & 15;
				draw_game_tile (rdx, rdy, rdt);
			}
		}
	}
	
	// Clear open locks
#ifndef DEACTIVATE_KEYS	
	for (gpit = 0; gpit < MAX_CERROJOS; gpit ++) {
		if (n_pant == lknp [gpit]) {
			if (!lkact [gpit]) {
				rdx = (lkxy [gpit] >> 4);
				rdy = (lkxy [gpit] & 15);
				draw_game_tile (rdx, rdy, rdt);
			}
		}
	}	
#endif

#ifdef BREAKABLE_ANIM
	do_process_breakable = 0;
	for (gpit = 0; gpit < MAX_BREAKABLE; gpit ++) brkf [gpit] = 0;
#endif
}

void pr_str (unsigned char x, unsigned char y, unsigned char *s) {
	gp_addr = ((y<<5) + x + 0x2000);
	vram_adr (gp_addr);
	while (gpit = *s++) {
		//vram_adr (gp_addr++);
		if (gpit != '_') vram_put (gpit - 32); else vram_put (0);
	}
}

void pr_str_upd (unsigned char *s) {
	gp_addr = 0x2000 + (LINE_OF_TEXT << 5) + LINE_OF_TEXT_X;
	while (1) {
		if ( (gpit = *s ++) == 0x0) break;
		update_list [update_index++] = MSB (gp_addr);
		update_list [update_index++] = LSB (gp_addr ++);
		update_list [update_index++] = gpit - 32;
	}
}
