// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Throw fire script

#ifdef ENABLE_FAST_FIRE_ZONE
	#ifndef FIRE_SCRIPT_WITH_ANIMATION			
		i = pad_poll (0);
		if (i & PAD_B) run_fire_script ();
	#endif			
		if (f_zone_ac) 
			if (pry >= fzy1 && pry <= fzy2)
				if (prx >= fzx1 && prx <= fzx2)
					run_script (n_pant + n_pant + 1);
#else
	#ifndef FIRE_SCRIPT_WITH_ANIMATION
		#ifdef ENABLE_FIRE_ZONE
		if (i & PAD_B || (f_zone_ac && (prx >= fzx1 && prx <= fzx2 && pry >= fzy1 && pry <= fzy2))) 
		#else
		if (i & PAD_B)
		#endif
			run_fire_script ();
	#else
		#ifdef ENABLE_FIRE_ZONE
		if (f_zone_ac) 
			if (pry >= fzy1 && pry <= fzy2)
				if (prx >= fzx1 && prx <= fzx2)
					run_fire_script ();
		#endif
	#endif 
#endif

