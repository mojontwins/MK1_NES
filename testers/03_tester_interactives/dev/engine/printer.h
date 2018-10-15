// NES MK1 v2.0
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
void fade_in (void) {
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
	vram_adr (0x2000); vram_fill(0x00, 
		#ifdef DOUBLE_WIDTH
			0x800
		#else
			0x400
		#endif
	);
}

// Needs gp_addr, _n set.
void ul_putc (void) {
	/*
	update_list [update_index++] = MSB (gp_addr);
	update_list [update_index++] = LSB (gp_addr++);
	update_list [update_index++] = _n;
	*/

	// Manually rewrote this

		__asm__ ("ldx %v", update_index);
		__asm__ ("lda %v + 1", gp_addr);
		__asm__ ("sta %v, x", update_list);
		__asm__ ("inx");
		__asm__ ("lda %v", gp_addr);
		__asm__ ("sta %v, x", update_list);
		__asm__ ("inx");
		__asm__ ("lda %v", _n);
		__asm__ ("sta %v, x", update_list);
		__asm__ ("inc %v", gp_addr);
		__asm__ ("bne %g", ul_putc_inc16_0);
		__asm__ ("inc %v + 1", gp_addr);
	ul_putc_inc16_0:
		__asm__ ("inc %v", update_index);
		__asm__ ("inc %v", update_index);
		__asm__ ("inc %v", update_index);
}

// Needs _x, _y, _n set.
void p_t (void) {
	gp_addr = (_y << 5) + _x + 0x2000;

	// Adapted from code by Bregalad

		// A = number (0-99)
		__asm__ ("lda %v", _n);

		__asm__ ("ldx #$ff");
		__asm__ ("sec");
	p_t_loop_0:
		__asm__ ("inx");
		__asm__ ("sbc #10");
		__asm__ ("bcs %g", p_t_loop_0);
		__asm__ ("adc #10");

		// A = lower digit (0-9), X=upper digit(0-9)
		__asm__ ("sta %v", rda);
		__asm__ ("stx %v", _n);

		_n += 16;      ul_putc ();
		_n = rda + 16; ul_putc ();
}

// Needs _x, _y, _t set.
void upd_attr_table (void) {
	#ifdef DOUBLE_WIDTH
		//rdc = (_x >> 2) + ((_y >> 2) << 3);
		// ((_y >> 2) << 3) is the same as (_y << 1) & 0xf8;
		
		__asm__ ("lda %v", _x);
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("sta %v", rdc);	// rdc = (_x >> 2)

		__asm__ ("lda %v", _y);
		__asm__ ("asl a");
		__asm__ ("and #$f8");		// A = (_y << 1) & 0xf8

		__asm__ ("clc");
		__asm__ ("adc %v", rdc);
		__asm__ ("sta %v", rdc);	// Result

		// rde = rdc + attr_table_offset;
		// rdc is already on A
		__asm__ ("clc");
		__asm__ ("adc %v", attr_table_offset);
		__asm__ ("sta %v", rde);

		// rda = attr_table [rde];
		// rde is already on A
		__asm__ ("tax");
		__asm__ ("lda %v, x", attr_table);
		__asm__ ("sta %v", rda);

		// rdb = ((_x >> 1) & 1) + (((_y >> 1) & 1) << 1);
		
		__asm__ ("lda %v", _x);
		__asm__ ("lsr a");
		__asm__ ("and #1");
		__asm__ ("sta %v", rdb);	// rdb = ((_x >> 1) & 1)

		__asm__ ("lda %v", _y);
		__asm__ ("lsr a");
		__asm__ ("and #1");
		__asm__ ("asl a");			// A = (((_y >> 1) & 1) << 1)

		__asm__ ("clc");
		__asm__ ("adc %v", rdb);
		__asm__ ("sta %v", rdb);	// Result
		
		//rda = (rda & bitmasks [rdb]) | (c_ts_pals [_t] << (rdb << 1));		
		
		__asm__ ("lda %v", c_ts_pals);
		__asm__ ("ldx %v + 1", c_ts_pals);
		__asm__ ("ldy %v", _t);
		__asm__ ("sta ptr1");
		__asm__ ("stx ptr1+1");
		__asm__ ("lda (ptr1), y");
									// A = c_ts_pals [_t];

		// Shift left (rdb << 1) times

		__asm__ ("ldx %v", rdb);	
		__asm__ ("beq %g", upd_attr_table_loop_0_skip);

	upd_attr_table_loop_0:
		__asm__ ("asl a");
		__asm__ ("asl a");
		__asm__ ("dex");
		__asm__ ("bne %g", upd_attr_table_loop_0);

	upd_attr_table_loop_0_skip:
		__asm__ ("sta %v", ast1);	// ast1 = (c_ts_pals [_t] << (rdb << 1));

		__asm__ ("lda %v", rda);
		__asm__ ("ldx %v", rdb);
		__asm__ ("and %v, x", bitmasks);
									// A = (rda & bitmasks [rdb]);

		__asm__ ("ora %v", ast1);
		__asm__ ("sta %v", rda);	// Result

		// attr_table [rde] = rda;
		// rda is already on A
		__asm__ ("ldx %v", rde);
		__asm__ ("sta %v, x", attr_table);
	#else
		// rdc = (_x >> 2) + ((_y >> 2) << 3);
		// ((_y >> 2) << 3) is the same as (_y << 1) & 0xf8;
		
		__asm__ ("lda %v", _x);
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("sta %v", rdc);	// rdc = (_x >> 2)

		__asm__ ("lda %v", _y);
		__asm__ ("asl a");
		__asm__ ("and #$f8");		// A = (_y << 1) & 0xf8

		__asm__ ("clc");
		__asm__ ("adc %v", rdc);
		__asm__ ("sta %v", rdc);	// Result

		// rda = attr_table [rdc];
		// rdc is on A
		__asm__ ("tax");
		__asm__ ("lda %v, x", attr_table);
		__asm__ ("sta %v", rda);

		// rdb = ((_x >> 1) & 1) + (((_y >> 1) & 1) << 1);
		
		__asm__ ("lda %v", _x);
		__asm__ ("lsr a");
		__asm__ ("and #1");
		__asm__ ("sta %v", rdb);	// rdb = ((_x >> 1) & 1)

		__asm__ ("lda %v", _y);
		__asm__ ("lsr a");
		__asm__ ("and #1");
		__asm__ ("asl a");			// A = (((_y >> 1) & 1) << 1)

		__asm__ ("clc");
		__asm__ ("adc %v", rdb);
		__asm__ ("sta %v", rdb);	// Result
		
		//rda = (rda & bitmasks [rdb]) | (c_ts_pals [_t] << (rdb << 1));		
		
		__asm__ ("lda %v", c_ts_pals);
		__asm__ ("ldx %v + 1", c_ts_pals);
		__asm__ ("ldy %v", _t);
		__asm__ ("sta ptr1");
		__asm__ ("stx ptr1+1");
		__asm__ ("lda (ptr1), y");
									// A = c_ts_pals [_t];

		// Shift left (rdb << 1) times

		__asm__ ("ldx %v", rdb);	
		__asm__ ("beq %g", upd_attr_table_loop_0_skip);

	upd_attr_table_loop_0:
		__asm__ ("asl a");
		__asm__ ("asl a");
		__asm__ ("dex");
		__asm__ ("bne %g", upd_attr_table_loop_0);

	upd_attr_table_loop_0_skip:
		__asm__ ("sta %v", ast1);	// ast1 = (c_ts_pals [_t] << (rdb << 1));

		__asm__ ("lda %v", rda);
		__asm__ ("ldx %v", rdb);
		__asm__ ("and %v, x", bitmasks);
									// A = (rda & bitmasks [rdb]);

		__asm__ ("ora %v", ast1);
		__asm__ ("sta %v", rda);	// Result

		// attr_table [rdc] = rda;
		// rda is already on A
		__asm__ ("ldx %v", rdc);
		__asm__ ("sta %v, x", attr_table);
	#endif
}

// Needs _x, _y, _t set.
void draw_tile (void) {
	// Pass _x, _y, _t directly.
	upd_attr_table ();
	
	gp_tmap = c_ts_tmaps + (_t << 2);
	gp_addr = ((_y << 5) + _x + NAMETABLE_BASE);
	vram_adr (gp_addr); ++ gp_addr;
	/*
	vram_put (*gp_tmap++);
	vram_put (*gp_tmap++);
	*/
	SET_FROM_PTR (rda, gp_tmap); ++ gp_tmap; vram_put (rda);
	SET_FROM_PTR (rda, gp_tmap); ++ gp_tmap; vram_put (rda);
	gp_addr += 31;
	vram_adr (gp_addr); ++ gp_addr;
	/*
	vram_put (*gp_tmap++);
	vram_put (*gp_tmap);	
	*/
	SET_FROM_PTR (rda, gp_tmap); ++ gp_tmap; vram_put (rda);
	SET_FROM_PTR (rda, gp_tmap);             vram_put (rda);
}

// Needs _x, _y, _t set.
void update_list_tile (void) {
	#ifdef DOUBLE_WIDTH
		caux = _x;
		if (_x > 31) {
			_x -= 32;
			attr_table_offset = 64;
			NAMETABLE_BASE = 0x2400;
		} else {
			attr_table_offset = 0;
			NAMETABLE_BASE = 0x2000;			
		}
	#endif

	// Pass _x, _y, _t directly.
	upd_attr_table ();
	// rda contains the attribute byte.
	// rdc contains the offset in the attribute nametable.
	gp_addr = (NAMETABLE_BASE + 0x03c0) + rdc;
	_n = rda; ul_putc ();
	
	// tiles
	gp_tmap = c_ts_tmaps + (_t << 2);
	gp_addr = ((_y << 5) + _x + NAMETABLE_BASE);
	
	SET_FROM_PTR (_n, gp_tmap); ++ gp_tmap; ul_putc ();
	SET_FROM_PTR (_n, gp_tmap); ++ gp_tmap; ul_putc ();
	gp_addr += 30;
	
	SET_FROM_PTR (_n, gp_tmap); ++ gp_tmap; ul_putc ();
	SET_FROM_PTR (_n, gp_tmap);             ul_putc ();

	#ifdef DOUBLE_WIDTH
		_x = caux;
	#endif
}

// Needs _x, _y, _t set.
// Destroys _x, _y!.
void map_set (void) {
	#ifdef DOUBLE_WIDTH
		map_buff [COORDS (_x, _y)] = _t;
		map_attr [COORDS (_x, _y)] = c_behs [_t];
		// _x = _x << 1; 
		__asm__ ("asl %v", _x);
		_y = TOP_ADJUST + (_y << 1);
	#else
		// In single screen mode all operations fit within a byte so I can use this
		// code instead, which is smaller + faster

		// map_buff [COORDS (_x, _y)] = _t;
		// map_attr [COORDS (_x, _y)] = c_behs [_t];
		__asm__ ("lda %v", _y);
		__asm__ ("asl a");
		__asm__ ("asl a");
		__asm__ ("asl a");
		__asm__ ("asl a");
		__asm__ ("sta %v", ast1);
		__asm__ ("lda %v", _x);
		__asm__ ("ora %v", ast1);
		__asm__ ("tax");				// X = COORDS (_x, _y)

		__asm__ ("lda %v", _t);
		__asm__ ("sta %v, x", map_buff);
										// map_buff [COORDS (_x, _y)] = _t;
		__asm__ ("lda %v", c_behs);
		__asm__ ("sta ptr1");
		__asm__ ("lda %v + 1", c_behs);
		__asm__ ("sta ptr1 + 1");
		__asm__ ("ldy %v", _t);
		__asm__ ("lda (ptr1), y");		// A = c_behs [_t]
		__asm__ ("sta %v, x", map_attr);
										// map_attr [COORDS (_x, _y)] = c_behs [_t];

		// _x = _x << 1; 
		__asm__ ("asl %v", _x);
		_y = TOP_ADJUST + (_y << 1);		
	#endif	

	update_list_tile (); 
}

unsigned char get_byte (void) {
	-- rdit; return *gp_gen ++;
}

#if defined (MAP_RENDERER_COMPLEX)
	#include "engine/mapmods/map_renderer_complex.h"
#else
	#include "engine/mapmods/map_renderer_fast.h"
#endif

#ifdef DOUBLE_WIDTH
	#include "engine/mapmods/map_renderer_double.h"
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

