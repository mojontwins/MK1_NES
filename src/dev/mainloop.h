// NES MK1 v2.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

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

	hotspots_load ();
	#ifndef DEACTIVATE_KEYS		
		// bolts_load
		memfill (lkact, 1, c_max_bolts);
	#endif		

	if (!warp_to_level)	{
		n_pant = SCR_INI;
		px = (4 + (PLAYER_INI_X << 4)) << FIXBITS;
		py = (PLAYER_INI_Y << 4) << FIXBITS;
	}
	player_to_pixels ();
	player_init ();
		
	#ifdef PERSISTENT_ENEMIES
		enems_persistent_load ();
	#endif		

	#ifdef PERSISTENT_DEATHS
		enems_persistent_deaths_load ();
	#endif

	#ifndef DEACTIVATE_OBJECTS
		pobjs = 0;
	#endif

	#ifdef HOTSPOT_TYPE_STAR
		pstars = 0;
	#endif

	#ifndef DEACTIVATE_KEYS	
		pkeys = 0;
	#endif

	#ifdef ENABLE_RESONATORS
		res_on = 0;
		res_disable = 0;
	#endif

	#ifdef ENABLE_USE_ANIM
		if (!warp_to_level) use_ct = 0;
	#endif

	#ifdef ENABLE_NO
		no_ct = 0;
	#endif	

	#ifdef ENABLE_PROPELLERS
		#ifdef PROPELLERS_ON_BY_DEFAULT
			propellers_on = 1;
		#else
			propellers_on = 0;
		#endif
	#endif

	#ifdef ENABLE_SPRINGS
		#ifdef SPRINGS_ON_BY_DEFAULT
			springs_on = 1;
		#else
			springs_on = 0;
		#endif
	#endif

	#ifdef ENABLE_TIMER
		#ifndef TIMER_RESET_ON_ENTER
			timer = TIMER_INITIAL;
			timer_frames = 50;
			timer_zero = 0;
			otimer = 0xff;
		#endif
		#ifdef TIMER_START_ON
			timer_on = 1;
		#else
			timer_on = 0;
		#endif
	#endif

	#if defined (ENABLE_TILE_GET) && defined (PERSISTENT_TILE_GET)
		// Clear tile_got persistence
		vram_adr (MAP_CLEAR_LIST_ADDRESS);
		vram_fill (0, MAP_SIZE*24);
	#endif

	olife = oammo = oobjs = okeys = 0xff;
	okilled = 0xff;

	#include "my/extra_inits.h"

	#if defined (ENABLE_INTERACTIVES) && defined (INTERACTIVES_FROM_CODE)
		#include "my/interactives_setup.h"
	#endif
}

void prepare_scr (void) {
	if (!ft) {
		fade_out (); 
		ppu_off ();
		#if defined (ENABLE_TILE_GET) && defined (PERSISTENT_TILE_GET)
			// Update tile_got persistence
			rda = on_pant << 3;
			vram_write (tile_got, MAP_CLEAR_LIST_ADDRESS + (rda << 1) + rda, 24);
		#endif
	} else {
		ft = 0;
		ppu_off ();
	}

	clear_update_list ();

	#ifdef ENABLE_PROPELLERS
		// Clear propellers
		prp_idx = 0;
	#endif

	#ifdef PERSISTENT_ENEMIES
		// Preserve enems
		enems_persistent_update ();
	#endif

	enems_load ();

	#ifdef DOUBLE_WIDTH
		for (gpit = 0; gpit < 2; gpit ++) {
			hotspots_create ();

			hrx &= 0x00ff;
			if (gpit) hrx |= 0x0100;
			d_hrx [gpit] = hrx;
			d_hry [gpit] = hry;
			d_hrt [gpit] = hrt;

			*((unsigned char *) 0xfc + gpit*2) = MSB(d_hrx [gpit]);
			*((unsigned char *) 0xfd + gpit*2) = LSB(d_hrx [gpit]);
		}
	#else
		hotspots_create ();	
	#endif

	#ifdef ENABLE_COCOS
		cocos_init ();
	#endif	

	#if defined (ACTIVATE_SCRIPTING) && defined (ENABLE_FIRE_ZONE)
		f_zone_ac = 0;
		fzx1 = fzx2 = fzy1 = fzy2 = 240;
	#endif

	#ifdef ENABLE_SHINES
		shine_active_ct = 0;
		max_shines = 0;
	#endif

	#ifdef ENABLE_TILE_CHAC_CHAC
		chac_chacs_queue_write = chac_chacs_queue_read = 0;
		max_chac_chacs = 0;
	#endif

	#if defined (ENABLE_TILE_GET) && defined (PERSISTENT_TILE_GET)
		// Read tile_got persistence
		rda = n_pant << 3;
		vram_read (tile_got, MAP_CLEAR_LIST_ADDRESS + (rda << 1) + rda, 24);
	#endif

		draw_scr ();

	#if defined (ENABLE_BREAKABLE) && defined (BREAKABLE_ANIM)
		do_process_breakable = 0;
		gpit = BREAKABLE_MAX; while (gpit --) brkf [gpit] = 0;
	#endif

	#ifdef LINE_OF_TEXT
		_x = LINE_OF_TEXT_X; _y = LINE_OF_TEXT;
		pr_str ("                              ");
	#endif

	#if defined (DIE_AND_RESPAWN) && (defined (PLAYER_SWIMS) || defined (PLAYER_TOP_DOWN))
		player_register_safe_spot ();
	#endif

	#ifdef PLAYER_CAN_FIRE
		for (gpit = 0; gpit < MAX_BULLETS; gpit ++) {
			b_slots [gpit] = gpit; by [gpit] = 0;
		}
		b_slots_i = MAX_BULLETS;
	#endif

	#ifdef ENABLE_INTERACTIVES
		#ifdef INTERACTIVES_FROM_CODE
			interactives_create ();
		#else
			interactives_index = 0;
		#endif

		interactives_interact_with = 0xff;
	#endif
	
	// Reenable sprites and tiles now we are finished.
	#ifdef CNROM
		bankswitch (l_chr_rom_bank [level]);
	#endif

	ppu_on_all ();

	#ifdef ACTIVATE_SCRIPTING
		#if defined (ENABLE_PUSHED_SCRIPT)
			just_pushed = 0;
		#endif
		#if defined (ENABLE_INTERACTIVES)
			just_interacted = 0;
		#endif
		// Entering any script
		run_script (2 * MAP_SIZE + 1);
		// This room script
		run_script (n_pant << 1);
	#endif

	#include "my/on_entering_screen.h"

	gpit = 3; while (gpit --) en_spr_id [gpit] = en_s [gpit];

	oam_index = 4;
	prx = px >> FIXBITS; pry = py >> FIXBITS;
	#if defined (PLAYER_PUNCHES) || defined (PLAYER_KICKS)
		phitteract = 0;
	#endif	

	player_move ();
	#ifdef DOUBLE_WIDTH
		calc_scroll_pos ();
	#endif
	enems_move ();


	#ifdef DOUBLE_WIDTH
		for (gpit = 0; gpit < 2; gpit ++)  {
			hrx = d_hrx [gpit];
			hrt = d_hrt [gpit];
			if (hrx < scroll_x || hrx > scroll_x + 240 || hrt == 0) continue;
			hry = d_hry [gpit];

			hotspots_paint ();
		}
	#else
		if (hrt) hotspots_paint ();
	#endif
	
	#ifdef ENABLE_INTERACTIVES	
		interactives_paint ();
	#endif

	#ifdef ENABLE_TIMER
		#ifdef TIMER_RESET_ON_ENTER
			timer = TIMER_INITIAL;
			timer_frames = 50;
			timer_zero = 0;
			otimer = 0xff;
		#endif
	#endif

	hud_update ();
	update_cycle ();
	fade_in ();
}

void game_loop (void) {
	#ifdef MULTI_LEVEL
		music_play (l_music [level]);
	#else
		music_play (MUSIC_INGAME);
	#endif

	clear_update_list ();
	set_vram_update (UPDATE_LIST_SIZE, update_list);

	on_pant = 99; ft = 1; fade_delay = 1;

	// MAIN LOOP

	pal_bright (0);
	ppu_on_all ();
	
	#ifdef ACTIVATE_SCRIPTING
		#ifdef CLEAR_FLAGS
			msc_clear_flags ();
		#endif
		
		// Entering game script
		run_script (2 * MAP_SIZE);
	#endif

	half_life = frame_counter = real_frame_counter = 0;
	ntsc_frame = level_reset = warp_to_level = 0; 
	oam_index = 4; ticker = 50;
	
	while (1) {

		// Update hud

		#ifdef ACTIVATE_SCRIPTING
			if (n_pant != 0xfe && on_pant != 0xfe) 
		#endif
			hud_update ();

		// Finish him

		if (pkill) {
			#ifdef KILL_PLAYER_CUSTOM
				#include "my/player_kill_custom.h"
			#else
				// standard way
				player_kill ();	
			#endif
		} 
		if (game_over || level_reset) break;			

		// Flick the screen

		#ifdef KILL_PLAYER_CUSTOM
		if (!pkill) 
		#endif
		{
			flick_override = 0;
			#include "my/custom_flickscreen.h"
			if (flick_override == 0) {
				flickscreen_do_horizontal ();
				flickscreen_do_vertical ();
			}
		}
		
		// Change screen ?

		if (on_pant != n_pant) {
			prepare_scr ();
			on_pant = n_pant;
		}

		// Relocate player if spawned on a broken tile

		#include "mainloop/relocate_player.h"

		// Shake the screen ? 

		#ifdef ENABLE_SHAKER
			#include "mainloop/shaker.h"
		#endif

		// Effects

		#include "my/effects.h"

		// Finish frame and wait for NMI

		update_cycle ();
		
		// Poll pads

		pad_read ();
		a_button = (pad_this_frame & PAD_A);
		b_button = (pad_this_frame & PAD_B);

		// Update actors if not paused...

		++ real_frame_counter;
		++ ntsc_frame; if (ntsc_frame == 6) ntsc_frame = 0;

		if (paused == 0 && (ntsc == 0 || ntsc_frame)) {
			// Count frames		
			if (ticker) -- ticker; else ticker = 50;
			half_life ^= 1;
			++ frame_counter;

			// Detect interactions

			#ifdef ENABLE_INTERACTIVES
				#include "mainloop/interactives.h"
			#endif	

			// Update / collide hotspots

			#ifdef DOUBLE_WIDTH
				for (gpit = 0; gpit < 2; gpit ++)  {
					hrx = d_hrx [gpit];
					hrt = d_hrt [gpit];
					if (hrx < scroll_x || hrx > scroll_x + 240 || hrt == 0) continue;
					hry = d_hry [gpit];

					#include "mainloop/hotspots.h"
					hotspots_paint ();

					d_hrt [gpit] = hrt;
				}
			#else
				#include "mainloop/hotspots.h"
			#endif

			// Automatic scripting calls (USE_ANIM & fire zone)

			#ifdef ACTIVATE_SCRIPTING
				#include "mainloop/scripting.h"
			#endif
			
			// Update player

			if (!warp_to_level && !pkill) {
				player_move ();
			}

			// Scroll position

			#ifdef DOUBLE_WIDTH
				calc_scroll_pos ();
			#endif

			// Timer

			#ifdef ENABLE_TIMER
				#include "mainloop/timer.h"
			#endif

			// Extra checks

			#include "my/extra_checks.h"

			// Win level condition

			#include "mainloop/win_level_condition.h"

			// Update propellers

			#ifdef ENABLE_PROPELLERS
				if (propellers_on) propellers_do ();
			#endif

			// Update bullets

			#ifdef PLAYER_CAN_FIRE
				bullets_move ();
			#endif

			// Paint player

			oam_index_player = oam_index; 
			if (!warp_to_level)	player_render ();

			// Update enemies
		
			enems_move ();

			// Update cocos

			#ifdef ENABLE_COCOS
				cocos_do ();
			#endif			

			// Warp to level

			if (warp_to_level) {
				update_cycle (); music_stop (); break;
			}

			// Do resonators

			#ifdef ENABLE_RESONATORS
				#include "mainloop/resonators.h"
			#endif

			// Paint hotspots

			#ifndef DOUBLE_WIDTH
				if (hrt) hotspots_paint ();
			#endif

			// Paint interactives

			#ifdef ENABLE_INTERACTIVES
				interactives_paint ();
			#endif

			// Do breakable tiles

			#if defined (ENABLE_BREAKABLE) && defined (BREAKABLE_ANIM)
				if (do_process_breakable) breakable_do_anim ();
			#endif

			// Update shines

			#ifdef ENABLE_SHINES
				shines_do ();
			#endif

			// Update no

			#ifdef ENABLE_NO
				#include "mainloop/no.h"
			#endif

			// Update chac-chacs

			#ifdef ENABLE_TILE_CHAC_CHAC
				chac_chacs_do ();
			#endif

			#include "my/extra_routines.h"
		}

		// Cheat to skip level

		#include "mainloop/cheat.h"

		// Pause

		#include "mainloop/pause.h"
	}

	fade_delay = 4;
	music_stop ();
	fade_out ();
	set_vram_update (0, 0);
	ppu_off ();
	oam_clear ();
}
