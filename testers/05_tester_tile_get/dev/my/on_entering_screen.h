// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Add here your code. Just entered n_pant.
// Background is rendered, everything initialized.
// Screen is ON.

// Turn off ring if it is on
ring_timer = 0;

// Invalidate emmeralds
if (
	hrt == HOTSPOT_TYPE_EMMERALD && 
	pemmeralds & bits [level_world]
) hrt = 0;

// Make bosses more resistant
// Boss is always enemy on screen
if (en_t [0] == 0x14) en_life [0] = 8;

if (level_world == 3) {
	// Set underwater screens
	timer_on = underwater = (level_act == 2 || n_pant >= 10);
	if (!underwater) timer = 20;

	pal_bg (underwater ? palts3a : palts3);
	pal_spr (underwater ? palss0a : palss0);
}
