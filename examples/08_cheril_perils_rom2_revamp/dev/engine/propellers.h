// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017

// Simple propellers

void propellers_add (void) {
	// Store YX for animation
	prp_yx [prp_idx ++] = rdm;
}

void propellers_do (void) {
	rdb = half_life << 1;
	gpit = prp_idx; while (gpit --) {
		rda = prp_yx [gpit];
		gp_addr = 0x2000 + (
			((rda & 0xf) << 1) | 
			((((rda >> 3) & 0xfe) + TOP_ADJUST) << 5)
		);
		ul_putc (rdb + PROPELLERS_BASE_PATTERN);
		ul_putc (rdb + PROPELLERS_BASE_PATTERN + 1);
	}
}

