// NES MK1 v0.5
// Copyleft Mojon Twins 2013, 2015

// Uses neslib and related tools by Shiru

// Comment this when you are done
//#define DEBUG
#define DEBUG_LEVEL		1
#define DEBUG_SCR_INI	79
#define DEBUG_INI_X		13
#define DEBUG_INI_Y		2
//

#define MSB(x)			(((x)>>8))
#define LSB(x)			(((x)&0xff))
#define FIXBITS			6
#define SPRITE_ADJUST	7
#define TOP_ADJUST		2
#define COMM_POOL		((unsigned char*)0x0300)

#define MAX_CERROJOS 16 // max 32, make it as small as possible.

#include "neslib-CNROM.h"

// OAM TABLE
/*
	0-23:	Sprite enemies	(OAM bytes	0-95)
	24-27:	Object in inv.	(OAM bytes	96-111)
	28-31:	NO!				(OAM bytes	112-127)
	32-37:	player 			(OAM bytes  128-151)
	44-47: 	hotspot 		(OAM bytes  176-191)
*/

// **************
// * const data *
// **************

#include "definitions.h"
#include "config.h"
#include "assets/palettes.h"
#include "assets/pal_arrays.h"
#include "assets/metasprites.h"
#include "assets/tiledata.h"
#include "assets/mapa.h"
#include "assets/enems.h"
#include "assets/title_rle.h"
#include "assets/cuts_rle.h"
#include "assets/intro_rle.h"
#include "assets/manual_rle.h"
#include "assets/cuts_text.h"

#ifdef ACTIVATE_SCRIPTING
extern const unsigned char *e_scripts [];
extern const unsigned char *f_scripts [];
#endif

// Music
extern const unsigned char m_intro [];
extern const unsigned char m_cuts [];
extern const unsigned char m_ingame_1 [];

// Push to zero page:
#pragma bssseg (push,"ZEROPAGE")
#pragma dataseg(push,"ZEROPAGE")

#include "ram/zp.h"

// Everything else on normal RAM
#pragma bssseg (push,"BSS")
#pragma dataseg(push,"BSS")

#include "ram/bss.h"

// *************
// Main includes
// *************

#ifdef ENABLE_PROPELLERS
void add_propeller (unsigned char x, unsigned char y);
#endif
#include "engine/printer.h"
#include "engine/general.h"
#ifndef DEACTIVATE_KEYS
#include "engine/bolts.h"
#endif
#ifdef BREAKABLE_WALLS
#include "engine/breakable.h"
#endif
#include "engine/hotspots.h"
#ifdef ENABLE_PROPELLERS
#include "engine/propellers.h"
#endif
#include "engine/player.h"
#include "engine/enengine.h"
#include "engine/frame.h"
#include "engine/pres.h"

// **************
// Main functions
// **************

void prepare_scr (void) {
	if (!ft) fade_out (); else ft = 0;

	fade_delay = 1;

	oam_spr (0, 240, 0, 0, 156);
	oam_meta_spr (0, 240, 160, spr_empty);
#ifdef ENABLE_PROPELLERS
	clear_propellers ();
#endif

#ifdef PERSISTENT_ENEMIES
	// Preserve enems
	persistent_update ();
#endif

	enems_load ();

#ifdef ENABLE_FIRE_ZONE
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

	// Clear line of text
	//pr_str (LINE_OF_TEXT_X, LINE_OF_TEXT, "                              ");

	player_move ();
	enems_move ();
	render_player ();

#ifdef PLAYER_CAN_FIRE
	for (gpit = 0; gpit < MAX_BULLETS; gpit ++) bst [gpit] = 0;
	bullets_move ();
#endif

	// Reenable sprites and tiles now we are finished.
	ppu_on_all ();
	ppu_waitnmi ();
	fade_in ();

#ifdef ACTIVATE_SCRIPTING
	// Entering any script
	script = (unsigned char *) e_scripts [MAP_W * MAP_H + 1];
	run_script ();
	// This room script
	script = (unsigned char *) e_scripts [n_pant];
	run_script ();
#endif
}

void main(void) {
	bank_spr (1);
	bank_bg (0);

	ppu_off ();
	pal_bright (0);

	credits ();
	game_on = 0;

	c_map = (unsigned char **) (map_0);
	c_decos = (unsigned char **) (map_0_decos);
	c_enems = (unsigned char *) (enems_0);
	c_hotspots = (unsigned char *) (hotspots_0);
	c_pal_bg = (unsigned char *) mypal_game_bg0;
	c_pal_fg = (unsigned char *) mypal_game_fg0;
	tsmap = (unsigned char *) (ts1_tmaps);
	tileset_pals = (unsigned char *) (ts1_pals);	

	while (1) {
		title ();
		
		plife = 5;
		game_over = 0;
		game_on = 1;

		n_pant = SCR_INI;
		on_pant = 99;

		player_init ();
#ifdef PERSISTENT_ENEMIES
		persistent_enems_load ();
#endif		
#ifdef PERSISTENT_DEATHS
		persistent_deaths_load ();
#endif
#ifdef ACTIVATE_SCRIPTING
			msc_init_all ();
#endif

		if (reset_all) {
			bankswitch (2);
			cutscene (intro_text, intro_rle);
			manual ();
			hotspots_load ();
#ifndef DEACTIVATE_KEYS		
			bolts_load ();
#endif		
		}

		half_life = 0;
		frame_counter = 0;
		olife = oammo = oobjs = okeys = 0xff;
		okilled = 0xff;

		pal_bg (c_pal_bg);
		pal_spr (c_pal_fg);

		draw_game_frame ();
		scroll (0, 8);

// Debug combinations
		/*
		n_pant = SCR_END;
		hact [61] = 0;
		hact [77] = 0;
		hact [0] = 0;		// Gates
		hact [17] = 0;		// Column
		pinv = HS_INV_EMPTY;
		ppodewwwr = 3;
		*/
//

		music_play (m_ingame_1);
		set_vram_update (UPDATE_LIST_SIZE, update_list);

#ifdef ACTIVATE_SCRIPTING
		script_result = 0;
		// Entering game script
		script = (unsigned char *) e_scripts [MAP_W * MAP_H];
		run_script ();
#endif
		ft = 1;	fade_delay = 1;

		// MAIN LOOP

		pal_bright (0);
		ppu_on_all ();
		palfx_init ();

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

#ifdef BREAKABLE_ANIM
			if (do_process_breakable) process_breakable ();
#endif
			render_player ();

// CUSTOM {
			if (no_ct) {
				no_ct --;
				if (no_ct == 0) oam_meta_spr (0, 240, 112, spr_empty); else oam_meta_spr (prx + 8, pry - 16 + SPRITE_ADJUST, 112, spr_enems [19]);
			}
// } END_OF_CUSTOM

#ifdef CARRY_ONE_HS_OBJ
			oam_meta_spr (HS_INV_X, HS_INV_Y, 96, spr_hs [pinv - 1]);
#endif

			//#include "mainloop/resonators.h"

			#include "mainloop/cheat.h"

			#include "mainloop/pause.h"

			#include "mainloop/flickscreen.h"

			#include "mainloop/hud.h"

			#include "mainloop/hotspots.h"

// CUSTOM {
			if (i & PAD_SELECT) show_minimap ();
// } END_OF_CUSTOM

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
			if (ft && !reset_all) {
				ft = 0;
				short_cutscene (99);				
			} else if (on_pant != n_pant) {
				prepare_scr ();
				on_pant = n_pant;
			}

#ifdef ACTIVATE_SCRIPTING
			// Run fire script
#ifdef ENABLE_FIRE_ZONE
			// with fire zone
			if (i & PAD_B || (f_zone_ac && (prx >= fzx1 && prx <= fzx2 && pry >= fzy1 && pry <= fzy2))) {
#else
			if (i & PAD_B) {
#endif
				run_fire_script ();
			}
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
						break;
					}
				}
			}
		}
	
		music_stop ();
		fade_out ();
		set_vram_update (0, 0);
		ppu_off ();
		oam_clear ();

		scroll (0, 0);
		
		if (game_over) {
			game_over_scr ();
		} else {
			bankswitch (1);
			cutscene (ending_text, cuts_rle);
			bankswitch (1);
			game_on = 0;
		}
		
	}
}

#ifdef ACTIVATE_SCRIPTING
#include "assets/mscnes.h"
#endif

