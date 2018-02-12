// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017

// bss

// Game flow

unsigned char half_life, frame_counter;
unsigned char n_pant, on_pant;
unsigned char olife, okeys, oobjs, oammo, okilled;
unsigned char use_ct, no_ct;
unsigned char level, game_over, win_level;

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

#if defined (ACTIVATE_SCRIPTING) || defined (ENABLE_CONTAINERS)
	#define MAX_FLAGS 16
	unsigned char flags [MAX_FLAGS];
#endif

#ifdef ENABLE_CONTAINERS
	unsigned char upd_cont_index;
	unsigned char c_x [MAX_CONTAINERS], c_y [MAX_CONTAINERS], c_f [MAX_CONTAINERS], last_container;
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

// Baddies count. Properly fill this value!

#ifdef PLAYER_KILLS_ENEMIES
	unsigned char baddies_count;
	#define BADDIES_COUNT baddies_count
#endif

// Make enemies persistent

#ifdef PERSISTENT_ENEMIES
	// Persistent enemies
	unsigned char ep_x [3 * MAP_SIZE];
	unsigned char ep_y [3 * MAP_SIZE];
	signed char ep_mx [3 * MAP_SIZE];
	signed char ep_my [3 * MAP_SIZE];
	//unsigned char ep_t [3 * MAP_W * MAP_H];
	unsigned int ep_it;
#endif

#ifdef PERSISTENT_DEATHS
	unsigned char ep_flags [3 * MAP_SIZE];
#endif

#ifdef SCRIPTING_TEXT_BOX
	unsigned char stbl;
#endif

#ifdef ENABLE_PROPELLERS
	unsigned char prp_idx;
	unsigned char prp_yx [PROPELLERS_MAX];
#endif

#include "my/extra_vars.h"
