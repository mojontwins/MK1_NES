// NES MK1 v0.2
// Copyleft Mojon Twins 2013, 2015

// Uses neslib and related tools by Shiru

// Use 0 form ROM1, 1 for ROM2, 2 for ROM3
#define BASE_LEVEL		0

// Comment this when you are done
//#define DEBUG
#define DEBUG_LEVEL		1
#define DEBUG_SCR_INI	11
#define DEBUG_INI_X		11
#define DEBUG_INI_Y		1
//

#define MSB(x)			(((x)>>8))
#define LSB(x)			(((x)&0xff))
#define FIXBITS			6
#define SPRITE_ADJUST	7
#define TOP_ADJUST		2
#define COMM_POOL		((unsigned char*)0x0300)

#define MAX_CERROJOS 16 // max 32, make it as small as possible.

// Update list
#define UPDATE_LIST_SIZE 32
unsigned char update_index;
unsigned char update_list [UPDATE_LIST_SIZE * 3];

#include "neslib.h"

// OAM TABLE
/*
	0-15:	Sprite enemies	(OAM bytes	0-63)
	32:		player 			(OAM bytes  128-155)
	39:		Resonator # 	(OAM bytes	156-159)
	40-43:	NO!				(OAM bytes 	160-175)
	44-47: 	hotspot 		(OAM bytes  176-191)
*/

// **************
// * const data *
// **************

#include "definitions.h"
#include "config.h"
#include "assets/palettes.h"
#include "assets/metasprites.h"
#include "assets/tiledata1.h"
#include "assets/mapa0.h"
#include "assets/enems0.h"
#include "assets/mapa1.h"
#include "assets/enems1.h"

#ifdef ACTIVATE_SCRIPTING
extern const unsigned char *e_scripts [];
extern const unsigned char *f_scripts [];
#endif

// Music
extern const unsigned char m_ingame_1 [];
extern const unsigned char m_ingame_2 [];
extern const unsigned char m_sclear [];

// Push to zero page:
#pragma bssseg (push,"ZEROPAGE")
#pragma dataseg(push,"ZEROPAGE")

unsigned char i, gpit, gpjt, gpaux;
signed char fader;
unsigned char half_life;

unsigned char n_pant, on_pant;

unsigned char *gp_gen, *gp_tmap;
unsigned int gp_addr;
unsigned char rdx, rdy, rdt, rdit;
unsigned char rda, rdb, rdc;
unsigned char rdct;

unsigned char olife, okeys, oobjs, oammo, okilled;

// Enemies
unsigned char en_x [3], en_y [3];
unsigned char en_x1 [3], en_y1 [3];
unsigned char en_x2 [3], en_y2 [3];
signed char en_mx [3], en_my [3];
unsigned char en_t [3], en_s [3], en_facing;
unsigned char en_life [3], en_alive [3], en_status [3], en_rawv [3], en_ct [3];
unsigned char en_fr, etx1, etx2, ety1, ety2;
unsigned char en_collx, en_colly;
unsigned char touched;
unsigned char en_touched [3], en_cttouched [3];
#ifdef ENABLE_FANTY
signed int enf_x [3], enf_y [3], enf_vx [3], enf_vy [3];
#endif

// Main player
signed int px, py, pvx, pvy;
unsigned char pfacing, pfr, pctfr, psprid;
unsigned char pj, pctj, pjb, pgotten, ppossee, psprint;
unsigned char pobjs, plife, pcontinues, pkeys, pkilled, pstars;
unsigned char pammo, pfiring;
unsigned char pushed_any;
unsigned char pstate, pctstate;
unsigned char phit;
signed int pgtmx, pgtmy;

// Aux player
unsigned char prx, pry, ptx1, ptx2, pty1, pty2;
unsigned char pfacingv, pfacingh, wall, hitv, hith;

// Bullets
#ifdef PLAYER_CAN_FIRE
unsigned char bst [MAX_BULLETS], bx [MAX_BULLETS], by [MAX_BULLETS], bmx [MAX_BULLETS], bmy [MAX_BULLETS];
unsigned char btx, bty;
#endif

// Hotspots
unsigned char hrx, hry, hrt;

// Process breakable?
#ifdef BREAKABLE_ANIM
unsigned char do_process_breakable;
#endif

unsigned char ft;

// Gfx sets
//char **spr_enems, **spr_player;

// Level control

// Current level
unsigned char *c_map;
unsigned char **c_decos;
unsigned char *c_locks;
unsigned char *c_enems;
unsigned char *c_hotspots;
unsigned char *c_pal_bg;
unsigned char *c_pal_fg;

unsigned char *tileset_pals;
unsigned char *tsmap;

// Resonator
unsigned char res_ct, res_subct, res_on;

// NO!
unsigned char no_on, no_ct;

// Everything else on normal RAM
#pragma bssseg (push,"BSS")
#pragma dataseg(push,"BSS")

unsigned char map_attr [192];
unsigned char map_buff [192];
#ifdef BREAKABLE_WALLS
unsigned char brk_buff [192];
#ifdef BREAKABLE_ANIM
#define MAX_BREAKABLE 4
unsigned char brkf [MAX_BREAKABLE];
unsigned char brkx [MAX_BREAKABLE];
unsigned char brky [MAX_BREAKABLE];
#endif
#endif
unsigned char fade_delay;
#ifndef HOTSPOTS_WONT_CHANGE
unsigned char hxy [MAP_W * MAP_H];
unsigned char ht [MAP_W * MAP_H];
#endif
unsigned char hact [MAP_W * MAP_H];
#ifndef DEACTIVATE_KEYS
unsigned char lkxy [MAX_CERROJOS], lknp [MAX_CERROJOS], lkact [MAX_CERROJOS];
unsigned char xy;
#endif
#ifdef ACTIVATE_SCRIPTING
void __fastcall__ msc_init_all (void);
void __fastcall__ run_script (void);
#define MAX_FLAGS 8
unsigned char flags [MAX_FLAGS];
unsigned char script_something_done;
unsigned char script_result;
unsigned char *script;
#ifdef ENABLE_FIRE_ZONE
unsigned char f_zone_ac, fzx1, fzx2, fzy1, fzy2;
#endif
#endif

// Baddies count. Properly fill this value!
#ifdef PLAYER_KILLS_ENEMIES
unsigned char baddies_count;
#define BADDIES_COUNT baddies_count
#endif

#ifdef PERSISTENT_ENEMIES
// Persistent enemies
unsigned char ep_x [3 * MAP_W * MAP_H];
unsigned char ep_y [3 * MAP_W * MAP_H];
signed char ep_mx [3 * MAP_W * MAP_H];
signed char ep_my [3 * MAP_W * MAP_H];
unsigned char ep_t [3 * MAP_W * MAP_H];
unsigned int ep_it;
#endif

// Die & respawn
#ifdef DIE_AND_RESPAWN
unsigned char n_pant_safe;
signed int px_safe, py_safe;
#endif

unsigned char level, game_over;

// *************
// Main includes
// *************

#include "engine/printer.h"
#include "engine/general.h"
#ifndef DEACTIVATE_KEYS
#include "engine/bolts.h"
#endif
#ifdef BREAKABLE_WALLS
#include "engine/breakable.h"
#endif
#include "engine/hotspots.h"
#include "engine/player.h"
#include "engine/enengine.h"
#include "engine/frame.h"
#include "engine/pres.h"

// **************
// Main functions
// **************

void __fastcall__ prepare_scr (void) {
	if (!ft) fade_out (); else ft = 0;
	oam_spr (0, 240, 0, 0, 156);
	oam_meta_spr (0, 240, 160, spr_empty);

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

	draw_scr ();
	
	hotspots_create ();
	
	// Write attributes
	vram_write (attr_table, 0x23c0, 64);

	// Clear line of text
	pr_str (LINE_OF_TEXT_X, LINE_OF_TEXT, "                              ");

	enems_move ();
	player_move ();
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

	scroll (0, 8);
	ppu_off ();

	while (1) {	// This while(1) is to make this NROM-compatible for testing purposes.

		// Decode shared RAM

		// Security code: *04 = *05 + *06 + 7
		rda = COMM_POOL [4];
		plife = COMM_POOL [5];
		pcontinues = COMM_POOL [6];
		level = COMM_POOL [7];

		if (rda != plife + pcontinues + 7) {
			plife = PLAYER_LIFE;
			pcontinues = 0;
#ifdef DEBUG
			level = DEBUG_LEVEL;
#else		
			level = 0;
#endif			
		}

		// Decoding done
	
		game_over = 0;

		if (level == 0)  {
			c_map = (unsigned char *) (map_0_tiles);
			c_decos = (unsigned char **) (map_0_decos);
			c_locks = (unsigned char *) (map_0_locks);
			c_enems = (unsigned char *) (enemies_ROM_enems0);
			c_hotspots = (unsigned char *) (hotspots_ROM_enems0);
			c_pal_bg = (unsigned char *) mypal_game_bg0;
			c_pal_fg = (unsigned char *) mypal_game_fg0;
			tsmap = (unsigned char *) (ts1_tmaps);
			tileset_pals = (unsigned char *) (ts1_pals);
			baddies_count = 36;
		} else {
			c_map = (unsigned char *) (map_1_tiles);
			c_decos = (unsigned char **) (map_1_decos);
			c_locks = (unsigned char *) (map_1_locks);
			c_enems = (unsigned char *) (enemies_ROM_enems1);
			c_hotspots = (unsigned char *) (hotspots_ROM_enems1);
			c_pal_bg = (unsigned char *) mypal_game_bg1;
			c_pal_fg = (unsigned char *) mypal_game_fg1;
			tsmap = (unsigned char *) (ts1_tmaps);
			tileset_pals = (unsigned char *) (ts1_pals);
			baddies_count = 37;
		}
		
		level_screen ();
		pstars = 0;

		pal_bg (c_pal_bg);
		pal_spr (c_pal_fg);		
		cls ();

		draw_game_frame ();

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
	
#ifdef ACTIVATE_SCRIPTING
		msc_init_all ();
#endif
		half_life = 0;
		olife = oammo = oobjs = okeys = 0xff;
		okilled = 0xff;

		res_on = 0;

		music_play (level ? m_ingame_2 : m_ingame_1);
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

		while (1) {
			half_life = 1 - half_life;
			
			if (pstate == EST_PARP) {
				pctstate --;
				if (!pctstate) pstate = EST_NORMAL;
			}

			if (no_on) {
				if (no_ct) {
					no_ct --;
				} else {
					oam_meta_spr (0, 240, 160, spr_empty);
					no_on = 0;
				}
			}

			player_move ();
	
			enems_move ();
#ifdef PLAYER_CAN_FIRE
			bullets_move ();
#endif

#ifdef BREAKABLE_ANIM
			if (do_process_breakable) process_breakable ();
#endif
			render_player ();

			#include "mainloop/resonators.h"

			#include "mainloop/cheat.h"

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
			ppu_waitnmi ();
			clear_update_list ();
			update_index = 0;
			
			// Change screen
			if (on_pant != n_pant) {
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
			if (pkilled == baddies_count) {
				music_stop ();
				// music zone clear!
				// Stupid animation
				stupid_animation ();
				break;
			}
		}
	
		music_stop ();
		set_vram_update (0, 0);
		ppu_off ();
		cls ();
		oam_clear ();	
		
		ppu_on_all (); // Why? Shall I remove this, the game hangs.
		
		// HERE
		// When the game is finished, set flags and jump ROMs upon the value of game_over

		if (game_over) {
			// Write 6 6 6 18 to $301 on, launch ROM0
			COMM_POOL [1] = 6;
			COMM_POOL [2] = 6;
			COMM_POOL [3] = 6;
			COMM_POOL [4] = 18;
		} else {
			// Write 1 2 3 6 to $301 on, launch ROM0
			COMM_POOL [1] = 1;
			COMM_POOL [2] = 2;
			COMM_POOL [3] = 3;
			COMM_POOL [4] = 6;
		}
		//
		COMM_POOL [5] = plife;
		COMM_POOL [6] = pcontinues;
		COMM_POOL [7] = BASE_LEVEL + level; 
		// PRG-ROM 0, CHR-ROM 0; $300 = 0b0000 (0x00)
		COMM_POOL [0] = 0x00;
		__asm__ ("jmp _change_rom");
	}
}

#ifdef ACTIVATE_SCRIPTING
#include "assets/mscnes.h"
#endif

