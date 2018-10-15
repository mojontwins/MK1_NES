// NES MK1 v2.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Hotspot interaction

if (hrt) {
	if (
		prx + 4 >= hrx && 
		prx <= hrx + 11 &&
		pry + 8 >= hry && 
		pry <= hry + 7
	) {
		#ifdef ENABLE_RESONATORS
			if (hrt == HOTSPOT_TYPE_RESONATOR) {
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
			} else
		#endif
		{
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

				if (use_ct == USE_ANIM_INTERACT_ON && use_sub_ct == USE_ANIM_FRAMES_PER_STEP && use_type == USE_TYPE_HOTSPOT)
			#endif
			{
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
					rda = 0;
					switch (hrt) {
						#ifndef DEACTIVATE_OBJECTS
							case HOTSPOT_TYPE_OBJECT:
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
									++ pobjs;
									rda = SFX_OBJECT;
								#endif
								break;
						#endif
						#ifndef DEACTIVATE_KEYS
							case HOTSPOT_TYPE_KEYS:
								++ pkeys;
								rda = SFX_OBJECT;
								break;
						#endif
							case HOTSPOT_TYPE_REFILL:
								plife += PLAYER_REFILL;
								#ifdef LIMIT_LIFE
									if (plife > PLAYER_LIFE) plife = PLAYER_LIFE;
								#endif
								rda = SFX_USE;
								break;
						#ifdef MAX_AMMO
							case HOTSPOT_TYPE_AMMO:
								rda = SFX_OBJECT;
								if (MAX_AMMO - pammo > AMMO_REFILL)
									pammo += AMMO_REFILL;
								else
									pammo = MAX_AMMO;
								break;
						#endif
						#if defined (ENABLE_TIMER) && defined (HOTSPOT_TYPE_TIME)
							case HOTSPOT_TYPE_TIME:
								rda = SFX_OBJECT;
								#if TIMER_REFILL == 0
									timer = TIMER_INITIAL;
								#else
									if (TIMER_INITIAL - timer > TIMER_REFILL)
										timer += TIMER_REFILL;
									else
										timer = TIMER_INITIAL;
								#endif
								break;
						#endif
						#ifdef HOTSPOT_TYPE_STAR
							case HOTSPOT_TYPE_STAR:
								++ pstars; rda = SFX_OBJECT;
								break;
						#endif
						#include "my/extra_hotspots.h"
					}
					if (rda) {
						sfx_play (rda, 1);
						hrt = 0;
						hact [HS_CUR_PANT] = 0;
					}
				}

				#if defined (ENABLE_ONLY_ONE_OBJECT) || defined (ENABLE_EASY_OBJECTS)
					if (opinv != pinv) {
						// Object has been got. You may complete here
						#include "my/on_object_got.h"
					}
				#endif
			}		
		}
	}
}
