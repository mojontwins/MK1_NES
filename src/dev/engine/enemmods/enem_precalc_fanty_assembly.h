// NES MK1 v2.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Precalculated, all-integer fanty from Goddess. 

// Needs FANTY_INCS_MAX and fanty_incs
// Needs fanty_retreat_incs

// Like homing fanties but with states removed, i.e. always "pursuing"

// Assembly translation

	// ------------------------------------------------------
	// Accelerate X
	// ------------------------------------------------------

	// if (prx > FANTY_ENX)
	#ifdef DOUBLE_WIDTH
		// 16 bits comparison
		// if (FANTY_ENX < prx)
		__asm__ ("lda %v", FANTY_ENX);
		__asm__ ("cmp %v", prx);
		__asm__ ("lda %v+1", FANTY_ENX);
		__asm__ ("sbc %v+1", prx);
		__asm__ ("bcs %g", ef_ax_else);
		
	#else
		// 8 bits comparison
		// if (FANTY_ENX < prx)
		__asm__ ("lda %v", FANTY_ENX);
		__asm__ ("cmp %v", prx);
		__asm__ ("bcs %g", ef_ax_else);
	#endif

		__asm__ ("inc %v", _en_mx);

		// if (_en_mx >= FANTY_INCS_MAX - 1)
		__asm__ ("lda %v", _en_mx);
		__asm__ ("sec");
		__asm__ ("sbc #%b", FANTY_INCS_MAX - 1);
		__asm__ ("bvc %g", ef_ax_gt_l0);
		__asm__ ("eor #$80");
	ef_ax_gt_l0:
		__asm__ ("bmi %g", ef_ax_endif);

		// _en_mx = FANTY_INCS_MAX - 1;
		__asm__ ("lda #%b", FANTY_INCS_MAX - 1);
		__asm__ ("sta %v", _en_mx);

		__asm__ ("jmp %g", ef_ax_endif);

	ef_ax_else:
	// } else if (prx < FANTY_ENX) {
	#ifdef DOUBLE_WIDTH
		// 16 bits comparison
		__asm__ ("lda %v", prx);
		__asm__ ("cmp %v", FANTY_ENX);
		__asm__ ("lda %v+1", prx);
		__asm__ ("sbc %v+1", FANTY_ENX);
		__asm__ ("bcs %g", ef_ax_endif);
	#else
		// 8 bits comparison
		__asm__ ("lda %v", prx);
		__asm__ ("cmp %v", FANTY_ENX);
		__asm__ ("bcs %v", ef_ax_endif);
	#endif

		__asm__ ("dec %v", _en_mx);
		// if (_en_mx <= -(FANTY_INCS_MAX - 1))
		// if (-(FANTY_INCS_MAX - 1) >= _en_mx)
		__asm__ ("lda #%b", -(FANTY_INCS_MAX - 1));
		__asm__ ("sec");
		__asm__ ("sbc %v", _en_mx);
		__asm__ ("bvc %g", ef_ax_lt_l0);
		__asm__ ("eor #$80");
	ef_ax_lt_l0:
		__asm__ ("bmi %g", ef_ax_endif);

		// _en_mx = - (FANTY_INCS_MAX - 1);
		__asm__ ("lda #%b", -(FANTY_INCS_MAX - 1));
		__asm__ ("sta %v", _en_mx);

	ef_ax_endif:

	// ------------------------------------------------------
	// Move X
	// ------------------------------------------------------

	FANTY_RDX = FANTY_ENX;	// cc65 resolves this just fine.

		__asm__ ("jsr _rand8");
		__asm__ ("and #7");
		__asm__ ("beq %g", ef_mx_done);

		// if (_en_mx < 0)
		__asm__ ("lda %v", _en_mx);
		__asm__ ("asl a");
		__asm__ ("bcc %g", ef_mx_positive_check);

		// FANTY_ENX -= fanty_incs [- _en_mx];
		__asm__ ("lda #0");
		__asm__ ("sec");
		__asm__ ("sbc %v", _en_mx);
		__asm__ ("tay");

		__asm__ ("lda %v", FANTY_ENX);
		__asm__ ("sec");
		__asm__ ("sbc %v, y", fanty_incs);
		__asm__ ("sta %v", FANTY_ENX);

		#ifdef DOUBLE_WIDTH
			__asm__ ("bcs %g", ef_mx_neg_dw);
			__asm__ ("dec %v+1", FANTY_ENX);
		ef_mx_neg_dw:		
		#endif

		__asm__ ("jmp %g", ef_mx_done);

	ef_mx_positive_check:
		// If we are here, _en_mx >= 0, so
		// if (_en_mx > 0)
		__asm__ ("lda %v", _en_mx);
		__asm__ ("beq %g", ef_mx_done);

		// FANTY_ENX += fanty_incs [_en_mx];
		__asm__ ("ldy %v", _en_mx);
		__asm__ ("lda %v", FANTY_ENX);
		__asm__ ("clc");
		__asm__ ("adc %v, y", fanty_incs);
		__asm__ ("sta %v", FANTY_ENX);
		
		#ifdef DOUBLE_WIDTH
			__asm__ ("bcc %g", ef_mx_pos_dw);
			__asm__ ("inc %v+1", FANTY_ENX);
		ef_mx_pos_dw:
		#endif

	ef_mx_done:

	// ------------------------------------------------------
	// Collide X
	// ------------------------------------------------------

		// if ((FANTY_ENX < 8 && _en_mx < 0) 
		#ifdef DOUBLE_WIDTH
			__asm__ ("lda %v", FANTY_ENX);
			__asm__ ("cmp #8");
			__asm__ ("lda %v+1", FANTY_ENX);
			__asm__ ("sbc #0");
		#else
			__asm__ ("lda %v", FANTY_ENX);
			__asm__ ("cmp #8");	
		#endif
		__asm__ ("bcs %g", ef_mx_change_2);

		__asm__ ("lda %v", _en_mx);
		__asm__ ("asl a");						// Sign to carry
		__asm__ ("bcc %g", ef_mx_change_2);		// CC = positive

		__asm__ ("jmp %g", ef_mx_change_do);

	ef_mx_change_2:
		// || (FANTY_ENX > MAX_ENX && _en_mx > 0)) 
		#ifdef DOUBLE_WIDTH
			__asm__ ("lda #%w", LSB(MAX_ENX));
			__asm__ ("cmp %v", FANTY_ENX);
			__asm__ ("lda #%b", MSB(MAX_ENX));
			__asm__ ("sbc %v+1", FANTY_ENX);
		#else
			__asm__ ("lda %v", MAX_ENX);
			__asm__ ("cmp %v", FANTY_ENX);	
		#endif
		__asm__ ("bcs %g", ef_mx_change_done);

		__asm__ ("lda %v", _en_mx);
		__asm__ ("asl a");						// Sign to carry
		__asm__ ("bcs %g", ef_mx_change_done);	// CS = negative
		__asm__ ("beq %g", ef_mx_change_done);	// == 0

	ef_mx_change_do:
		// _en_mx = -_en_mx;
		__asm__ ("lda #0");
		__asm__ ("sec");
		__asm__ ("sbc %v", _en_mx);
		__asm__ ("sta %v", _en_mx);

	ef_mx_change_done:

	#ifdef FANTY_COLLIDES
		__asm__ ("lda %v", _en_mx);
		__asm__ ("beq %g", ef_mx_collide_done);

		// cx1 = cx2 = _en_mx < 0 ? ((FANTY_ENX + 4) >> 4) : ((FANTY_ENX + 11) >> 4);
		__asm__ ("asl a");
		__asm__ ("bcc %g", ef_mx_collide_calccx_positive);

		#ifdef DOUBLE_WIDTH
			// 16 bits
			__asm__ ("lda %v", FANTY_ENX);
			__asm__ ("ldx %v+1", FANTY_ENX);
			__asm__ ("clc");
			__asm__ ("adc #4");
			__asm__ ("jmp %g", ef_mx_collide_calccx_set);
		#else
			// 8 bits
			__asm__ ("lda %v", FANTY_ENX);
			__asm__ ("clc");
			__asm__ ("adc #4");
			__asm__ ("jmp %g", ef_mx_collide_calccx_set);
		#endif

	ef_mx_collide_calccx_positive:
		#ifdef DOUBLE_WIDTH
			// 16 bits
			__asm__ ("lda %v", FANTY_ENX);
			__asm__ ("ldx %v+1", FANTY_ENX);
			__asm__ ("clc");
			__asm__ ("adc #11");
		#else
			// 8 bits
			__asm__ ("lda %v", FANTY_ENX);
			__asm__ ("clc");
			__asm__ ("adc #11");
		#endif

	ef_mx_collide_calccx_set:
		#ifdef DOUBLE_WIDTH
			// 16 bits
			__asm__ ("bcc %g", ef_mx_collide_calccx_set_skip);
			__asm__ ("inx");
	ef_mx_collide_calccx_set_skip:
			__asm__ ("jsr asrax4");
		#else
			// 8 bits
			__asm__ ("lsr a");
			__asm__ ("lsr a");
			__asm__ ("lsr a");
			__asm__ ("lsr a");
		#endif

		__asm__ ("sta %v", cx1);
		__asm__ ("sta %v", cx2);

		// cy1 = (_en_y + 4) >> 4; 
		__asm__ ("lda %v", _en_y);
		__asm__ ("clc");
		__asm__ ("adc #4");
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("sta %v", cy1);

		// cy2 = (_en_y + 11) >> 4;
		__asm__ ("lda %v", _en_y);
		__asm__ ("clc");
		__asm__ ("adc #11");
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("sta %v", cy2);

		cm_two_points ();

		// if ((at1 & 8) || (at2 & 8))
		__asm__ ("lda %v", at1);
		__asm__ ("and #8");
		__asm__ ("bne %g", ef_mx_collide_do);

		__asm__ ("lda %v", at2);
		__asm__ ("and #8");
		__asm__ ("beq %g", ef_mx_collide_done);

	ef_mx_collide_do:
		// _en_mx = -_en_mx; 
		__asm__ ("lda #0");
		__asm__ ("sec");
		__asm__ ("sbc %v", _en_mx);
		__asm__ ("sta %v", _en_mx);

		// FANTY_ENX = FANTY_RDX;
		__asm__ ("lda %v", FANTY_RDX);
		__asm__ ("sta %v", FANTY_ENX);
		#ifdef DOUBLE_WIDTH
			__asm__ ("lda %v+1", FANTY_RDX);
			__asm__ ("sta %v+1", FANTY_ENX);
		#endif

	ef_mx_collide_done:

	#endif

	// ------------------------------------------------------
	// Accelerate Y
	// ------------------------------------------------------

	// if (pry > _en_y)

		// if (_en_y < pry)
		__asm__ ("lda %v", _en_y);
		__asm__ ("cmp %v", pry);
		__asm__ ("bcs %g", ef_ay_else);

		__asm__ ("inc %v", _en_my);

		// if (_en_my >= FANTY_INCS_MAX - 1)
		__asm__ ("lda %v", _en_my);
		__asm__ ("sec");
		__asm__ ("sbc #%b", FANTY_INCS_MAX - 1);
		__asm__ ("bvc %g", ef_ay_gt_l0);
		__asm__ ("eor #$80");
	ef_ay_gt_l0:
		__asm__ ("bmi %g", ef_ay_endif);

		// _en_my = FANTY_INCS_MAX - 1;
		__asm__ ("lda #%b", FANTY_INCS_MAX - 1);
		__asm__ ("sta %v", _en_my);

		__asm__ ("jmp %g", ef_ay_endif);

	ef_ay_else:
	// } else if (pry < _en_y) {
		// 8 bits comparison
		__asm__ ("lda %v", pry);
		__asm__ ("cmp %v", _en_y);
		__asm__ ("bcs %g", ef_ay_endif);

		__asm__ ("dec %v", _en_my);
		// if (_en_my <= -(FANTY_INCS_MAX - 1))
		// if (-(FANTY_INCS_MAX - 1) >= _en_my)
		__asm__ ("lda #%b", -(FANTY_INCS_MAX - 1));
		__asm__ ("sec");
		__asm__ ("sbc %v", _en_my);
		__asm__ ("bvc %g", ef_ay_lt_l0);
		__asm__ ("eor #$80");
	ef_ay_lt_l0:
		__asm__ ("bmi %g", ef_ay_endif);

		// _en_my = - (FANTY_INCS_MAX - 1);
		__asm__ ("lda #%b", -(FANTY_INCS_MAX - 1));
		__asm__ ("sta %v", _en_my);

	ef_ay_endif:

	// ------------------------------------------------------
	// Move Y
	// ------------------------------------------------------

		rdy = _en_y;	// cc65 resolves this just fine.

		/*
		__asm__ ("jsr _rand8");
		__asm__ ("and #7");
		__asm__ ("beq %g", ef_my_done);
		*/

		// if (_en_my < 0)
		__asm__ ("lda %v", _en_my);
		__asm__ ("asl a");
		__asm__ ("bcc %g", ef_my_positive_check);

		// _en_y -= fanty_incs [- _en_my];
		__asm__ ("lda #0");
		__asm__ ("sec");
		__asm__ ("sbc %v", _en_my);
		__asm__ ("tay");

		__asm__ ("lda %v", _en_y);
		__asm__ ("sec");
		__asm__ ("sbc %v, y", fanty_incs);
		__asm__ ("sta %v", _en_y);

		__asm__ ("jmp %g", ef_my_done);

	ef_my_positive_check:
		// If we are here, _en_my >= 0, so
		// if (_en_my > 0)
		__asm__ ("lda %v", _en_my);
		__asm__ ("beq %g", ef_my_done);

		// _en_y += fanty_incs [_en_my];
		__asm__ ("ldy %v", _en_my);
		__asm__ ("lda %v", _en_y);
		__asm__ ("clc");
		__asm__ ("adc %v, y", fanty_incs);
		__asm__ ("sta %v", _en_y);
		
	ef_my_done:

	// ------------------------------------------------------
	// Collide Y
	// ------------------------------------------------------

		// if ((_en_y < 8 && _en_my < 0) 
		__asm__ ("lda %v", _en_y);
		__asm__ ("cmp #8");	
		__asm__ ("bcs %g", ef_my_change_2);

		__asm__ ("lda %v", _en_my);
		__asm__ ("asl a");						// Sign to carry
		__asm__ ("bcc %g", ef_my_change_2);		// CC = positive

		__asm__ ("jmp %g", ef_my_change_do);

	ef_my_change_2:
		// || (_en_y > 232 && _en_my > 0)) 
		__asm__ ("lda #232");
		__asm__ ("cmp %v", _en_y);	
		__asm__ ("bcs %g", ef_my_change_done);

		__asm__ ("lda %v", _en_my);
		__asm__ ("asl a");						// Sign to carry
		__asm__ ("bcs %g", ef_my_change_done);	// CS = negative
		__asm__ ("beq %g", ef_my_change_done);	// == 0

	ef_my_change_do:
		// _en_my = -_en_my;
		__asm__ ("lda #0");
		__asm__ ("sec");
		__asm__ ("sbc %v", _en_my);
		__asm__ ("sta %v", _en_my);

	ef_my_change_done:

	#ifdef FANTY_COLLIDES
		__asm__ ("lda %v", _en_my);
		__asm__ ("beq %g", ef_my_collide_done);

		// if (_en_my < 0) {		
		__asm__ ("asl a");
		__asm__ ("bcc %g", ef_my_collide_calccy_positive);

		// rdb = 8
		__asm__ ("lda #8");
		__asm__ ("sta %v", rdb);

		// cy1 = cy2 = ((_en_y + 4) >> 4);

		__asm__ ("lda %v", _en_y);
		__asm__ ("clc");
		__asm__ ("adc #4");
		__asm__ ("jmp %g", ef_my_collide_calccy_set);

	ef_my_collide_calccy_positive:
		// rdb = 12
		__asm__ ("lda #12");
		__asm__ ("sta %v", rdb);		

		__asm__ ("lda %v", _en_y);
		__asm__ ("clc");
		__asm__ ("adc #11");

	ef_my_collide_calccy_set:
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("lsr a");

		__asm__ ("sta %v", cy1);
		__asm__ ("sta %v", cy2);

		// cx1 = (FANTY_ENX + 4) >> 4;
		#ifdef DOUBLE_WIDTH
			__asm__ ("lda %v", FANTY_ENX);
			__asm__ ("ldx %v+1", FANTY_ENX);
			__asm__ ("clc");
			__asm__ ("adc #4");
			__asm__ ("bcc %g", ef_my_collide_calccx1_set_skip);
			__asm__ ("inx");
	ef_my_collide_calccx1_set_skip:
			__asm__ ("jsr asrax4");
		#else
			__asm__ ("lda %v", _en_y);
			__asm__ ("clc");
			__asm__ ("adc #4");
			__asm__ ("lsr a");
			__asm__ ("lsr a");
			__asm__ ("lsr a");
			__asm__ ("lsr a");			
		#endif
		__asm__ ("sta %v", cx1);

		// cy2 = (_en_y + 11) >> 4;
		#ifdef DOUBLE_WIDTH
			__asm__ ("lda %v", FANTY_ENX);
			__asm__ ("ldx %v+1", FANTY_ENX);
			__asm__ ("clc");
			__asm__ ("adc #11");
			__asm__ ("bcc %g", ef_my_collide_calccx2_set_skip);
			__asm__ ("inx");
		ef_my_collide_calccx2_set_skip:
			__asm__ ("jsr asrax4");		
		#else
			__asm__ ("lda %v", _en_y);
			__asm__ ("clc");
			__asm__ ("adc #11");
			__asm__ ("lsr a");
			__asm__ ("lsr a");
			__asm__ ("lsr a");
			__asm__ ("lsr a");			
		#endif
		__asm__ ("sta %v", cx2);

		cm_two_points ();

		// if ((at1 & 8) || (at2 & 8))
		__asm__ ("lda %v", at1);
		__asm__ ("and %v", rdb);
		__asm__ ("bne %g", ef_my_collide_do);

		__asm__ ("lda %v", at2);
		__asm__ ("and %v", rdb);
		__asm__ ("beq %g", ef_my_collide_done);

	ef_my_collide_do:
		// _en_my = -_en_my; 
		__asm__ ("lda #0");
		__asm__ ("sec");
		__asm__ ("sbc %v", _en_my);
		__asm__ ("sta %v", _en_my);

		// _en_y = rdy;
		__asm__ ("lda %v", rdy);
		__asm__ ("sta %v", _en_y);
		#ifdef DOUBLE_WIDTH
			__asm__ ("lda %v+1", rdy);
			__asm__ ("sta %v+1", _en_y);
		#endif	

	ef_my_collide_done:

	#endif

	#ifdef FANTY_KILLED_BY_TILE
	cx1 = (EN_X_ABSOLUTE + 8) >> 4;
	cy1 = (_en_y + 8) >> 4;
	cm_two_points ();
	if (at1 & 1) {
		en_cttouched [gpit] = 8;
		enems_kill ();
	}
#endif

#ifdef FANTY_FAST_ANIM
	en_fr = half_life;
#else
	en_fr = (EN_X_ABSOLUTE >> 3) & 1;
#endif

#ifdef FANTY_WITH_FACING
	//_en_facing = ((EN_X_ABSOLUTE < prx) ? 0 : 4);
	rda = (prx < EN_X_ABSOLUTE); enems_facing ();
	en_spr = _en_s + en_fr + _en_facing;
#else
	en_spr = _en_s + en_fr;
#endif
