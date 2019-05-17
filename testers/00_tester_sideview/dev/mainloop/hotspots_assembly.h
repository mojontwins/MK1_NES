// NES MK1 v2.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Hotspot interaction (being rewritten in assembly)

void hotspots_do (void) {

		__asm__ ("lda %v", hrt);
		__asm__ ("bne %g", hrt_interaction_coll_1);
		__asm__ ("rts");

	hrt_interaction_coll_1:
		// if (prx + 4 >= hrx
		__asm__ ("lda %v", prx);
		__asm__ ("clc");
		__asm__ ("adc #4");
		__asm__ ("cmp %v", hrx);

		// Here, carry set if >=, so if carry clear, skip
		__asm__ ("bcc %g", hrt_interaction_done);

		// && prx <= hrx + 11 -> hrx + 11 >= prx
		__asm__ ("lda %v", hrx);
		__asm__ ("clc");
		__asm__ ("adc #11");
		__asm__ ("cmp %v", prx);
		
		// Here, carry set if >=, so if carry clear, skip
		__asm__ ("bcc %g", hrt_interaction_done);

		// && pry + 8 >= hry
		__asm__ ("lda %v", pry);
		__asm__ ("clc");
		__asm__ ("adc #8"); 
		__asm__ ("cmp %v", hry);

		// Here, carry set if >=, so if carry clear, skip
		__asm__ ("bcc %g", hrt_interaction_done);

		// && pry <= hry + 7 -> hry + 7 >= pry
		__asm__ ("lda %v", hry);
		__asm__ ("clc");
		__asm__ ("adc #7");
		__asm__ ("cmp %v", pry);

		// Here, carry set if >=, so if carry clear, skip
		__asm__ ("bcc %g", hrt_interaction_done);
		__asm__ ("jmp %g", hrt_interaction_pre);

	hrt_interaction_done:
		__asm__ ("rts");
	hrt_interaction_pre:

		// DO

	#ifdef ENABLE_RESONATORS
		__asm__ ("lda %v", hrt);
		__asm__ ("cmp #%b", HOTSPOT_TYPE_RESONATOR);
		__asm__ ("bne %g", hrt_resonators_done);

		if (pvy > 0 && pry < hry) {
			if (res_on == 0) {
				res_on = 1; res_ct = 9; res_subct = 50;
				jump_start ();
				
				#ifdef RESONATOR_CHANGE_BG_PAL
					pal_bg (RESONATOR_CHANGE_BG_PAL);
				#endif

				#ifdef RESONATOR_CHANGE_SPR_PAL
					pal_spr (RESONATOR_CHANGE_SPR_PAL);
				#endif

				sfx_play (SFX_BREAKH, 1);
			} 
		}

		__asm__ ("rts");
	hrt_resonators_done:
	#endif

	#ifdef ENABLE_USE_ANIM
		if (
			use_ct == 0 && b_button 
			#ifndef PLAYER_TOP_DOWN
				&& ppossee
			#endif
		) {
			use_ct = 1; use_sub_ct = USE_ANIM_FRAMES_PER_STEP;
			b_button = a_button = 0;
			use_type = USE_TYPE_HOTSPOT;
		}

		if (
			use_ct != USE_ANIM_INTERACT_ON ||
			use_sub_ct != USE_ANIM_FRAMES_PER_STEP ||
			use_type |= USE_TYPE_HOTSPOT
		) return;
	#endif

	#ifdef CARRY_ONE_HS_OBJECT
		opinv = pinv;

		if (hrt == HS_OBJ_EMPTY) {
			// Empty hotspot. Drop object

			#ifndef ENABLE_USE_ANIM
			if (b_button) 
			#endif
			{
				hrt = ht [HS_CUR_PANT] = pinv;
				pinv = HS_OBJ_EMPTY;

				b_button = 0;
				sfx_play (SFX_TILE, 1);
			}

		} else if (hrt >= HS_OBJ_MIN && hrt <= HS_OBJ_MAX) {
			// Object

			#ifndef ENABLE_USE_ANIM
			if (b_button) 
			#endif
			{
				// Interchange pinv and hrt, and register in array.
				rda = hrt;
				hrt = ht [HS_CUR_PANT] = pinv;
				pinv = rda;

				b_button = 0;
				sfx_play (SFX_OBJECT, 1);
			}

		} else if (hrt >= HS_OBJ_MIN + HS_USE_OFFS && hrt <= HS_OBJ_MAX + HS_USE_OFFS) {
			// Where to use object

			#ifndef ENABLE_USE_ANIM
			if (b_button) 
			#endif
			{
				if (pinv == hrt - HS_USE_OFFS) {
					// Save them
					rda = pinv;
					rdb = hrt;

					// Update hotspot
					#ifdef HS_FIX_ON_USE
						hrt = ht [HS_CUR_PANT] = pinv + 2*HS_USE_OFFS;
					#else
						hrt = ht [HS_CUR_PANT] = pinv;
					#endif

					// Clear carried object
					pinv = HS_OBJ_EMPTY;

					// Object has been used. You may complete here
					#include "my/on_object_used.h"

					b_button = 0;
					sfx_play (SFX_USE, 1);

				} 
				#ifdef ENABLE_NO
					else no_ct = 100;
				#endif	
			}
		}
		#ifdef HS_FIX_ON_USE				
		else if (hrt >= HS_OBJ_MIN + 2*HS_USE_OFFS && hrt <= HS_OBJ_MAX + 2*HS_USE_OFFS) {
			#ifndef ENABLE_USE_ANIM
			if (b_button) 
			#endif
			{
				// Already used object. do nothing.
				#ifdef ENABLE_NO
					no_ct = 100;
				#endif	
			}
		}
		#endif			
		else			
	#endif

	{
		__asm__ ("lda #0");
		__asm__ ("sta %v", rda);

		// switch hrt
		__asm__ ("lda %v", hrt);

	#ifndef DEACTIVATE_OBJECTS
		__asm__ ("cmp #%b", HOTSPOT_TYPE_OBJECT);
		__asm__ ("beq %g", hrt_type_object);
	#endif

	#ifndef DEACTIVATE_KEYS
		__asm__ ("cmp #%b", HOTSPOT_TYPE_KEYS);
		__asm__ ("beq %g", hrt_type_keys);
	#endif

	#ifdef MAX_AMMO
		__asm__ ("cmp #%b", HOTSPOT_TYPE_AMMO);
		__asm__ ("beq %g", hrt_type_ammo);
	#endif

	#if defined (ENABLE_TIMER) && defined (HOTSPOT_TYPE_TIME)
		__asm__ ("cmp #%b", HOTSPOT_TYPE_TIME);
		__asm__ ("beq %g", hrt_type_time);
	#endif

	#ifdef HOTSPOT_TYPE_STAR
		__asm__ ("cmp #%b", HOTSPOT_TYPE_STAR);
		__asm__ ("beq %g", hrt_type_star);
	#endif

		// By default, it is a refill XD
		// __asm__ ("cmp #%b", HOTSPOT_TYPE_REFILL);
		// __asm__ ("beq %g", hrt_type_refill);

		__asm__ ("lda %v", plife);
		__asm__ ("clc");
		__asm__ ("adc #%b", PLAYER_REFILL);
	#ifdef LIMIT_LIFE
		__asm__ ("cmp #%b", PLAYER_LIFE);
		__asm__ ("bcc hrt_type_refill_limit_skip")
		__asm__ ("lda #%b", PLAYER_LIFE);
	hrt_type_refill_limit_skip:
	#endif
		__asm__ ("sta %v", plife);
		__asm__ ("lda #%b", SFX_USE);
		__asm__ ("sta %v", rda);

		__asm__ ("jmp %g", hrt_interaction_finish);

	#ifndef DEACTIVATE_OBJECTS
	hrt_type_object:
		#ifdef ENABLE_ONLY_ONE_OBJECT
			#ifdef ONLY_ONE_OBJECT_FLAG
				if (flags [ONLY_ONE_OBJECT_FLAG] == 0) {
					flags [ONLY_ONE_OBJECT_FLAG] = 1;
					rda = SFX_OBJECT;
				}
			#else
				if (pinv == 0) {
					pinv = 1;
					rda = SFX_OBJECT;
				}
			#endif
		#else
			__asm__ ("inc %v", pobjs);
			__asm__ ("lda #%b", SFX_OBJECT);
			__asm__ ("sta %v", rda);
		#endif
		__asm__ ("jmp %g", hrt_interaction_finish);	
	#endif

	#ifndef DEACTIVATE_KEYS
	hrt_type_keys:
		__asm__ ("inc %v", pkeys);
		__asm__ ("lda #%b", SFX_OBJECT);
		__asm__ ("sta %v", rda);
		__asm__ ("jmp %g", hrt_interaction_finish);	
	#endif

	#ifdef MAX_AMMO
	hrt_type_ammo:
		__asm__ ("lda #%b", SFX_OBJECT);
		__asm__ ("sta %v", rda);

		// if (MAX_AMMO - pammo > AMMO_REFILL)
		__asm__ ("lda #%b", MAX_AMMO);
		__asm__ ("sec");
		__asm__ ("sbc %v", pammo);
		__asm__ ("cmp #%b", AMMO_REFILL);
		__asm__ ("bcc %g", hrt_type_ammo_do_set);

		__asm__ ("lda %v", pammo);
		__asm__ ("clc");
		__asm__ ("adc #%b", AMMO_REFILL);
		__asm__ ("sta %v", pammo);
		__asm__ ("jmp %g", hrt_interaction_finish);

	hrt_type_ammo_do_set:
		__asm__ ("lda #%b", MAX_AMMO);
		__asm__ ("sta %v", pammo);

		__asm__ ("jmp %g", hrt_interaction_finish);	
	#endif

	#if defined (ENABLE_TIMER) && defined (HOTSPOT_TYPE_TIME)
	hrt_type_time:
		__asm__ ("lda #%b", SFX_OBJECT);
		__asm__ ("sta %v", rda);

	#if TIMER_REFILL == 0
		timer = TIMER_INITIAL;
	#else
		__asm__ ("lda #%b", TIMER_INITIAL);
		__asm__ ("sec");
		__asm__ ("sbc %v", timer);
		__asm__ ("cmp #%b", TIMER_REFILL);
		__asm__ ("bcc %g", hrt_type_timer_do_set);

		__asm__ ("lda %v", timer);
		__asm__ ("clc");
		__asm__ ("adc #%b", TIMER_REFILL);
		__asm__ ("sta %v", timer);
		__asm__ ("jmp %g", hrt_interaction_finish);

	hrt_type_timer_do_set:
		__asm__ ("lda #%b", TIMER_INITIAL);
		__asm__ ("sta %v", timer);
	#endif

		__asm__ ("jmp %g", hrt_interaction_finish);	
	#endif

	#ifdef HOTSPOT_TYPE_STAR
	hrt_type_star:
		__asm__ ("inc %v", pstars);
		__asm__ ("lda #%b", SFX_OBJECT);
		__asm__ ("sta %v", rda);
		__asm__ ("jmp %g", hrt_interaction_finish);	
	#endif

	hrt_interaction_finish:
		if (rda) {
			sfx_play (rda, 1);
			hrt = 0;
			hact [HS_CUR_PANT] = 0;
		}

	}

}
