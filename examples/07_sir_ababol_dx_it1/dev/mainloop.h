// NES MK1 v0.7
// Copyleft Mojon Twins 2013, 2015, 2016

// Main loop & helpers

void game_init (void) {
	game_over = 0;

	c_map = (unsigned char **) (map_0);
	#ifdef MAP_WITH_DECORATIONS
		c_decos = (unsigned char **) (map_0_decos);
	#endif
	c_enems = (unsigned char *) (enems_0);
	c_locks = (unsigned char *) (map_0_locks);
	c_hotspots = (unsigned char *) (hotspots_0);
	c_pal_bg = (unsigned char *) palts0;
	c_pal_fg = (unsigned char *) palss0;
	c_max_bolts = MAX_BOLTS;
	tsmap = (unsigned char *) (ts0_tmaps);
	tileset_pals = (unsigned char *) (ts0_pals);
	
	pal_bg (c_pal_bg);
	pal_spr (c_pal_fg);

	cls ();

	draw_game_frame ();
	//clean_gauge ();

	n_pant = SCR_INI;
	
	hotspots_load ();
#ifndef DEACTIVATE_KEYS		
	bolts_load ();
#endif		
	player_init ();
#ifdef PERSISTENT_ENEMIES
	enems_persistent_load ();
#endif		
#ifdef PERSISTENT_DEATHS
	enems_persistent_deaths_load ();
#endif

#ifdef CLEAR_FLAGS
	msc_clear_flags ();
#endif

	plife = PLAYER_LIFE;
	pobjs = 0;
	pkeys = 0;

	half_life = 0;
	frame_counter = 0;
	olife = oammo = oobjs = okeys = 0xff;
	okilled = 0xff;

	// n_pant = 2; pkeys = 1;
}

void prepare_scr (void) {
	clear_update_list ();
	if (!ft) fade_out (); else ft = 0;
	
#ifdef ENABLE_PROPELLERS
	clear_propellers ();
#endif

#ifdef PERSISTENT_ENEMIES
	// Preserve enems
	enems_persistent_update ();
#endif

	enems_load ();

#if defined (ENABLE_FIRE_ZONE) || defined (ENABLE_FAST_FIRE_ZONE)
	f_zone_ac = 0;
	fzx1 = fzx2 = fzy1 = fzy2 = 240;
#endif

	// Disable sprites and tiles so we can write to VRAM.
	ppu_off ();

	oam_index = 4+24; // 4 + what the player takes.

	oam_hide_rest (0);
	draw_scr ();
	hotspots_create ();	
	// Write attributes
	vram_write (attr_table, 0x23c0, 64);

#ifdef LINE_OF_TEXT
	pr_str (LINE_OF_TEXT_X, LINE_OF_TEXT, "                              ");
#endif

	player_move ();
	enems_move ();
	render_player ();

#ifdef PLAYER_CAN_FIRE
	for (gpit = 0; gpit < MAX_BULLETS; gpit ++) bst [gpit] = 0;
	bullets_move ();
#endif

#ifdef PLAYER_GARGAJO
	gpit = MAX_GARGAJOS; while (gpit --) gst [gpit] = 0;
	gargajos_move ();
#endif

#ifdef ENABLE_CONTAINERS
	containers_init ();
#endif
	
	// Reenable sprites and tiles now we are finished.
	ppu_on_all ();

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
	ppu_waitnmi ();
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

		// Sync
		oam_hide_rest (oam_index);
		ppu_waitnmi ();
		clear_update_list ();

#ifdef ACTIVATE_SCRIPTING
		#include "mainloop/scripting.h"
#endif

#ifdef ACTIVATE_SCRIPTING
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
			fade_out ();
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
		player_move ();

		enems_move ();
#ifdef PLAYER_CAN_FIRE
		bullets_move ();
#endif

#ifdef PLAYER_GARGAJO
		gargajos_move ();
#endif

#ifdef BREAKABLE_ANIM
		if (do_process_breakable) process_breakable ();
#endif
		render_player ();

#ifdef CARRY_ONE_HS_OBJ
		oam_index = oam_meta_spr (HS_INV_X, HS_INV_Y, oam_index, spr_hs [pinv]);
#endif
#ifdef CARRY_ONE_FLAG_OBJ
		oam_index = oam_meta_spr (HS_INV_X, HS_INV_Y, oam_index, spr_hs [flags [HS_INV_FLAG]]);
#endif

		#include "mainloop/hotspots.h"

		//#include "mainloop/cheat.h"

		#include "mainloop/pause.h"

		#include "mainloop/flickscreen.h"

		#include "mainloop/hud.h"

		// Conditions
		if (plife == 0) {					
			game_over = 1;
			break;
		}
			
	}

	music_stop ();
	set_vram_update (0, 0);
	ppu_off ();
	oam_clear ();
	
	cls ();
	oam_clear ();	
	
		
}
