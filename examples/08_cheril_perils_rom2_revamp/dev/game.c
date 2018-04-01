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
#include "assets/chr_rom_maps.h"
#include "assets/enems0.h"
#include "assets/enems1.h"
#include "assets/enems2.h"
#include "assets/enems3.h"
#include "assets/enems4.h"
#include "assets/spritedata.h"
#include "assets/spritedata2.h"
#include "assets/tiledata.h"
#include "assets/tiledata2.h"
#include "assets/metasprites.h"
#ifdef ENABLE_TEXT_BOX
	#include "assets/custom_texts.h"
#endif
#ifdef ENABLE_COMPILED_ENEMS
	#include "assets/compiled_enems.h"
#endif
#if defined (ENABLE_INTERACTIVES) && defined (INTERACTIVES_FROM_CODE)
	#include "assets/interactives.h"
#endif
#ifdef MULTI_LEVEL
	#include "assets/levelset.h"
#endif

#include "assets/hud_rle.h"

 // Custom bgs
#include "assets/bg0.h"
#include "assets/bg3.h"
#include "my/bgs.h"

// Music
extern const unsigned char m_ingame [];

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
#ifdef ENABLE_TEXT_BOX
	#include "engine/textbox.h"
#endif
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
#ifdef ENABLE_INTERACTIVES
	#include "engine/interactives.h"
#endif
#ifdef ENABLE_COCOS
	#include "engine/cocos.h"
#endif
#include "engine/player.h"
#include "engine/enengine.h"
#include "engine/frame.h"
#include "my/pres.h"
#ifdef ACTIVATE_SCRIPTING
	#ifdef ENABLE_EXTERN_CODE
		#include "my/extern.h"
	#endif
	#include "assets/mscnes.h"
#endif
#include "mainloop.h"

// *************
// Main function
// *************

void main(void) {
	bank_spr (1);
	bank_bg (0);

	ppu_off ();
	first_game = 1;

	// Main loop

	while (1) {	

		//title ();

#ifdef MULTI_LEVEL		
		level = 2;
#endif
		plife = PLAYER_LIFE;

		// Game loop

		while (1) {
			scroll (0, SCROLL_Y);
			game_init (); 
			game_loop ();

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

		first_game = 0;
	}
}
