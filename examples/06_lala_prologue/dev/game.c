// NES MK1 v0.7
// Copyleft Mojon Twins 2013, 2015, 2016

// Uses neslib and related tools by Shiru

// Use 0 form ROM1, 1 for ROM2, 2 for ROM3
#define BASE_LEVEL		0

// Comment this when you are done
//#define DEBUG
#define DEBUG_LEVEL		0
#define DEBUG_SCR_INI	20
#define DEBUG_INI_X		14
#define DEBUG_INI_Y		5
//

#define MSB(x)			(((x)>>8))
#define LSB(x)			(((x)&0xff))
#define FIXBITS			6
#define SPRITE_ADJUST	31
#define TOP_ADJUST		4
#define COMM_POOL		((unsigned char*)0x0300)

#define MAX_CERROJOS 16 // max 32, make it as small as possible.

#include "neslib.h"

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

// Music
extern const unsigned char m_ingame_1 [];
extern const unsigned char m_cuts [];

// Push to zero page:
#pragma bssseg (push,"ZEROPAGE")
#pragma dataseg(push,"ZEROPAGE")

unsigned char i, gpit, gpjt, gpaux;
unsigned char half_life, frame_counter;

unsigned char n_pant, on_pant;

unsigned char *gp_gen, *gp_tmap, *gp_tma2;
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
#ifdef ENABLE_GENERATORS
unsigned char en_generator_life [3], gen_was_hit [3];
#endif
unsigned char en_fr, etx1, etx2, ety1, ety2;
unsigned char en_collx, en_colly;
unsigned char touched;
unsigned char en_cttouched [3];
#if defined (ENABLE_FANTY) || defined (ENABLE_HOMING_FANTY)
signed int enf_x [3], enf_vx [3];
#endif
#if defined (ENABLE_FANTY) || defined (ENABLE_HOMING_FANTY) || defined (ENABLE_PEZONS)
signed int enf_y [3], enf_vy [3];
#endif

// Main player
signed int px, py, pvx, pvy;
unsigned char pfacing, pfr, pctfr, psprid;
#ifdef PLAYER_MOGGY_STYLE
unsigned char pfacinghlast;
#endif
unsigned char pj, pctj, pjb, pgotten, ppossee, psprint;
//#ifndef DEACTIVATE_OBJECTS
unsigned char pobjs;
//#endif
unsigned char plife, pcontinues;
#ifndef DEACTIVATE_KEYS
unsigned char pkeys;
#endif
unsigned char pammo, pfiring;
#ifdef PLAYER_TURRET
unsigned char pfixct;
#endif
unsigned char pushed_any;
unsigned char pstate, pctstate;
unsigned char phit;
signed int pgtmx, pgtmy;
#ifdef CARRY_ONE_HS_OBJ
unsigned char pinv;
#endif

// Aux player
unsigned char prx, pry, ptx1, ptx2, pty1, pty2;
#ifdef PLAYER_MOGGY_STYLE
unsigned char pfacingv, pfacingh;
#endif
unsigned char wall, hitv, hith;
#ifdef ENABLE_PROPELLERS
unsigned char ppropelled;
#endif
#if defined (PLAYER_KILLS_ENEMIES) || defined (PLAYER_CAN_FIRE) || defined (FANTY_KILLED_BY_TILE)
unsigned char pkilled;
#endif

// Bullets
#ifdef PLAYER_CAN_FIRE
unsigned char bst [MAX_BULLETS], bx [MAX_BULLETS], by [MAX_BULLETS];
signed char bmx [MAX_BULLETS];
#ifdef PLAYER_MOGGY_STYLE
signed char bmy [MAX_BULLETS];
#endif
unsigned char btx, bty;
#endif

#ifdef PLAYER_GARGAJO
unsigned char gst [MAX_GARGAJOS], gx [MAX_GARGAJOS], gy [MAX_GARGAJOS];
signed char gmx [MAX_GARGAJOS];
unsigned char ghsx, ghsy, pgargajocounter;
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
unsigned char **c_map;
unsigned char **c_decos;
unsigned char *c_locks;
unsigned char *c_enems;
unsigned char *c_hotspots;
unsigned char *c_pal_bg;
unsigned char *c_pal_fg;

unsigned char *tileset_pals;
unsigned char *tsmap;

// CUSTOM LIGHTNING FRIGHTENING! {
unsigned char lightning_ct;
// } END_OF_CUSTOM

// KINDA CUSTOM {
unsigned char no_ct, use_ct, guay_ct;
unsigned char idle_on, idle_ctr;
// } END_OF_CUSTOM

// Everything else on normal RAM
#pragma bssseg (push,"BSS")
#pragma dataseg(push,"BSS")

// Update list
#define UPDATE_LIST_SIZE 32
unsigned char update_index;
unsigned char update_list [UPDATE_LIST_SIZE * 3];

signed char fader;
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
void msc_clear_flags (void);
void run_script (unsigned char whichs);
#if defined (ENABLE_FIRE_ZONE) || defined (ENABLE_FAST_FIRE_ZONE)
unsigned char f_zone_ac, fzx1, fzx2, fzy1, fzy2;
#endif
#endif
#if defined (ACTIVATE_SCRIPTING) || defined (ENABLE_CONTAINERS)
#define MAX_FLAGS 16
unsigned char flags [MAX_FLAGS];
#endif
#ifdef ENABLE_CONTAINERS
unsigned char upd_cont_index;
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
//unsigned char ep_t [3 * MAP_W * MAP_H];
unsigned int ep_it;
#endif
#ifdef PERSISTENT_DEATHS
unsigned char ep_flags [3 * MAP_W * MAP_H];
#endif

// Die & respawn
#ifdef DIE_AND_RESPAWN
unsigned char n_pant_safe;
signed int px_safe, py_safe;
#endif

unsigned char level, game_over;

#ifdef SCRIPTING_TEXT_BOX
unsigned char stbl;
#endif

// *************
// Main includes
// *************

#ifdef ENABLE_PROPELLERS
void add_propeller (unsigned char x, unsigned char y);
#endif
#include "engine/general.h"
#include "engine/printer.h"
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
#ifdef ENABLE_CONTAINERS
#include "engine/containers.h"
#endif
#include "engine/player.h"
#include "engine/enengine.h"
#include "engine/frame.h"
#include "engine/pres.h"
#ifdef ACTIVATE_SCRIPTING
#include "assets/mscnes.h"
#endif

// **************
// Main functions
// **************

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

	// Clear line of text
	//pr_str (LINE_OF_TEXT_X, LINE_OF_TEXT, "                              ");

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
	run_script (2 * MAP_W * MAP_H + 1);
	// This room script
	run_script (n_pant + n_pant);
#endif
#ifdef ENABLE_CONTAINERS
	containers_draw ();
#endif

	ppu_waitnmi ();
	fade_in ();
}

void main(void) {
	bank_spr (1);
	bank_bg (0);

	scroll (0, 0);
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
	
		//cutscene ((unsigned char *) cuts1_tmaps, (unsigned char *) cuts1_pals, (unsigned char *) intro_text);
	
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

		music_play (m_ingame_1);
		set_vram_update (UPDATE_LIST_SIZE, update_list);

		ft = 1;	fade_delay = 1;

		// MAIN LOOP

		pal_bright (0);
		ppu_on_all ();
		
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
		//if (!game_over) cutscene ((unsigned char *) cuts1_tmaps, (unsigned char *) cuts1_pals, (unsigned char *) outtro_text);
		cls ();
		oam_clear ();	


	}
}
