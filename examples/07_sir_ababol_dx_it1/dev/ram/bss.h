// NES MK1 v0.7
// Copyleft Mojon Twins 2013, 2015, 2016

// bss

// Update list

#define UPDATE_LIST_SIZE 32
unsigned char update_index;
unsigned char update_list [UPDATE_LIST_SIZE * 3];

// Attributes

unsigned char map_attr [192];
unsigned char map_buff [192];

// Breakable walls

#ifdef BREAKABLE_WALLS
	unsigned char brk_buff [192];
	#ifdef BREAKABLE_ANIM
		#define MAX_BREAKABLE 4
		unsigned char brkf [MAX_BREAKABLE];
		unsigned char brkx [MAX_BREAKABLE];
		unsigned char brky [MAX_BREAKABLE];
	#endif
#endif

// Fader

signed char fader;
unsigned char fade_delay;

// Hotspots

#ifndef HOTSPOTS_WONT_CHANGE
	unsigned char hyx [MAP_W * MAP_H];
	unsigned char ht [MAP_W * MAP_H];
#endif

unsigned char hact [MAP_W * MAP_H];

// Bolts

#ifndef DEACTIVATE_KEYS
	unsigned char lkyx [MAX_BOLTS], lknp [MAX_BOLTS], lkact [MAX_BOLTS];
	unsigned char xy;
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
#endif

// Baddies count. Properly fill this value!

#ifdef PLAYER_KILLS_ENEMIES
	unsigned char baddies_count;
	#define BADDIES_COUNT baddies_count
#endif

// Make enemies persistent

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
