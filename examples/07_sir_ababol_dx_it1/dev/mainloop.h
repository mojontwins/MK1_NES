// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017

// Main loop & helpers

void game_init (void) {

	win_level = game_over = 0;

	// Assets setup. Selects tileset, map, palettes, etc.
	#include "mainloop/asset_setup.h"

	pal_bg (c_pal_bg);
	pal_spr (c_pal_fg);

	cls ();

	draw_game_frame ();
	//clean_gauge ();

	// CUSTOM {
	if (level_switching) {
		n_pant = n_pant_switch; 
	} else
	// } END_OF_CUSTOM
	n_pant = SCR_INI;
	
	hotspots_load ();
#ifndef DEACTIVATE_KEYS		
	bolts_load ();
#endif		
	player_init ();
	// CUSTOM {
	/*
	px = (4 + (PLAYER_INI_X << 4)) << FIXBITS;
	py = (PLAYER_INI_Y << 4) << FIXBITS;
	*/
	switch (level) {
		case 0:
			if (n_pant >= 20) {
				pvy = -PLAYER_VY_FALLING_MAX;
				py = 176 << FIXBITS;
			}
			break;
		case 1:
			pvy = PLAYER_VY_SWIM_MAX << 1;
			py = 16 << FIXBITS;
			break;
	}
	// } END_OF_CUSTOM

#ifdef PERSISTENT_ENEMIES
	enems_persistent_load ();
#endif		
#ifdef PERSISTENT_DEATHS
	enems_persistent_deaths_load ();
#endif

#ifdef CLEAR_FLAGS
	msc_clear_flags ();
#endif

	// CUSTOM {
	/*
	pobjs = 0;
	pkeys = 0;
	*/
	// } END_OF_CUSTOM

	half_life = 0;
	frame_counter = 0;
	olife = oammo = oobjs = okeys = 0xff;
	okilled = 0xff;

	// n_pant = 2; pkeys = 1;
}

void prepare_scr (void) {
	if (!ft) fade_out (); else ft = 0;
	
#ifdef ENABLE_PROPELLERS
	clear_propellers ();
#endif

#ifdef PERSISTENT_ENEMIES
	// Preserve enems
	enems_persistent_update ();
#endif

	enems_load ();
	hotspots_create ();	

#ifdef ENABLE_COCOS
	cocos_init ();
#endif	

#if defined (ACTIVATE_SCRIPTING) && (defined (ENABLE_FIRE_ZONE) || defined (ENABLE_FAST_FIRE_ZONE))
	f_zone_ac = 0;
	fzx1 = fzx2 = fzy1 = fzy2 = 240;
#endif

	// Disable sprites and tiles so we can write to VRAM.
	ppu_off ();

	draw_scr ();

#if defined (ENABLE_BREAKABLE) && defined (BREAKABLE_ANIM)
	do_process_breakable = 0;
	gpit = BREAKABLE_MAX; while (gpit --) brkf [gpit] = 0;
#endif

#ifdef LINE_OF_TEXT
	pr_str (LINE_OF_TEXT_X, LINE_OF_TEXT, "                              ");
#endif

	// CUSTOM {
	//#if defined (DIE_AND_RESPAWN) && (defined (PLAYER_SWIMS) || defined (PLAYER_TOP_DOWN))
	if (level == 1) {
		// } END_OF_CUSTOM
		player_register_safe_spot ();
	// CUSTOM {
	//#endif
	}
	// } END_OF_CUSTOM

#ifdef PLAYER_CAN_FIRE
	for (gpit = 0; gpit < MAX_BULLETS; gpit ++) {
		b_slots [gpit] = gpit; bst [gpit] = 0;
	}
	b_slots_i = MAX_BULLETS;
#endif

#ifdef ENABLE_CONTAINERS
	containers_init ();
#endif
	
	// Reenable sprites and tiles now we are finished.
	ppu_on_all ();

	oam_index = 4+24; // 4 + what the player takes.
	prx = px >> FIXBITS; pry = py >> FIXBITS;
	player_render ();
	enems_move ();
	if (hrt) hotspots_paint ();
	player_render ();

#ifdef ACTIVATE_SCRIPTING
	// Entering any script
	run_script (2 * MAP_SIZE + 1);
	// This room script
	run_script (n_pant + n_pant);
#endif

#ifdef ENABLE_CONTAINERS	
	containers_draw ();
#endif

	oam_hide_rest (oam_index);
	hud_update ();
	ppu_waitnmi ();
	clear_update_list ();
	fade_in ();
}

void game_loop (void) {
	music_play (m_ingame);

	clear_update_list ();
	set_vram_update (UPDATE_LIST_SIZE, update_list);

	on_pant = 99; ft = 1; fade_delay = 1;

	// MAIN LOOP

	pal_bright (0);
	ppu_on_all ();
	
#ifdef ACTIVATE_SCRIPTING
	script_result = 0;
	// Entering game script
	run_script (2 * MAP_SIZE);
#endif

	oam_index = 0;

	while (1) {
		half_life = 1 - half_life;
		frame_counter ++;

		// Change screen
		if (on_pant != n_pant) {
			prepare_scr ();
			on_pant = n_pant;
		}

		hud_update ();

		// Finish frame and wait for NMI
		oam_hide_rest (oam_index);
		ppu_waitnmi ();
		clear_update_list ();

		// Poll pads

		pad_read ();
		a_button = (pad_this_frame & PAD_A);
		b_button = (pad_this_frame & PAD_B);

		// Flick the screen

		#include "mainloop/flickscreen.h"

		// Finish him

		if (pkill) player_kill ();
		if (game_over) break;			

		// Change screen
		
		if (on_pant != n_pant) {
			prepare_scr ();
			on_pant = n_pant;
		}

#ifdef ACTIVATE_SCRIPTING
		#include "mainloop/scripting.h"
#endif

		// Extra checks
		#include "mainloop/extra_checks.h"

#if defined (WIN_LEVEL_CUSTOM)
		if (win_level)
#elif defined (ACTIVATE_SCRIPTING)
		if (script_result) 
#elif defined (PLAYER_MAX_OBJECTS)
		if (pobjs == PLAYER_MAX_OBJECTS) 
#elif defined (SCR_END)
		if (
			n_pant == SCR_END && 
			((prx + 8) >> 4) == PLAYER_END_X &&
			((pry + 8) >> 4) == PLAYER_END_Y
		) 
#endif
		{
			music_stop ();
			delay (50);
			break;
		}

		oam_index = 4+24; // 4 + what the player takes.
		
		if (pstate) {
			pctstate --;
			if (!pctstate) pstate = EST_NORMAL;
		}

#ifdef ENABLE_PROPELLERS
		move_propellers ();
#endif

		#include "mainloop/hotspots.h"

		player_move ();

#ifdef PLAYER_CAN_FIRE
		bullets_move ();
#endif

#ifdef ENABLE_COCOS
		cocos_do ();
#endif
	
		enems_move ();

#if defined (ENABLE_BREAKABLE) && defined (BREAKABLE_ANIM)
		if (do_process_breakable) breakable_do_anim ();
#endif
		player_render ();

		//#include "mainloop/cheat.h"

		#include "mainloop/pause.h"
	}

	music_stop ();
	fade_out ();
	set_vram_update (0, 0);
	ppu_off ();
	oam_clear ();
}
