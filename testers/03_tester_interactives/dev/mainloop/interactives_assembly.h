// NES MK1 v2.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Interactives interaction

	__asm__ ("lda %v", interactives_index);
	__asm__ ("jeq %g", ii_done)

	// If (b_button)
	__asm__ ("lda %v", b_button);
	__asm__ ("jeq %g", ii_check_done);

	#ifdef ENABLE_USE_ANIM
		// && use_ct == 0
		__asm__ ("lda %v", use_ct);
		__asm__ ("jne %g", ii_check_done);
	#endif

	// PRXA = prx + 4;	

	__asm__ ("lda %v", prx);
	__asm__ ("clc");
	__asm__ ("adc #4");
	__asm__ ("sta %v", PRXA);
	#ifdef DOUBLE_WIDTH
		__asm__ ("lda %v+1", prx);
		__asm__ ("adc #0");
		__asm__ ("sta %v+1", PRXA);
	#endif

	// rdy = pry + 8;
	__asm__ ("lda %v", pry);
	__asm__ ("clc");
	__asm__ ("adc #8");
	__asm__ ("sta %v", rdy);

	__asm__ ("lda %v", interactives_index);
	__asm__ ("sta %v", gpit);

ii_check_loop:
	__asm__ ("lda %v", gpit);
	__asm__ ("beq %g", ii_check_done);

	__asm__ ("dec %v", gpit);
	__asm__ ("ldy %v", gpit);

	#ifdef DOUBLE_WIDTH
		// rdc = interactives_y [gpit];
		__asm__ ("lda %v, y", interactives_y);
		__asm__ ("sta %v", rdc);

		// rdaa = interactives_x [gpit] << 4;
		__asm__ ("lda %v, y", interactives_x);
		__asm__ ("ldx #0");
		__asm__ ("jsr aslax4");
		__asm__ ("sta %v", rdaa);
		__asm__ ("stx %v+1", rdaa);

		// if (PRXA + 4 >= rdaa)
		__asm__ ("lda %v", PRXA);
		__asm__ ("clc");
		__asm__ ("adc #4");
		__asm__ ("sta %v", rds16);
		__asm__ ("lda %v+1", PRXA);
		__asm__ ("adc #0");
		__asm__ ("sta %v+1", rds16);

		// if (rds16 >= rdaa)
		// (inverted a < b)
		__asm__ ("lda %v", rds16);
		__asm__ ("cmp %v", rdaa);
		__asm__ ("lda %v+1", rds16);
		__asm__ ("sbc %v+1", rdaa);
		// bcs DO; jmp SKIP
		__asm__ ("bcs %g", ii_check_if1)
		__asm__ ("jmp %g", ii_check_done);
	
	ii_check_if1:
		// && PRXA <= rdaa + 19
		__asm__ ("lda %v", rdaa);
		__asm__ ("clc");
		__asm__ ("adc #19");
		__asm__ ("sta %v", rds16);
		__asm__ ("lda #v+1", rdaa);
		__asm__ ("adc #0");
		__asm__ ("sta %v+1", rds16);

		// && PRXA <= rds16
		// && rds16 >= PRXA
		// (inverted a < b)
		__asm__ ("lda %v", rds16);
		__asm__ ("cmp %v", PRXA);
		__asm__ ("lda %v+1", rds16);
		__asm__ ("sbc %v+1", PRXA);
		// bcs DO; jmp SKIP
		__asm__ ("bcs %g", ii_check_if2);
		__asm__ ("jmp %g", ii_check_done);

	ii_check_if2:
		// && rdy >= rdc (skip if rdy < rdc (BCC))

		// && rdy <= rdc + 15
		// && rdc+15 >= rdy (skip if rdc+16 < rdc (BCC))

	
	#else
	#endif

ii_check_done:


ii_done:
