// NES MK1 v2.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// bss

// NTSC system

unsigned char ntsc, ntsc_frame;

// Update list (neslib)

#define UPDATE_LIST_SIZE 32
unsigned char update_index;
unsigned char update_list [UPDATE_LIST_SIZE * 3];

// Attribute table
#ifdef DOUBLE_WIDTH
    unsigned char attr_table [128];
#else
    unsigned char attr_table [64];
#endif

// Fader (neslib)

signed char fader;                      // Brightness level
unsigned char fade_delay;               // # of frames per brightness level in fades

// Current screen

unsigned char map_attr [BUFF_SIZE];     // Current screen (room) tile behaviours
unsigned char map_buff [BUFF_SIZE];     // Current screen (room) tile numbers

// Game flow

unsigned char first_game;               // True if it's the first time the game runs

unsigned char level;                    // Current level number

unsigned char c_max_enems;              // Number of killable enems. in current level.

unsigned char n_pant;                   // Current screen (room) number.
unsigned char on_pant;                  // Current screen (room) numberm last frame value (used to detect changes)

unsigned char half_life;                // 1-0 flip flop (each game frame)
unsigned char frame_counter;            // Game grame counter, wraps 255->0

unsigned char paused;                   // True if the game is paused.
unsigned char win_level;                // If WIN_LEVEL_CUSTOM is defined, set to true to win the level.
unsigned char game_over;                // True if the player lost the game.
unsigned char warp_to_level;            // True if the player is jumping to a different level 
unsigned char level_reset;              // True if the level should be reset.
unsigned char flick_override;           // If set, code to flick screen is overriden for current frame

// General player values, last frame copies

unsigned char olife;                    // Player life, last frame value (used to detect changes)
unsigned char okeys;                    // Player keys, last frame value (used to detect changes)
unsigned char oobjs;                    // Player items, last frame value (used to detect changes) 
unsigned char oammo;                    // Player ammo, last frame value (used to detect changes)
unsigned char okilled;                  // Player # of killed enems., last frame value (used to detect changes)

unsigned char oam_index_player;         // Index copy

// Custom hotspot type star (used as an extra collectible).

#ifdef HOTSPOT_TYPE_STAR
    unsigned char pstars;               // Player # of collected stars
    unsigned char opstars;              // Player # of collected stars, last frame value (used to detect changes) 
#endif
    
// Player "use" animation values.

#ifdef ENABLE_USE_ANIM
    unsigned char use_ct;               // General counter, serves as a state indicator.
    unsigned char use_sub_ct;           // Subcounter.
    #ifdef ACTIVATE_SCRIPTING
        unsigned char run_fire_script_do;   
                                        // If true, the PRESS_FIRE scripts should be run.
    #endif
    unsigned char use_type;             // Action type. May be USE_TYPE_HOTSPOT, USE_TYPE_INTERACTIVE or USE_TYPE_SCRIPTING
#endif

// A small sprite showing a text balloon with the NO! word,
// Of course, may be repurposed for other uses.

#ifdef ENABLE_NO
    unsigned char no_ct;                // General counter. When != 0, display sprite & decrement.
#endif

// Bullets (projectiles shot by the player)

#ifdef PLAYER_CAN_FIRE
    unsigned char b_slots [MAX_BULLETS];
    unsigned char b_slots_i;            // Array of free slots for active bullets & index.

    #ifdef PLAYER_BULLET_LIFE
        unsigned char bst [MAX_BULLETS];    // Bullets states
    #endif
    
    #ifdef DOUBLE_WIDTH
        unsigned int bx [MAX_BULLETS];      // Bullets, X coordinates
        #if !defined (PLAYER_TOP_DOWN) && !defined (PLAYER_CAN_FIRE_8_WAY)
            unsigned char b_cy1 [MAX_BULLETS];  
                                            // Precalculate cy1
        #endif
    #else
        unsigned char bx [MAX_BULLETS];     // Bullets, X coordinates.
    #endif

    unsigned char by [MAX_BULLETS];     // Bullets, Y coordinates.
    signed char bmx [MAX_BULLETS];      // Bullets, direction + speed in the X axis.
    signed char bmy [MAX_BULLETS];      // Bullets, direction + speed in the Y axis.

    #ifdef PLAYER_CHARGE_AND_FIRE
        unsigned char pfiregauge;       // Fire gauge 
        unsigned char opfiregauge;      // Fire gauge, last frame value (used to detect changes)
    #endif

    #ifdef PLAYER_FIRE_RELOAD
        unsigned char pfirereload;      // Reload time. When != 0, block B_BUTTON & decrement.
    #endif

	#ifdef PLAYER_TOP_DOWN
		unsigned char pctfr;            // Frame subcounter
	#endif
#endif

// Cocos (projectiles shot by enemies)

#ifdef ENABLE_COCOS
    unsigned char coco_slots [COCOS_MAX];
    unsigned char coco_slots_i;         // Array of free slots for active cocos & index.

    unsigned char coco_on [COCOS_MAX];  // Cocos states
    
    signed int coco_x [COCOS_MAX];      // Cocos, X coordinates (fixed point 10.6)
    signed int coco_y [COCOS_MAX];      // Cocos, Y coordinates (fixed point 10.6)
    signed int coco_vx [COCOS_MAX];     // Cocos, direction + speed in the X axis.
    signed int coco_vy [COCOS_MAX];     // Cocos, direction + speed in the Y axis.
#endif  

// Breakable walls

#ifdef ENABLE_BREAKABLE
    #ifndef BREAKABLES_SOFT
        unsigned char brk_buff [BUFF_SIZE];
                                        // A "life gauge" for each tile on screen, used when BREAKABLES_SOFT is unset.
    #endif
    #ifdef BREAKABLE_ANIM
        unsigned char brkf [BREAKABLE_MAX];
        unsigned char brkx [BREAKABLE_MAX];
        unsigned char brky [BREAKABLE_MAX];
                                        // List of tiles to update when BREAKABLE_ANIM is set
    #endif
    unsigned char pmayneedrelocation;   // If 1, check if the coordinates to respawn the player are safe, relocate if not.
#endif

// Hotspots

#if defined (HOTSPOTS_DYNAMIC) || defined (ENEMS_IN_CHRROM)       
                                        // If HOTSPOTS_DYNAMIC is defined, hotspots are copied to RAM and may be modified
                                        // Otherwise they are read from ROM when entering each screen.
    unsigned char hyx [MAP_SIZE];       // Hotspot position per screen (room), packed 0xYX
    unsigned char ht [MAP_SIZE];        // Hotspot type per screen (room)
#endif

unsigned char hact [MAP_SIZE];          // Hotspots active per screen (room)

// Hotspots for double width

#ifdef DOUBLE_WIDTH
    unsigned int d_hrx [2];
    unsigned char d_hry [2], d_hrt [2]; // Store two screens worth of hotspots
#endif

// Bolts (locks, unlockable with keys)

#ifndef DEACTIVATE_KEYS
    unsigned char lkact [MAX_BOLTS];    // If lkact [N], lock #N is "locked"
#endif

// Scripting system

#ifdef ACTIVATE_SCRIPTING
    void msc_clear_flags (void);
    void run_script (unsigned char whichs);

    #if defined (ENABLE_FIRE_ZONE) || defined (ENABLE_FAST_FIRE_ZONE)
        unsigned char f_zone_ac;        // "Fire zone" is active in current screen (room)
        unsigned char fzx1;
        unsigned char fzx2;
        unsigned char fzy1;
        unsigned char fzy2;             // Coordinates of the "fire zone" rectangle's opposite corners.
    #endif

    #if defined (ENABLE_PUSHED_SCRIPT)
        unsigned char just_pushed;      // True if a pushable has just been pushed. Used by the interpreter.
    #endif

    unsigned char fire_script_success;  // True if the command section of a clausule in a PRESS_FIRE script was run.

    #if defined (ENABLE_INTERACTIVES)
        unsigned char just_interacted;  // True if the player just interacted. Used by the interpreter.
        unsigned char script_arg;       // Identifier of the interactive the used interacted with. Used by the interpreter.
    #endif

    #if defined (DOUBLE_WIDTH)
        unsigned char odd;
    #endif
#endif

// Flags

unsigned char flags [MAX_FLAGS];        // Array of flags used by interactives, the scripting system, etc.

// Interactives

#ifdef ENABLE_INTERACTIVES
    unsigned char interactives_interact_with;
                                        // The player pressed B on which interactive?
    unsigned char interactives_index;
                                        // Index of active interactives on current screen (room) (when creating),
                                        // Number of active interactives (in the game loop)
    unsigned char interactives_f [INTERACTIVES_MAX];
                                        // Flag bound to the interactive | 0x80 (if container) or sprite number (if sprite).
                                        // If this value & 0x80 => container. Sprite otherwise.
    #ifdef DOUBLE_WIDTH
        unsigned char interactives_x [INTERACTIVES_MAX], interactives_y [INTERACTIVES_MAX];
    #else
        unsigned char interactives_yx [INTERACTIVES_MAX];
    #endif
    #ifdef INTERACTIVES_FROM_CODE
        const unsigned char *c_interactives;
                                        // Pointer to current level's interactive definitions array
    #endif
#endif

// More globals for baddies (for current screen)

unsigned char en_t [NENEMS];            // Enemy types.

unsigned char en_x [NENEMS];            // Enemy X coordinates.
unsigned char en_y [NENEMS];            // Enemy Y coordinates.

unsigned char en_x1 [NENEMS];
unsigned char en_y1 [NENEMS];           // Enemy starting point coordinates (for patrollers, repurposed for other types)

unsigned char en_x2 [NENEMS];
unsigned char en_y2 [NENEMS];           // Enemy ending point coordinates (for patrollers, repurposed for other types)

signed char en_mx [NENEMS];             // Enemy direction + speed in the X axis.
signed char en_my [NENEMS];             // Enemy direction + speed in the Y axis.

unsigned char en_s [NENEMS];            // Enemy base sprite index in spr_enems.
unsigned char en_facing [NENEMS];       // Generally, 0 = facing right, 4 = facing left.

unsigned char en_state [NENEMS];        // Enemy State
unsigned char en_flags [NENEMS];        // Enemies flags

unsigned char en_cttouched [NENEMS];    // Counters used to show explosions / flickering
unsigned char en_life [NENEMS];         // Enemies life gauges
unsigned char en_status [NENEMS];       // Enemies statused, repurposed per enemy type
unsigned char en_ct [NENEMS];           // Enemies General repurposeable counter

#if defined (ENABLE_COMPILED_ENEMS) || defined ENABLE_PURSUERS
    unsigned char en_rawv [NENEMS];         // Speed, used for pursuer-type enemies
#endif

unsigned char en_spr_x_mod;             // Modifier to X position of sprite (for effects)

#ifdef ENEMS_CAN_RESPAWN
    unsigned char en_respawn [NENEMS];  // If true, enems can respawn.
    unsigned char en_resx [NENEMS];
    unsigned char en_resy [NENEMS];     // Respawn coordinates.
    signed char en_resmx [NENEMS];
    signed char en_resmy [NENEMS];      // Respawn mx, my.
#endif

// Fixed point variables (coordiantes, velocity) for some kinds of enemies.

#ifdef ENEMS_NEED_FP
    signed int enf_x [NENEMS];
    signed int enf_vx [NENEMS];
    signed int enf_y [NENEMS];
    signed int enf_vy [NENEMS];
#endif

// Generator for PURSUER type enemies.

#ifdef ENABLE_GENERATORS
    unsigned char en_generator_life [NENEMS];
                                        // Generators life gauge.
    unsigned char gen_was_hit [NENEMS];      // True if generator was hit.
#endif

unsigned char en_spr_id [NENEMS];

// Temporal invincibility

#ifdef ENEMS_INVINCIBILITY
    unsigned char en_invincible [NENEMS];
#endif

// Persistent enemies (position / movement is remembered)

#ifdef PERSISTENT_ENEMIES
    unsigned char ep_x [NENEMS * MAP_SIZE];  // Buffer for X coordinates.
    unsigned char ep_y [NENEMS * MAP_SIZE];  // Buffer for Y coordinates.
    signed char ep_mx [NENEMS * MAP_SIZE];   // Buffer for direction + speed in the X axis.
    signed char ep_my [NENEMS * MAP_SIZE];   // Buffer for direction + speed in the Y axis.
#endif

// Persistent deaths (death enemies stay dead in current level)

#ifdef PERSISTENT_DEATHS
    unsigned char ep_dead [NENEMS * MAP_SIZE];
                                        // Buffer for dead enemies (true if dead).
#endif

// Propellers

#ifdef ENABLE_PROPELLERS
    unsigned char prp_idx;              // Index of active propellers on current screen (room) (when creating)
                                        // Number of active propellers on current screen (room) (in the game loop)
    unsigned int prp_addr [PROPELLERS_MAX];
                                        // PPU Addresses of active propellers.
    unsigned char propellers_on;        // Propellers are on (animate & affect player)
#endif

// Shines

#ifdef ENABLE_SHINES
    unsigned char max_shines;           // Index of active shines on current screen (room) (when creating)
                                        // Number of active shines on current screen (room) (in the game loop)

    unsigned char shines [SHINES_MAX];  // Coordinates of active shines, packed 0xYX

    unsigned char shine_active_x;
    unsigned char shine_active_y;       // Animating shine X and Y coordinates.
    unsigned char shine_active_ct;      // Animation frame counter. 
#endif

// Tile Chac Chacs

#ifdef ENABLE_TILE_CHAC_CHAC
    unsigned char max_chac_chacs;       // Index of active chac-chac blades on current screen (room) (when creating)
                                        // Number of active chac-chac blades on current screen (room) (in the game loop)
    unsigned char chac_chacs_yx [MAX_CHAC_CHACS];
                                        // Coordinates of active chac-chac blades, packed 0xYX
    unsigned char chac_chacs_state [MAX_CHAC_CHACS];
                                        // States of active chac-chac blades
    unsigned char chac_chacs_idlewait [MAX_CHAC_CHACS];
                                        // # of frames to wait in state 0 (idle)
    unsigned char chac_chacs_ct [MAX_CHAC_CHACS];
                                        // Frame counters
    
    // Those are used to implement a "update queue" to send changes to VRAM.
    // Changes are sent "one entity per frame", therefore this queue is needed.
    unsigned char chac_chacs_queue [MAX_CHAC_CHACKS_QUEUED];
    unsigned char chac_chacs_queue_read, chac_chacs_queue_write;
#endif

// Resonators

#ifdef ENABLE_RESONATORS
    unsigned char res_disable;          // If true, resonators don't work (but show!)
    unsigned char res_on;               // Resonators are "on" (player pressed one)
    unsigned char res_ct;               // This holds resonators' count down
    unsigned char res_subct;            // Frame counter for each "tick"
#endif

// More conveyors

#ifdef ENABLE_CONVEYORS
    signed char cfx;                    // Auxiliary variable, copy of the added speed when the player is on a conveyor,
                                        // used to detect if the player is pushed out of the screen while on a conveyor belt.
#endif

// Timer

#ifdef ENABLE_TIMER
    unsigned char timer;                // Timer, current value.
    unsigned char otimer;               // Timer, current value, last frame (to detect changes).
    unsigned char timer_frames;         // Timer, frame counter for each "ticke"
    unsigned char timer_zero;           // Set to true when timer == 0; must be reset manually!
    unsigned char timer_on;             // If true, timer is running.
#endif

// Springs

#ifdef ENABLE_SPRINGS
    unsigned char springs_on;           // Springs are ON, level-wide setting.
#endif

// Shaker

#ifdef ENABLE_SHAKER
    unsigned char shaker_ct;            // If != 0, shake the screen & decrement.
#endif

// Collectible map tiles

#ifdef ENABLE_TILE_GET
    #ifdef PERSISTENT_TILE_GET
        unsigned char tile_got [24];    // Buffer for current screen
    #endif
#endif

#include "my/extra_vars.h"              // Custom extra variables
