// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// bss

// Game flow

unsigned char first_game;

unsigned char half_life, frame_counter;
unsigned char n_pant, on_pant;
unsigned char olife, okeys, oobjs, oammo, okilled;
#ifdef HOTSPOT_TYPE_STAR
	unsigned char pstars, opstars;
#endif
	
#ifdef ENABLE_USE_ANIM
	unsigned char use_ct, use_sub_ct;
	#ifdef ACTIVATE_SCRIPTING
		unsigned char run_fire_script_do;
	#endif
	unsigned char use_type;
#endif
#ifdef ENABLE_NO
	unsigned char no_ct;
#endif
unsigned char level, game_over, win_level, paused;

unsigned char c_max_enems;

// Update list

#define UPDATE_LIST_SIZE 32
unsigned char update_index;
unsigned char update_list [UPDATE_LIST_SIZE * 3];

// Attributes

unsigned char map_attr [192];
unsigned char map_buff [192];

// Bullets

#ifdef PLAYER_CAN_FIRE
	unsigned char b_slots [MAX_BULLETS], b_slots_i;
	unsigned char bst [MAX_BULLETS], bx [MAX_BULLETS], by [MAX_BULLETS];
	signed char bmx [MAX_BULLETS];
	signed char bmy [MAX_BULLETS];

	#ifdef PLAYER_CHARGE_AND_FIRE
		unsigned char pfiregauge, opfiregauge;
	#endif

	#ifdef PLAYER_FIRE_RELOAD
		unsigned char pfirereload;
	#endif
#endif

// Cocos

#ifdef ENABLE_COCOS
	signed int coco_x [COCOS_MAX], coco_y [COCOS_MAX];
	signed int coco_vx [COCOS_MAX], coco_vy [COCOS_MAX];
	unsigned char coco_on [COCOS_MAX];
	unsigned char coco_slots [COCOS_MAX], coco_slots_i;
#endif	

// Breakable walls

#ifdef ENABLE_BREAKABLE
	#ifndef BREAKABLES_SOFT
		unsigned char brk_buff [192];
	#endif
	#ifdef BREAKABLE_ANIM
		unsigned char brkf [BREAKABLE_MAX];
		unsigned char brkx [BREAKABLE_MAX];
		unsigned char brky [BREAKABLE_MAX];
	#endif
	unsigned char pmayneedrelocation;
#endif

// Fader

signed char fader;
unsigned char fade_delay;

// Hotspots

#ifdef HOTSPOTS_DYNAMIC
	unsigned char hyx [MAP_SIZE];
	unsigned char ht [MAP_SIZE];
#endif

unsigned char hact [MAP_SIZE];

// Bolts

#ifndef DEACTIVATE_KEYS
	unsigned char lkact [MAX_BOLTS];
#endif

// Scripting

#ifdef ACTIVATE_SCRIPTING
	void msc_clear_flags (void);
	void run_script (unsigned char whichs);
	#if defined (ENABLE_FIRE_ZONE) || defined (ENABLE_FAST_FIRE_ZONE)
		unsigned char f_zone_ac, fzx1, fzx2, fzy1, fzy2;
	#endif
	#if defined (ENABLE_PUSHED_SCRIPT)
		unsigned char just_pushed;
	#endif
	unsigned char fire_script_success;
	#if defined (ENABLE_INTERACTIVES)
		unsigned char script_arg; 
		unsigned char just_interacted;
	#endif
#endif

// Flags

unsigned char flags [MAX_FLAGS];

#ifdef ENABLE_INTERACTIVES
	unsigned char interactives_index, interactives_interact_with;
	unsigned char interactives_f [INTERACTIVES_MAX] , interactives_yx [INTERACTIVES_MAX];
	#ifdef INTERACTIVES_FROM_CODE
		const unsigned char *c_interactives;
	#endif
#endif

// More baddies

unsigned char en_x [3], en_y [3];
unsigned char en_x1 [3], en_y1 [3];
unsigned char en_x2 [3], en_y2 [3];
signed char en_mx [3], en_my [3];
unsigned char en_t [3], en_s [3], en_facing [3];

#ifdef ENEMS_CAN_RESPAWN
	unsigned char en_respawn [3];
	unsigned char en_resx [3], en_resy [3];
	signed char en_resmx [3], en_resmy [3];
#endif

#if defined (ENABLE_FANTY) || defined (ENABLE_HOMING_FANTY)
	signed int enf_x [3], enf_vx [3];
#endif
	
#if defined (ENABLE_FANTY) || defined (ENABLE_HOMING_FANTY) || defined (ENABLE_PEZONS)
	signed int enf_y [3], enf_vy [3];
#endif

#ifdef ENABLE_GENERATORS
	unsigned char en_generator_life [3], gen_was_hit [3];
#endif

unsigned char en_spr_id [3];

// Make enemies persistent

#ifdef PERSISTENT_ENEMIES
	unsigned char ep_x [3 * MAP_SIZE];
	unsigned char ep_y [3 * MAP_SIZE];
	signed char ep_mx [3 * MAP_SIZE];
	signed char ep_my [3 * MAP_SIZE];
#endif

#ifdef PERSISTENT_DEATHS
	unsigned char ep_dead [3 * MAP_SIZE];
#endif

// Text box

#ifdef SCRIPTING_TEXT_BOX
	unsigned char stbl;
#endif

// Propellers

#ifdef ENABLE_PROPELLERS
	unsigned char prp_idx;
	unsigned char prp_yx [PROPELLERS_MAX];
	unsigned char propellers_on;
#endif

// Shines

#ifdef ENABLE_SHINES
	unsigned char max_shines;
	unsigned char shine_active_x, shine_active_y;
	unsigned char shines [SHINES_MAX];
	unsigned char shine_active_ct;
#endif

// Resonators

#ifdef ENABLE_RESONATORS
	unsigned char res_on, res_disable, res_ct, res_subct;
#endif

// More conveyors

#ifdef ENABLE_CONVEYORS
	signed char cfx;
#endif

// Timer

#ifdef ENABLE_TIMER
	unsigned char timer, otimer, timer_frames, timer_zero, timer_on;
#endif

#include "my/extra_vars.h"

// Springs

#ifdef ENABLE_SPRINGS
	unsigned char springs_on;
#endif
