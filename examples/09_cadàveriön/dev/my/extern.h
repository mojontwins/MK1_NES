// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// extern.h
// this function is ran from the script:

const unsigned char ct_spacer []    = "            ";
const unsigned char ct_time_over [] = " TIME OVER! ";

void do_extern_action (unsigned char n) {
	if (n == 0) {
		// TIME OVER
		oam_hide_rest (0); 
		music_pause (1);
		_x = 10; _y = 14; gp_gen = ct_spacer;       pr_ul_str ();
		_x = 10; _y = 15; gp_gen = ct_time_over;    pr_ul_str ();
		_x = 10; _y = 16; gp_gen = ct_spacer;       pr_ul_str ();
		while (1) {
			pad_read ();
			if (pad_this_frame & (PAD_A|PAD_B|PAD_START)) break;
			ppu_waitnmi ();
		}
		music_pause (0);
		hotspots_load ();
	}

}
