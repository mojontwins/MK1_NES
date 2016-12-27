// NES MK1 v0.1b
// Copyleft Mojon Twins 2013, 2015

// Uses neslib and related tools by Shiru

// Comment when you are done
//#define DEBUG

#define MSB(x)			(((x)>>8))
#define LSB(x)			(((x)&0xff))
#define FIXBITS			6
#define SPRITE_ADJUST 	7
#define TOP_ADJUST		2
#define COMM_POOL		((unsigned char*)0x0300)

#define MAX_CERROJOS 16 // max 32, make it as small as possible.

#define INITIAL_PLIFE 	5
#define INITIAL_PCONTINUES 1

// Update list
#define UPDATE_LIST_SIZE 30
unsigned char update_index;
unsigned char update_list [UPDATE_LIST_SIZE * 3];

#include "neslib.h"

// **************
// * const data *
// **************

#include "definitions.h"
#include "config.h"
#include "assets/cuts_texts.h"
#include "assets/palettes.h"
#include "assets/metasprites.h"
#include "assets/tiledatab.h"
#include "assets/tiledatatitle.h"
#include "assets/cutsb.h"
#include "assets/cutsl.h"
#include "assets/map_b.h"
#include "assets/enems.h"
#include "assets/screens.h"

#ifdef ACTIVATE_SCRIPTING
extern const unsigned char *e_scripts [];
extern const unsigned char *f_scripts [];
#endif

// Music
extern const unsigned char m_title [];
extern const unsigned char m_ingame [];
extern const unsigned char m_cuts [];
extern const unsigned char m_gover [];

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

// Main player
signed int px, py, pvx, pvy;
unsigned char pfacing, pfr, pctfr, psprid;
unsigned char pj, pctj, pjb, pgotten, ppossee;
unsigned char pobjs, plife, pcontinues, pkeys, pkilled;
unsigned char pammo, pfiring;
unsigned char pushed_any;
unsigned char pstate, pctstate;
unsigned char phit, phan, phct;
signed int pgtmx, pgtmy;

// Aux player
unsigned char prx, pry, ptx1, ptx2, pty1, pty2;
unsigned char pfacingv, pfacingh, pfacinghlast, wall, hitv, hith;

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
unsigned char *c_locks;
unsigned char *c_enems;
unsigned char *c_hotspots;

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
unsigned char hxy [MAP_W * MAP_H];
unsigned char ht [MAP_W * MAP_H];
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

unsigned char level, game_on, game_over, rom;

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
	fade_delay = 1;
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

void jump_to (unsigned char l) {
	// MCPPPCCC

	COMM_POOL [4] = plife + pcontinues + 7;
	COMM_POOL [5] = plife;
	COMM_POOL [6] = pcontinues;
	COMM_POOL [7] = l & 1;
	
	rom = l >> 1;
	
	// Show cutscene
	// First page in CHR ROM #rom, no change in PRG-ROM #rom
	// PPP = 0, CCCC = rom + 1
	COMM_POOL [0] = rom + 1;
	__asm__ ("jsr _change_reg");

	// Now the cutscene
	pal_bg (mypal_cuts [rom]);
	cutscene ((unsigned char *) cuts_tmaps [rom], (unsigned char *) cuts_pals [rom], (unsigned char *) cutscene_texts [l]);

	// Now launch level. Page in CHR-ROM #rom, PRG-ROM #rom
	// PPP = rom + 1, CCCC = rom + 1
	rom ++;
	COMM_POOL [0] = (rom << 3) | rom;
	__asm__ ("jmp _change_rom");
}

void main(void) {
	bank_spr (1);
	bank_bg (0);

	scroll (0, 8);
	ppu_off ();

	// Decode shared RAM

	/*
		$00 paging control
		$01 CMD1
		$02 CMD2
		$03 CMD3
		$04 CONTROL
		$05 PLIFE
		$06 PCONTINUES
		$07 CURRENT LEVEL
	*/

	// Security code: *0x01 + *0x02 + *0x03 = *0x04
	rda = COMM_POOL [1]; rdb = COMM_POOL [2]; rdc = COMM_POOL [3]; rdt = COMM_POOL [4];
	plife = COMM_POOL [5];
	pcontinues = COMM_POOL [6];
	level = COMM_POOL [7];

#ifdef DEBUG
	cls ();
	pal_bg (mypal_title);
	
	set_vram_update (UPDATE_LIST_SIZE, update_list);
	ppu_on_all ();
	pal_bright (3);
	
	for (gpit = 0; gpit < 15; gpit ++) {
		p_th (1, 2 + gpit, gpit);
		p_th (4, 2 + gpit, COMM_POOL [gpit]);
		ppu_waitnmi ();
		clear_update_list ();
		update_index = 0;
	}
	
	while (!pad_poll (0)) ppu_waitnmi ();

	ppu_off ();
	set_vram_update (0, 0);
#endif

	if (rdt == rda + rdb + rdc) {
		// Command!
		if (rda == 6 && rdb == 6 && rdc == 6) {
			if (game_over_scr ()) {
				pcontinues --; 
				plife = INITIAL_PLIFE;
				jump_to (level);
			}
		}
		if (rda == 1 && rdb == 2 && rdc == 3) {
			// Level OK! advance to next
			level ++;
			if (level < 6)
				jump_to (level);
			else {
				// Game ending
				cutscene ((unsigned char *) cutsb_tmaps, (unsigned char *) cutsb_pals, (unsigned char *) game_ending_text);
				// Credits ? TODO
			}
		}
	}

	// Decoding done.

	while (1) {
		if (title ()) {
			// Launch first level.
			plife = INITIAL_PLIFE; 
			pcontinues = INITIAL_PCONTINUES;
			jump_to (0);
		}

		// Cheril of the bosque!
		cutscene (0, 0, (unsigned char *) bosque_intro_text);

		game_on = 1;
		game_over = 0;

		while (game_on) {
			c_map = (unsigned char *) (map_b);
			c_locks = (unsigned char *) (locks_map_b);
			c_enems = (unsigned char *) (enemies_ROM_enems);
			c_hotspots = (unsigned char *) (hotspots_ROM_enems);
			pal_bg (mypal_game_bg0);
			pal_spr (mypal_game_fg0);
			tsmap = (unsigned char *) (tsb_tmaps);
			tileset_pals = (unsigned char *) (tsb_pals);
			
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

			music_play (m_ingame);
			set_vram_update (UPDATE_LIST_SIZE, update_list);

#ifdef ACTIVATE_SCRIPTING
			script_result = 0;
			// Entering game script
			script = (unsigned char *) e_scripts [MAP_W * MAP_H];
			run_script ();
#endif
			ft = 1;

			// MAIN LOOP

			pal_bright (0);
			ppu_on_all ();

			while (1) {
				half_life = 1 - half_life;
				update_index = 0;
				if (pstate) {
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

				#include "mainloop/hotspots.h"

				#include "mainloop/pause.h"

				#include "mainloop/flickscreen.h"

				#include "mainloop/hud.h"

				#include "mainloop/cheat.h"

				// Sync
				ppu_waitnmi ();
				clear_update_list ();

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

				// Conditions
				if (plife == 0) {					
					game_on = 0;
					game_over = 1;
					break;
				}

#ifdef ACTIVATE_SCRIPTING
				if (script_result) {
#else
				if (pobjs == PLAYER_MAX_OBJECTS) {
#endif
					game_on = 0;
					break;
				}

			}

			music_stop ();
			set_vram_update (0, 0);
			ppu_off ();
			oam_clear ();
		}

		// HERE

		if (game_over) {
			pcontinues = 0;
			game_over_scr ();
		} else {
			// WIN screen for cheril of the bosque
			// Change CHR ROM to 1, PRG ROM stays 0. $300 = MCPPPCCCC = 000000001
			COMM_POOL [0] = 0x01;
			__asm__ ("jsr _change_reg");

			pal_bg (mypal_cutscene);	// Wrong palette chosen deliberately.
			cutscene ((unsigned char *) cuts1_tmaps, (unsigned char *) cuts1_pals, (unsigned char *) bosque_ending_text);

			// Back to 0:
			// Change CHR BANK to 0, PRG BANK stays 0. $300 = 0000
			COMM_POOL [0] = 0x00;
			__asm__ ("jsr _change_reg");
		}
		//
	}
}

#ifdef ACTIVATE_SCRIPTING
#include "assets/mscnes.h"
#endif

