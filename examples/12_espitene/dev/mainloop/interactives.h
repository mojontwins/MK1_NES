// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Interactives interaction

if (interactives_index) {
	if (
		b_button
		#ifdef ENABLE_USE_ANIM
			&& use_ct == 0
		#endif
	) {
		rdx = prx + 4; rdy = pry + 8;

		gpit = interactives_index; while (gpit --) {
			rda = interactives_yx [gpit]; rdb = rda << 4; rdc = rda & 0xf0;
			if (
				(rdx >= rdb - 4 && rdx <= rdb + 19 && rdy >= rdc && rdy <= rdc + 15)
				#ifndef PLAYER_TOP_DOWN
					/*&& ppossee*/
				#endif
			) {
				#if defined (ENABLE_USE_ANIM) && !defined (INTERACTIVES_ONLY_SPRITES)
					// Only if it is a genuine interactive				
					if (interactives_f [gpit] & 0x80) {
						use_ct = 1; use_sub_ct = USE_ANIM_FRAMES_PER_STEP;
						use_type = USE_TYPE_INTERACTIVE;
					}
				#endif
				interactives_interact_with = gpit;
				b_button = 0;
				break;
			}
		}
	}

	if (interactives_interact_with != 0xff) {
		rdc = interactives_f [interactives_interact_with];
		#ifdef ACTIVATE_SCRIPTING
			script_arg = rdc;
		#endif

		#if defined (ENABLE_USE_ANIM) && !defined (INTERACTIVES_ONLY_SPRITES)
			if (
				(use_ct == USE_ANIM_INTERACT_ON && use_sub_ct == USE_ANIM_FRAMES_PER_STEP)
				|| (rdc & 0x80) == 0
			)
		#endif
		{
			#if !defined (INTERACTIVES_ONLY_SPRITES)
				if (rdc & 0x80) {	
					rda = rdc & 0x7f;
					rdb = flags [rda];
					flags [rda] = flags [FLAG_INVENTORY];
					flags [FLAG_INVENTORY] = rdb;
					sfx_play (SFX_OBJECT, 1);
				} 
			#endif

			#include "my/on_interactive.h"

			#ifdef ACTIVATE_SCRIPTING
				// Run script; script_arg is set, always success
				just_interacted = 1;
				run_fire_script ();
				fire_script_success = 1;
			#endif	

			interactives_interact_with = 0xff;
		}
	}
}
