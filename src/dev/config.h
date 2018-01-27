// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017

// ============================================================================
// I. General configuration
// ============================================================================

// In this section we define map dimmensions, initial and authomatic ending conditions, etc.

#define MAP_W 					5		//
#define MAP_H					5		// Map dimmensions in screens
#define MAP_SIZE				MAP_W*MAP_H

#define PLAYER_LIFE				5		// Vida máxima (con la que empieza, además)
#define PLAYER_REFILL			1		// Recarga de vida.

//#define MULTI_LEVEL					// Comment for single level
#define MAX_LEVELS				2

#ifndef MULTI_LEVEL

	// If you aim for a single level ROM, fill in those:

	#define SCR_INI					0		// Initial screen
	#define PLAYER_INI_X			3		//
	#define PLAYER_INI_Y			3		// Initial position
	//#define SCR_END				99		// Final screen, undefine if N/A
	#define PLAYER_END_X			99		//
	#define PLAYER_END_Y			99		// Ending position
	#define PLAYER_MAX_OBJECTS		25		// Objects to finish game

#else

	// If you want multiple levels, please don't touch this much. 
	// Just comment SCR_END and / or PLAYER_MAX_OBJECTS if N/A.
	// You should be fiddling with assets/levelset.h

	#define SCR_INI 				(l_scr_ini [level])
	#define PLAYER_INI_X			(l_ini_x [level])
	#define PLAYER_INI_Y			(l_ini_y [level])
	//#define SCR_END				(l_scr_end [level])
	#define PLAYER_END_X			(l_end_x [level])
	#define PLAYER_END_Y			(l_end_y [level])
	#define PLAYER_MAX_OBJECTS		(l_player_max_objects [level])

#endif

// Some flexibility
#define HOTSPOTS_WONT_CHANGE			// types of hotspots won't change
#define HOTSPOT_TYPE_OBJECT		1
#define HOTSPOT_TYPE_KEYS		2
#define HOTSPOT_TYPE_REFILL		3
#define HOTSPOT_TYPE_BOOT		4		// Custom for this game.

//#define HOTSPOT_TYPE_AMMO		4

//#define MAP_FORMAT_PACKED				// Each byte in map data = 2 tiles, 16 tiles max.
#define MAP_FORMAT_RLE16				// RLE'd by rlemap2. 16 tiles max.
//#define MAP_WITH_DECORATIONS			// Add decorations when use a 16 tiles mode.

// Use the complex renderer if you need to post-process the map buffer before
// printing.

//#define MAP_RENDERER_COMPLEX			// Comment for the simple, old renderer

// ============================================================================
// II. Engine type
// ============================================================================

// This section is used to define the game engine behaviour. Many directives are related,
// and others are mutually exclusive. I think this will be pretty obvious when you look at them. 

// Right now the shooting engine is only compatible with the side-view engine.

// Bounding box size
// -----------------
                                        // Comment both for normal 16x16 bounding box
#define PLAYER_COLLISION_TOP		4   // Player is 8x16, but this can make him "shorter" for collision
#define SMALL_COLLISION               	// 8x8 centered collision instead of 12x12

// General directives:
// -------------------

//#define PLAYER_PUSH_BOXES 			// If defined, tile #14 is pushable
//#define FIRE_TO_PUSH
//#define DEACTIVATE_KEYS				// If defined, keys are not present.
//#define DEACTIVATE_OBJECTS			// If defined, objects are not present.
//#define PLAYER_BOUNCES
//#define DOUBLE_BOUNCE
#define DIE_AND_RESPAWN					// If defined, dying = respawn on latest safe.
#define DIE_AND_REENTER					// Reenter screen on death
#define PLAYER_FLICKERS 			 	// If defined, collisions make player flicker instead.
//#define WALLS_STOP_ENEMIES			// If defined, enemies react to the scenary

// Extra special tiles
// -------------------

// Quicksands, beh = 2.
// For player movement values, see section 4
// (PLAYER_VY_SINKING)

//#define ENABLE_QUICKSANDS

// Breakable, beh = 16
//#define ENABLE_BREAKABLE				// Breakable walls
#define BREAKABLE_LIFE	2				// Amount of hits to break wall
#define BREAKABLE_ANIM					// Show explosion when breaking
#define BREAKABLE_MAX 			4		// Max # of concurrent explosions
#define BREAKABLE_MAX_FRAMES	8		// Frames to show explosion
#define BREAKABLE_ERASE			0		// Tile to erase broken tiles
#define BREAKABLE_BREAKING		8		// Tile to display while breaking
#define BREAKABLE_WALKABLE				// If defined (side view), tiles break when stepped on

// Conveyors, beh = 32 [+1]
// For player movement values, see section 4
// (PLAYER_VX_CONVEYORS)

//#define ENABLE_CONVEYORS

// Slippery, beh = 64. 
// For player movement values, see section 4
// (PLAYER_AX_ICE & PLAYER_RX_ICE)

//#define ENABLE_SLIPPERY

// Enemy types and definitions
// ---------------------------

//#define PERSISTENT_ENEMIES
//#define PERSISTENT_DEATHS

#define SPRITE_BADDIE_DYING 			16
//#define ENEMS_OCCLUDING_FRAME			16 // If you use pezons or saws you need a flame for occlusion

// Fanties / Homing fanties

//#define ENABLE_FANTY
//#define ENABLE_HOMING_FANTY

#define FANTY_BASE_SPRID				16
#define FANTY_WITH_FACING
#define FANTY_COLLIDES
#define FANTY_KILLED_BY_TILE
#define FANTY_LIFE_GAUGE				5

#define FANTY_A 						4
#define FANTY_MAXV 						48

#define FANTY_DISTANCE					80
#define FANTY_V_RETREAT					16

// Pursuers (for top-down)

//#define ENABLE_PURSUERS				// If defined, type 7 enemies are active
#define DEATH_COUNT_EXPRESSION			50+(rand8()&63)
#define TYPE_7_FIXED_SPRITE 			4	// If defined, type 7 enemies are always #

// Saws

//#define ENABLE_SAW
#define SAW_BASE_SPRID					16
#define SAW_V_DISPL						4
#define SAW_EMERGING_STEPS				10

// Monococos

//#define ENABLE_MONOCOCO
#define MONOCOCO_BASE_TIME_HIDDEN		150
#define MONOCOCO_BASE_TIME_APPEARING	50
#define MONOCOCO_BASE_TIME_ONBOARD		50
#define MONOCOCO_FIRE_COCO_AT			MONOCOCO_BASE_TIME_ONBOARD/2
// Make code more readable:
#define MONOCOCO_X 						en_x
#define MONOCOCO_Y						en_y
#define MONOCOCO_COUNTER 				en_my
#define MONOCOCO_STATE 					en_mx

// If you enable monococos, I think you should enable cocos:

//#define ENABLE_COCOS
#define COCOS_MAX						3
#define COCO_V							192
#define COCO_PATTERN					34
#define COCO_FAIR_D						32

// Pezons

//#define ENABLE_PEZONS
#define PEZONS_BASE_SPRID		16
#define PEZON_WAIT				50
#define PEZON_THRUST			384
#define PEZON_VY_FALLING_MAX	256
#define PEZON_G					16

// Chac chacs

//#define ENABLE_CHAC_CHAC
#define CHAC_CHAC_BASE_TILE		48
#define CHAC_CHAC_IDLE_2		16
#define CHAC_CHAC_IDLE_3		1
#define CHAC_CHAC_IDLE_4		50

// Carry directives

/*
#define CARRY_ONE_HS_OBJ
#define HS_INV_X				136
#define HS_INV_Y				210
#define HS_INV_EMPTY			3
#define HS_INV_MIN				3
#define HS_INV_MAX				7
#define HS_USE_OFFS				10
*/

/*
#define CARRY_ONE_FLAG_OBJ
#define HS_INV_X				136
#define HS_INV_Y				210
#define HS_INV_EMPTY			0
#define HS_INV_FLAG				0
*/

// Shooting behaviour
// ------------------
/*
#define PLAYER_CAN_FIRE 				// If defined, shooting engine is enabled.
#define PLAYER_BULLET_SPEED 	4		// Pixels/frame. 
#define MAX_BULLETS 			4		// Max number of bullets on screen. Be careful!.
#define PLAYER_BULLET_Y_OFFSET	6		// vertical offset from the player's top.
#define PLAYER_BULLET_X_OFFSET	0		// vertical offset from the player's left/right.
#define ENEMIES_LIFE_GAUGE		2		// Amount of shots needed to kill enemies.
#define RESPAWN_ON_ENTER				// Enemies respawn when entering screen
#define FIRE_MIN_KILLABLE 		1		// If defined, only enemies >= N can be killed.

#define MAX_AMMO				99		// If defined, ammo is not infinite!
#define AMMO_REFILL				50		// type 3 hotspots refill amo, using tile 20
//#define INITIAL_AMMO 		0			// If defined, ammo = X when entering game.

#define BULLET_PALETTE			1
#define BULLET_PATTERN			1		// To paint the bullet. Can be an expresion.
*/

/*
#define BREAKABLE_WALLS					// Breakable walls
#define BREAKABLE_WALLS_LIFE	2		// Amount of hits to break wall
#define BREAKABLE_ANIM					// Show explosion when breaking
#define MAX_BREAKABLE_FRAMES	8		// Frames to show explosion
#define BREAKABLE_ERASE			29		// Tile to erase broken tiles
#define BREAKABLE_BREAKING		30		// Tile to display while breaking
*/

// Scripting
// ---------
/*
#define ACTIVATE_SCRIPTING				// Activates msc scripting and flag related stuff.
#define ENABLE_FAST_FIRE_ZONE			// Fire zone only triggers FIRE n, not FIRE ANY
#define SCRIPTING_TEXT_BOX
#define FIRE_SCRIPT_WITH_ANIMATION		// So I don't have to make a custom for something 
										// I will be reusing, most likely
*/

//#define FIRE_ON_KILL					// run fire script on enemy kill
/*
//#define ENABLE_EXTERN_CODE			// Enables custom code to be run from the script using EXTERN n
#define ENABLE_FIRE_ZONE				// Allows to define a zone which auto-triggers "FIRE"
*/
// Top view:
// ---------
/*
#define PLAYER_TOP_DOWN            		// Enable top view.
#define TOP_OVER_SIDE                 	// UP/DOWN has priority over LEFT/RIGHT
*/
// Side view:
// ----------

#define PLAYER_HAS_JUMP					// If defined, player is able to jump.
//#define PLAYER_SWIMS					// If defined, player swims a la Ninjajar!
//#define ENABLE_CONVEYORS				// Conveyors
//#define PLAYER_HAS_JETPAC             // If defined, player can thrust a vertical jetpac
//#define PLAYER_KILLS_ENEMIES			// If defined, stepping on enemies kills them
//#define PLAYER_SAFE_LANDING			// Like KILLS_ENEMIES but without the killing.
//#define PLAYER_MIN_KILLABLE     3     // Only kill enemies with id >= PLAYER_MIN_KILLABLE

// ============================================================================
// III. Screen configuration
// ============================================================================

// This sections defines how stuff is rendered, where to show counters, etcetera

#define LIFE_X					7		//
#define LIFE_Y					3		// Life gauge counter character coordinates

#define OBJECTS_X				18		//
#define OBJECTS_Y				3		// Objects counter character coordinates

#define KEYS_X					28		//
#define KEYS_Y					3		// Keys counter character coordinates

//#define KILLED_X				16		//
//#define KILLED_Y				2		// Kills counter character coordinates

//#define AMMO_X				8		// 
//#define AMMO_Y				2		// Ammo counter character coordinates

// Text
//#define LINE_OF_TEXT			26		// If defined, scripts can show text @ Y = #
//#define LINE_OF_TEXT_X		1		// X coordinate.

// ============================================================================
// IV. Player movement configuration
// ============================================================================

// This section is used to define which constants are used in the gravity/acceleration engine.
// If a side-view engine is configured, we have to define vertical and horizontal constants
// separately. If a top-view engine is configured instead, the horizontal values are also
// applied to the vertical component, vertical values are ignored.

// IV.1. Vertical movement. Only for side-view.

#define PLAYER_VY_FALLING_MAX	256		// Max. velocity when falling
#define PLAYER_VY_FALLING_MIN	64		// Use for animating if you need
#define PLAYER_VY_SINKING		2
#define PLAYER_G				16		// Gravity

#define PLAYER_VY_JUMP_INITIAL	64
#define PLAYER_VY_JUMP_MAX		192		// Max. velocity when jumping
#define PLAYER_AY_JUMP 			12		// Jumpin acceleration 

#define PLAYER_AY_JETPAC		32		// Jetpac increment
#define PLAYER_VY_JETPAC_MAX	256		// Max jetpac vertical speed

#define PLAYER_AY_SWIM			8		// Swimming acceleration.
#define PLAYER_VY_SWIM_MAX		64		// Swimming max. speed

// IV.2. Horizontal (side view) or general (top view) movement.

#define PLAYER_VX_MAX			128		// Max. horizontal speed
#define PLAYER_VX_SPRINT_MAX	192
#define PLAYER_VX_MAX_PODEWWWR	256
#define PLAYER_VX_CONVEYORS 	64
#define PLAYER_AX				16		// Horizontal acceleration
#define PLAYER_AX_ICE			4
#define PLAYER_AX_SPRINT		12
#define PLAYER_RX				16		// Horizontal friction
#define PLAYER_RX_ICE			2

#define PLAYER_VX_MIN (PLAYER_AX << 2)

#define PLAYER_V_REBOUND		224

// Animation cells 

#ifdef PLAYER_TOP_DOWN

	// Cell definitions for top-down view

	#define CELL_FACING_RIGHT 	0
	#define CELL_FACING_LEFT 	2
	#define CELL_FACING_UP 		4
	#define CELL_FACING_DOWN 	6

#else

	// Cell definitions for side view

	#define CELL_FACING_RIGHT	0
	#define CELL_FACING_LEFT	8

	#define CELL_IDLE			0
	#define CELL_WALK_CYCLE		1
	#define CELL_AIRBORNE		5
	//#define CELL_SWIM_CYCLE		6

	//#define CELL_ASCENDING		5
	//#define CELL_DESCENDING		6

#endif
