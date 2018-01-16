// NES MK1 v0.7
// Copyleft Mojon Twins 2013, 2015, 2016

// Main loop & helpers

void game_init (void) {
	game_over = 0;

	c_map = (unsigned char **) (map_0);
	c_decos = (unsigned char **) (map_0_decos);
	c_enems = (unsigned char *) (enems_0);
	c_hotspots = (unsigned char *) (hotspots_0);
	c_pal_bg = (unsigned char *) mypal_game_bg0;
	c_pal_fg = (unsigned char *) mypal_game_fg0;
	tsmap = (unsigned char *) (ts1_tmaps);
	tileset_pals = (unsigned char *) (ts1_pals);
	
	pal_bg (c_pal_bg);
	pal_spr (c_pal_fg);

	cls ();

	draw_game_frame ();
	//clean_gauge ();

	n_pant = SCR_INI;
	on_pant = 99;

	hotspots_load ();
#ifndef DEACTIVATE_KEYS		
	bolts_load ();
#endif		
	player_init ();
#ifdef PERSISTENT_ENEMIES
	persistent_enems_load ();
#endif		
#ifdef PERSISTENT_DEATHS
	persistent_deaths_load ();
#endif

#ifdef CLEAR_FLAGS
	msc_clear_flags ();
#endif
	half_life = 0;
	frame_counter = 0;
	olife = oammo = oobjs = okeys = 0xff;
	okilled = 0xff;
}

void prepare_scr (void) {
	if (!ft) fade_out (); else ft = 0;
	
#ifdef ENABLE_PROPELLERS
	clear_propellers ();
#endif

#ifdef PERSISTENT_ENEMIES
	// Preserve enems
	persistent_update ();
#endif

	enems_load ();

#if defined (ENABLE_FIRE_ZONE) || defined (ENABLE_FAST_FIRE_ZONE)
	f_zone_ac = 0;
	fzx1 = fzx2 = fzy1 = fzy2 = 240;
#endif

	// Disable sprites and tiles so we can write to VRAM.
	ppu_off ();

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

	containers_init ();
	
	// Reenable sprites and tiles now we are finished.
	ppu_on_all ();

#ifdef ACTIVATE_SCRIPTING
	// Entering any script
	run_script (2 * MAP_W * MAP_H + 1);
	// This room script
	run_script (n_pant + n_pant);
#endif
	containers_draw ();
	ppu_waitnmi ();
	fade_in ();
}

void game_loop (void) {

	music_play (m_ingame_1);
	set_vram_update (UPDATE_LIST_SIZE, update_list);

	ft = 1;	fade_delay = 1;

	// MAIN LOOP

	pal_bright (0);
	ppu_on_all ();
	palfx_init ();

#ifdef ACTIVATE_SCRIPTING
	script_result = 0;
	// Entering game script
	run_script (2 * MAP_W * MAP_H);
#endif

	while (1) {
		half_life = 1 - half_life;
		frame_counter ++;
		
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
		oam_meta_spr (HS_INV_X, HS_INV_Y, OAM_INVENTORY, spr_hs [pinv]);
#endif
#ifdef CARRY_ONE_FLAG_OBJ
		oam_meta_spr (HS_INV_X, HS_INV_Y, OAM_INVENTORY, spr_hs [flags [HS_INV_FLAG]]);
#endif

		//#include "mainloop/resonators.h"

		//#include "mainloop/cheat.h"

		#include "mainloop/pause.h"

		#include "mainloop/flickscreen.h"

		#include "mainloop/hud.h"

		#include "mainloop/hotspots.h"

		// Conditions
		if (plife == 0) {					
			game_over = 1;
			break;
		}

		// Sync
		palfx_do ();
		ppu_waitnmi ();
		clear_update_list ();
		update_index = 0;
		
		// Change screen
		if (on_pant != n_pant) {
			prepare_scr ();
			on_pant = n_pant;
		}

		// Throw fire script
#ifdef ACTIVATE_SCRIPTING
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
#endif


/*
#ifdef ACTIVATE_SCRIPTING
		if (script_result) {
#else
		if (pobjs == PLAYER_NUM_OBJETOS) {
#endif
*/
//			if (pkilled == baddies_count) {
		if (n_pant == SCR_END) {;
			if (((prx + 8) >> 4) == PLAYER_END_X) {
				if (((pry + 8) >> 4) == PLAYER_END_Y) {
					music_stop ();
					delay (50);
					fade_out ();
					break;
				}
			}
		}
			
	}

	music_stop ();
	set_vram_update (0, 0);
	ppu_off ();
	oam_clear ();
	
	cls ();
	oam_clear ();	
	
		
}
