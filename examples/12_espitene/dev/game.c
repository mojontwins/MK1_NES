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
#include "assets/enem_constants.h"
#include "assets/enem_index.h"
#include "assets/spritedata.h"
#include "assets/tiledata.h"
#include "assets/metasprites.h"
#include "assets/custom_texts.h"
#ifdef ENABLE_COMPILED_ENEMS
	#include "assets/compiled_enems.h"
#endif
#if defined (ENABLE_INTERACTIVES) && defined (INTERACTIVES_FROM_CODE)
	#include "assets/interactives.h"
#endif
#ifdef MULTI_LEVEL
	#include "assets/levelset.h"
#endif

#include "assets/cuts0_rle.h"
#include "assets/cuts1_rle.h"
#include "assets/cuts2_rle.h"
#include "assets/title_rle.h"

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
#include "engine/printer.h"
#include "engine/general.h"
#ifdef ENABLE_TEXT_BOX
	#include "engine/textbox.h"
#endif
#include "my/extra_modules.h"
#ifdef ENABLE_BREAKABLE
	#include "engine/breakable.h"
#endif
#include "engine/hotspots.h"
#ifdef ENABLE_PROPELLERS
	#include "engine/propellers.h"
#endif
#ifdef ENABLE_TILE_CHAC_CHAC
	#include "engine/tile_chac_chac.h"
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
#if (defined (ACTIVATE_SCRIPTING) && defined (ENABLE_EXTERN_CODE)) || defined (ENABLE_COMPILED_ENEMS)
	#include "my/extern.h"
#endif
#include "engine/frame.h"
#include "my/pres.h"
#ifdef ACTIVATE_SCRIPTING
	#include "assets/mscnes.h"
#endif
#include "mainloop/flickscreen.h"
#include "mainloop.h"

// *************
// Main function
// *************

void game_do (void) {
	pres (paltstitle, scr_level);
	game_init (); 
	game_loop ();
}

void main(void) {
	bank_spr (1);
	bank_bg (0);

	ppu_off ();
	first_game = 1;
	ntsc = ppu_system ();

	// Main loop

	while (1) {	
		title ();	// level* vars are set there.
		plife = PLAYER_LIFE;

		if (level == 0) {
			music_play (MUSIC_CUTS);
			rdm = 0; cutscene ();
			rdm = 1; cutscene ();
			music_stop ();
		}

		// Game loop

		while (1) {
			if (select_level) {
				zone_select ();
				select_level = 0;
			}

			scroll (0, SCROLL_Y);

			level_world = base_world [level];
			level_act = base_act [level];

			game_do ();

			if (game_over) {
				level_reset = 0;
				pres (palts0, scr_game_over);
				break;
			} else if (level_reset) {
				// Do nothing
			} else {
				// Level was finished
				if (!free_play || level_act < 2) {
					level ++;
					if (level == MAX_LEVELS) {
						music_play (MUSIC_CUTS);
						rdm = 2; cutscene ();
						if (pemmeralds != 0x3f) { 
							rdm = 3; 
							cutscene (); 
							free_play = select_level = 1;
							level = 0;
						}
					}
				} 

				if (free_play && level_act == 2) select_level = 1;

				if (pemmeralds == 0x3f) {
					rdm = 4; cutscene ();
					first_game = 1;
					break;
				}
			}
		}

		first_game = 0;
	}
}
