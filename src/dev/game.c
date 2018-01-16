// NES MK1 v0.7
// Copyleft Mojon Twins 2013, 2015, 2016

// Uses neslib and related tools by Shiru

// Use 0 form ROM1, 1 for ROM2, 2 for ROM3
#define BASE_LEVEL		0

// Comment this when you are done
//#define DEBUG
#define DEBUG_LEVEL		0
#define DEBUG_SCR_INI	20
#define DEBUG_INI_X		14
#define DEBUG_INI_Y		5
//

#define MSB(x)			(((x)>>8))
#define LSB(x)			(((x)&0xff))
#define FIXBITS			6
#define SPRITE_ADJUST	7
#define TOP_ADJUST		2
#define COMM_POOL		((unsigned char*)0x0300)

#define MAX_CERROJOS 16 // max 32, make it as small as possible.

#include "neslib.h"

// OAM TABLE
/*
	00-23 - Enemigos 		(OAM 0-95)
	24-27 - Objeto Inv.		(OAM 96-111)
	28-31 - NO!				(OAM 112-127)
	32-37 - player 			(OAM 128-151)
	38-41 - hotspot 		(OAM 152-167) <- Mover
	42-61 - contenedores	(OAM 168-247)
	62    - resonador cd.	(OAM 248-251)
	63    - patata patam ?
*/

#define OAM_OCCLU		0
#define OAM_ENEMS		48
#define OAM_INVENTORY	96
#define OAM_NO			112
#define OAM_PLAYER		128
#define OAM_HOTSPOTS	152
#define OAM_CONTAINERS	168
#define OAM_RESONATOR	248

// **************
// * const data *
// **************

#include "definitions.h"
#include "config.h"
#include "assets/palettes.h"
#include "assets/metasprites.h"
#include "assets/tiledata1.h"
#include "assets/mapa.h"
#include "assets/enems.h"

// Music
extern const unsigned char m_ingame_1 [];
extern const unsigned char m_cuts [];

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

#ifdef ENABLE_PROPELLERS
void add_propeller (unsigned char x, unsigned char y);
#endif
#include "engine/general.h"
#include "engine/printer.h"
#ifndef DEACTIVATE_KEYS
#include "engine/bolts.h"
#endif
#ifdef BREAKABLE_WALLS
#include "engine/breakable.h"
#endif
#include "engine/hotspots.h"
#ifdef ENABLE_PROPELLERS
#include "engine/propellers.h"
#endif
#ifdef ENABLE_CONTAINERS
#include "engine/containers.h"
#endif
#include "engine/player.h"
#include "engine/enengine.h"
#include "engine/frame.h"
#include "engine/pres.h"
#ifdef ACTIVATE_SCRIPTING
#include "assets/mscnes.h"
#endif
#include "mainloop.h"

// *************
// Main function
// *************

void main(void) {
	bank_spr (1);
	bank_bg (0);

	scroll (0, 8);
	ppu_off ();

	while (1) {	// This while(1) is to make this NROM-compatible for testing purposes.

		//title ();
		game_init ();
		game_loop ();

		if (game_over) {
			// game_over ();
		} else {
			// game_ending ();
		}
	}
}
