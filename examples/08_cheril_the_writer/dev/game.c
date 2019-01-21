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

#include "engine/precalcs.h"
#include "assets/palettes.h"
#include "assets/behs.h"
#include "assets/chr_rom_maps.h"		// Maps in CHR-ROM
#include "assets/enem_constants.h"		// 
#include "assets/enem_index.h"			// Enems & hotspots in CHR-ROM
#include "assets/spritedata.h"
#include "assets/spritedata2.h"
#include "assets/tiledata.h"
#include "assets/tiledata2.h"
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

#ifdef SPANISH
	#include "assets/title_sp_rle.h"
#else
	#include "assets/title_en_rle.h"
#endif

#include "assets/hud_rle.h"

#include "assets/cuts0_rle.h"
#include "assets/cuts1_rle.h"
#include "assets/cuts2_rle.h"
#include "assets/cuts3_rle.h"

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

void main(void) {
	bank_spr (1);
	bank_bg (0);
	scroll (0, SCROLL_Y);

	ppu_off ();
	first_game = 1;
	ntsc = ppu_system ();

	// Main loop

	while (1) {	

		//pres_title ();
		bankswitch (2); bank_bg (1);
		//music_play (MUSIC_TITLE);
		pres (paltitle, scr_title);
		music_stop ();

#ifdef MULTI_LEVEL		
		level = 0;
#endif
		plife = PLAYER_LIFE;

		// Game loop

		while (1) {
			bankswitch (2); 
			pres (palcuts0, scr_cutscene);
			pres (palcuts0, scr_level);
			game_init (); 
			game_loop ();

			if (game_over) {
				pres (palcuts0, scr_game_over);
				break;
			} else {
#ifdef MULTI_LEVEL
				level ++;
				if (level == MAX_LEVELS) 
#endif
				{
					bankswitch (2);
					pres (palcuts0, scr_cutscene);
					pres (palcuts0, scr_the_end);
					break;
				}
			}
		}

		first_game = 0;
	}
}
