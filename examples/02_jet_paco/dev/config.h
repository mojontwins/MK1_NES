// NES MK1 v0.1
// Copyleft Mojon Twins 2013

// Config.h
// Churrera v.4
// Copyleft 2010, 2011 The Mojon Twins

// ============================================================================
// I. General configuration
// ============================================================================

// In this section we define map dimmensions, initial and authomatic ending conditions, etc.

#define MAP_W					7		//
#define MAP_H					5		// Map dimmensions in screens
#define SCR_INICIO				28		// Initial screen
#define PLAYER_INI_X			5		//
#define PLAYER_INI_Y			6		// Initial tile coordinates
#define SCR_FIN 				99		// Last screen. 99 = deactivated.
#define PLAYER_FIN_X			99		//
#define PLAYER_FIN_Y			99		// Player tile coordinates to finish game
#define PLAYER_NUM_OBJETOS		20		// Objects to get to finish game
#define PLAYER_LIFE 			10		// Max and starting life gauge.
#define PLAYER_REFILL			1		// Life recharge
//#define COMPRESSED_LEVELS				// use levels.h instead of mapa.h and enems.h (!)
//#define MAX_LEVELS			2		// # of compressed levels
//#define REFILL_ME						// If defined, refill player on each level

// ============================================================================
// II. Engine type
// ============================================================================

// This section is used to define the game engine behaviour. Many directives are related,
// and others are mutually exclusive. I think this will be pretty obvious when you look at them. 

// Right now the shooting engine is only compatible with the side-view engine.

// Bounding box size
// -----------------
                                        // Comment both for normal 16x16 bounding box
#define BOUNDING_BOX_8_BOTTOM           // 8x8 aligned to bottom center in 16x16
//#define BOUNDING_BOX_8_CENTERED       // 8x8 aligned to center in 16x16
#define SMALL_COLLISION               	// 8x8 centered collision instead of 12x12

// General directives:
// -------------------

#define PLAYER_AUTO_CHANGE_SCREEN		// Player changes screen automaticly (no need to press direction)
//#define PLAYER_PUSH_BOXES 			// If defined, tile #14 is pushable
//#define FIRE_TO_PUSH
#define DIRECT_TO_PLAY					// If defined, title screen is also the game frame.
#define DEACTIVATE_KEYS					// If defined, keys are not present.
//#define DEACTIVATE_OBJECTS			// If defined, objects are not present.
//#define ONLY_ONE_OBJECT				// If defined, only one object can be carried at a time.
//#define OBJECT_COUNT			1		// Defines which FLAG will be used to store the object count.
//#define DEACTIVATE_EVIL_TILE			// If defined, no killing tiles (behaviour 1) are detected.
#define FULL_BOUNCE						// If defined, evil tile bounces equal MAX_VX, otherwise v/2
//#define DOUBLE_BOUNCE
//#define PLAYER_BOUNCES				// If defined, collisions make player bounce
//#define SLOW_DRAIN					// Works with bounces. Drain is 4 times slower
#define PLAYER_FLICKERS 			 	// If defined, collisions make player flicker instead.
//#define MAP_BOTTOM_KILLS				// If defined, exiting the map bottomwise kills.
//#define WALLS_STOP_ENEMIES			// If defined, enemies react to the scenary
//#define EVERYTHING_IS_A_WALL			// If defined, any tile <> type 0 is a wall, otherwise just 8.
//#define ENABLE_PURSUERS				// If defined, type 7 enemies are active
//#define DEATH_COUNT_EXPRESSION	50+(rand8()&63)
//#define TYPE_7_FIXED_SPRITE 	4		// If defined, type 7 enemies are always #
//#define ENABLE_CONVEYORS

// Shooting behaviour
// ------------------
/*
#define PLAYER_CAN_FIRE 				// If defined, shooting engine is enabled.
#define PLAYER_BULLET_SPEED 	4		// Pixels/frame. 
#define MAX_BULLETS 			4		// Max number of bullets on screen. Be careful!.
#define PLAYER_BULLET_Y_OFFSET	6		// vertical offset from the player's top.
#define PLAYER_BULLET_X_OFFSET	0		// vertical offset from the player's left/right.
#define ENEMS_LIFE_GAUGE		2		// Amount of shots needed to kill enemies.
#define RESPAWN_ON_ENTER				// Enemies respawn when entering screen
#define FIRE_MIN_KILLABLE 		3		// If defined, only enemies >= N can be killed.
#define MAX_AMMO				99		// If defined, ammo is not infinite!
#define AMMO_REFILL				50		// type 3 hotspots refill amo, using tile 20
//#define INITIAL_AMMO 		0			// If defined, ammo = X when entering game.

#define BREAKABLE_WALLS					// Breakable walls
#define BREAKABLE_WALLS_LIFE	2		// Amount of hits to break wall
#define BREAKABLE_ANIM					// Show explosion when breaking
#define MAX_BREAKABLE_FRAMES	8		// Frames to show explosion
*/
//#define ENABLE_RANDOM_RESPAWN           // If defined, automatic flying enemies spawn on killed enemies
//#define FANTY_MAX_V             256     // Flying enemies max speed.
//#define FANTY_A                 12      // Flying enemies acceleration.
//#define FANTIES_LIFE_GAUGE      10      // Amount of shots needed to kill flying enemies.

// Scripting
// ---------
/*
#define ACTIVATE_SCRIPTING			// Activates msc scripting and flag related stuff.
//#define SCRIPTING_DOWN			// Use DOWN as the action key.
#define SCRIPTING_KEY_M				// Use M as the action key instead.
//#define ENABLE_EXTERN_CODE		// Enables custom code to be run from the script using EXTERN n
#define ENABLE_FIRE_ZONE			// Allows to define a zone which auto-triggers "FIRE"
*/
// Top view:
// ---------
/*
#define PLAYER_MOGGY_STYLE            // Enable top view.
#define TOP_OVER_SIDE                 // UP/DOWN has priority over LEFT/RIGHT
*/
// Side view:
// ----------

//#define PLAYER_HAS_JUMP               // If defined, player is able to jump.
#define PLAYER_HAS_JETPAC             	// If defined, player can thrust a vertical jetpac
//#define PLAYER_KILLS_ENEMIES          // If defined, stepping on enemies kills them
//#define PLAYER_MIN_KILLABLE     3     // Only kill enemies with id >= PLAYER_MIN_KILLABLE
//#define PLAYER_BOOTEE                 // Always jumping engine. Don't forget to disable "HAS_JUMP" and "HAS_JETPAC"!!!
//#define PLAYER_STEP_SOUND             // Sound while walking. No effect in the BOOTEE engine.
//#define PLAYER_BOUNCE_WITH_WALLS      // Bounce when hitting a wall. Only really useful in MOGGY_STYLE mode
//#define PLAYER_CUMULATIVE_JUMP        // Keep pressing JUMP to JUMP higher in several bounces

// ============================================================================
// III. Screen configuration
// ============================================================================

// This sections defines how stuff is rendered, where to show counters, etcetera

#define VIEWPORT_X				1		//
#define VIEWPORT_Y				1		// Viewport character coordinates
#define LIFE_X					25		//
#define LIFE_Y					28		// Life gauge counter character coordinates
#define OBJECTS_X				5		//
#define OBJECTS_Y				28		// Objects counter character coordinates
#define OBJECTS_ICON_X			99		// 
#define OBJECTS_ICON_Y			99		// Objects icon character coordinates (use with ONLY_ONE_OBJECT)
#define KEYS_X					22		//
#define KEYS_Y					29		// Keys counter character coordinates
#define KILLED_X				99		//
#define KILLED_Y				99		// Kills counter character coordinates
#define AMMO_X					8		// 
#define AMMO_Y					29		// Ammo counter character coordinates
//#define PLAYER_SHOW_KILLS

// Text
#define LINE_OF_TEXT			26		// If defined, scripts can show text @ Y = #
#define LINE_OF_TEXT_X			1		// X coordinate.
#define LINE_OF_TEXT_ATTR		71		// Attribute

// Graphic FX, uncomment which applies...

//#define USE_AUTO_SHADOWS              // Automatic shadows made of darker attributes
//#define USE_AUTO_TILE_SHADOWS         // Automatic shadows using specially defined tiles 32-47.
//#define UNPACKED_MAP                  // Full, uncompressed maps. Shadows settings are ignored.
//#define NO_MASKS                      // Sprites are rendered using OR instead of masks.
//#define PLAYER_ALTERNATE_ANIMATION    // If defined, animation is 1,2,3,1,2,3... 
#define MAPPED_TILESETS					// If defined, tileset is mapped (Perils, preliminar)

// ============================================================================
// IV. Player movement configuration
// ============================================================================

// This section is used to define which constants are used in the gravity/acceleration engine.
// If a side-view engine is configured, we have to define vertical and horizontal constants
// separately. If a top-view engine is configured instead, the horizontal values are also
// applied to the vertical component, vertical values are ignored.

// IV.1. Vertical movement. Only for side-view.

#define PLAYER_MAX_VY_CAYENDO	64 		// Max falling speed (512/64 = 8 pixels/frame)
#define PLAYER_G				4		// Gravity acceleration (32/64 = 0.5 p�xeles/frame^2)

#define PLAYER_VY_INICIAL_SALTO 32		// Initial junp velocity (64/64 = 1 p�xel/frame)
#define PLAYER_MAX_VY_SALTANDO	160 	// Max jump velocity (320/64 = 5 p�xels/frame)
#define PLAYER_INCR_SALTO		24		// acceleration while JUMP is pressed (48/64 = 0.75 p�xeles/frame^2)

#define PLAYER_INCR_JETPAC		12		// Vertical jetpac gauge
#define PLAYER_MAX_VY_JETPAC	64	 	// Max vertical jetpac speed

// IV.2. Horizontal (side view) or general (top view) movement.

#define PLAYER_MAX_VX			128 	// Max velocity (192/64 = 3 p�xels/frame)
#define PLAYER_AX				16		// Acceleration (24/64 = 0,375 p�xels/frame^2)
#define PLAYER_RX				16		// Friction (32/64 = 0,5 p�xels/frame^2)

// ============================================================================
// V. Tile behaviour
// ============================================================================

// Defines the behaviour for each tile. Remember that if keys are activated, tile #15 is a bolt
// and, therefore, it should be made a full obstacle!

// 0 = Walkable (no action)
// 1 = Walkable and kills.
// 2 = Walkable and hides.
// 4 = Platform (only stops player if falling on it)
// 8 = Full obstacle (blocks player from all directions)
// 10 = special obstacle (pushing blocks OR locks!)
// 16 = Breakable (#ifdef BREAKABLE_WALLS)
// You can add the numbers to get combined behaviours
// Save for 10 (special), but that's obvious, innit?

#ifndef COMPRESSED_LEVELS
unsigned char comportamiento_tiles [] = {
	0, 1, 8, 8, 8, 8, 0, 0, 0, 4, 8, 8, 4, 8, 8, 8,
	0, 1, 8, 8, 8, 8, 0, 0, 0, 0, 0, 4, 4, 8, 8, 8,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
#endif
