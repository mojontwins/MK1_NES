// NES MK1 v0.7
// Copyleft Mojon Twins 2013, 2015, 2016

// ============================================================================
// I. General configuration
// ============================================================================

// In this section we define map dimmensions, initial and authomatic ending conditions, etc.

#define MAP_W 					6		//
#define MAP_H					5		// Dimensiones del mapa, en pantallas.
#ifdef DEBUG
#define SCR_INI DEBUG_SCR_INI
#define PLAYER_INI_X DEBUG_INI_X
#define PLAYER_INI_Y DEBUG_INI_Y
#else
#define SCR_INI					24		// Pantalla de inicio
#define PLAYER_INI_X			2		//
#define PLAYER_INI_Y			3		// Coordenadas de inicio del jugador, a nivel de tiles
#endif
#define SCR_END					99		// Pantalla del final. 99 si da igual.
#define PLAYER_END_X			99		//
#define PLAYER_END_Y			99		// Posición del jugador para terminar, a nivel de tiles
#define PLAYER_MAX_OBJECTS		25		// Número de objetos para terminar el juego
#define PLAYER_LIFE				5		// Vida máxima (con la que empieza, además)
#define PLAYER_REFILL			1		// Recarga de vida.

// Some flexibility
#define HOTSPOTS_WONT_CHANGE			// types of hotspots won't change
#define HOTSPOT_TYPE_OBJECT		1
#define HOTSPOT_TYPE_KEYS		2
#define HOTSPOT_TYPE_REFILL		3
//#define HOTSPOT_TYPE_STAR		4
//#define HOTSPOT_TYPE_AMMO		4
//#define HOTSPOT_TYPE_RESONATOR	3

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
#define SMALL_COLLISION               	// 8x8 centered collision instead of 12x12

// General directives:
// -------------------

//#define SPRITE_BADDIE_DYING 	spr_en_0A
#define SPRITE_BADDIE_DYING 	spr_en_0E_a
//#define PLAYER_PUSH_BOXES 			// If defined, tile #14 is pushable
//#define FIRE_TO_PUSH
//#define DEACTIVATE_KEYS				// If defined, keys are not present.
//#define DEACTIVATE_OBJECTS			// If defined, objects are not present.
//#define PLAYER_BOUNCES
//#define DOUBLE_BOUNCE
#define DIE_AND_RESPAWN					// If defined, dying = respawn on latest safe.
#define PLAYER_FLICKERS 			 	// If defined, collisions make player flicker instead.
//#define WALLS_STOP_ENEMIES			// If defined, enemies react to the scenary

//#define ENABLE_PURSUERS				// If defined, type 7 enemies are active
//#define DEATH_COUNT_EXPRESSION	50+(rand8()&63)
//#define TYPE_7_FIXED_SPRITE 	4		// If defined, type 7 enemies are always #


#define ENABLE_FANTY
//#define ENABLE_HOMING_FANTY
#define FANTY_BASE_SPRID		18
#define FANTY_A 				4
#define FANTY_MAXV 				48
#define FANTY_COLLIDES
//#define FANTY_DISTANCE		80
//#define FANTY_V_RETREAT		16
//#define FANTY_KILLED_BY_TILE

/*
#define ENABLE_SAW
#define SAW_BASE_SPRID			16
#define SAW_V_DISPL				4
#define SAW_EMERGING_STEPS		10
*/

/*
#define ENABLE_PEZONS
#define PEZONS_BASE_SPRID		16
#define PEZON_WAIT				50
#define PEZON_THRUST			384
#define PEZON_VY_FALLING_MAX	256
#define PEZON_G					16
*/

/*
#define ENABLE_CHAC_CHAC
#define CHAC_CHAC_BASE_TILE		48
#define CHAC_CHAC_IDLE_2		16
#define CHAC_CHAC_IDLE_3		1
#define CHAC_CHAC_IDLE_4		50
*/

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

/*
#define ENABLE_CONTAINERS
#define MAX_CONTAINERS 			5
#define CONT_EMPTY				0
*/

//#define PERSISTENT_ENEMIES
//#define ENABLE_CONVEYORS
//#define PERSISTENT_DEATHS

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
*/

/*
#define PLAYER_GARGAJO					// For bootee.
#define MAX_GARGAJOS 			2
#define GARGAJO_L_MIN			8
#define GARGAJO_L_MAX			48
//#define GARGAJO_L_DIVIDER		1  		// Number of SHL to # of frames when calculating L, if defined.
#define GARGAJO_V 				4
#define GARGAJO_BASE_PATTERN	248
#define GARGAJO_CELL_MOD		1 		// 1 = 2 frames, 3 = 4 frames, 7 = 8 frames... (will be AND'd)
#define GARGAJO_FACING_OFFSET	2
#define GARGAJO_Y_OFFSET		0		// From the player's top
#define GARGAJO_X_OFFSET		0		// From the player's left/right
#define GARGAJO_RELOAD			16
*/

/*
#define BREAKABLE_WALLS					// Breakable walls
#define BREAKABLE_WALLS_LIFE	2		// Amount of hits to break wall
#define BREAKABLE_ANIM					// Show explosion when breaking
#define MAX_BREAKABLE_FRAMES	8		// Frames to show explosion
*/

// Scripting
// ---------
/*
#define ACTIVATE_SCRIPTING			// Activates msc scripting and flag related stuff.
#define ENABLE_FAST_FIRE_ZONE		// Fire zone only triggers FIRE n, not FIRE ANY
#define SCRIPTING_TEXT_BOX
#define FIRE_SCRIPT_WITH_ANIMATION	// So I don't have to make a custom for something 
									// I will be reusing, most likely
*/

//#define FIRE_ON_KILL				// run fire script on enemy kill
/*
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

#define PLAYER_HAS_JUMP					// If defined, player is able to jump.
//#define PLAYER_BOOTEE					// Jump is automatic
//#define PLAYER_HAS_JETPAC             // If defined, player can thrust a vertical jetpac
//#define PLAYER_STEPS_ON_ENEMIES			// If defined, stepping on enemies kills them
//#define PLAYER_SAFE_LANDING			// Like KILLS_ENEMIES but without the killing.
//#define PLAYER_MIN_KILLABLE     3     // Only kill enemies with id >= PLAYER_MIN_KILLABLE
#define NO_HORIZONTAL_EVIL_TILE			// Only check for colisions with evil tiles vertically

// ============================================================================
// III. Screen configuration
// ============================================================================

// This sections defines how stuff is rendered, where to show counters, etcetera

#define LIFE_X					6		//
#define LIFE_Y					2		// Life gauge counter character coordinates
#define OBJECTS_X				0x1d	//
#define OBJECTS_Y				2		// Objects counter character coordinates
#define KEYS_X					0x10	//
#define KEYS_Y					2		// Keys counter character coordinates
#define KILLED_X				16		//
#define KILLED_Y				2		// Kills counter character coordinates
#define AMMO_X					8		// 
#define AMMO_Y					29		// Ammo counter character coordinates
//#define PLAYER_SHOW_KILLS

// Text
#define LINE_OF_TEXT			3		// If defined, scripts can show text @ Y = #
#define LINE_OF_TEXT_X			1		// X coordinate.
#define LINE_OF_TEXT_ATTR		71		// Attribute

// ============================================================================
// IV. Player movement configuration
// ============================================================================

// This section is used to define which constants are used in the gravity/acceleration engine.
// If a side-view engine is configured, we have to define vertical and horizontal constants
// separately. If a top-view engine is configured instead, the horizontal values are also
// applied to the vertical component, vertical values are ignored.

// IV.1. Vertical movement. Only for side-view.

#define PLAYER_VY_FALLING_MAX	256		// Velocidad máxima cuando cae (512/64 = 8 píxels/frame)
#define PLAYER_G				16		// Aceleración de la gravedad (32/64 = 0.5 píxeles/frame^2)

#define PLAYER_VY_JUMP_INITIAL	64
#define PLAYER_VY_JUMP_MAX		256		// Velocidad máxima al saltar (320/64 = 5 píxels/frame)
#define PLAYER_AY_JUMP 			16		// Aceleración al pulsar "salto" (48/64 = 0.75 píxeles/frame^2)

#define PLAYER_AY_JETPAC		32		// Incremento al usar el jetpac
#define PLAYER_VY_JETPAC_MAX	256		// Máxima velocidad vertical con jetpac

//a 128 b 64 X 64 c 128 d
#define PLAYER_VY_AN1 			-128
#define PLAYER_VY_AN2 			-64
#define PLAYER_VY_AN3 			64
#define PLAYER_VY_AN4 			128

#define PLAYER_AY_UNTHRUST 		8 		// Used in the Bootee engine.

// IV.2. Horizontal (side view) or general (top view) movement.

#define PLAYER_VX_MAX			128		// Velocidad máxima horizontal (192/64 = 3 píxels/frame)
#define PLAYER_VX_SPRINT_MAX	192
#define PLAYER_VX_MAX_PODEWWWR	256
#define PLAYER_AX				8		// Aceleración horizontal (24/64 = 0,375 píxels/frame^2)
#define PLAYER_AX_SPRINT		12
#define PLAYER_RX				8		// Fricción horizontal (32/64 = 0,5 píxels/frame^2)

#define PLAYER_VX_MIN (PLAYER_AX << 2)

#define PLAYER_V_REBOUND		224

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
// 32 = Slipper (+0 = left, +1 = right
// You can add the numbers to get combined behaviours
// Save for 10 (special), but that's obvious, innit?

const unsigned char tbehs [] = {
	0, 8, 8, 0, 0, 8, 4, 1, 8, 8, 8, 4, 4, 8, 8, 10,
	0, 8, 8, 8, 0, 0, 8, 8, 0, 8, 1, 8, 8, 8, 4, 10,
	0, 0
};
