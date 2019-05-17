// NES MK1 v2.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

void fire_bullet (void) {
	#ifdef MAX_AMMO
		if (!pammo) return;
		-- pammo;
		__asm__ ("lda %v", pammo);
		__asm__ ("bne %g", fire_bullet_has_ammo);
		__asm__ ("rts");
	fire_bullet_has_ammo:
	#endif

	// Creates a new bullet (if possible):

	/*
	if (b_slots_i == 0) return;
	-- b_slots_i; bi = b_slots [b_slots_i];
	*/

		__asm__ ("lda %v", b_slots_i);
		__asm__ ("bne %g", fire_bullet_has_slot);
		__asm__ ("rts");
	fire_bullet_has_slot:

		__asm__ ("dec %v", b_slots_i);
		__asm__ ("ldy %v", b_slots_i);
		__asm__ ("lda %v, y", b_slots);
		__asm__ ("sta %v", bi);
		__asm__ ("tax");

		// X = bi

	#ifdef DOUBLE_WIDTH
		__asm__ ("asl a");
		__asm__ ("sta %v", rdm);

		// rdm = bi*2
	#endif		

	#ifdef PLAYER_BULLET_LIFE		
		__asm__ ("lda #%b", PLAYER_BULLET_LIFE);
		__asm__ ("sta %v, x", bst);
	#endif

	#ifdef PLAYER_FIRE_RELOAD		
		__asm__ ("lda #%b", PLAYER_FIRE_RELOAD);
		__asm__ ("sta %v", pfirereload);
	#endif

	#ifdef PLAYER_JUST_FIRED_COUNTER
		__asm__ ("lda #%b", PLAYER_JUST_FIRED_COUNTER);
		__asm__ ("sta %v", pjustfiredcounter);
	#endif

	#if defined (PLAYER_TOP_DOWN) || defined (PLAYER_CAN_FIRE_8_WAY)
		__asm__ ("lda #0");
		__asm__ ("sta %v, x", bmx);
		__asm__ ("sta %v, x", bmy);
	#endif

	#ifdef DOUBLE_WIDTH
		// 16 bits bx [bi] = prx;
		__asm__ ("ldx %v", rdm);
		__asm__ ("lda %v", prx);
		__asm__ ("sta %v, x", bx);
		__asm__ ("lda %v + 1", prx);
		__asm__ ("sta %v + 1, x", bx);
		__asm__ ("ldx %v", bi);
	#else
		// 8 bits bx [bi] = prx;
		__asm__ ("lda %v", prx);
		__asm__ ("sta %v, x", bx);
	#endif
		
		__asm__ ("lda %v", pfacing);
		/*
		__asm__ ("cmp #%b", CELL_FACING_LEFT);
		__asm__ ("beq %g", fire_bullet_facing_left);
		*/
		__asm__ ("cmp #%b", CELL_FACING_RIGHT);
		__asm__ ("beq %g", fire_bullet_facing_right);

	#ifdef PLAYER_TOP_DOWN
		__asm__ ("cmp #%b", CELL_FACING_DOWN);
		__asm__ ("beq %g", fire_bullet_facing_down);
		__asm__ ("cmp #%b", CELL_FACING_UP);
		__asm__ ("beq %g", fire_bullet_facing_up);
	#endif
	
	//fire_bullet_facing_left:
		/*
		bmx [bi] = -PLAYER_BULLET_SPEED;
		bx [bi] = prx - 8;
		by [bi] = pry + PLAYER_BULLET_Y_OFFSET;
		bmy [bi] = 0;
		*/
		__asm__ ("lda #%b", -PLAYER_BULLET_SPEED);
		__asm__ ("sta %v, x", bmx);		

		__asm__ ("lda %v", pry);
		#if PLAYER_BULLET_Y_OFFSET != 0
			__asm__ ("clc");
			__asm__ ("adc #%b", PLAYER_BULLET_Y_OFFSET);
		#endif
		__asm__ ("sta %v, x", by);

		#ifdef DOUBLE_WIDTH
			// 16 bits bx [bi] = prx - 8
			__asm__ ("ldx %v", rdm);
			__asm__ ("sec");
			__asm__ ("lda %v", prx);
			__asm__ ("sbc #8");
			__asm__ ("sta %v, x", bx);
			__asm__ ("lda %v+1", prx);
			__asm__ ("sbc #0");
			__asm__ ("sta %v+1, x", bx);
			__asm__ ("ldx %v", bi);
		#else
			// 8 bits bx [bi] = prx - 8;
			__asm__ ("lda %v", prx);
			__asm__ ("sec");
			__asm__ ("sbc #%b", 8);
			__asm__ ("sta %v, x", bx);
		#endif

		__asm__ ("jmp %g", fire_bullet_facing_done);

	fire_bullet_facing_right:
		/*
		bmx [bi] = PLAYER_BULLET_SPEED;
		bx [bi] = prx + 8;
		by [bi] = pry + PLAYER_BULLET_Y_OFFSET;
		bmy [bi] = 0;
		*/
		__asm__ ("lda #%b", PLAYER_BULLET_SPEED);
		__asm__ ("sta %v, x", bmx);

		__asm__ ("lda %v", pry);
		#if PLAYER_BULLET_Y_OFFSET != 0
			__asm__ ("clc");
			__asm__ ("adc #%b", PLAYER_BULLET_Y_OFFSET);
		#endif
		__asm__ ("sta %v, x", by);			

		#ifdef DOUBLE_WIDTH
			// 16 bits bx [bi] = prx + 8
			__asm__ ("ldx %v", rdm);
			__asm__ ("clc");
			__asm__ ("lda %v", prx);
			__asm__ ("adc #8");
			__asm__ ("sta %v, x", bx);
			__asm__ ("lda %v+1", prx);
			__asm__ ("adc #0");
			__asm__ ("sta %v+1, x", bx);
			__asm__ ("ldx %v", bi);
		#else
			// 8 bits bx [bi] = prx + 8;
			__asm__ ("lda %v", prx);
			__asm__ ("clc");
			__asm__ ("adc #%b", 8);
			__asm__ ("sta %v, x", bx);
		#endif

		__asm__ ("jmp %g", fire_bullet_facing_done);

	#ifdef PLAYER_TOP_DOWN
		fire_bullet_facing_down:		
			/*
			bx [bi] = prx + PLAYER_BULLET_X_OFFSET;
			by [bi] = pry + 12;
			bmy [bi] = PLAYER_BULLET_SPEED;
			bmx [bi] = 0;
			*/
			// TODO: DOUBLE_WIDTH THIS:
			__asm__ ("lda %v", prx);
			__asm__ ("clc");
			__asm__ ("adc #%b", PLAYER_BULLET_X_OFFSET);
			__asm__ ("sta %v, x", bx);

			__asm__ ("lda %v", pry);
			__asm__ ("clc");
			__asm__ ("adc #%b", 12);
			__asm__ ("sta %v, x", by);

			__asm__ ("lda #%b", PLAYER_BULLET_SPEED);
			__asm__ ("sta %v, x", bmy);

			__asm__ ("jmp %g", fire_bullet_facing_done);

		fire_bullet_facing_up:
			/*
			bx [bi] = prx - PLAYER_BULLET_X_OFFSET;
			by [bi] = pry - 4;
			bmy [bi] = -PLAYER_BULLET_SPEED;
			bmx [bi] = 0;
			*/
			// TODO: DOUBLE_WIDTH THIS:
			__asm__ ("lda %v", prx);
			__asm__ ("sec");
			__asm__ ("sbc #%b", PLAYER_BULLET_X_OFFSET);
			__asm__ ("sta %v, x", bx);

			__asm__ ("lda %v", pry);
			__asm__ ("sec");
			__asm__ ("sbc #%b", 4);
			__asm__ ("sta %v, x", by);

			__asm__ ("lda #%b", -PLAYER_BULLET_SPEED);
			__asm__ ("sta %v, x", bmy);
			
			__asm__ ("jmp %g", fire_bullet_facing_done);
	#endif
	fire_bullet_facing_done:
	
	// Diagonals
	#ifdef PLAYER_CAN_FIRE_8_WAY
		#ifdef PLAYER_TOP_DOWN
			// if (pfacing == CELL_FACING_LEFT || pfacing == CELL_FACING_RIGHT)
			__asm__ ("lda %v", pfacing);
			__asm__ ("cmp %b", CELL_FACING_LEFT);
			__asm__ ("beq %g", fire_bullet_diagonal_if1_do)
			__asm__ ("cmp %b", CELL_FACING_RIGHT);
			__asm__ ("bne %g", fire_bullet_diagonal_else2)
		fire_bullet_diagonal_if1_do:
		#endif

			__asm__ ("lda %v", pad0);
			__asm__ ("and #%b", PAD_UP);
			__asm__ ("beq %g", fire_bullet_diagonal_else1);
			__asm__ ("lda #%b", -PLAYER_BULLET_SPEED);
			__asm__ ("jmp %g", fire_bullet_diagonal_endif1);
		fire_bullet_diagonal_else1:	
			__asm__ ("lda %v", pad0);
			__asm__ ("and #%b", PAD_DOWN);
			__asm__ ("beq %g", fire_bullet_diagonal_skip1);
			__asm__ ("lda #%b", PLAYER_BULLET_SPEED);
		fire_bullet_diagonal_endif1:
			__asm__ ("sta %v, x", bmy);

			#ifndef PLAYER_TOP_DOWN
				__asm__ ("lda %v", pad0);
				//__asm__ ("and #%b", 0xC0); // PAD_LEFT|PAD_RIGHT
				__asm__ ("and #%w", (PAD_LEFT|PAD_RIGHT));
				__asm__ ("bne %g", fire_bullet_diagonal_no_cancel);
				__asm__ ("lda #0");
				__asm__ ("sta %v, x", bmx);
			fire_bullet_diagonal_no_cancel:
			#endif
		fire_bullet_diagonal_skip1:

		#ifdef PLAYER_TOP_DOWN
			__asm__ ("jmp %g", fire_bullet_diagonal_if1_done);

		fire_bullet_diagonal_else2:
			__asm__ ("lda %v", pad0);
			__asm__ ("and #%b", PAD_LEFT);
			__asm__ ("beq %g", fire_bullet_diagonal_else2);
			__asm__ ("lda #%b", -PLAYER_BULLET_SPEED);
			__asm__ ("jmp %g", fire_bullet_diagonal_endif2);
		fire_bullet_diagonal_else2:	
			__asm__ ("lda %v", pad0);
			__asm__ ("and #%b", PAD_RIGHT);
			__asm__ ("beq %g", fire_bullet_diagonal_skip2);
			__asm__ ("lda #%b", PLAYER_BULLET_SPEED);
		fire_bullet_diagonal_endif2:
			__asm__ ("sta %v, x", bmx);
		fire_bullet_diagonal_skip2:

		fire_bullet_diagonal_if1_done:
		#endif
	#endif

	#if defined (DOUBLE_WIDTH) && !defined (PLAYER_TOP_DOWN) && !defined (PLAYER_CAN_FIRE_8_WAY)
		b_cy1 [bi] = ((by [bi] + 4 - 16) >> 4);
	#endif

	sfx_play (SFX_BULLET, 2);
	#ifdef DOUBLE_WIDTH
		bullets_disable = 1;
	#endif
}

void bullets_destroy (void) {
	by [bi] = 0;
	b_slots [b_slots_i] = bi; ++ b_slots_i;
	sfx_play (SFX_DUMMY1, 2);
}

void bullets_move (void) {
	#ifdef DOUBLE_WIDTH
	__asm__ ("lda %v", bullets_disable);
	__asm__ ("beq %g", bullets_move_do);

	__asm__ ("lda #0");
	__asm__ ("sta %v", bullets_disable);
	__asm__ ("rts");
	bullets_move_do:
	#endif

	__asm__ ("lda #0");
	__asm__ ("sta %v", bi);

bullets_move_loop:
	__asm__ ("lda %v", bi);
	__asm__ ("cmp #%b", MAX_BULLETS);
	__asm__ ("bcc %g", bullets_move_loop_do);

	__asm__ ("rts");

bullets_move_loop_do:
	#ifdef DOUBLE_WIDTH
		__asm__ ("asl a");
		__asm__ ("sta %v", rdm);
	#endif

	// rde = (bi + half_life) & 1;
	__asm__ ("lda %v", bi);	
	__asm__ ("clc");
	__asm__ ("adc %v", half_life);
	__asm__ ("and 1");
	__asm__ ("sta %v", rde);

	// if (by [bi]) {
	__asm__ ("ldy %v", bi);				// Y = bi
	__asm__ ("lda %v, y", by);
	__asm__ ("jeq %g", bullets_move_loop_continue);

	#if defined (PLAYER_TOP_DOWN) || defined (PLAYER_CAN_FIRE_8_WAY)
		// _by = by [bi] + bmy [bi];
		__asm__ ("clc");
		__asm__ ("adc %v, y", bmy);
		__asm__ ("sta %v", _by);
	#else
		// _by = by [bi];
		__asm__ ("sta %v", _by);
	#endif

	// _bx = bx [bi] + bmx [bi];
	#ifdef DOUBLE_WIDTH
		// bx, _bx are signed int
		__asm__ ("lda %v, y", bmx);
		__asm__ ("ora #$7f");
		__asm__ ("bmi %g", bmx_is_negative);
		__asm__ ("lda #0");
	bmx_is_negative:
		__asm__ ("tax");				// Save hi byte in X
		__asm__ ("lda %v, y", bmx);
		
		__asm__ ("ldy %v", rdm);		// Y = bi*2
		__asm__ ("clc");
		__asm__ ("adc %v, y", bx);		// bx [bi], lo byte, add to bmx
		__asm__ ("sta %v", _bx);		// Store result, lo byte
		__asm__ ("txa");				// Extended sign of bmx
		__asm__ ("adc %v+1, y", bx);	// bx [bi], hi byte, add to X
		__asm__ ("sta %v+1", _bx);		// Store result, hi byte

		__asm__ ("ldy %v", bi);			// Y = bi
	#else
		// Variables are unsigned char
		__asm__ ("lda %v, y", bx);
		__asm__ ("clc");
		__asm__ ("adc %v, y", bmx);
		__asm__ ("sta %v", _bx);
	#endif

	// Out of screen checks

	#ifdef DOUBLE_WIDTH
		// if (_bx < scroll_x ||
		__asm__ ("lda %v", _bx);
		__asm__ ("cmp %v", scroll_x);
		__asm__ ("lda %v + 1", _bx);
		__asm__ ("sbc %v + 1", scroll_x);
		__asm__ ("bvc %g", BMGL1);
		__asm__ ("eor #$80");
	BMGL1:
		__asm__ ("bmi %g", bullets_move_out_of_view);
		// _bx > scroll_x + 248)
		__asm__ ("lda %v", scroll_x);
		__asm__ ("ldx %v+1", scroll_x);
		__asm__ ("clc");
		__asm__ ("adc #248");
		__asm__ ("bcc %g", BMGL2);
		__asm__ ("inx");
	BMGL2:
		// Now X:A contains scroll_x + 248
		__asm__ ("sta %v", rds16);
		__asm__ ("stx %v+1", rds16);
		
		__asm__ ("lda %v", _bx);
		__asm__ ("sec");
		__asm__ ("sbc %v", rds16);
		__asm__ ("sta tmp1");
		__asm__ ("lda %v+1", _bx);
		__asm__ ("sbc %v+1", rds16);
		__asm__ ("ora tmp1");
		__asm__ ("bcs %g", bullets_move_out_of_view);
	#else
		// if (_bx < PLAYER_BULLET_SPEED ||
		__asm__ ("lda %v", _bx);
		__asm__ ("cmp #%b", PLAYER_BULLET_SPEED);
		__asm__ ("bcc %g", bullets_move_out_of_view);

		// _bx > 255 - PLAYER_BULLET_SPEED)
		__asm__ ("cmp #%w", 255-PLAYER_BULLET_SPEED);
		__asm__ ("bcs %g", bullets_move_out_of_view);
	#endif

		// || _by < PLAYER_BULLET_SPEED
		__asm__ ("lda %v", _by);
		__asm__ ("cmp #%b", PLAYER_BULLET_SPEED);
		__asm__ ("bcc %g", bullets_move_out_of_view);

		// || _by > 207 - PLAYER_BULLET_SPEED
		__asm__ ("cmp #%w", 207-PLAYER_BULLET_SPEED);
		__asm__ ("beq %g", bullets_move_out_of_view_skip);
		__asm__ ("bcc %g", bullets_move_out_of_view_skip);

bullets_move_out_of_view:
	bullets_destroy ();
	__asm__ ("jmp %g", bullets_move_loop_continue);
bullets_move_out_of_view_skip:

	#ifdef PLAYER_BULLET_FLICKERS
		// if (bst [bi] > PLAYER_BULLET_FLICKERS || half_life)
		__asm__ ("lda %v", half_life);
		__asm__ ("bne %g", bullets_draw_do);

		__asm__ ("lda %v, y", bst);
		__asm__ ("cmp %b", PLAYER_BULLET_FLICKERS);
		__asm__ ("beq %g", bullets_draw_skip);
		__asm__ ("bcc %g", bullets_draw_skip);
	bullets_draw_do:
	#endif

	oam_index = oam_spr (
		#ifdef DOUBLE_WIDTH
			_bx - scroll_x, 
		#else
			_bx, 
		#endif
		SPRITE_ADJUST + _by, 
		BULLET_PATTERN, BULLET_PALETTE,
		oam_index
	);

	#ifdef PLAYER_BULLET_FLICKERS
	bullets_draw_skip:
	#endif

	// Collision with BG
	__asm__ ("lda %v", rde);
	__asm__ ("beq %g", bullets_collision_done);

	// cx1 = ((_bx + 4) >> 4);
	#ifdef DOUBLE_WIDTH
		// _bx is 16 bits
		cx1 = ((_bx + 4) >> 4);
	#else
		// _bx is 8 bits
		__asm__ ("lda %v", _bx);
		__asm__ ("clc");
		__asm__ ("adc #4");
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("sta %v", cx1);
	#endif

	#if defined (DOUBLE_WIDTH) && !defined (PLAYER_TOP_DOWN) && !defined (PLAYER_CAN_FIRE_8_WAY)
		cy1 = b_cy1 [bi];
	#else
		// cy1 = ((_by + 4 - 16) >> 4);
		__asm__ ("lda %v", _by);
		__asm__ ("adc #%b", -12);
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("lsr a");
		__asm__ ("sta %v", cy1);
	#endif

	rdm = MAP_ATTR (COORDS (cx1, cy1));
	
	#ifdef PLAYER_BULLET_LIFE
		-- bst [bi]; 
		if (bst [bi] == 0) bullets_destroy (); 
		else
	#endif
	#ifdef ENABLE_BREAKABLE
		if (rdm & 16) {
			_x = cx1; _y = cy1; breakable_break ();
			bullets_destroy ();
			__asm__ ("jmp %g", bullets_move_loop_continue);
		} else
	#endif

	if (rdm & 8) {
		bullets_destroy (); 
		__asm__ ("jmp %g", bullets_move_loop_continue);
	}	
bullets_collision_done:

	bx [bi] = _bx;
	#if defined (PLAYER_TOP_DOWN) || defined (PLAYER_CAN_FIRE_8_WAY)
		by [bi] = _by;
	#endif

bullets_move_loop_continue:
	// Loop 
	__asm__ ("inc %v", bi);
	__asm__ ("jmp %g", bullets_move_loop);
}
