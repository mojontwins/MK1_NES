// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// bss

// Game flow

unsigned char half_life, frame_counter;
unsigned char n_pant, on_pant;
unsigned char olife, okeys, oobjs, oammo, okilled;
#ifdef ENABLE_USE_ANIM
	unsigned char use_ct, use_sub_ct;
#endif
#ifdef ENABLE_NO
	unsigned char no_ct;
#endif
unsigned char level, game_over, win_level;

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
	unsigned char brk_buff [192];
	#ifdef BREAKABLE_ANIM
		unsigned char brkf [BREAKABLE_MAX];
		unsigned char brkx [BREAKABLE_MAX];
		unsigned char brky [BREAKABLE_MAX];
	#endif
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
#endif

// Flags

unsigned char flags [MAX_FLAGS];

#ifdef ENABLE_CONTAINERS
	unsigned char containers_index, containers_interact_with;
	unsigned char containers_f [CONTAINERS_MAX] , containers_yx [CONTAINERS_MAX];
#endif

// More baddies

unsigned char en_x [3], en_y [3];
unsigned char en_x1 [3], en_y1 [3];
unsigned char en_x2 [3], en_y2 [3];
signed char en_mx [3], en_my [3];
unsigned char en_t [3], en_s [3], en_facing [3];

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
	unsigned char ep_flags [3 * MAP_SIZE];
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
	unsigned char res_on, res_ct, res_subct;
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
