// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Throw fire script

	#ifdef ENABLE_USE_ANIM
		if (b_button && use_ct == 0) {
			use_ct = 1;
			run_fire_script_do = 1;
			b_button = 0;
			use_type = USE_TYPE_SCRIPTING;
		}
	#else
		if (b_button) {
			run_fire_script ();
			if (fire_script_success) b_button = 0;
		}
	#endif
