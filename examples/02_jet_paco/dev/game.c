// Churrera for NES 1.0
// Copyleft Mojon Twins 2013

// Uses neslib and related tools by Shiru

#define MSB(x)		(((x)>>8))
#define LSB(x)		(((x)&0xff))
#define FIXBITS		6
#define SPRITE_ADJUST 7
#define TOP_ADJUST 2

#define MAX_CERROJOS 16 // max 32, make it as small as possible.

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
#include "palettes.h"
#include "metasprites.h"
#include "mapa0.h"
#include "mapa1.h"
#include "mapa2.h"
#include "enems0.h"
#include "enems1.h"
#include "enems2.h"
#include "screens.h"
#ifdef ACTIVATE_SCRIPTING
extern const unsigned char *e_scripts [];
extern const unsigned char *f_scripts [];
#endif

// Music
extern const unsigned char m_ingame_a [];
extern const unsigned char m_ingame_b [];
extern const unsigned char m_title [];
extern const unsigned char m_gover [];
unsigned char which_level;

// Push to zero page:
#pragma bssseg (push,"ZEROPAGE")
#pragma dataseg(push,"ZEROPAGE")

unsigned char i, gpit, gpjt, gpaux;
signed char fader;
unsigned char half_life;

unsigned char n_pant, on_pant;

unsigned char *gp_gen;
unsigned int gp_addr;
unsigned char rdx, rdy, rdt, rdit;
unsigned char rda, rdb, rdc;

unsigned char olife, okeys, oobjs, oammo;

// Enemies
unsigned char en_x [3], en_y [3];
unsigned char en_x1 [3], en_y1 [3];
unsigned char en_x2 [3], en_y2 [3];
signed char en_mx [3], en_my [3];
unsigned char en_t [3], en_s [3];
unsigned char en_life [3], en_alive [3], en_status [3], en_rawv [3], en_ct [3];
unsigned char en_fr, etx1, etx2, ety1, ety2;
unsigned char en_collx, en_colly;
unsigned char touched;
unsigned char en_touched [3], en_cttouched [3];

// Main player
signed int px, py, pvx, pvy;
unsigned char pfacing, pfr, pctfr, psprid;
unsigned char pj, pctj, pgotten, ppossee;
unsigned char pobjs, plife, pkeys;
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
unsigned char **spr_enems, **spr_player;

// Level control
unsigned char ts_offs, alt_bg;

#ifdef MAPPED_TILESETS
unsigned char *tileset_pals;
unsigned char *tsmap;
#endif
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

// Current level
unsigned char *c_map;
unsigned char *c_locks;
unsigned char *c_enems;
unsigned char *c_hotspots;

// *************
// Main includes
// *************

#ifdef MAPPED_TILESETS
#include "tsmap.h"
#endif
#include "printer.h"
#include "general.h"
#ifndef DEACTIVATE_KEYS
#include "bolts.h"
#endif
#ifdef BREAKABLE_WALLS
#include "breakable.h"
#endif
#include "hotspots.h"
#include "player.h"
#include "enengine.h"
#include "frame.h"
#include "pres.h"

// **************
// Main functions
// **************

void __fastcall__ prepare_scr (void) {
	fade_delay = 1;
	if (!ft) fade_out (); else ft = 0;

	enems_load ();
	//enems_sprites ();

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
	oam_meta_spr (prx, 240, 128, spr_empty);

#ifdef PLAYER_CAN_FIRE
	for (gpit = 0; gpit < MAX_BULLETS; gpit ++) bst [gpit] = 0;
	bullets_move ();
#endif

	// Reenable sprites and tiles now we are finished.
	ppu_on_all ();

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

	pal_spr (mypal_game_fg0);
	pal_bg (mypal_game_bg0);
	
	credits ();
	
	scroll (0, 8);

	while (1) {
		// Title here
		pal_bg (mypal_game_bg0);
		pal_spr (mypal_game_fg0);
		
		gpit = title ();
		
		switch (gpit) {
			case 0:
				c_map = (unsigned char *) (mapa0);
				//c_locks = (unsigned char *) (locks_mapa0);
				c_enems = (unsigned char *) (enems_ROM0);
				c_hotspots = (unsigned char *) (hotspots_ROM0);
				spr_enems = (unsigned char **) (spr_enems0);
				spr_player = (unsigned char **) (spr_player0);
				//ts_offs = 0; alt_bg = 1;
				tsmap = (unsigned char *) (tsmap0); alt_bg = 32;
				tileset_pals = (unsigned char *) (tileset_pals0);
				which_level = 1;
				break;
			case 1:
				c_map = (unsigned char *) (mapa1);
				//c_locks = (unsigned char *) (locks_mapa1);
				c_enems = (unsigned char *) (enems_ROM1);
				c_hotspots = (unsigned char *) (hotspots_ROM1);
				pal_bg (mypal_game_bg1);
				pal_spr (mypal_game_fg1);
				spr_enems = (unsigned char **) (spr_enems1);
				spr_player = (unsigned char **) (spr_player1);
				//ts_offs = 16; alt_bg = 0;
				tsmap = (unsigned char *) (tsmap1); alt_bg = 32;
				tileset_pals = (unsigned char *) (tileset_pals1);
				which_level = 0;
				break;
			case 2:
				c_map = (unsigned char *) (mapa2);
				//c_locks = (unsigned char *) (locks_mapa2);
				c_enems = (unsigned char *) (enems_ROM2);
				c_hotspots = (unsigned char *) (hotspots_ROM2);
				pal_bg (mypal_game_bg2);
				pal_spr (mypal_game_fg2);
				spr_enems = (unsigned char **) (spr_enems0);
				spr_player = (unsigned char **) (spr_player0);
				//ts_offs = 16; alt_bg = 0;
				tsmap = (unsigned char *) (tsmap2); alt_bg = 32;
				tileset_pals = (unsigned char *) (tileset_pals2);
				which_level = rand8 () & 1;
				break;
		}
				
		cls ();
		draw_game_frame ();

		oam_clear ();

		ppu_on_all ();

		n_pant = SCR_INICIO;
		on_pant = 99;

		hotspots_load ();
#ifndef DEACTIVATE_KEYS		
		bolts_load ();
#endif		
		player_init ();
		
#ifdef ACTIVATE_SCRIPTING
		msc_init_all ();
#endif
		half_life = 0;
		olife = oammo = oobjs = okeys = 0xff;

		if (which_level) music_play (m_ingame_a); else music_play (m_ingame_b);
		set_vram_update (UPDATE_LIST_SIZE, update_list);

#ifdef ACTIVATE_SCRIPTING
		script_result = 0;
		// Entering game script
		script = (unsigned char *) e_scripts [MAP_W * MAP_H];
		run_script ();
#endif
		ft = 1;
		while (1) {
			half_life = 1 - half_life;
			update_index = 0;
			if (pstate == EST_PARP) {
				pctstate --;
				if (!pctstate) pstate = EST_NORMAL;
			}

			player_move ();
			enems_move ();
#ifdef PLAYER_CAN_FIRE
			bullets_move ();
#endif
#ifdef BREAKABLE_ANIM
			if (do_process_breakable) process_breakable ();
#endif
			if (pstate == EST_NORMAL || half_life) {
				oam_meta_spr (prx, pry + SPRITE_ADJUST, 128, spr_player [psprid]);	
			} else {
				oam_meta_spr (prx, 240, 128, spr_empty);
			}
			// Hotspot interaction
			if (hrt) {
				if (collide_in (prx + 8, pry + 8, hrx, hry)) {
#ifdef MAPPED_TILESETS
					map_set (hrx >> 4, hry >> 4, tsmap [0]);
#else
					map_set (hrx >> 4, hry >> 4, ts_offs);
#endif
					switch (hrt) {
#ifndef DEACTIVATE_OBJECTS
						case 1:
							pobjs ++;
							sfx_play (3, 1);
							break;
#endif
#ifndef DEACTIVATE_KEYS
						case 2:
							pkeys ++;
							sfx_play (2, 1);
							break;
#endif
						case 3:
							plife += PLAYER_REFILL;
							if (plife > PLAYER_LIFE) plife = PLAYER_LIFE;
							sfx_play (3, 1);
							break;
#ifdef MAX_AMMO
						case 4:
							sfx_play (2, 1);
							if (MAX_AMMO - pammo > AMMO_REFILL)
								pammo += AMMO_REFILL;
							else
								pammo = MAX_AMMO;
							break;
#endif
					}
					hry = 240;
					if (hrt == 3) hact [n_pant] = 0; else hact [n_pant] = 2;
				}
			}

			// Pause?
			if (i & PAD_START) {
				while (pad_poll (0) & PAD_START);
				music_pause (1);
				sfx_play (0, 1);
				pal_bright (3);
				ppu_waitnmi ();
				while (!(pad_poll (0) & PAD_START));
				pal_bright (4);
				music_pause (0);
				while (pad_poll (0) & PAD_START);
				sfx_play (0, 1);
			}

			// Change screen
			// Very basic. Extend when needed.
			if (prx == 0 && (i & PAD_LEFT)) {
				if (n_pant) {
					n_pant --;
					px = 240 * 64;
				}
			} else if (prx >= 240 && (i & PAD_RIGHT)) {
				if (n_pant < MAP_W * MAP_H) {
					n_pant ++;
					px = 0;
				}
#ifdef PLAYER_MOGGY_STYLE				
			} else if (pry == 0 && (i & PAD_UP)) {
				if (n_pant >= MAP_W) {
					n_pant -= MAP_W; 
					py = 176 * 64;
				}
			} else if (pry >= 176 && (i & PAD_DOWN)) {
				if (n_pant <= MAP_W * MAP_H - MAP_W - 1) {
					n_pant += MAP_W;
					py = 0;
				}
#else
			} else if (pry == 0 && (pvy < 0)) {
				if (n_pant >= MAP_W) {
					n_pant -= MAP_W;
					py = 176 * 64;
				}
			} else if (pry >= 176 && (pvy > 0)) {
				if (n_pant <= MAP_W * MAP_H - MAP_W - 1) {
					n_pant += MAP_W;
					py = 0;
				}
#endif				
			}

			// Update frame
#ifndef DEACTIVATE_OBJECTS
			if (oobjs != pobjs) {
				oobjs = pobjs;
				p_t (OBJECTS_X, OBJECTS_Y, PLAYER_NUM_OBJETOS - pobjs);
			}
#endif
#ifndef DEACTIVATE_KEYS
			if (okeys != pkeys) {
				okeys = pkeys;
				p_t (KEYS_X, KEYS_Y, pkeys);
			}
#endif
			if (olife != plife) {
				olife = plife;
				p_t (LIFE_X, LIFE_Y, plife);
			}
#ifdef MAX_AMMO
			if (oammo != pammo) {
				oammo = pammo;
				p_t (AMMO_X, AMMO_Y, pammo);
			}
#endif

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
				game_over ();
				break;
			}

#ifdef ACTIVATE_SCRIPTING
			if (script_result) {
#else
			if (pobjs == PLAYER_NUM_OBJETOS) {
#endif
				game_ending ();
				break;
			}

		}
		set_vram_update (0, 0);
		ppu_off ();
	}
}

#ifdef ACTIVATE_SCRIPTING
#include "mscnes.h"
#endif

