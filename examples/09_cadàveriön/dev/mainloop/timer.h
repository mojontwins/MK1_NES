// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// time advance in the mainloop

if (timer_on && timer) {
	if (timer_frames) timer_frames --; else {
		timer_frames = 50;
		timer --; 
		if (timer == 0) {
			#ifdef TIMER_ZERO_FLAG
				flags [TIMER_ZERO_FLAG] = 
			#endif
			timer_zero = 1;
			#ifdef ACTIVATE_SCRIPTING
				run_script ((n_pant << 1) + 3);
			#endif
		}
	}
}
#ifdef TIMER_TIME_FLAG
	flags [TIMER_TIME_FLAG] = timer;
#endif
