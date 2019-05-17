// NES MK1 v2.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Good ol' patrollers

#if defined (ENABLE_SHOOTIES)
	__asm__ ("lda %v", en_x);
	__asm__ ("sta %v", rdx);
	__asm__ ("lda %v", en_y);
	__asm__ ("sta %v", rdy);
#endif

	__asm__ ("ldx %v", gpit);

	__asm__ ("lda %v, x", en_status);
	__asm__ ("beq %g", enl_movement_do);
	__asm__ ("lda %v", half_life);
	__asm__ ("beq %g", enl_movement_done);

enl_movement_do:
	
	// Horizontal
	__asm__ ("lda %v", _en_x);
	__asm__ ("clc");
	__asm__ ("adc %v", _en_mx);
	__asm__ ("sta %v", _en_x);

	#ifdef WALLS_STOP_ENEMIES
		__asm__ ("lda %v", _en_y);
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("sta %v", cy1);

		__asm__ ("lda %v", _en_y);
		__asm__ ("clc");
		__asm__ ("adc #15");
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("sta %v", cy2);

		// if (_en_mx < 0) {
		__asm__ ("lda %v", _en_mx);
		__asm__ ("asl a");
		__asm__ ("bcs %g", enl_h_wse_neg);

		__asm__ ("lda %v", _en_x);
		__asm__ ("clc");
		__asm__ ("adc #15");
		
		__asm__ ("jmp %g", enl_h_wse_set_cx);

	enl_h_wse_neg:
		__asm__ ("lda %v", _en_x);
		
	enl_h_wse_set_cx:
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("sta %v", cx1);
		__asm__ ("sta %v", cx2);

		__asm__ ("jsr %v", cm_two_points);

		__asm__ ("lda %v", at1);
		__asm__ ("ora %v", at2);
		__asm__ ("sta %v", en_collx);
	#endif

	// Vertical
	__asm__ ("lda %v", _en_y);
	__asm__ ("clc");
	__asm__ ("adc %v", _en_my);
	__asm__ ("sta %v", _en_y);

	#ifdef WALLS_STOP_ENEMIES
		__asm__ ("lda %v", _en_x);
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("sta %v", cx1);

		__asm__ ("lda %v", _en_x);
		__asm__ ("clc");
		__asm__ ("adc #15");
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("sta %v", cx2);

		// if (_en_mx < 0) {
		__asm__ ("lda %v", _en_my);
		__asm__ ("asl a");
		__asm__ ("bcs %g", enl_v_wse_neg);

		__asm__ ("lda %v", _en_y);
		__asm__ ("clc");
		__asm__ ("adc #15");
		
		__asm__ ("jmp %g", enl_v_wse_set_cy);

	enl_v_wse_neg:
		__asm__ ("lda %v", _en_y);
		
	enl_v_wse_set_cy:
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("sta %v", cy1);
		__asm__ ("sta %v", cy2);

		__asm__ ("jsr %v", cm_two_points);

		__asm__ ("lda %v", at1);
		__asm__ ("ora %v", at2);
		__asm__ ("sta %v", en_colly);
	#endif

	// Change direction?

	// Horizontal
		// if (_en_x == _en_x1 || _en_x == _en_x2 
		__asm__ ("lda %v", _en_x);
		__asm__ ("cmp %v", _en_x1);
		__asm__ ("beq %g", enl_h_chgd_do);
		__asm__ ("cmp %v", _en_x2);
	#ifdef WALLS_STOP_ENEMIES
		// || en_collx) 
		__asm__ ("beq %g", enl_h_chgd_do);
		__asm__ ("lda %v", en_collx);
		__asm__ ("beq %g", enl_h_chgd_done);
	#else
		__asm__ ("bne %g", enl_h_chgd_done);
	#endif

	enl_h_chgd_do:
		// _en_mx = -_en_mx;
		__asm__ ("lda #$0");
		__asm__ ("sec");
		__asm__ ("sbc %v", _en_mx);
		__asm__ ("sta %v", _en_mx);
	enl_h_chgd_done:
	
	// Vertical
	#ifndef PLAYER_SAFE_LANDING
		// if (_en_y == _en_y1 || _en_y == _en_y2 
		__asm__ ("lda %v", _en_y);
		__asm__ ("cmp %v", _en_y1);
		__asm__ ("beq %g", enl_v_chgd_do);
		__asm__ ("cmp %v", _en_y2);
		#ifdef WALLS_STOP_ENEMIES
			// || en_colly) 
			__asm__ ("beq %g", enl_v_chgd_do);
			__asm__ ("lda %v", en_colly);
			__asm__ ("beq %g", enl_v_chgd_done);
		#else
			__asm__ ("bne %g", enl_v_chgd_done);
		#endif

	enl_v_chgd_do:
		// _en_my = -_en_my;
		__asm__ ("lda #$0");
		__asm__ ("sec");
		__asm__ ("sbc %v", _en_my);
		__asm__ ("sta %v", _en_my);
	enl_v_chgd_done:

	#else

		__asm__ ("lda %v", en_colly);
		__asm__ ("beq %g", enl_v_chgd_nocoll);

		__asm__ ("lda #$0");
		__asm__ ("sec");
		__asm__ ("sbc %v", _en_my);
		__asm__ ("sta %v", _en_my);

		__asm__ ("jmp %g", enl_v_chgd_done);

	enl_v_chgd_nocoll:
		// if (_en_y <= _en_y1) { _en_y = _en_y1; _en_my = ABS (_en_my); }
		__asm__ ("lda %v", _en_y);
		__asm__ ("cmp %v", _en_y1);
		__asm__ ("beq %g", enl_v_chgd_min_do);
		__asm__ ("bcs %g", enl_v_chgd_min_done);

	enl_v_chgd_min_do:
		__asm__ ("lda %v", _en_y1);
		__asm__ ("sta %v", _en_y);

		__asm__ ("lda %v", _en_my);
		__asm__ ("asl a");	// Sign goes to carry
		__asm__ ("bcc %g", enl_v_chgd_done);

		// _en_my is negative, make positive.
		// _en_my = -_en_my;
		__asm__ ("lda #$0");
		__asm__ ("sec");
		__asm__ ("sbc %v", _en_my);
		__asm__ ("sta %v", _en_my);
		__asm__ ("jmp %g", enl_v_chgd_done);

	enl_v_chgd_min_done:
		// else if (_en_y >= _en_y2) { _en_y = _en_y2; _en_my = -ABS (_en_my); }
		__asm__ ("lda %v", _en_y);
		__asm__ ("cmp %v", _en_y2);
		__asm__ ("bcc %g", enl_v_chgd_max_done);

		__asm__ ("lda %v", _en_y2);
		__asm__ ("sta %v", _en_y);

		__asm__ ("lda %v", _en_my);
		__asm__ ("asl a");	// Sign goes to carry
		__asm__ ("bcs %g", enl_v_chgd_done);

		// _en_my is negative, make positive.
		// _en_my = -_en_my;
		__asm__ ("lda #$0");
		__asm__ ("sec");
		__asm__ ("sbc %v", _en_my);
		__asm__ ("sta %v", _en_my);
		__asm__ ("jmp %g", enl_v_chgd_done);

	enl_v_chgd_max_done:

	enl_v_chgd_done:

	#endif

enl_movement_done:
	// rda = _en_mx ? (_en_mx < 0) : (_en_my < 0); enems_facing ();
	__asm__ ("lda %v", _en_mx);
	__asm__ ("beq %g", enl_facing_vert);
	__asm__ ("jmp %g", enl_facing_do);
enl_facing_vert:
	__asm__ ("lda %v", _en_my);
enl_facing_do:
	__asm__ ("asl a");
	__asm__ ("lda #0");
	__asm__ ("rol a");
	__asm__ ("sta %v", rda);
	__asm__ ("jsr %v", enems_facing);

	// en_spr = _en_s + en_fr + _en_facing;
	__asm__ ("lda %v", _en_s);
	__asm__ ("clc");
	__asm__ ("adc %v", en_fr);
	__asm__ ("adc %v", _en_facing);
	__asm__ ("sta %v", en_spr);
