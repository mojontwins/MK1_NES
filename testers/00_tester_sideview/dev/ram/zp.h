// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// zp

// Avoid parameters using these in critical functions.

unsigned char _x, _y, _n, _t, _z;

// General, all-purpose variables

unsigned char pad0;                     // (neslib) pad 0 read.
unsigned char pad_this_frame;           // (neslib) pad 0 read, current frame presses.
unsigned char gpit, gpjt;               // General purpose iterators.
unsigned char gpitu, gpaux;             // Auxiliary iterators.
signed int rds16;                       // General purpose 16 bit signed variable.
unsigned int gpint;                     // General purpose 16 bit unsinged variable.

const unsigned char *gp_gen;            // General purpose pointer to read data in ROM.
const unsigned char *gp_tmap, *gp_tma2; // Pointers used to read map data in ROM.
unsigned char *gp_ram, *gp_ram_aux;     // General purpose pointers to read / write data in RAM.
unsigned int gp_addr;                   // General purpose address, used for PPU addresses.
unsigned char rdx, rdy;                 // General purpose coordinates
unsigned char rdt;                      // General purpose "type"
unsigned char rdit;                     // General purpose iterator
unsigned char rda, rdb, rdc, rdd, rdm;  // General purpose temporal value holders.
unsigned char rde;                      // General purpose temporal value holders.
signed char rds;                        // General purpose temporal value holder, signed.
unsigned char rdct;                     // General purpose counter
unsigned char ticker;                   // Ticker. 0 for a frame every second.
#ifdef DOUBLE_WIDTH
    signed int rdaa, rdbb;              // Genearl purpose integer
    signed int en_x_offs;               // Precalculated pixel offset
#endif

// Used for two-points collision

unsigned char at1, at2;                 // Temporally store tile attributes at the checked collision points
unsigned char cx1, cx2;                 // Check those collision points, X coordinates.
unsigned char cy1, cy2;                 // Check those collision points, Y coordinates.
unsigned char caux;                     // Auxiliar variable

#if PLAYER_COLLISION_VSTRETCH_BG > 0
    unsigned char cy3, at3;             // If player collision box is > 16 pixels, a third point is used
#endif

// Sprite related (neslib)

unsigned char oam_index;

// Enemies

unsigned char en_initial;               // First enemy to process in current screen, current frame (cycles 0, 1, 2, ...)
unsigned char en_offs;                  // Precalculated value: n_pant * 3; used to index data related to enemy persistence

unsigned char en_fr;                    // (Temporal) enemies frame number
unsigned char en_collx, en_colly;       // (Temporal) collision flags
unsigned char en_spr;                   // Current enemy sprite index
unsigned char touched;                  // (Temporal) an enemy collided with the player
unsigned char en_is_alive;              // (Temporal) current enemy is alive, used when enemies respawning is on.
unsigned char pregotten;                // (Temporal) player <-> current enemy horizontal overlap flag.

#ifdef ENEMS_RECOIL_ON_HIT
    signed char en_rmx [NENEMS];        // If recoiling, recoil direction in the X axis.
    #ifdef PLAYER_TOP_DOWN
        signed char en_rmy [NENEMS];    // If recoiling, recoil direction in the Y axis.
    #endif
#endif

#ifdef ENABLE_COMPILED_ENEMS
    const unsigned char *en_behptr [NENEMS]; // A pointer to curren enemy script for compiled enemies.
#endif

// Those variables are used as temporal copies of general arrays defined in BSS (check bss.h)

unsigned char _en_t, _en_s;
unsigned char _en_x, _en_y;
unsigned char _en_x1, _en_y1, _en_x2, _en_y2;
signed char _en_mx, _en_my;
unsigned char _en_ct, _en_facing, _en_state;
signed int _enf_x, _enf_y, _enf_vx, _enf_vy;

// Signals, all purpose, for enemies

unsigned char en_sg_1, en_sg_2;

// Main player

unsigned char vertical_engine_type;     // Player engine type. Se ENGINE_TYPE_* constants in definitions.h
unsigned int px;
signed int py;                          // Player X, Y coordinates, fixed point 10.6
signed int pvx, pvy;                    // Player VX, VY velocities, fixed point 10.6
#ifdef DOUBLE_WIDTH
    unsigned int prx;
#else
    unsigned char prx;
#endif
unsigned char pry;                      // Player pixel coordinates, calculated from px, py.
#ifdef DOUBLE_WIDTH
    unsigned int pcx;
#else
    unsigned char pcx;
#endif
unsigned char pcy;                      // Coordinates prior to movement.
unsigned char pfacing;                  // Player facing left, right
unsigned char pfr;                      // Player frame
unsigned char pctfr;                    // Player counter (for animation)
unsigned char psprid;                   // Player sprite ID set in my/player_frame_selector.h
unsigned char a_button, b_button;       // True if A or B have been pressed *this* frame, respectively

#ifdef PLAYER_TOP_DOWN
    unsigned char pfacinghlast;         // Last horizontal direction registered
#endif

unsigned char pj;                       // "Player is jumping" flag
unsigned char pctj;                     // "Player is jumping" counter
unsigned char pgotten;                  // "Player is on moving platform or similiar" flag
unsigned char ppossee;                  // "Player is on walkable scenery" flag
unsigned char oppossee;                 // Same, but prev. frame
unsigned char pflickering;              // "Player is flickering", also a frame counter.
unsigned char pbouncing;                // "Player is bouncing", also a frame counter.
unsigned char phit;                     // "Player was hit by killing tile" flag
signed int pgtmx, pgtmy;                // X, Y components of velocity in "player is being moved by external entity" conditions

#if defined (ENABLE_EASY_OBJECTS) || defined (ENABLE_ONLY_ONE_OBJECT)
    unsigned char pinv, opinv;          // Player inventory (which object is being carried) & last frame copy.
#endif

#ifdef ENABLE_LADDERS
    unsigned char ponladder;            // True if player is on a ladder.
    unsigned char phalfladder;          // True if player's feet are on a ladder.
    unsigned char ponladderctr;         // Counter used for animation when player is on ladders in my/player_frame_selector.h
#endif

#ifdef ENABLE_TRAMPOLINES
    unsigned char ptrampoline;          // "Player got propelled by a trampoline" flag
#endif

#ifdef PLAYER_SPINS 
    unsigned char pspin;                // "Player is spinning" flag
#endif  

#ifdef PLAYER_TOP_DOWN
    unsigned char pfacingv, pfacingh;   // Player facing variables in each axis for top-down games
#endif

unsigned char hitv, hith;               // True if the player collided with a killing tile vertically or horizontally

#ifdef PLAYER_CAN_FLOAT
    unsigned char pfloating;            // Player is floating (for example, when on a propeller)
#endif

unsigned char prx_old, pry_old;

unsigned char pkill;                    // Set to 1 to kill player
unsigned char pice;                     // Player is walking on ice
unsigned char pnotsafe;                 // Player is walking a non safe (for example, breakable) tile 

unsigned char pcontinues;               // Number of continues
unsigned char plife;                    // Player life gauge
#ifndef DEACTIVATE_KEYS
    unsigned char pkeys;                // Player # of collected keys
#endif
unsigned char pobjs;                    // Player # of collected (collectible) items
unsigned char pammo;                    // Player ammo
#if defined (ENEMS_MAY_DIE)
    unsigned char pkilled;              // Player # of killed enemies
#endif

unsigned char pfiring;                  // Flag to control actions spawned by the B button

#ifdef DIE_AND_RESPAWN
    unsigned char n_pant_safe;
    signed int px_safe, py_safe;        // Screen (room) number and coordinates to respawn player
#endif

#ifdef PLAYER_PUNCHES
    unsigned char ppunching;            // "Player is punching" flag
#endif

#ifdef PLAYER_KICKS
    unsigned char pkicking;             // "Player is kicking" flag
#endif

#if defined (PLAYER_PUNCHES) || defined (PLAYER_KICKS)
    #ifdef DOUBLE_WIDTH
        unsigned int phitterx;
    #else
        unsigned char phitterx;
    #endif
    unsigned char phittery;             // If player is punching or kicking, hitbox coordinates
    unsigned char phitteract;           // True if hitbox is active
    unsigned char pfrozen;              // != 0 if player is frozen (after landing a hit). Invalidate input & decrement
#endif

// Bullets

#ifdef PLAYER_CAN_FIRE
    unsigned char bi;                   // Iterator for player bullets (projectiles shot by the player)
    #ifdef DOUBLE_WIDTH
        signed int _bx;
    #else
        unsigned char _bx;
    #endif
    unsigned char _by;                  // Fast copies
    #ifdef DOUBLE_WIDTH
        unsigned char bullets_disable;      // Disable bullets processing on current frame
    #endif
#endif

// Cocos

#ifdef ENABLE_COCOS
    unsigned char coco_it;              // Iterator for cocos (projectiles shot by enemies)
#endif

// Hotspots

#ifdef DOUBLE_WIDTH
    unsigned int hrx;
#else
    unsigned char hrx;
#endif
unsigned char hry, hrt;                 // Current screen hotspot X, Y coordinates and type, respectively

// Process breakable?
#ifdef BREAKABLE_ANIM
    unsigned char do_process_breakable; // There's animation of tiles breaking, check next frame.
#endif

unsigned char ft;                       // "This is the first time rendering a screen" flag, 
                                        // used internally to control fading in/out of rooms

// Current level

unsigned char c_map_w;                  // Map width for current level
unsigned char c_map_h;                  // Map height for current level
unsigned char c_map_size;               // Map size for current level

const unsigned char *c_ts_pals;         // Pointer to metatile palettes array
const unsigned char *c_ts_tmaps;        // Pointer to metatile tilemaps array
const unsigned char *c_behs;            // Pointer to metatile behaviours (attributes) array

const unsigned char * const *spr_enems; // Pointer to metasprite array for enemies

const unsigned char *c_pal_bg;          // Pointer to current background palette
const unsigned char *c_pal_fg;          // Pointer to current foreground palette (sprites)

// Pointer to current map, several formats:

#ifdef MAP_FORMAT_PACKED
    const unsigned char *c_map;         
#endif
#if defined (MAP_FORMAT_RLE16) || defined (MAP_FORMAT_RLE53) || defined (MAP_FORMAT_RLE44)
    const unsigned char * const *c_map;
#endif
#if defined (MAP_FORMAT_RLE53_CHRROM) || defined (MAP_FORMAT_RLE44_CHRROM)
    const unsigned int *c_map;
    unsigned char c_map_chr_rom_bank;
#endif
    
// Pointer to decorations, several formats:

#ifdef MAP_WITH_DECORATIONS
    #if defined (MAP_FORMAT_RLE16) || defined (MAP_FORMAT_PACKED)
        const unsigned char * const *c_decos;
    #else
        unsigned char c_decos;
    #endif
#endif

const unsigned char *c_locks;           // Pointer to current level list of locks in ROM

// Pointers to enemies & hotspots data structures, different formats (CHR-ROM or PRG-ROM based)

#ifdef ENEMS_IN_CHRROM
    unsigned int c_enems;
    unsigned int c_hotspots;
#else
    const unsigned char *c_enems;
    const unsigned char *c_hotspots;
#endif

unsigned char c_max_bolts;              // Number of locks in current level

// Scripting system interpreter internal variables.

#ifdef ACTIVATE_SCRIPTING
    unsigned char sc_x, sc_y, sc_n, sc_c;
    unsigned char *next_script;
    unsigned char *script;
    unsigned char script_result, sc_terminado, sc_continuar;
#endif
    
// Double screen helpers

#ifdef DOUBLE_WIDTH
    unsigned char w_pant;               // (Working) Current screen (backup).
    unsigned int nametable_base;        // 0x2000 or 0x2400
    unsigned int buff_offset;           // 0 or 192
    unsigned char attr_table_offset;    // 0 or 64
    unsigned char *buff_ptr;            // Points to parts of the buffer
    unsigned char *attr_ptr;            // Points to parts of the buffer
    signed int scroll_x;                // Scroller position
    unsigned char on_screen;            // Flag used in enengine.h
#endif
