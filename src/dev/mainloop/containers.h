// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Containers interaction

if (containers_index) {
	if (b_button) {
		rdx = prx + 4; rdy = pry + 8;

		gpit = containers_index; while (gpit --) {
			rda = containers_yx [gpit];
			if (collide_in (rdx, rdy, rda << 4, rda & 0xf0)) {
				#ifdef ENABLE_USE_ANIM
					if (	
						use_ct == 0
						#ifndef PLAYER_TOP_DOWN
							&& ppossee
						#endif
					) {
						use_ct = 1; use_sub_ct = USE_ANIM_FRAMES_PER_STEP;
						b_button = a_button = 0;
					}
				#endif
				containers_interact_with = gpit;
				break;
			}
		}
	}

	if (
		containers_interact_with
		#ifdef ENABLE_UESE_ANM
			&& use_ct == USE_ANIM_INTERACT_ON && use_sub_ct == USE_ANIM_FRAMES_PER_STEP
		#endif
	) {
		containers_interact_with = 0;
		rda = flags [containers_f [containers_interact_with]];
		flags [containers_f [containers_interact_with]] = flags [FLAG_INVENTORY];
		flags [FLAG_INVENTORY] = rda;
		sfx_play (2, 1);

		// Object has been got. You may complete here. You are using scripting but just in case.
		#include "my/on_object_got.h"
	}
}
