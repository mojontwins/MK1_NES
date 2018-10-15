// NES MK1 v2.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Simple propellers

	void propellers_add (void) {
		prp_addr [prp_idx ++] = 
		#ifdef DOUBLE_WIDTH
			nametable_base + 
		#else
			0x2000 + 
		#endif
		(
			((rdm & 0xf) << 1) |
			((((rdm >> 3) & 0xfe) + TOP_ADJUST) << 5)
		);
	}

	void propellers_do (void) {
		rdb = half_life << 1;
		gpit = prp_idx; while (gpit --) {			
			gp_addr = prp_addr [gpit];
			_n = rdb + PROPELLERS_BASE_PATTERN; ul_putc ();
			++ _n; ul_putc ();
		}
	}
	