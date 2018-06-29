// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// printer.h
// Draw map, print text, etcetera.

// fade out
void fade_out (void) {
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

// Clear update list
void clear_update_list (void) {
	memfill (update_list, 0, UPDATE_LIST_SIZE*3);
	update_index = 0;
}

void cls (void) {
	vram_adr(0x2000); vram_fill(0x00,0x400);
}

// Needs gp_addr, _n set.
void ul_putc (void) {
	update_list [update_index++] = MSB (gp_addr);
	update_list [update_index++] = LSB (gp_addr++);
	update_list [update_index++] = _n;
}

// Needs _x, _y, _n set.
void p_t (void) {
	rda = _n; gp_addr = (_y << 5) + _x + 0x2000;
	_n = ((rda/10)+16); ul_putc ();
	_n = ((rda%10)+16); ul_putc ();
}

const unsigned char bitmasks [] = {0xfc, 0xf3, 0xcf, 0x3f};
unsigned char attr_table [64];

// Needs _x, _y, _t set.
void upd_attr_table (void) {
	rdc = (_x >> 2) + ((_y >> 2) << 3);
	rdb = ((_x >> 1) & 1) + (((_y >> 1) & 1) << 1);
	rda = attr_table [rdc];
	rda = (rda & bitmasks [rdb]) | (c_ts_pals [_t] << (rdb << 1));
	attr_table [rdc] = rda;
}

// Needs _x, _y, _t set.
void draw_tile (void) {
	// Pass _x, _y, _t directly.
	upd_attr_table ();
	
	gp_tmap = c_ts_tmaps + (_t << 2);
	gp_addr = ((_y << 5) + _x + 0x2000);
	vram_adr (gp_addr++);
	/*
	vram_put (*gp_tmap++);
	vram_put (*gp_tmap++);
	*/
	SET_FROM_PTR (_z, gp_tmap); gp_tmap ++; vram_put (_z);
	SET_FROM_PTR (_z, gp_tmap); gp_tmap ++; vram_put (_z);
	gp_addr += 31;
	vram_adr (gp_addr++);
	/*
	vram_put (*gp_tmap++);
	vram_put (*gp_tmap);	
	*/
	SET_FROM_PTR (_z, gp_tmap); gp_tmap ++; vram_put (_z);
	SET_FROM_PTR (_z, gp_tmap);             vram_put (_z);
}

// Needs _x, _y, _t set.
void update_list_tile (void) {
	// Pass _x, _y, _t directly.
	upd_attr_table ();
	// rda contains the attribute byte.
	// rdc contains the offset in the attribute nametable.
	
	gp_addr = 0x23c0 + rdc;
	_n = rda; ul_putc ();
	
	// tiles
	//tl = (16 + tl) << 2;
	gp_tmap = c_ts_tmaps + (_t << 2);
	gp_addr = ((_y << 5) + _x + 0x2000);
	/*
	_n = *gp_tmap ++; ul_putc ();
	_n = *gp_tmap ++; ul_putc ();
	*/
	SET_FROM_PTR (_n, gp_tmap); gp_tmap ++; ul_putc ();
	SET_FROM_PTR (_n, gp_tmap); gp_tmap ++; ul_putc ();
	gp_addr += 30;
	/*
	_n = *gp_tmap ++; ul_putc ();
	_n = *gp_tmap   ; ul_putc ();
	*/
	SET_FROM_PTR (_n, gp_tmap); gp_tmap ++; ul_putc ();
	SET_FROM_PTR (_n, gp_tmap);             ul_putc ();
}

// Needs _x, _y, _t set.
// Destroys _x, _y!.
void map_set (void) {
	map_buff [COORDS (_x, _y)] = _t;
	map_attr [COORDS (_x, _y)] = c_behs [_t];
	_x = _x << 1; _y = TOP_ADJUST + (_y << 1);
	update_list_tile (); 
}

unsigned char get_byte (void) {
	-- rdit; return *gp_gen ++;
}

#ifdef MAP_RENDERER_COMPLEX
	#include "engine/mapmods/map_renderer_complex.h"
#else
	#include "engine/mapmods/map_renderer_fast.h"
#endif

// Needs _x, _y set.
void pr_str (unsigned char *s) {
	vram_adr (((_y << 5) | _x) + 0x2000);
	while (gpit = *s++) {
		if (gpit == '%') {
			++ _y; vram_adr (((_y << 5) | _x) + 0x2000);
		}
		else vram_put (gpit - 32); 
	}
}

#ifdef ENABLE_UL_PRINTER
	// Needs _x, _y set and gp_gen pointing to the string
	void pr_ul_str () {
		ppu_waitnmi (); clear_update_list ();
		gp_addr = 0x2000 + ((_y << 5) | _x);
		while (_n = *gp_gen ++) { _n -= 32; ul_putc (); }
		ppu_waitnmi (); clear_update_list ();
	}
#endif

#ifdef DEBUG
unsigned char get_hex_digit (unsigned char n) {
	if (n < 10) return n + 16;
	return n + 23;
}

void debug_print_hex_16_dl (unsigned char x, unsigned char y, unsigned int n) {
	clear_update_list ();

	gp_addr = (y << 5) + x + 0x2000;
	_n = get_hex_digit (n >> 12); 			ul_putc ();
	_n = get_hex_digit ((n >> 8) & 0xf); 	ul_putc ();
	_n = get_hex_digit ((n >> 4) & 0xf); 	ul_putc ();
	_n = get_hex_digit ((n & 0xf)); 		ul_putc ();

	ppu_waitnmi ();
}
#endif

