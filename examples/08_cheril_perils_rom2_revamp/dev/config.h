// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

//#define GAME_AREA_TOP
//#define GAME_AREA_MIDDLE
//#define GAME_AREA_BOTTOM
#define GAME_AREA_CUSTOM

// ============================================================================
// I. General configuration
// ============================================================================

// In this section we define map dimmensions, initial and authomatic ending conditions, etc.

#define MAP_W 					6		//
#define MAP_H					4		// Map dimmensions in screens
#define MAP_SIZE				MAP_W*MAP_H

#define PLAYER_LIFE				5		// Vida máxima (con la que empieza, además)
#define PLAYER_REFILL			1		// Recarga de vida.

#define MULTI_LEVEL						// Comment for single level
#define MAX_LEVELS				2

#define MAX_FLAGS 				2		// Keep it low if you don't need flags

#ifndef MULTI_LEVEL

	// If you aim for a single level ROM, fill in those:

	#define SCR_INI					18		// Initial screen
	#define PLAYER_INI_X			2		//
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
#define HOTSPOT_TYPE_OBJECT		1
#define HOTSPOT_TYPE_KEYS		2
#define HOTSPOT_TYPE_REFILL		3
//#define HOTSPOT_TYPE_AMMO		4		// For ammo refills
//#define HOTSPOT_TYPE_TIME		5		// For time refills

#define HOTSPOT_TYPE_RESONATOR	4		// Custom for this game.

#define WIN_LEVEL_CUSTOM				// A level ends when win_level == 1
										// And such a thing has to be setup by YOU

//#define MAP_FORMAT_PACKED				// Each byte in map data = 2 tiles, 16 tiles max.
//#define MAP_FORMAT_RLE16				// RLE'd by rlemap2. 16 tiles max.
#define MAP_FORMAT_RLE53				// RLE'd by rle53mapMK1. 32 tiles max.

//#define MAP_WITH_DECORATIONS			// Add decorations when use a 16 tiles mode.

// Use the complex renderer if you need to post-process the map buffer before
// printing.

#define MAP_RENDERER_COMPLEX			// Comment for the simple, old renderer

// ============================================================================
// II. Engine type
// ============================================================================

// This section is used to define the game engine behaviour. Many directives are related,
// and others are mutually exclusive. I think this will be pretty obvious when you look at them. 

// Right now the shooting engine is only compatible with the side-view engine.

// Bounding box size
// -----------------

// Player is 8x16 for collision with BG but can be made taller by this amount. Negative values=shorter
#define PLAYER_COLLISION_VSTRETCH_BG		-4

// This defines how the player will collide with enemies. 
// Player is always 8 pixels wide and 16 pixel tall PLUS the value of this variable.
// this offset goes to the head. 
// So if you define it as -8, the collision box will be 8x8 bottom centerd.
// And if you define it as 4 the collision box will be 8x20 bottom centered.
// For 16x24 sprites it's safe to leave it at 0. For 16x16 should be better -2.
#define PLAYER_COLLISION_VSTRETCH_FG 	4

// Same for enemies.
// I usually leave it at 0 for <=24 pixels tall, 8 for 32 pixels tall.
#define ENEMS_COLLISION_VSTRETCH_FG		0

// General directives:
// -------------------

// Push boxes

//#define PLAYER_PUSH_BOXES 			// If defined, tile #14 is pushable
#define FIRE_TO_PUSH

//#define ENABLE_PUSHED_SCRIPT
#define PUSHED_TILE_FLAG				1
#define PUSHED_TO_X_FLAG				2
#define PUSHED_TO_Y_FLAG				3

// General stuff

//#define DEACTIVATE_KEYS				// If defined, keys are not present.
#define DEACTIVATE_OBJECTS				// If defined, objects are not present.
//#define PLAYER_BOUNCES
//#define DOUBLE_BOUNCE
#define DIE_AND_RESPAWN					// If defined, dying = respawn on latest safe.
//#define DIE_AND_REENTER				// Reenter screen on death
#define PLAYER_FLICKERS 			 	// If defined, collisions make player flicker instead.
//#define WALLS_STOP_ENEMIES			// If defined, enemies react to the scenary

// Extra special tiles
// -------------------

// Quicksands, beh == 2.
// For player movement values, see section 4
// (PLAYER_VY_SINKING)

#define ENABLE_QUICKSANDS

// Breakable, beh & 16
//#define ENABLE_BREAKABLE				// Breakable walls
#define BREAKABLE_LIFE	2				// Amount of hits to break wall
#define BREAKABLE_ANIM					// Show explosion when breaking
#define BREAKABLE_MAX 			4		// Max # of concurrent explosions
#define BREAKABLE_MAX_FRAMES	8		// Frames to show explosion
#define BREAKABLE_ERASE			0		// Tile to erase broken tiles
#define BREAKABLE_BREAKING		8		// Tile to display while breaking
//#define BREAKABLE_WALKABLE			// If defined (side view), tiles break when stepped on

// Conveyors, beh & 32 [+1] (must be & 8!)
// For player movement values, see section 4
// (PLAYER_VX_CONVEYORS)

#define ENABLE_CONVEYORS

// Slippery, beh & 64. (must be & 12!)
// For player movement values, see section 4
// (PLAYER_AX_ICE & PLAYER_RX_ICE)

//#define ENABLE_SLIPPERY

// Ladders, beh == 32

//#define ENABLE_LADDERS

// Extra special stuff
// -------------------

// Propellers

#define ENABLE_PROPELLERS
#define PROPELLERS_MAX 					4
#define PROPELLERS_BASE_PATTERN			64	// First of 4 patterns to draw/animate propellers
#define PROPELLERS_MAX_LENGTH			6	// In tiles; undef for infinite
#define PROPELLER_TILE					24	// Tile # in map to detect a propeller
//#define PROPELLERS_ON_BY_DEFAULT			// Define so propellers are on by default

// Killing tiles shine, beware!

#define ENABLE_SHINES
#define SHINES_MAX 						8
#define SHINES_BASE_PATTERN				10	// In the sprite bank, two patterns needed
#define SHINES_PALETTE					3
#define SHINING_TILE					23	// Tile # in map to add shines

// Resonators. Not sure if you need this, but.

#define ENABLE_RESONATORS
#define RESONATOR_BASE_PATTERN			0
#define RESONATOR_PALETTE				3
#define RESONATOR_COUNTER_OFFS_X		4
#define RESONATOR_COUNTER_OFFS_Y		7
//#define RESONATOR_CHANGE_BG_PAL		paltsgrey	// Define to change pal for this palette
#define RESONATOR_CHANGE_SPR_PAL		palssgrey	// Define to change pal for this palette

// Simple warpers. Put as 0xff type enemies.
// Destination is attr:s1 (n_pant:YX).

//#define ENABLE_SIMPLE_WARPERS
#define SIMPLE_WARPERS_BASE_SPRID
#define SIMPLE_WARPERS_FIRE_BUTTON

// No!

#define ENABLE_NO 
#define NO_METASPRITE					ssit_06
#define NO_OFFS_X						0
#define NO_OFFS_Y						-24

// Use animation.
// Note: When activated hotspots need b_button press and, in side view, possee.

#define ENABLE_USE_ANIM
#define USE_ANIM_MAX_FRAMES				13
#define USE_ANIM_INTERACT_ON			7
#define USE_ANIM_FRAMES_PER_STEP		4

// Text boxes. If you need them, you call them

#define ENABLE_TEXT_BOX
#define TEXT_BOX_FRAME_TILE_OFFSET		0x20	// We need tiles in the active metatileset to draw the frame

// Timer. 

//#define ENABLE_TIMER
#define TIMER_INITIAL					5
//#define TIMER_START_ON
#define TIMER_REFILL					0
#define TIMER_RESET_ON_ENTER
//#define TIMER_TIME_FLAG				0	// Useful with scripting. Copies time to flag
//#define TIMER_ZERO_FLAG 				1	// Useful with scripting. raises flag when time zero
#define TIMER_SOUND						10  // play a sound if timer < value

// Springs

#define ENABLE_SPRINGS
#define SPRING_TILE 					10
#define SPRING_SPIKE_TILE 				11
//#define SPRINGS_ON_BY_DEFAULT

// Enemy types and definitions
// ---------------------------

#define ENEMIES_LIFE_GAUGE				1	// Amount of shots/punches/kicks needed to kill enemies.
//#define ENEMS_FLICKER						// Ifdef: flicker, if not: explosion

#define PERSISTENT_ENEMIES
#define PERSISTENT_DEATHS

#define ENEMS_TOUCHED_FRAMES			8	// # frames to stay frozen after hit

#define ENEMS_ENABLE_DYING_FRAME

#define ENEMS_EXPLODING_CELL 			43
#define ENEMS_EXPLODING_CELLS_HIDES			// Define and the baddie will be substituted by the explosion

#define ENEMS_OCCLUDING_CELL			42	// If you use pezons or saws you need a flame for occlusion

// Beware: only activate this if enemies are killable by any means:
//#define ENEMIES_SUFFER_ON_PLAYER_COLLISION

// Fanties / Homing fanties

#define ENABLE_FANTY
//#define ENABLE_HOMING_FANTY

#define FANTY_BASE_SPRID				32
#define FANTY_WITH_FACING
#define FANTY_COLLIDES
#define FANTY_KILLED_BY_TILE
//#define FANTY_LIFE_GAUGE				5	// Define if you need these to be tougher

#define FANTY_A 						4
#define FANTY_MAXV 						48

#define FANTY_DISTANCE					80
#define FANTY_V_RETREAT					16

// Pursuers (for top-down)

//#define ENABLE_PURSUERS				// If defined, type 7 enemies are active
#define DEATH_COUNT_EXPRESSION			50+(rand8()&63)
#define TYPE_7_FIXED_SPRITE 			4	// If defined, type 7 enemies are always #

// Saws

#define ENABLE_SAW
#define SAW_BASE_SPRID					40
#define SAW_V_DISPL						4
#define SAW_EMERGING_STEPS				10

// Pezons

//#define ENABLE_PEZONS
#define PEZONS_BASE_SPRID				40
#define PEZON_WAIT						50
#define PEZON_THRUST					384
#define PEZON_VY_FALLING_MAX			256
#define PEZON_G							16

// Chac chacs

//#define ENABLE_CHAC_CHAC
#define CHAC_CHAC_BASE_TILE				16
#define CHAC_CHAC_IDLE_2				16
#define CHAC_CHAC_IDLE_3				1
#define CHAC_CHAC_IDLE_4				50

// Monococos

//#define ENABLE_MONOCOCOS
//#define MONOCOCO_TYPE_A				// Comment for 4 cells monococo (appearing/disappearing & hidden)
#define MONOCOCO_BASE_SPRID				56
#define MONOCOCO_BASE_TIME_HIDDEN		150
#define MONOCOCO_BASE_TIME_APPEARING	50
#define MONOCOCO_BASE_TIME_ONBOARD		50
#define MONOCOCO_FIRE_COCO_AT			MONOCOCO_BASE_TIME_ONBOARD/2

// Shooties

//#define ENABLE_SHOOTIES
#define SHOOTIES_BASE_SPRID				40
#define SHOOTIES_SHOOT_OFFS_X			16
#define SHOOTIES_SHOOT_OFFS_Y			-2
#define SHOOT_FREQ						(pry+23>=en_y[gpit]&&pry<=en_y[gpit]+23&&((en_facing[gpit]&&en_x[gpit]>prx)||(en_facing[gpit]==0&&en_x[gpit]<prx))&&(rand8()&0x1f)==0)

// Punchies

//#define ENABLE_PUNCHIES
#define PUNCHIES_BASE_SPRID				32
#define PUNCHIES_PUNCH_OFFS_X			16
#define PUNCHIES_PUNCH_OFFS_Y			-7
#define PUNCH_FREQ						(pry+23>=en_y[gpit]&&pry<=en_y[gpit]+23&&((en_facing[gpit]&&en_x[gpit]>prx)||(en_facing[gpit]==0&&en_x[gpit]<prx))&&DELTA(prx,en_x [gpit]+4)<16)

// Steady shooters

#define ENABLE_STEADY_SHOOTERS
#define STEADY_SHOOTERS_BASE_SPRID		44
#define STEADY_SHOOTER_KILLABLE
#define STEADY_SHOOTER_COUNT				// If Killable, add to body count as well

// Compiled enemies

#define ENABLE_COMPILED_ENEMS
#define COMPILED_ENEMS_SHOOT
#define COMPILED_ENEMS_BASE_SPRID		48

// Cocos will get enabled automaticly on choosing monococos or shooties.

//#define ENABLE_COCOS
#define COCOS_MAX						4
#define COCO_V							128
#define COCO_COLLIDES

#define COCO_PATTERN					12
#define COCO_PALETTE 					3
#define COCO_FAIR_D						32

// Carry directives

// Easy objects mode: A range of hotspot id's are considered
// 'objects'. The user can carry one of those. Contents of hotspots
// can change in this mode.

#define ENABLE_EASY_OBJECTS

#define HS_OBJ_EMPTY					1
#define HS_OBJ_MIN						6
#define HS_OBJ_MAX						13
#define HS_USE_OFFS						8

//#define HS_TYPE_A 					// If defined, render receptors = HS_OBJ_EMPTY, object used = object
										// Otherwise, use its own graphics.
#define HS_FIX_ON_USE					// If defined, object N used at N+H_USE_OFFS becomes N+H_USE_OFFS*2

// Silly Brawlers
// --------------
//#define PLAYER_PUNCHES				// When on floor
#define PLAYER_PUNCH_OFFS_X				15
#define PLAYER_PUNCH_OFFS_Y				-7

//#define PLAYER_KICKS					// While airborne
#define PLAYER_KICK_OFFS_X				12
#define PLAYER_KICK_OFFS_Y				-3

#define PLAYER_FROZEN_FRAMES			16	// Frames to be frozen when hit landed

// Shooting behaviour
// ------------------
/*
#define PLAYER_CAN_FIRE 					// If defined, shooting engine is enabled.
#define PLAYER_BULLET_SPEED 			4	// Pixels/frame. 
#define MAX_BULLETS 					4	// Max number of bullets on screen. Be careful!.
#define PLAYER_BULLET_Y_OFFSET			6	// vertical offset from the player's top.
#define PLAYER_BULLET_X_OFFSET			-4	// vertical offset from the player's left/right.
#define RESPAWN_ON_ENTER					// Enemies respawn when entering screen
#define PLAYER_MIN_KILLABLE 			3	// If defined, only enemies >= N can be killed.

#define MAX_AMMO						99	// If defined, ammo is not infinite!
#define AMMO_REFILL						50	// type 3 hotspots refill amo, using tile 20
//#define INITIAL_AMMO 					0	// If defined, ammo = X when entering game.

#define BULLET_PALETTE					3
#define BULLET_PATTERN					0	// To paint the bullet. Can be an expresion.
*/

// Scripting
// ---------

//#define ACTIVATE_SCRIPTING				// Activates msc scripting and flag related stuff.
#define CLEAR_FLAGS						// Zero all flags when entering a level

//#define ENABLE_CONTAINERS				// Graphical flag representations!
//#define CONTAINERS_FROM_CODE			// Add arrays for containers instead of using scripting.
#define CONTAINERS_MAX 					4	// As per screen
#define FLAG_INVENTORY					0

//#define FIRE_ON_KILL					// run fire script on enemy kill

//#define ENABLE_EXTERN_CODE			// Enables custom code to be run from the script using EXTERN n
//#define ENABLE_FIRE_ZONE				// Allows to define a zone which auto-triggers "FIRE"

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
#define PLAYER_STEPS_ON_ENEMS			// If defined, stepping on enemies kills them
//#define PLAYER_SAFE_LANDING			// Like KILLS_ENEMIES but without the killing.
//#define PLAYER_MIN_KILLABLE     3     // Only kill enemies with id >= PLAYER_MIN_KILLABLE

// ============================================================================
// III. Screen configuration
// ============================================================================

// This sections defines how stuff is rendered, where to show counters, etcetera

#define LIFE_X					4		//
#define LIFE_Y					4		// Life gauge counter character coordinates

//#define OBJECTS_X				18		//
//#define OBJECTS_Y				3		// Objects counter character coordinates

#define KEYS_X					12		//
#define KEYS_Y					4		// Keys counter character coordinates

#define KILLED_X				28		//
#define KILLED_Y				4		// Kills counter character coordinates

//#define AMMO_X				8		// 
//#define AMMO_Y				2		// Ammo counter character coordinates

#define HS_INV_X				160		//
#define HS_INV_Y				15		// Object you are carrying

//#define TIMER_X				0		//
//#define TIMER_Y				5		// Current timer value

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
#define PLAYER_VY_JUMP_MAX		256		// Max. velocity when jumping
#define PLAYER_AY_JUMP 			16		// Jumpin acceleration 

#define PLAYER_AY_JETPAC		32		// Jetpac increment
#define PLAYER_VY_JETPAC_MAX	256		// Max jetpac vertical speed

#define PLAYER_AY_SWIM			8		// Swimming acceleration.
#define PLAYER_VY_SWIM_MAX		64		// Swimming max. speed

#define PLAYER_VY_LADDERS		96

#define PLAYER_AY_FLOAT			16	
#define PLAYER_VY_FLOAT_MAX		256

// IV.2. Horizontal (side view) or general (top view) movement.

#define PLAYER_VX_MAX			128		// Max. horizontal speed
#define PLAYER_VX_SPRINT_MAX	192
#define PLAYER_VX_MAX_PODEWWWR	256
#define PLAYER_VX_CONVEYORS 	64
#define PLAYER_AX				8		// Horizontal acceleration
#define PLAYER_AX_ICE			4
#define PLAYER_AX_SPRINT		12
#define PLAYER_RX				8		// Horizontal friction
#define PLAYER_RX_ICE			2

#define PLAYER_VX_MIN (PLAYER_AX << 3)

#define PLAYER_V_REBOUND		224

// Animation cells 

#ifdef PLAYER_TOP_DOWN

	// Cell definitions for top-down view

	#define CELL_FACING_RIGHT 	0
	#define CELL_FACING_LEFT 	6
	#define CELL_FACING_UP 		18
	#define CELL_FACING_DOWN 	12

	#define CELL_IDLE			0
	#define CELL_WALK_CYCLE		1
	#define CELL_PUSHING		5

	#define CELL_USE			24

#else

	// Cell definitions for side view

	#define CELL_FACING_RIGHT	0
	#define CELL_FACING_LEFT	6

	#define CELL_IDLE			0
	#define CELL_WALK_INIT		1
	#define CELL_WALK_CYCLE		1
	#define CELL_AIRBORNE		5
	#define CELL_ASCENDING		5
	#define CELL_DESCENDING		6

	#define CELL_SWIM_CYCLE		6

	#define CELL_USE			12

	#define CELL_PUNCHING		8
	#define CELL_KICKING		9

	#define CELL_CLIMB_CYCLE	20
#endif

// Music

#define MUSIC_INGAME 			0

// SFX

#define SFX_START				0
#define SFX_TILE				1
#define SFX_OBJECT				2
#define SFX_USE					3
#define SFX_PHIT				4
#define SFX_DUMMY1				5
#define SFX_ENHIT 				6
#define SFX_DUMMY2				7
#define SFX_JUMP				8
#define SFX_BULLET				9
#define SFX_COCO				10
#define SFX_SPRING				11
#define SFX_COUNT				12
#define SFX_BREAKH				13
#define SFX_HITTER				14
#define SFX_STEPON				15
#define SFX_FLOAT				16
#define SFX_BREAKB				17
