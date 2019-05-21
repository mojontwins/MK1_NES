// NES MK1 v2.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Throw fire script
#ifdef ENABLE_USE_ANIM
	if (run_fire_script_do && 
		use_ct == USE_ANIM_INTERACT_ON && 
		use_sub_ct == USE_ANIM_FRAMES_PER_STEP &&
		use_type == USE_TYPE_SCRIPTING
	) {
		run_fire_script ();
		sfx_play (fire_script_success ? SFX_OBJECT : SFX_SPRING, 1);
		run_fire_script_do = 0;
	}
	#ifdef ENABLE_FIRE_ZONE
		else
	#endif
#endif

#ifdef ENABLE_FIRE_ZONE			
	if (f_zone_ac && (prx >= fzx1 && prx <= fzx2 && pry >= fzy1 && pry <= fzy2)) {		
		run_fire_script ();
	} 
#endif
