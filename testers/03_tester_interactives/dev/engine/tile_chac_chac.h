// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Tile based chac chacs

void chac_chacs_add (void) {
	// Adds chac chac @ YX = rdm
	if (max_chac_chacs == MAX_CHAC_CHACS) return;
	rdc = chac_chacs_initial_times [rdt - CHAC_CHAC_DETECT_TILE];
	
	__asm__ ("ldy %v", max_chac_chacs);

	__asm__ ("lda %v", rdm);
	__asm__ ("sta %v,y", chac_chacs_yx);

	__asm__ ("lda #0");
	__asm__ ("sta %v,y", chac_chacs_state);

	__asm__ ("lda %v", rdc);
	__asm__ ("sta %v,y", chac_chacs_idlewait);

	__asm__ ("lda %v", rdc);
	__asm__ ("sta %v,y", chac_chacs_ct);
	
	++ max_chac_chacs;
}

void chac_chacs_do (void) {
	gpit = max_chac_chacs; while (gpit --) {
		if (chac_chacs_ct [gpit]) -- chac_chacs_ct [gpit]; else {
			++ chac_chacs_state [gpit];
			if (chac_chacs_state [gpit] == CHAC_CHAC_MAX_STATES) chac_chacs_state [gpit] = 0;
			chac_chacs_ct [gpit] = chac_chacs_state [gpit] ? 
				(chac_chacs_times [chac_chacs_state [gpit]]) :
				chac_chacs_idlewait [gpit];

			// Add to update list
			chac_chacs_queue [chac_chacs_queue_write] = gpit;
			chac_chacs_queue_write = (chac_chacs_queue_write + 1) & (MAX_CHAC_CHACKS_QUEUED-1);
		}
	}

	if (chac_chacs_queue_write != chac_chacs_queue_read) {
		gpit = chac_chacs_queue [chac_chacs_queue_read];
		chac_chacs_queue_read = (chac_chacs_queue_read + 1) & (MAX_CHAC_CHACKS_QUEUED-1);
		rdm = chac_chacs_state [gpit];
		if (rdm == CHAC_CHAC_BOUM_STATE) { shaker_ct = 8; sfx_play (SFX_STEPON, 1); }
		rdx = chac_chacs_yx [gpit]; rdy = rdx >> 4; rdx &= 0xf;
		_x = rdx; _y = rdy    ; _t = chac_chacs_t1 [rdm]; map_set ();
		_x = rdx; _y = rdy + 1; _t = chac_chacs_t2 [rdm]; map_set ();
		_x = rdx; _y = rdy + 2; _t = chac_chacs_t3 [rdm]; map_set ();
	}
}

