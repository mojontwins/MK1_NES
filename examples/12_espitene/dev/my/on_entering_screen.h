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
// Boss is always enemy 0 on screen
if (en_t [0] == 0x14) en_life [0] = 8;

if (level_world == 3) {
	// Set underwater screens
	rda = underwater;
	timer_on = underwater = (level_act == 2 || n_pant >= 10);
	if (!underwater) timer = 20;
	if (underwater != rda) sfx_play (SFX_FLOAT, 1);

	pal_bg (underwater ? palts3a : palts3);
	pal_spr (underwater ? palss0a : palss0);
}

// For the split effect in the fridge zone
water_strip = (level_world == 1 && (level != 3 || n_pant > 14));
oam_spr (128, water_strip ? 211 : 240, 1, 2, 0);

// Reset toggle timer
toggle_timer = 0;

// Clear bridge bits
memfill (bridge_y, 0, 4);
