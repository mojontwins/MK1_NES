// Cutscene

void game_over_scr (void) {
	cls ();
	pal_bg (mypal_cuts);
	
	rda = 0;
	
	pr_str (11, 15, "64M3 0V3>!");

	pal_bright (0);
	ppu_on_all ();
	while (pad_poll (0));
	
	fade_delay = 4;
	fade_in ();

	while (0 == pad_poll (0));

	fade_out ();
	ppu_off ();
}
