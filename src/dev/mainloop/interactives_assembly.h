// NES MK1 v2.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Interactives interaction

	__asm__ ("lda %v", interactives_index);
	__asm__ ("jeq %g", ii_done);

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
		// y is already in pixels
		__asm__ ("lda %v, y", interactives_y);
		__asm__ ("sta %v", rdc);

		// rdaa = interactives_x [gpit] << 4;
		// x is in tile coordinates
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
		__asm__ ("bcs %g", ii_check_if1);
		__asm__ ("jmp %g", ii_check_loop);
	
	ii_check_if1:
		// && PRXA <= rdaa + 19
		__asm__ ("lda %v", rdaa);
		__asm__ ("clc");
		__asm__ ("adc #19");
		__asm__ ("sta %v", rds16);
		__asm__ ("lda %v+1", rdaa);
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
		__asm__ ("jmp %g", ii_check_loop);

	ii_check_if2:
		// && rdy >= rdc (skip if rdy < rdc (BCC))
		__asm__ ("lda %v", rdy);
		__asm__ ("cmp %v", rdc);
		__asm__ ("bcc %g", ii_check_loop);

		// && rdy <= rdc + 15
		// && rdc+15 >= rdy (skip if rdc+16 < rdc (BCC))
		__asm__ ("lda %v", rdc);
		__asm__ ("clc");
		__asm__ ("adc #15");
		__asm__ ("cmp %v", rdy);
		__asm__ ("bcc %g", ii_check_loop);
	#else
		// rda = interactives_yx [gpit]; 
		__asm__ ("lda %v, y", interactives_yx);
		__asm__ ("sta %v", rda);

		// rdb = rda << 4; 
		__asm__ ("asl a");
		__asm__ ("asl a");
		__asm__ ("asl a");
		__asm__ ("asl a");
		__asm__ ("sta %v", rdb);

		// rdc = rda & 0xf0;
		__asm__ ("lda %v", rda);
		__asm__ ("and #$f0");
		__asm__ ("sta %v", rdc);

		// if (PRXA + 4 >= rdb && 
		// (skip if a < b (BCC))
		__asm__ ("lda %v", PRXA);
		__asm__ ("clc");
		__asm__ ("adc #4");
		__asm__ ("cmp %v", rdb);
		__asm__ ("bcc %g", ii_check_loop);

		// PRXA <= rdb + 19 && 
		// rdb + 19 >= PRXA &&
		// (skip if a < b (BCC))
		__asm__ ("lda %v", rdb);
		__asm__ ("clc");
		__asm__ ("adc #19");
		__asm__ ("cmp %v", PRXA);
		__asm__ ("bcc %g", ii_check_loop);

		// rdy >= rdc && 
		// (skip if a < b (BCC))
		__asm__ ("lda %v", rdy);
		__asm__ ("cmp %v", rdc);
		__asm__ ("bcc %g", ii_check_loop);

		// rdy <= rdc + 15) 
		// rdc + 15 >= rdy)
		// (skip if a < b (BCC))
		__asm__ ("lda %v", rdc);
		__asm__ ("clc");
		__asm__ ("adc #15");
		__asm__ ("cmp %v", rdy);
		__asm__ ("bcc %g", ii_check_loop);		
	#endif

	// ii_check_do:

	#if defined (ENABLE_USE_ANIM) && !defined (INTERACTIVES_ONLY_SPRITES)
		// Only if it's a genuine interactive (bit 7)
		__asm__ ("lda %v, y", interactives_f);
		__asm__ ("and #$80");
		__asm__ ("beq %g", ii_check_do_skip);

		__asm__ ("lda #1");
		__asm__ ("sta %v", use_ct);
		__asm__ ("lda #%b", USE_ANIM_FRAMES_PER_STEP);
		__asm__ ("sta %v", use_sub_ct);
		__asm__ ("lda #%b", USE_TYPE_INTERACTIVE);
		__asm__ ("sta %v", use_type);

	ii_check_do_skip:
	#endif

		__asm__ ("tya");
		__asm__ ("sta %v", interactives_interact_with);
		__asm__ ("lda #0");
		__asm__ ("sta %v", b_button);

	ii_check_done:

	// Interact with an interactive?

	__asm__ ("lda %v", interactives_interact_with);
	__asm__ ("cmp #$FF");
	__asm__ ("beq %g", ii_done);

	__asm__ ("tay");
	__asm__ ("lda %v, y", interactives_f);
	__asm__ ("sta %v", rdc);

	#ifdef ACTIVATE_SCRIPTING
		__asm__ ("sta %v", script_arg);
	#endif

	#if defined (ENABLE_USE_ANIM) && !defined (INTERACTIVES_ONLY_SPRITES)
		// if (
		// (use_ct == USE_ANIM_INTERACT_ON && 
		__asm__ ("lda %v", use_ct);
		__asm__ ("cmp #%b", USE_ANIM_INTERACT_ON);
		__asm__ ("bne %g", ii_if2);

		// use_sub_ct == USE_ANIM_FRAMES_PER_STEP)
		__asm__ ("lda %v", use_sub_ct);
		__asm__ ("cmp #%b", USE_ANIM_FRAMES_PER_STEP);
		__asm__ ("beq %g", ii_do);

	ii_if2:
		// || (rdc & 0x80) == 0))
		__asm__ ("lda %v", rdc);
		__asm__ ("and #$80");
		__asm__ ("bne %g", ii_done);

	ii_do:	
	#endif

	#if !defined (INTERACTIVES_ONLY_SPRITES)
		__asm__ ("lda %v", rdc);
		__asm__ ("and #$80");
		__asm__ ("beq %g", ii_genuine_done);

		__asm__ ("lda %v", rdc);
		__asm__ ("and #$7f");
		__asm__ ("tay");
		__asm__ ("lda %v, y", flags);
		__asm__ ("sta %v", rdb);
		__asm__ ("ldx #%b", FLAG_INVENTORY);
		__asm__ ("lda %v, x", flags);
		__asm__ ("sta %v, y", flags);
		__asm__ ("lda %v", rdb);
		__asm__ ("sta %v, x", flags);
		
		sfx_play (SFX_OBJECT, 1);

	ii_genuine_done:
	#endif

	#include "my/on_interactive.h"

	#ifdef ACTIVATE_SCRIPTING
		// Run script; script_arg is set, always success
		just_interacted = 1;
		run_fire_script ();
		fire_script_success = 1;
	#endif

	__asm__ ("lda #$FF");
	__asm__ ("sta %v", interactives_interact_with);

ii_done:
