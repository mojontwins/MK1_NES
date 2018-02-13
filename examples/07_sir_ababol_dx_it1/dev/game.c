// NES MK1 v1.0
// Copyleft Mojon Twins 2013, 2015, 2017, 2018

// Uses neslib and related tools by Shiru

// Comment this when you are done
//#define DEBUG
#define DEBUG_LEVEL		0
#define DEBUG_SCR_INI	20
#define DEBUG_INI_X		14
#define DEBUG_INI_Y		5
//

#define MAX_BOLTS 16 // max 32, make it as small as possible.

#include "neslib.h"

#include "definitions.h"
#include "config.h"
#include "autodefs.h"

// **************
// * const data *
// **************

#include "assets/precalcs.h"
#include "assets/palettes.h"
#include "assets/behs.h"
#include "assets/map0.h"
#include "assets/map1.h"
#include "assets/enems0.h"
#include "assets/enems1.h"
#include "assets/spritedata.h"
#include "assets/tiledata.h"
#include "assets/metasprites.h"
#ifdef MULTI_LEVEL
	#include "assets/levelset.h"
#endif

#include "assets/custom_texts.h"

// Music
extern const unsigned char m_title [];
extern const unsigned char m_ingame [];
extern const unsigned char m_gameover [];
extern const unsigned char m_ending [];

// Push to zero page:
#pragma bssseg (push,"ZEROPAGE")
#pragma dataseg(push,"ZEROPAGE")

#include "ram/zp.h"

// Everything else on normal RAM
#pragma bssseg (push,"BSS")
#pragma dataseg(push,"BSS")

#include "ram/bss.h"

// *************
// Main includes
// *************

#include "engine/prototypes.h"
#include "engine/general.h"
#include "engine/printer.h"
#ifndef DEACTIVATE_KEYS
	#include "engine/bolts.h"
#endif
#ifdef ENABLE_BREAKABLE
	#include "engine/breakable.h"
#endif
#include "engine/hotspots.h"
#ifdef ENABLE_PROPELLERS
	#include "engine/propellers.h"
#endif
#ifdef ENABLE_SHINES
	#include "engine/shines.h"
#endif
#ifdef ENABLE_CONTAINERS
	#include "engine/containers.h"
#endif
#ifdef ENABLE_COCOS
	#include "engine/cocos.h"
#endif
#include "engine/player.h"
#include "engine/enengine.h"
#include "engine/frame.h"
#include "my/pres.h"
#ifdef ACTIVATE_SCRIPTING
	#include "assets/mscnes.h"
#endif
// CUSTOM {
	#include "engine/textbox.h"
// } END_OF_CUSTOM
#include "mainloop.h"

// *************
// Main function
// *************

void main(void) {
	bank_spr (1);
	bank_bg (0);

	ppu_off ();

	// Main loop

	while (1) {	

		//title ();

#ifdef MULTI_LEVEL
		level = 0;
		// CUSTOM {
			pobjs = 0;
			pkeys = 0;

			px = (signed int) (PLAYER_INI_X << 4) << FIXBITS;
			py = (signed int) (PLAYER_INI_Y << 4) << FIXBITS;
			pfacing = 0;

			level_switching = 0;
			has_boots = 0;
		// } END_OF_CUSTOM
#endif
		plife = PLAYER_LIFE;

		// Game loop

		while (1) {
			scroll (0, SCROLL_Y);
			game_init (); 
			game_loop ();

			// CUSTOM {
			if (level_switching) {
				level_switching = 0;
				level = 1 - level;
			} else
			// } END_OF_CUSTOM
			if (game_over) {
				// game_over ();
				break;
			} else {
#ifdef MULTI_LEVEL
				level ++;
				if (level == MAX_LEVELS) 
#endif
				{
					// game_ending ();
					break;
				}
			}
		}
	}
}
